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

#include <cmath>
#include <QLabel>
#include <QEvent>
#include <QDBusConnection>

#include <kapplication.h>
#include <kmainwindow.h>
#include <klocale.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kconfig.h>
#include <kglobal.h>

#include "kmetronome.h"
#include "kmetropreferences.h"
#include "sequenceradapter.h"
#include "kmetronomeadaptor.h"
#include "defs.h"

KMetronome::KMetronome(QWidget *parent) : KXmlGuiWindow(parent)
{
    new KmetronomeAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/", this);
    
    m_view = new KmetronomeView(this);
    m_seq = new SequencerAdapter(this);
    setCentralWidget(m_view);
    setupActions();
    setAutoSaveSettings();
    readConfiguration();
}

void KMetronome::setupActions()
{
    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
    m_prefs = KStandardAction::preferences(this, SLOT(optionsPreferences()), 
                                           actionCollection());
    createGUI();
}

bool KMetronome::queryExit()
{
    saveConfiguration();
    return true;
}

void KMetronome::saveConfiguration()
{
    KConfigGroup config = KGlobal::config()->group("Settings");
    config.writeEntry("channel", m_seq->getChannel());
    config.writeEntry("program", m_seq->getProgram());
    config.writeEntry("weakNote", m_seq->getWeakNote());
    config.writeEntry("strongNote", m_seq->getStrongNote());
    config.writeEntry("weakVelocity", m_seq->getWeakVelocity());
    config.writeEntry("strongVelocity", m_seq->getStrongVelocity());
    config.writeEntry("volume", m_seq->getVolume());
    config.writeEntry("balance", m_seq->getBalance());
    config.writeEntry("resolution", m_seq->getResolution());
    config.writeEntry("sendNoteOff", m_seq->getSendNoteOff());
    config.writeEntry("duration", m_seq->getNoteDuration());
    config.writeEntry("tempo", m_seq->getBpm());
    config.writeEntry("rhythmNumerator", m_seq->getRhythmNumerator());
    config.writeEntry("rhythmDenominator", m_seq->getRhythmDenominator());
    config.writeEntry("autoconnect", m_seq->getAutoConnect());
    config.writeEntry("outputConn", m_seq->getOutputConn());
    config.writeEntry("inputConn", m_seq->getInputConn());
    config.sync();
}

void KMetronome::readConfiguration()
{
	KConfigGroup config = KGlobal::config()->group("Settings");
	m_seq->setChannel(config.readEntry("channel", METRONOME_CHANNEL));
	m_seq->setProgram(config.readEntry("program", METRONOME_PROGRAM));
	m_seq->setWeakNote(config.readEntry("weakNote", METRONOME_WEAK_NOTE));
	m_seq->setStrongNote(config.readEntry("strongNote", METRONOME_STRONG_NOTE));
	m_seq->setResolution(config.readEntry("resolution", METRONOME_RESOLUTION));
	int volume = config.readEntry("volume", METRONOME_VOLUME);
	int balance = config.readEntry("balance", METRONOME_BALANCE);
    int weakVel = config.readEntry("weakVelocity", METRONOME_VELOCITY);
    int strongVel = config.readEntry("strongVelocity", METRONOME_VELOCITY);
	int tempo = config.readEntry("tempo", 100);
	int ts_num = config.readEntry("rhythmNumerator", 4);
	int ts_div = config.readEntry("rhythmDenominator", 4);
	m_seq->setVolume(volume);
    m_seq->setBalance(balance);
	m_seq->setWeakVelocity(weakVel);
    m_seq->setStrongVelocity(strongVel);
	m_seq->setBpm(tempo);
	m_seq->setRhythmNumerator(ts_num);
	m_seq->setRhythmDenominator(ts_div);
	m_view->setBeatsBar(ts_num);
	m_view->setFigure(ts_div);
	m_view->setTempo(tempo);
	m_view->displayWeakVelocity(weakVel);
	m_view->displayStrongVelocity(strongVel);
	m_view->displayVolume(volume);
	m_view->displayBalance(balance);
	int duration = config.readEntry("duration", NOTE_DURATION);
	m_seq->setNoteDuration(duration);
	bool sendNoteOff = config.readEntry("sendNoteOff", true);
	m_seq->setSendNoteOff(sendNoteOff);
	bool autoconn = config.readEntry("autoconnect", false);
	m_seq->setAutoConnect(autoconn);
	if(autoconn) {
		m_seq->setOutputConn(config.readEntry("outputConn", "20:0"));
		m_seq->setInputConn(config.readEntry("inputConn", "20:0"));
		m_seq->connect_output();
		m_seq->connect_input();
	}
	m_seq->sendInitialControls();
}

void KMetronome::optionsPreferences()
{
	KMetroPreferences dlg;
	dlg.fillOutputConnections(m_seq->outputConnections());
	dlg.fillInputConnections(m_seq->inputConnections());
	dlg.setAutoConnect(m_seq->getAutoConnect());
	QString conn = m_seq->getOutputConn();
	if (conn != NULL && !conn.isEmpty())
		dlg.setOutputConnection(conn);
	conn = m_seq->getInputConn();
	if (conn != NULL && !conn.isEmpty())
		dlg.setInputConnection(conn);
	dlg.setChannel(m_seq->getChannel());
	dlg.setProgram(m_seq->getProgram());
	dlg.setResolution(m_seq->getResolution());
	dlg.setWeakNote(m_seq->getWeakNote());
	dlg.setStrongNote(m_seq->getStrongNote());
	dlg.setSendNoteOff(m_seq->getSendNoteOff());
	dlg.setDuration(m_seq->getNoteDuration());
	if (dlg.exec())
	{
		m_seq->disconnect_output();
		m_seq->disconnect_input();
		m_seq->setAutoConnect(dlg.getAutoConnect());
		m_seq->setOutputConn(dlg.getOutputConnection());
		m_seq->setInputConn(dlg.getInputConnection());
		m_seq->setChannel(dlg.getChannel());
		m_seq->setProgram(dlg.getProgram());
		m_seq->setResolution(dlg.getResolution());
		m_seq->setWeakNote(dlg.getWeakNote());
		m_seq->setStrongNote(dlg.getStrongNote());
		m_seq->setSendNoteOff(dlg.getSendNoteOff());
		m_seq->setNoteDuration(dlg.getDuration());
		m_seq->connect_output();
		m_seq->connect_input();
		m_seq->metronome_set_tempo();
	} 
}

void KMetronome::updateDisplay(int bar, int beat)
{
    m_view->display(bar, beat);
}

void KMetronome::tempoChanged(int newTempo)
{
    m_seq->setBpm(newTempo);
    m_seq->metronome_set_tempo();
}

void KMetronome::beatsBarChanged(int beats)
{
    m_seq->setRhythmNumerator(beats);
}

void KMetronome::rhythmFigureChanged(int figure)
{
    m_seq->setRhythmDenominator((int)pow(2, figure));
}

void KMetronome::weakVeloChanged(int vel)
{
    m_seq->setWeakVelocity(vel);
}

void KMetronome::strongVeloChanged(int vel)
{
    m_seq->setStrongVelocity(vel);
}

void KMetronome::volumeChanged(int vol)
{
    m_seq->setVolume(vol);
	m_seq->sendControlChange(VOLUME_CC, vol);
}

void KMetronome::balanceChanged(int bal)
{
    m_seq->setBalance(bal);
	m_seq->sendControlChange(BALANCE_CC, bal);
}

/**
 * Public D-Bus Interface Slots
 */

void KMetronome::play()
{
    m_view->enableControls(false);
    m_prefs->setEnabled(false);
    m_seq->metronome_start();
}

void KMetronome::stop()
{
    m_seq->metronome_stop();
    m_view->enableControls(true);
    m_prefs->setEnabled(true);
}

void KMetronome::cont()
{
    m_view->enableControls(false);
    m_prefs->setEnabled(false); 
    m_seq->metronome_continue();
}

void KMetronome::setTempo(int newTempo)
{
    if (newTempo < TEMPO_MIN || newTempo > TEMPO_MAX)
        return;
    m_view->setTempo(newTempo);    
}

void KMetronome::setTimeSignature(int numerator, int denominator)
{
    static const int valids[] = {1, 2, 4, 8, 16, 32, 64};
    bool invalid = true;
    for(int i=0; i<7; ++i) {
        if (denominator == valids[i]) {
            invalid = false;
            break;
        }
    }
    if (m_seq->isPlaying() ||  numerator < 1 || numerator > 32 || invalid)
        return;
    
    m_view->setBeatsBar(numerator);
    m_view->setFigure(denominator);
    m_seq->setRhythmDenominator(denominator);
}

