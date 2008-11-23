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

#ifndef _KMETROPREF_H_
#define _KMETROPREF_H_

#include <kdialog.h>
#include <kcombobox.h>
#include <knuminput.h>
#include "ui_kmetropreferencesbase.h"

class KMetroPreferences : public QDialog, Ui::KMetroPreferencesBase
{
    Q_OBJECT

public:
    KMetroPreferences(QWidget *parent = 0) 
        : QDialog(parent), 
          Ui::KMetroPreferencesBase() 
    { 
        setupUi(this); 
    }
    
    virtual ~KMetroPreferences() {}
    void fillInputConnections(QStringList lst) { m_in_connection->insertItems(0, lst); }
    void fillOutputConnections(QStringList lst) { m_out_connection->insertItems(0, lst); }
    
    bool getAutoConnect() { return m_autoconn->isChecked(); }
    QString getOutputConnection() { return m_out_connection->currentText(); }
    QString getInputConnection() { return m_in_connection->currentText(); }
    int getChannel() { return m_channel->value(); }
    int getProgram() { return m_program->value(); }
    int getResolution() { return m_resolution->value(); }
    int getWeakNote() { return m_weak_note->value(); }
    int getStrongNote() { return m_strong_note->value(); }
    int getDuration() { return m_duration->value(); }
    bool getSendNoteOff() { return m_use_noteoff->isChecked(); } 
    
    void setAutoConnect(bool newValue) { m_autoconn->setChecked(newValue); }
    void setOutputConnection(QString newValue);
    void setInputConnection(QString newValue);
    void setChannel(int newValue) { m_channel->setValue(newValue); }
    void setProgram(int newValue) { m_program->setValue(newValue); }
    void setResolution(int newValue) { m_resolution->setValue(newValue); }
    void setWeakNote(int newValue) { m_weak_note->setValue(newValue); }
    void setStrongNote(int newValue) { m_strong_note->setValue(newValue); }
    void setDuration(int newValue) { m_duration->setValue(newValue); }
    void setSendNoteOff(bool newValue) { m_use_noteoff->setChecked(newValue); }
};

#endif // _KMETROPREF_H_
