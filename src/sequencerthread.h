/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2008 Pedro Lopez-Cabanillas                        *
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
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA 02110-1301, USA                                                    *
 ***************************************************************************/

#ifndef SEQUENCERTHREAD_H
#define SEQUENCERTHREAD_H

#include <qthread.h>
#include <qevent.h>
#include <klocale.h>
#include <alsa/asoundlib.h>

#define METRONOME_EVENT_TYPE (QEvent::User + 1)
#define TRANSPORT_EVENT_TYPE (QEvent::User + 2)
#define NOTATION_EVENT_TYPE  (QEvent::User + 3)

class MetronomeEvent : public QCustomEvent
{
public:
     MetronomeEvent( int bar, int beat )
	: QCustomEvent( METRONOME_EVENT_TYPE ), m_bar(bar), m_beat(beat) {}
     int bar() const { return m_bar; }
     int beat() const { return m_beat; }
private:
     int m_bar;
     int m_beat;
};

class TransportEvent : public QCustomEvent
{
public:
    TransportEvent(int action)
	: QCustomEvent ( TRANSPORT_EVENT_TYPE ), m_action(action) {}
    int getAction() const { return m_action; }
private:
    int m_action;
};

class NotationEvent : public QCustomEvent
{
public:
     NotationEvent( int numerator, int denominator )
    : QCustomEvent( NOTATION_EVENT_TYPE ), m_numerator(numerator), m_denominator(denominator) {}
     int getNumerator() const { return m_numerator; }
     int getDenominator() const { return m_denominator; }
private:
     int m_numerator;
     int m_denominator;
};

class SequencerThread : public QThread
{
public:
    SequencerThread(QWidget *parent);

    ~SequencerThread();

    void setWeakNote(int newValue) { m_weak_note = newValue; }
    void setStrongNote(int newValue) { m_strong_note = newValue; }
    void setVelocity(int newValue) { m_velocity = newValue; }
    void setProgram(int newValue) { m_program = newValue; }
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
    int getVelocity() { return m_velocity; }
    int getProgram() { return m_program; }
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
    void sendControlChange(int cc, int value);
    
    virtual void run();    
    void metronome_start();
    void metronome_stop();
    void metronome_continue();
    void metronome_set_program();
    void metronome_set_tempo();
    void metronome_set_rhythm();
    void connect_output();
    void disconnect_output();
    void connect_input();
    void disconnect_input();
    QStringList inputConnections();
    QStringList outputConnections();
    
private:
    int checkAlsaError(int rc, const char *message);
    void updateView();
    void midi_play();
    void midi_stop();
    void midi_cont();
    void midi_notation(int numerator, int denominator);
    void parse_sysex(snd_seq_event_t *ev);
    void metronome_note(unsigned char note, unsigned int tick);
    void metronome_echo(unsigned int tick, int ev_type);
    void metronome_pattern(unsigned int tick);
    QStringList list_ports(unsigned int mask);

    QWidget *m_widget;
    snd_seq_t *m_handle;
    int m_client;
    int m_input;
    int m_output;
    int m_queue;

    int m_bar;
    int m_beat;
    int m_weak_note;
    int m_strong_note;
    int m_velocity;
    int m_program;
    int m_channel;
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
