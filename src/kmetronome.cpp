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

#include "kmetronome.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <qlabel.h>
#include <qevent.h>
#include <kapplication.h>
#include <kmainwindow.h>
#include <klocale.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kconfig.h>

#include "kmetropreferences.h"
#include "sequencerthread.h"
#include "defs.h"

KMetronome::KMetronome()
    : DCOPObject ( "transport" ), KMainWindow( 0, "KMetronome" )
{
    m_view = new KmetronomeView(this, "kmetronomeview");
    m_thread = new SequencerThread(this);
    setCentralWidget(m_view);
    setupActions();
    setAutoSaveSettings();
    if (!m_thread->running()) {
        m_thread->start();
    }
    readConfiguration();
}

KMetronome::~KMetronome()
{
    qWarning("KMetronome Destroyed");
    if (m_thread->running()) {
        m_thread->terminate();
	m_thread->wait();
    }
    delete m_thread;
    delete m_view;
}

void KMetronome::setupActions()
{
    KStdAction::quit(kapp, SLOT(quit()), actionCollection());
    m_prefs = KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    createGUI();
}

bool KMetronome::queryExit()
{
    saveConfiguration();
    return true;
}

void KMetronome::saveConfiguration()
{
    KConfig *config = kapp->config();
    config->setGroup("Settings");
    config->writeEntry("channel", m_thread->getChannel());
    config->writeEntry("program", m_thread->getProgram());
    config->writeEntry("weakNote", m_thread->getWeakNote());
    config->writeEntry("strongNote", m_thread->getStrongNote());
    config->writeEntry("velocity", m_thread->getVelocity());
    config->writeEntry("resolution", m_thread->getResolution());
    config->writeEntry("sendNoteOff", m_thread->getSendNoteOff());
    config->writeEntry("duration", m_thread->getNoteDuration());
    config->writeEntry("tempo", m_thread->getBpm());
    config->writeEntry("rhythmNumerator", m_thread->getRhythmNumerator());
    config->writeEntry("rhythmDenominator", m_thread->getRhythmDenominator());
    config->writeEntry("autoconnect", m_thread->getAutoConnect());
    config->writeEntry("outputConn", m_thread->getOutputConn());
    config->writeEntry("inputConn", m_thread->getInputConn());
}

void KMetronome::readConfiguration()
{
	KConfig *config = kapp->config();
	config->setGroup("Settings");
	m_thread->setChannel(config->readNumEntry("channel", METRONOME_CHANNEL));
	m_thread->setProgram(config->readNumEntry("program", METRONOME_PROGRAM));
	m_thread->setWeakNote(config->readNumEntry("weakNote", METRONOME_WEAK_NOTE));
	m_thread->setStrongNote(config->readNumEntry("strongNote", METRONOME_STRONG_NOTE));
	m_thread->setVelocity(config->readNumEntry("velocity", METRONOME_VELOCITY));
	m_thread->setResolution(config->readNumEntry("resolution", METRONOME_RESOLUTION));
	int tempo = config->readNumEntry("tempo", 100);
	int ts_num = config->readNumEntry("rhythmNumerator", 4);
	int ts_div = config->readNumEntry("rhythmDenominator", 4);
	m_thread->setBpm(tempo);
	m_thread->setRhythmNumerator(ts_num);
	m_thread->setRhythmDenominator(ts_div);
	m_view->setBeatsBar(ts_num);
	m_view->setFigure(ts_div);
	m_view->setTempo(tempo);
	int duration = config->readNumEntry("duration", NOTE_DURATION);
	m_thread->setNoteDuration(duration);
	bool sendNoteOff = config->readBoolEntry("sendNoteOff", true);
	m_thread->setSendNoteOff(sendNoteOff);
	bool autoconn = config->readBoolEntry("autoconnect", false);
	m_thread->setAutoConnect(autoconn);
	if(autoconn) {
		m_thread->setOutputConn(config->readEntry("outputConn", "64:0"));
		m_thread->setInputConn(config->readEntry("inputConn", "64:0"));
		m_thread->connect_output();
		m_thread->connect_input();
	}
}

void KMetronome::optionsPreferences()
{
	KMetroPreferences dlg;
	dlg.fillOutputConnections(m_thread->outputConnections());
	dlg.fillInputConnections(m_thread->inputConnections());
	dlg.setAutoConnect(m_thread->getAutoConnect());
	QString conn = m_thread->getOutputConn();
	if (conn != NULL && !conn.isEmpty())
		dlg.setOutputConnection(conn);
	conn = m_thread->getInputConn();
	if (conn != NULL && !conn.isEmpty())
		dlg.setInputConnection(conn);
	dlg.setChannel(m_thread->getChannel());
	dlg.setProgram(m_thread->getProgram());
	dlg.setResolution(m_thread->getResolution());
	dlg.setWeakNote(m_thread->getWeakNote());
	dlg.setStrongNote(m_thread->getStrongNote());
	dlg.setVelocity(m_thread->getVelocity());
	dlg.setSendNoteOff(m_thread->getSendNoteOff());
	dlg.setDuration(m_thread->getNoteDuration());
	if (dlg.exec())
	{
		m_thread->disconnect_output();
		m_thread->disconnect_input();
		m_thread->setAutoConnect(dlg.getAutoConnect());
		m_thread->setOutputConn(dlg.getOutputConnection());
		m_thread->setInputConn(dlg.getInputConnection());
		m_thread->setChannel(dlg.getChannel());
		m_thread->setProgram(dlg.getProgram());
		m_thread->setResolution(dlg.getResolution());
		m_thread->setWeakNote(dlg.getWeakNote());
		m_thread->setStrongNote(dlg.getStrongNote());
		m_thread->setVelocity(dlg.getVelocity());
		m_thread->setSendNoteOff(dlg.getSendNoteOff());
		m_thread->setNoteDuration(dlg.getDuration());
		m_thread->connect_output();
		m_thread->connect_input();
		m_thread->metronome_set_tempo();
	} 
}

void KMetronome::play()
{
    m_view->enableControls(false);
    m_prefs->setEnabled(false);
    m_thread->metronome_start();
}

void KMetronome::stop()
{
    m_thread->metronome_stop();
    m_view->enableControls(true);
    m_prefs->setEnabled(true);
}

void KMetronome::cont()
{
    m_view->enableControls(false);
    m_prefs->setEnabled(false); 
    m_thread->metronome_continue();
}

void KMetronome::processMetronomeEvent( QCustomEvent * e )
{
	MetronomeEvent *me = dynamic_cast<MetronomeEvent *>(e);
	m_view->display(me->bar(), me->beat());
}

void KMetronome::processTransportEvent( QCustomEvent * e )
{
	TransportEvent *te = dynamic_cast<TransportEvent *>(e);
	switch(te->getAction()) {
	case TRANSPORT_PLAY:
		play();
		break;
	case TRANSPORT_STOP:
		stop();
		break;
	case TRANSPORT_CONT:
		cont();
		break;
	default:
		break;
	}
}

void KMetronome::processNotationEvent( QCustomEvent * e )
{
    NotationEvent *ne = dynamic_cast<NotationEvent *>(e);
    setTimeSignature(ne->getNumerator(), ne->getDenominator());
}

void KMetronome::customEvent( QCustomEvent * e )
{
	switch (e->type()) {
	case METRONOME_EVENT_TYPE:
		processMetronomeEvent(e);
		break;
	case TRANSPORT_EVENT_TYPE:
		processTransportEvent(e);
		break;
	case NOTATION_EVENT_TYPE:
		processNotationEvent(e);
		break;
	default:
		break;
	}
}

void KMetronome::tempoChanged(int newTempo)
{
    m_thread->setBpm(newTempo);
    m_thread->metronome_set_tempo();
}

void KMetronome::beatsBarChanged(int beats)
{
    m_thread->setRhythmNumerator(beats);
}

void KMetronome::rhythmFigureChanged(int figure)
{
    m_thread->setRhythmDenominator((int)pow(2, figure));
}

int KMetronome::setTempo(int newTempo)
{
    if (newTempo < TEMPO_MIN || newTempo > TEMPO_MAX) {
    	return -1;   
    } else {
    	m_view->setTempo(newTempo);    
    }
    return 0;
}

int KMetronome::setTimeSignature(int numerator, int denominator)
{
	static const int valids[] = {1, 2, 4, 8, 16, 32, 64};
	bool invalid = true;
	for(int i=0; i<7; ++i) {
		if (denominator == valids[i]) {
			invalid = false;
			break;
		}
	}
	if (m_thread->isPlaying() ||  numerator < 1 || numerator > 32 || invalid) {
		return -1;
	} else {
		m_view->setBeatsBar(numerator);
		m_view->setFigure(denominator);
		m_thread->setRhythmDenominator(denominator);
	}
	return 0;
}

#include "kmetronome.moc"
