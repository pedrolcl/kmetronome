/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005 by Pedro Lopez-Cabanillas                          *
 *   plcl@users.sourceforge.net                                            *
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

#ifndef _KMETROPREF_H_
#define _KMETROPREF_H_

#include <kdialogbase.h>
#include <qcheckbox.h>
#include <kcombobox.h>
#include <knuminput.h>
#include "kmetropreferencesbase.h"

class KMetroPreferences : public KMetroPreferencesBase
{
    Q_OBJECT
public:
    KMetroPreferences():KMetroPreferencesBase() { }
    void fillInputConnections(QStringList lst) { m_in_connection->insertStringList(lst); }
    void fillOutputConnections(QStringList lst) { m_out_connection->insertStringList(lst); }
    
    bool getAutoConnect() { return m_autoconn->isChecked(); }
    QString getOutputConnection() { return m_out_connection->currentText(); }
    QString getInputConnection() { return m_in_connection->currentText(); }
    int getChannel() { return m_channel->value(); }
    int getProgram() { return m_program->value(); }
    int getResolution() { return m_resolution->value(); }
    int getWeakNote() { return m_weak_note->value(); }
    int getStrongNote() { return m_strong_note->value(); }
    int getVelocity() { return m_velocity->value(); }
    
    void setAutoConnect(bool newValue) { m_autoconn->setChecked(newValue); }
    void setOutputConnection(QString newValue) { m_out_connection->setCurrentText(newValue); }
    void setInputConnection(QString newValue) { m_in_connection->setCurrentText(newValue); }
    void setChannel(int newValue) { m_channel->setValue(newValue); }
    void setProgram(int newValue) { m_program->setValue(newValue); }
    void setResolution(int newValue) { m_resolution->setValue(newValue); }
    void setWeakNote(int newValue) { m_weak_note->setValue(newValue); }
    void setStrongNote(int newValue) { m_strong_note->setValue(newValue); }
    void setVelocity(int newValue) { m_velocity->setValue(newValue); }
};

#endif // _KMETROPREF_H_
