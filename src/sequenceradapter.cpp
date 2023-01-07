/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2023 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
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
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.*
 ***************************************************************************/

#include "sequenceradapter.h"
#include "defs.h"
#include "drumgridmodel.h"
#include <drumstick/alsaqueue.h>
#include <drumstick/alsaevent.h>
#include <QStringList>

using namespace drumstick::ALSA;

SequencerAdapter::SequencerAdapter(QObject *parent) :
    QObject(parent),
    m_Client(nullptr),
    m_Port(nullptr),
    m_Queue(nullptr),
    m_model(nullptr),
    m_clientId(-1),
    m_inputPortId(-1),
    m_outputPortId(-1),
    m_queueId(-1),
    m_bar(0),
    m_beat(0),
    m_instrument(METRONOME_INSTRUMENT),
    m_bank(METRONOME_BANK),
    m_program(METRONOME_PROGRAM),
    m_weak_note(METRONOME_WEAK_NOTE),
    m_strong_note(METRONOME_STRONG_NOTE),
    m_weak_velocity(METRONOME_VELOCITY),
    m_strong_velocity(METRONOME_VELOCITY),
    m_channel(METRONOME_CHANNEL),
    m_volume(METRONOME_VOLUME),
    m_balance(METRONOME_PAN),
    m_resolution(METRONOME_RESOLUTION),
    m_bpm(TEMPO_DEFAULT),
    m_ts_num(RHYTHM_TS_NUM),
    m_ts_div(RHYTHM_TS_DEN),
    m_noteDuration(NOTE_DURATION),
    m_bankSelMethod(3),
    m_autoconnect(false),
    m_playing(false),
    m_useNoteOff(true),
    m_patternMode(false),
    m_outputConn(""),
    m_inputConn("")
{
    retranslateUi();
    m_Client = new MidiClient(this);
    m_Client->open();
    m_Client->setClientName(QSTR_APPNAME);
    m_clientId = m_Client->getClientId();

    //connect(m_Client, SIGNAL(eventReceived(SequencerEvent*)), 
    //        SLOT(sequencerEvent(SequencerEvent*)), Qt::DirectConnection);
    m_Client->setHandler(this);

    m_Port = new MidiPort(this);
    m_Port->attach( m_Client );
    m_Port->setPortName(QSTR_APPNAME);
    m_Port->setCapability(SND_SEQ_PORT_CAP_WRITE |
                          SND_SEQ_PORT_CAP_SUBS_WRITE |
                          SND_SEQ_PORT_CAP_READ |
                          SND_SEQ_PORT_CAP_SUBS_READ);
    m_Port->setPortType(SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                        SND_SEQ_PORT_TYPE_APPLICATION);

    m_inputPortId = m_outputPortId = m_Port->getPortId();
    m_Port->subscribeFromAnnounce();

    m_Queue = m_Client->createQueue(QSTR_APPNAME);
    m_queueId = m_Queue->getId();

    m_Client->setRealTimeInput(true);
    m_Client->startSequencerInput();
}

SequencerAdapter::~SequencerAdapter() 
{
    m_Client->stopSequencerInput();    
    m_Port->detach();
    m_Client->close();
}

void SequencerAdapter::retranslateUi()
{
    NO_CONNECTION = tr("No connection");
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

void SequencerAdapter::metronome_event_output(SequencerEvent* ev)
{
    ev->setSource(m_outputPortId);
    ev->setSubscribers();
    ev->setDirect();
    m_Client->outputDirect(ev);
}

void SequencerAdapter::sendControlChange(int cc, int value)
{
    ControllerEvent ev(m_channel, cc, value);
    metronome_event_output(&ev);
}

void SequencerAdapter::sendInitialControls()
{
    metronome_set_bank();
    metronome_set_program();
    metronome_set_controls();
    metronome_set_tempo();
}

inline int SequencerAdapter::calc_lsb(int x) {
    return (x % 0x80);
}

inline int SequencerAdapter::calc_msb(int x) {
    return (x / 0x80);
}

void SequencerAdapter::metronome_set_bank()
{
    int lsb, msb;
    switch (m_bankSelMethod) {
    case 0:
        lsb = calc_lsb(m_bank);
        msb = calc_msb(m_bank);
        sendControlChange(MSB_CC, msb);
        sendControlChange(LSB_CC, lsb);
        break;
    case 1:
        sendControlChange(MSB_CC, m_bank);
        break;
    case 2:
        sendControlChange(LSB_CC, m_bank);
        break;
    default: /* if method is 3 or above, do nothing */
        break;
    }
}

void SequencerAdapter::metronome_set_program() 
{
    ProgramChangeEvent ev(m_channel, m_program);
    metronome_event_output(&ev);
}

void SequencerAdapter::metronome_note_output(SequencerEvent* ev)
{
    NoteOnEvent* note = static_cast<NoteOnEvent*>(ev);
    if (note->getTag() == TAG_WEAK)
        note->setVelocity(m_weak_velocity);
    else if (note->getTag() == TAG_STRONG)
        note->setVelocity(m_strong_velocity);
    metronome_event_output(note);
}

void SequencerAdapter::metronome_schedule_event(SequencerEvent* ev, int tick)
{
    ev->setSource(m_outputPortId);
    ev->setDestination(m_clientId, m_inputPortId);
    ev->scheduleTick(m_queueId, tick, false);
    m_Client->outputDirect(ev);
}

void SequencerAdapter::metronome_note(int note, int vel, int tick, int tag)
{
    SequencerEvent* ev;
    if (m_useNoteOff) 
        ev = new NoteEvent(m_channel, note, vel, m_noteDuration);
    else
        ev = new NoteOnEvent(m_channel, note, vel);
    ev->setTag(tag);
    metronome_schedule_event(ev, tick);
	delete ev;
}

void SequencerAdapter::metronome_echo(int tick, int ev_type) 
{
    SystemEvent ev(ev_type);
    metronome_schedule_event(&ev, tick);
}

void SequencerAdapter::metronome_simple_pattern(int tick) 
{
	int j, t, duration, tag, note;
	t = tick;
	duration = m_resolution * 4 / m_ts_div;
	for (j = 0; j < m_ts_num; j++) {
	    note= ( j ? m_weak_note : m_strong_note );
	    tag = ( j ? TAG_WEAK : TAG_STRONG );
	    metronome_note(note, METRONOME_VELOCITY, t, tag);
		metronome_echo(t, SND_SEQ_EVENT_USR1);
		t += duration;
	}
	metronome_echo(t, SND_SEQ_EVENT_USR0);
}

int SequencerAdapter::decodeVelocity(const QString drumVel)
{
    const qreal f = 127.0 / 9.0;
    int num = 0;
    bool isNum = false;
    if (drumVel.isEmpty())
        return 0;
    if (drumVel == "f")
        return m_strong_velocity;
    else if (drumVel == "p")
        return m_weak_velocity;
    num = drumVel.toInt(&isNum);
    if (isNum)
        return qRound(f * num);
    return 0;
}

int SequencerAdapter::decodeTag(const QString drumVel)
{
    if (drumVel == "f")
        return TAG_STRONG;
    else if (drumVel == "p")
        return TAG_WEAK;
    return TAG_FIXED;
}

void SequencerAdapter::metronome_grid_pattern(int tick)
{
    int i, j, t, duration, key, vel, tag;
    t = tick;
    duration = m_resolution * 4 / m_model->patternFigure();
    for(i=0; i<m_model->columnCount(); ++i) {
        for(j=0; j<m_model->rowCount(); ++j) {
            QString n = m_model->patternHit(j, i);
            if (!n.isEmpty()) {
                key = m_model->patternKey(j).toInt();
                vel = decodeVelocity(n);
                tag = decodeTag(n);
                metronome_note(key, vel, t, tag);
            }
        }
        metronome_echo(t, SND_SEQ_EVENT_USR1);
        t += duration;
    }
    metronome_echo(t, SND_SEQ_EVENT_USR0);
}

void SequencerAdapter::metronome_set_tempo() 
{
    QueueTempo t = m_Queue->getTempo();
    t.setPPQ(m_resolution);
    t.setNominalBPM(m_bpm);
	m_Queue->setTempo(t);
	m_Client->drainOutput();
}

void SequencerAdapter::metronome_set_controls()
{
    sendControlChange(VOLUME_CC, m_volume);
    sendControlChange(PAN_CC, m_balance);
}

void SequencerAdapter::parse_sysex(SequencerEvent *ev) 
{
	int num, den;
	SysExEvent* syx = static_cast<SysExEvent*>(ev);
	if (syx == nullptr)
	    return;
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
			(void) *ptr++;
			num = *ptr++;
			den = *ptr++;
			emit signalNotation(num, den);
			break;
		}
		break;
	case 0x06: // MMC
		switch (subId2) {
		case 0x01: // Stop
			emit signalStop();
			break;
		case 0x02: // Play
		case 0x03: // Deferred Play
			emit signalPlay();
			break;
		}
		break;
	}
}

void SequencerAdapter::handleSequencerEvent(SequencerEvent *ev)
{
    int when = 0;
    switch (ev->getSequencerType()) {
    case SND_SEQ_EVENT_USR0:
        when = ev->getTick() + m_patternDuration;
        if (m_patternMode)
            metronome_grid_pattern(when);
        else
            metronome_simple_pattern(when);
        m_bar++;
        m_beat = 0;
        break;
    case SND_SEQ_EVENT_USR1:
        m_beat++;
        emit signalUpdate(m_bar, m_beat);
        break;
    case SND_SEQ_EVENT_START:
        emit signalPlay();
        break;
    case SND_SEQ_EVENT_CONTINUE:
    	emit signalCont();
        break;
    case SND_SEQ_EVENT_STOP:
    	emit signalStop();
        break;
    case SND_SEQ_EVENT_SYSEX:
        parse_sysex(ev);
        break;
    case SND_SEQ_EVENT_NOTEON:
        metronome_note_output(ev);
        break;
    case SND_SEQ_EVENT_NOTEOFF:
        metronome_event_output(ev);
        break;
    }
    delete ev;
}

void SequencerAdapter::metronome_start() 
{
    m_Queue->start();
	if (m_patternMode) {
        m_patternDuration = m_resolution * 4 / m_model->patternFigure() * m_model->columnCount();
        metronome_grid_pattern(0);
        metronome_grid_pattern(m_patternDuration);
	} else {
        m_patternDuration = m_resolution * 4 / m_ts_div * m_ts_num;
        metronome_simple_pattern(0);
        metronome_simple_pattern(m_patternDuration);
	}
	m_bar = 1;
	m_beat = 0;
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
