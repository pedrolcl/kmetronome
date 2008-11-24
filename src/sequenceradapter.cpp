/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2008 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
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
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA 02110-1301, USA                                                    *
 ***************************************************************************/

#include <QStringList>
#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>

#include <queue.h>
#include <event.h>

#include "kmetronome.h"
#include "sequenceradapter.h"
#include "defs.h"

SequencerAdapter::SequencerAdapter(QObject *parent) :
	QObject(parent), 
	m_weak_note(METRONOME_WEAK_NOTE),
	m_strong_note(METRONOME_STRONG_NOTE),
	m_weak_velocity(METRONOME_VELOCITY), 
	m_strong_velocity(METRONOME_VELOCITY),
	m_program(METRONOME_PROGRAM),
	m_channel(METRONOME_CHANNEL),
	m_volume(METRONOME_VOLUME),
	m_balance(METRONOME_BALANCE),
	m_resolution(METRONOME_RESOLUTION),
	m_bpm(TEMPO_DEFAULT), 
	m_ts_num(RHYTHM_TS_NUM),
	m_ts_div(RHYTHM_TS_DEN), 
	m_noteDuration(NOTE_DURATION),
	m_autoconnect(false), 
	m_playing(false), 
	m_useNoteOff(true),
	m_outputConn(""), 
	m_inputConn(""),
	NO_CONNECTION(i18n("No connection")) 
{
	m_Client = new MidiClient(this);
	m_Port = new MidiPort(this);
    m_Client->setOpenMode(SND_SEQ_OPEN_DUPLEX);
    m_Client->setBlockMode(false);
    try {
        m_Client->open();
    } catch (SequencerError& ex) {
		QString errorstr = i18n("Fatal error opening the ALSA sequencer. Function: snd_seq_open().\n"
		                   "This usually happens when the kernel doesn't have ALSA support, "
		                   "or the device node (/dev/snd/seq) doesn't exists, "
				           "or the kernel module (snd_seq) is not loaded.\n"
				           "Please check your ALSA/MIDI configuration. Returned error was: %1").arg(ex.qstrError());
		KMessageBox::error(static_cast<QWidget*>(parent), errorstr, i18n("Error"));
		KApplication::exit(-1);
	}
    m_Client->setClientName("KMetronome");
	m_clientId = m_Client->getClientId();

    connect(m_Client, SIGNAL(eventReceived(SequencerEvent*)), 
            SLOT(sequencerEvent(SequencerEvent*))); //, Qt::DirectConnection);

	m_Port->setMidiClient(m_Client);
	m_Port->setPortName("KMetronome");
	m_Port->setCapability(SND_SEQ_PORT_CAP_WRITE |
	                      SND_SEQ_PORT_CAP_SUBS_WRITE | 
	                      SND_SEQ_PORT_CAP_READ |
	                      SND_SEQ_PORT_CAP_SUBS_READ);
	m_Port->setPortType(SND_SEQ_PORT_TYPE_MIDI_GENERIC |
	                    SND_SEQ_PORT_TYPE_APPLICATION);
	m_Port->attach();
	
	m_inputPortId = m_outputPortId = m_Port->getPortId();
    m_Port->subscribeFromAnnounce();

    m_Queue = m_Client->createQueue();
    m_queueId = m_Queue->getId();

    m_Client->startSequencerInput();
    
	metronome_set_program();
	metronome_set_tempo();
	metronome_set_controls();
}

SequencerAdapter::~SequencerAdapter() 
{
    m_Client->stopSequencerInput();    
    m_Port->detach();
    m_Client->close();
}

QStringList SequencerAdapter::inputConnections() 
{
    QStringList list;
    list += NO_CONNECTION;
    QListIterator<PortInfo> it(m_Client->getAvailableInputs());
    while(it.hasNext()) {
        PortInfo p = it.next();
        list << QString("%1:%2").arg(p.getClientName()).arg(p.getPort());
    }
    return list;
}

QStringList SequencerAdapter::outputConnections() 
{
    QStringList list;
    list += NO_CONNECTION;
    QListIterator<PortInfo> it(m_Client->getAvailableOutputs());
    while(it.hasNext()) {
        PortInfo p = it.next();
        list << QString("%1:%2").arg(p.getClientName()).arg(p.getPort());
    }
    return list;
}

void SequencerAdapter::connect_output() 
{
	if (m_outputConn.isEmpty() || m_outputConn == NO_CONNECTION)
		return;
	m_Port->subscribeTo(m_outputConn);
}

void SequencerAdapter::disconnect_output() 
{
	if (m_outputConn.isEmpty() || m_outputConn == NO_CONNECTION)
		return;
	m_Port->unsubscribeTo(m_outputConn);
}

void SequencerAdapter::connect_input() 
{
	if (m_inputConn.isEmpty() || m_inputConn == NO_CONNECTION)
		return;
	m_Port->subscribeFrom(m_inputConn);
}

void SequencerAdapter::disconnect_input() 
{
	if (m_inputConn.isEmpty() || m_inputConn == NO_CONNECTION)
		return;
	m_Port->unsubscribeFrom(m_inputConn);
}

void SequencerAdapter::sendControlChange(int cc, int value)
{
    ControllerEvent ev(m_channel, cc, value);
    ev.setSource(m_outputPortId);
    ev.setSubscribers();
    ev.setDirect();
    m_Client->outputDirect(&ev);
}

void SequencerAdapter::metronome_note(int note, int vel, int tick) 
{
    SequencerEvent* ev;
    if (m_useNoteOff) 
        ev = new NoteEvent(m_channel, note, vel, m_noteDuration);
    else
        ev = new NoteOnEvent(m_channel, note, vel);
    ev->scheduleTick(m_queueId, tick, false);
    ev->setSource(m_outputPortId);
    ev->setSubscribers();
	m_Client->output(ev);
	delete ev;
}

void SequencerAdapter::metronome_echo(int tick, int ev_type) 
{
    SystemEvent ev(ev_type);
    ev.setSource(m_outputPortId);
    ev.setDestination(m_clientId, m_inputPortId);
    ev.scheduleTick(m_queueId, tick, false);
    m_Client->output(&ev);
}

void SequencerAdapter::metronome_pattern(int tick) 
{
	int j, t, duration;
	t = tick;
	duration = m_resolution * 4 / m_ts_div;
	for (j = 0; j < m_ts_num; j++) {
		metronome_note(j ? m_weak_note : m_strong_note, 
		               j ? m_weak_velocity : m_strong_velocity, t);
		metronome_echo(t, SND_SEQ_EVENT_USR1);
		t += duration;
	}
	metronome_echo(t, SND_SEQ_EVENT_USR0);
	m_Client->drainOutput();
}

void SequencerAdapter::metronome_set_program() 
{
    ProgramChangeEvent ev(m_channel, m_program);
    ev.setSource(m_outputPortId);
    ev.setSubscribers();
    ev.setDirect();
    m_Client->outputDirect(&ev);
}

void SequencerAdapter::metronome_set_tempo() 
{
    QueueTempo t = m_Queue->getTempo();
    t.setPPQ(m_resolution);
    t.setTempo((int)(6e7 / m_bpm));
	m_Queue->setTempo(t);
}

void SequencerAdapter::metronome_set_controls()
{
    sendControlChange(VOLUME_CC, m_volume);
    sendControlChange(BALANCE_CC, m_balance);
}

void SequencerAdapter::updateView() 
{
    KMetronome* m = qobject_cast<KMetronome*>(parent());
    m->updateDisplay(m_bar, m_beat);
}

void SequencerAdapter::midi_play() 
{
    KMetronome* m = qobject_cast<KMetronome*>(parent());
    m->play();
}

void SequencerAdapter::midi_stop() 
{
    KMetronome* m = qobject_cast<KMetronome*>(parent());
    m->stop();
}

void SequencerAdapter::midi_cont() 
{
    KMetronome* m = qobject_cast<KMetronome*>(parent());
    m->cont();
}

void SequencerAdapter::midi_notation(int numerator, int denominator) {
    KMetronome* m = qobject_cast<KMetronome*>(parent());
    m->setTimeSignature(numerator, denominator);
}

void SequencerAdapter::parse_sysex(SequencerEvent *ev) 
{
	int num, den;
	SysExEvent* syx = dynamic_cast<SysExEvent*>(ev);
	if (syx == NULL) return;
	unsigned char *ptr =(unsigned char *) syx->getData();
	if (syx->getLength() < 6) return;
	if (*ptr++ != 0xf0) return;
	int msgId = *ptr++;
	if (msgId != 0x7f) return; // Universal Real Time
	int deviceId = *ptr++;
	if (deviceId != 0x7f) return; // broadcast
	int subId1 = *ptr++;
	int subId2 = *ptr++;
	switch (subId1) {
	case 0x03: // Notation
		switch (subId2) {
		case 0x02:
		case 0x42:
			if (syx->getLength() < 9) return;
			*ptr++;
			num = *ptr++;
			den = *ptr++;
			midi_notation(num, den);
			break;
		}
		break;
	case 0x06: // MMC
		switch (subId2) {
		case 0x01: // Stop
			midi_stop();
			break;
		case 0x02: // Play
		case 0x03: // Deferred Play
			midi_play();
			break;
		}
		break;
	}
}

void SequencerAdapter::sequencerEvent(SequencerEvent *ev)
{
    switch (ev->getSequencerType()) {
    case SND_SEQ_EVENT_USR0:
        metronome_pattern(ev->getTick() + m_patternDuration);
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
    case SND_SEQ_EVENT_SYSEX:
        parse_sysex(ev);
        break;
    }
    delete ev;
}

void SequencerAdapter::metronome_start() 
{
    m_Queue->start();
	m_patternDuration = m_resolution * 4 / m_ts_div * m_ts_num;
	metronome_pattern(0);
	metronome_pattern(m_patternDuration);
	m_bar = 1;
	m_beat = 0;
	updateView();
	m_playing = true;
}

void SequencerAdapter::metronome_stop() 
{
    m_Queue->stop();
	m_playing = false;
}

void SequencerAdapter::metronome_continue() 
{
    m_Queue->continueRunning();
	m_playing = true;
}