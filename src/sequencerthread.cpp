/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005 by Pedro Lopez-Cabanillas                          *
 *   <plcl@users.sourceforge.net>                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <qstringlist.h>
#include <kapplication.h>
#include <klocale.h>
#include <alsa/asoundlib.h>
#include "sequencerthread.h"

using std::cerr;
using std::endl;

SequencerThread::SequencerThread(QWidget *parent):QThread(),
	m_widget(parent),
        m_weak_note(METRONOME_WEAK_NOTE),
	m_strong_note(METRONOME_STRONG_NOTE),
	m_velocity(METRONOME_VELOCITY),
	m_program(METRONOME_PROGRAM),
	m_channel(METRONOME_CHANNEL),
	m_resolution(METRONOME_RESOLUTION),
	m_bpm(100),
	m_ts_num(4),
	m_ts_div(4),
	m_autoconnect(false),
	m_outputConn(""),
	m_inputConn("")
{
    int err;
    err = snd_seq_open(&m_handle, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK);
    checkAlsaError(err, "Sequencer open");
    m_client = snd_seq_client_id(m_handle);
    snd_seq_set_client_name(m_handle, "KMetronome");
    m_input = snd_seq_create_simple_port(m_handle, "input",
				     SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
				     SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    checkAlsaError(m_input, "Creating input port");
    m_output = snd_seq_create_simple_port(m_handle, "output",
				     SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
				     SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    checkAlsaError(m_output, "Creating output port");
    snd_seq_connect_from(m_handle, m_input, SND_SEQ_CLIENT_SYSTEM, SND_SEQ_PORT_SYSTEM_ANNOUNCE);
    m_queue = snd_seq_alloc_named_queue(m_handle, "metronome_queue");
    checkAlsaError(m_queue, "Creating queue");
    metronome_set_program();    
    metronome_set_tempo();
}

int SequencerThread::checkAlsaError(int rc, const char *message) 
{
    if (rc < 0) {
	cerr << "Error " << message 
	     << " rc: " << rc
	     << " (" << snd_strerror(rc) 
	     << " )"
	     << endl;
    }
    return rc;
}

SequencerThread::~SequencerThread()
{
    int err;
    err = snd_seq_free_queue(m_handle, m_queue);
    checkAlsaError(err, "Freeing queue");
    err = snd_seq_close(m_handle);
    checkAlsaError(err, "Closing");
}

QStringList SequencerThread::inputConnections()
{
    return list_ports(SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ);
}

QStringList SequencerThread::outputConnections()
{
    return list_ports(SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE);
}

QStringList SequencerThread::list_ports(unsigned int mask)
{
    QStringList list;
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_client_info_set_client(cinfo, -1);
    list += NO_CONNECTION;
    while (snd_seq_query_next_client(m_handle, cinfo) >= 0) {
	int client = snd_seq_client_info_get_client(cinfo);
	if (client == SND_SEQ_CLIENT_SYSTEM || client == m_client)
	    continue;
	snd_seq_port_info_set_client(pinfo, client);
	snd_seq_port_info_set_port(pinfo, -1);
	while (snd_seq_query_next_port(m_handle, pinfo) >= 0) {
	    if ((snd_seq_port_info_get_capability(pinfo) & mask) != mask)
		continue;
	    list += QString("%1:%2")
			   .arg(snd_seq_client_info_get_name(cinfo))
			   .arg(snd_seq_port_info_get_port(pinfo));
	}
    }
    return list;
}

void SequencerThread::connect_output()
{
    snd_seq_addr_t dest;
    if (m_outputConn.isEmpty() || m_outputConn == NO_CONNECTION)
    	return;
    checkAlsaError(snd_seq_parse_address(m_handle, &dest, m_outputConn.ascii()), 
    		   "snd_seq_parse_address");
    checkAlsaError(snd_seq_connect_to(m_handle, m_output, dest.client, dest.port), 
                   "snd_seq_connect_to");
}

void SequencerThread::disconnect_output()
{
    snd_seq_addr_t dest;
    if (m_outputConn.isEmpty() || m_outputConn == NO_CONNECTION)
    	return;
    checkAlsaError(snd_seq_parse_address(m_handle, &dest, m_outputConn.ascii()),
    		   "snd_seq_parse_address");
    checkAlsaError(snd_seq_disconnect_to(m_handle, m_output, dest.client, dest.port),
    		   "snd_seq_disconnect_to");
}

void SequencerThread::connect_input()
{
    snd_seq_addr_t src;
    if (m_inputConn.isEmpty() || m_inputConn == NO_CONNECTION)
    	return;
    checkAlsaError(snd_seq_parse_address(m_handle, &src, m_inputConn.ascii()), 
    		   "snd_seq_parse_address");
    checkAlsaError(snd_seq_connect_from(m_handle, m_input, src.client, src.port), 
                   "snd_seq_connect_from");
}

void SequencerThread::disconnect_input()
{
    snd_seq_addr_t src;
    if (m_inputConn.isEmpty() || m_inputConn == NO_CONNECTION)
    	return;
    checkAlsaError(snd_seq_parse_address(m_handle, &src, m_inputConn.ascii()),
    		   "snd_seq_parse_address");
    checkAlsaError(snd_seq_disconnect_from(m_handle, m_input, src.client, src.port),
    		   "snd_seq_disconnect_from");
}


void SequencerThread::metronome_note(unsigned char note, unsigned int tick)
{
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_note(&ev, m_channel, note, m_velocity, 1);
    snd_seq_ev_schedule_tick(&ev, m_queue, 0, tick);
    snd_seq_ev_set_source(&ev, m_output);
    snd_seq_ev_set_subs(&ev);
    snd_seq_event_output(m_handle, &ev);
}

void SequencerThread::metronome_echo(unsigned int tick, int ev_type)
{
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    ev.type = ev_type;
    snd_seq_ev_schedule_tick(&ev, m_queue, 0, tick);
    snd_seq_ev_set_source(&ev, m_output);
    snd_seq_ev_set_dest(&ev, m_client, m_input);
    snd_seq_event_output(m_handle, &ev);
}

void SequencerThread::metronome_pattern(unsigned int tick)
{
    int j, t, duration;
    t = tick;
    duration = m_resolution * 4 / m_ts_div;
    for (j = 0; j < m_ts_num; j++) {
	metronome_note(j ? m_weak_note : m_strong_note, t);
	metronome_echo(t, SND_SEQ_EVENT_USR1);
	t += duration;
    }
    metronome_echo(t, SND_SEQ_EVENT_USR0);
    snd_seq_drain_output(m_handle);
}

void SequencerThread::metronome_set_program()
{
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_pgmchange(&ev, m_channel, m_program);
    snd_seq_ev_set_source(&ev, m_output);
    snd_seq_ev_set_subs(&ev);
    snd_seq_event_output(m_handle, &ev);
    snd_seq_drain_output(m_handle);
}

void SequencerThread::metronome_set_tempo()
{
    snd_seq_queue_tempo_t *qtempo;
    int tempo = (int) (6e7 / m_bpm);
    snd_seq_queue_tempo_alloca(&qtempo);
    snd_seq_queue_tempo_set_tempo(qtempo, tempo);
    snd_seq_queue_tempo_set_ppq(qtempo, m_resolution);
    checkAlsaError(snd_seq_set_queue_tempo(m_handle, m_queue, qtempo), "queue_set_tempo");
    checkAlsaError(snd_seq_drain_output(m_handle), "drain_output");
}

void SequencerThread::updateView()
{
    KApplication::postEvent(m_widget, new MetronomeEvent(m_bar, m_beat));
}

void SequencerThread::midi_play()
{
    KApplication::postEvent(m_widget, new TransportEvent(TRANSPORT_PLAY));
}

void SequencerThread::midi_stop()
{
    KApplication::postEvent(m_widget, new TransportEvent(TRANSPORT_STOP));
}

void SequencerThread::midi_cont()
{
    KApplication::postEvent(m_widget, new TransportEvent(TRANSPORT_CONT));
}

void SequencerThread::run()
{
    struct pollfd *pfds;
    int npfds;
    int rt, err = 0;
    npfds = snd_seq_poll_descriptors_count(m_handle, POLLIN);
    pfds = (pollfd *)alloca(sizeof(*pfds) * npfds);
    snd_seq_poll_descriptors(m_handle, pfds, npfds, POLLIN);
    while(true) {
	rt = poll(pfds, npfds, 1000);
	if (rt >= 0)
	do {
	    snd_seq_event_t *ev;
	    err = snd_seq_event_input(m_handle, &ev);
	    if (err >= 0 && ev) {
		switch (ev->type) {
		case SND_SEQ_EVENT_USR0:
		    metronome_pattern(ev->time.tick);
		    m_bar++;
		    m_beat = 0;
		    break;
		case SND_SEQ_EVENT_USR1:
		    m_beat++;
		    updateView();
		    break;
		case SND_SEQ_EVENT_START:
		    midi_play();
		    break;
		case SND_SEQ_EVENT_CONTINUE:
		    midi_cont();
		    break;
		case SND_SEQ_EVENT_STOP:
		    midi_stop();
		    break;
		}
	    }
	} while (snd_seq_event_input_pending(m_handle, 0) > 0);
    }
}

void SequencerThread::metronome_start()
{
    checkAlsaError(snd_seq_start_queue(m_handle, m_queue, NULL), "Queue start");
    checkAlsaError(snd_seq_drain_output(m_handle), "Drain output");
    metronome_pattern(0);
    m_bar = 1;
    m_beat = 1;
    updateView();
}

void SequencerThread::metronome_stop()
{
    checkAlsaError(snd_seq_stop_queue(m_handle, m_queue, NULL), "Queue stop");
    checkAlsaError(snd_seq_drain_output(m_handle), "Drain output");
}

void SequencerThread::metronome_continue()
{
    checkAlsaError(snd_seq_continue_queue(m_handle, m_queue, NULL), "Queue continue");
    checkAlsaError(snd_seq_drain_output(m_handle), "Drain output");
}

