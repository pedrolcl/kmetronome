/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2009 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
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

#ifndef SEQUENCERADAPTER_H
#define SEQUENCERADAPTER_H

#include <client.h>
#include <event.h>

using namespace ALSA::Sequencer;

class SequencerAdapter : public QObject, public SequencerEventHandler
{
    Q_OBJECT

public:
    SequencerAdapter(QObject *parent);
    virtual ~SequencerAdapter();

    void setWeakNote(int newValue) { m_weak_note = newValue; }
    void setStrongNote(int newValue) { m_strong_note = newValue; }
    void setWeakVelocity(int newValue) { m_weak_velocity = newValue; }
    void setStrongVelocity(int newValue) { m_strong_velocity = newValue; }
    void setProgram(int newValue) { m_program = newValue; }
    void setVolume(int newValue) { m_volume = newValue; }
    void setBalance(int newValue) { m_balance = newValue; }
    void setChannel(int newValue) { m_channel = newValue; }
    void setResolution(int newValue) { m_resolution = newValue; }
    void setBpm(int newValue) { m_bpm = newValue; }
    void setRhythmNumerator(int newValue) { m_ts_num = newValue; }
    void setRhythmDenominator(int newValue) { m_ts_div = newValue; }
    void setAutoConnect(bool newValue) { m_autoconnect = newValue; }
    void setOutputConn(QString newValue) { m_outputConn = newValue; }
    void setInputConn(QString newValue) { m_inputConn = newValue; }
    void setNoteDuration(int newValue) { m_noteDuration = newValue; }
    void setSendNoteOff(bool newValue) { m_useNoteOff = newValue; }
    int getWeakNote() { return m_weak_note; }
    int getStrongNote() { return m_strong_note; }
    int getWeakVelocity() { return m_weak_velocity; }
    int getStrongVelocity() { return m_strong_velocity; }
    int getProgram() { return m_program; }
    int getVolume() { return m_volume; }
    int getBalance() { return m_balance; }
    int getChannel() { return m_channel; }
    int getResolution() { return m_resolution; }
    int getBpm() { return m_bpm; }
    int getRhythmNumerator() { return m_ts_num; }
    int getRhythmDenominator() { return m_ts_div; }
    bool getAutoConnect() { return m_autoconnect; }
    bool isPlaying() { return m_playing; }
    QString getOutputConn() { return m_outputConn; }
    QString getInputConn() { return m_inputConn; }
    int getNoteDuration() { return m_noteDuration; }
    bool getSendNoteOff() { return m_useNoteOff; }
    void sendControlChange( int cc, int value );
    void sendInitialControls();
    
    void metronome_start();
    void metronome_stop();
    void metronome_continue();
    void metronome_set_program();
    void metronome_set_tempo();
    void metronome_set_rhythm();
    void metronome_set_controls();
    void connect_output();
    void disconnect_output();
    void connect_input();
    void disconnect_input();
    QStringList inputConnections();
    QStringList outputConnections();

//public Q_SLOTS:    
//    void sequencerEvent(SequencerEvent *ev);

// SequencerEventHandler method
    void handleSequencerEvent(SequencerEvent *ev);

signals:
    void signalUpdate(int,int);
    void signalPlay();
    void signalStop();
    void signalCont();
    void signalNotation(int,int);
    
private:
    void parse_sysex(SequencerEvent *ev);
    void metronome_note(int note, int vel, int tick);
    void metronome_echo(int tick, int ev_type);
    void metronome_pattern(int tick);
    void metronome_event_output(SequencerEvent* ev);
    void metronome_note_output(SequencerEvent* ev);
    void metronome_schedule_event(SequencerEvent* ev, int tick);

    MidiClient* m_Client;
    MidiPort* m_Port;
    MidiQueue* m_Queue;
    int m_clientId;
    int m_inputPortId;
    int m_outputPortId;
    int m_queueId;

    int m_bar;
    int m_beat;
    int m_weak_note;
    int m_strong_note;
    int m_weak_velocity;
    int m_strong_velocity;
    int m_program;
    int m_channel;
    int m_volume;
    int m_balance;
    int m_resolution;
    int m_bpm;
    int m_ts_num; /* time signature: numerator */
    int m_ts_div; /* time signature: denominator */
    int m_noteDuration;
    int m_patternDuration;
    bool m_autoconnect;
    bool m_playing;
    bool m_useNoteOff;
   
    QString m_outputConn;
    QString m_inputConn;
    QString NO_CONNECTION;
};

#endif
