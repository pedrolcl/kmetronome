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
#ifndef SEQUENCERTHREAD_H
#define SEQUENCERTHREAD_H

#include <qthread.h>
#include <qevent.h>
#include <alsa/asoundlib.h>

#define METRONOME_EVENT_TYPE (QEvent::User + 1)
#define TRANSPORT_EVENT_TYPE (QEvent::User + 2)

#define METRONOME_CHANNEL 9
#define METRONOME_STRONG_NOTE 34
#define METRONOME_WEAK_NOTE 33
#define METRONOME_VELOCITY 100
#define METRONOME_PROGRAM 0
#define METRONOME_RESOLUTION 120

#define TRANSPORT_PLAY 0
#define TRANSPORT_STOP 1
#define TRANSPORT_CONT 2

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
    void setRithmNumerator(int newValue) { m_ts_num = newValue; }
    void setRithmDenominator(int newValue) { m_ts_div = newValue; }
    void setAutoConnect(bool newValue) { m_autoconnect = newValue; }
    void setOutputConn(QString newValue) { m_outputConn = newValue; }
    int getWeakNote() { return m_weak_note; }
    int getStrongNote() { return m_strong_note; }
    int getVelocity() { return m_velocity; }
    int getProgram() { return m_program; }
    int getChannel() { return m_channel; }
    int getResolution() { return m_resolution; }
    int getBpm() { return m_bpm; }
    int getRithmNumerator() { return m_ts_num; }
    int getRithmDenominator() { return m_ts_div; }
    bool getAutoConnect() { return m_autoconnect; }
    QString getOutputConn() { return m_outputConn; }

    virtual void run();    
    void metronome_start();
    void metronome_stop();
    void metronome_continue();
    void metronome_set_program();
    void metronome_set_tempo();
    void metronome_set_rithm();
    QStringList * list_ports();
    void connect_output();
    void disconnect_output();
    
private:
    int checkAlsaError(int rc, const char *message);
    void updateView();
    void midi_play();
    void midi_stop();
    void midi_cont();
    void metronome_note(unsigned char note, unsigned int tick);
    void metronome_echo(unsigned int tick, int ev_type);
    void metronome_pattern(unsigned int tick);

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
    bool m_autoconnect;
    QString m_outputConn;
};

#endif
