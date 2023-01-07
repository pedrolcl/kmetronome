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

#ifndef KMETROPREFERENCES_H
#define KMETROPREFERENCES_H

#include <QDialog>
#include "ui_kmetropreferencesbase.h"
#include "instrument.h"

class KMetroPreferences : public QDialog
{
    Q_OBJECT

public:
    KMetroPreferences(QWidget *parent = nullptr);
    virtual ~KMetroPreferences();
    void fillInputConnections(QStringList lst) { m_ui.m_in_connection->insertItems(0, lst); }
    void fillOutputConnections(QStringList lst) { m_ui.m_out_connection->insertItems(0, lst); }
    void fillInstruments(InstrumentList* instruments);
    void fillStyles();
    bool getAutoConnect() { return m_ui.m_autoconn->isChecked(); }
    QString getOutputConnection() { return m_ui.m_out_connection->currentText(); }
    QString getInputConnection() { return m_ui.m_in_connection->currentText(); }
    int getChannel() { return (m_ui.m_channel->value()-1); }
    int getResolution() { return m_ui.m_resolution->value(); }
    int getDuration() { return m_ui.m_duration->value(); }
    bool getSendNoteOff() { return m_ui.m_use_noteoff->isChecked(); }
    QString getInstrumentName();
    QString getProgramName();
    QString getBankName();
    QString getStyle();
    int getWeakNote();
    int getStrongNote();
    bool getDarkMode() { return m_ui.m_dark_mode->isChecked(); }
    bool getInternalIcons() { return m_ui.m_internal_icons->isChecked(); }

    void setAutoConnect(bool newValue) { m_ui.m_autoconn->setChecked(newValue); }
    void setOutputConnection(QString newValue);
    void setInputConnection(QString newValue);
    void setChannel(int newValue) { m_ui.m_channel->setValue(newValue+1); }
    void setResolution(int newValue) { m_ui.m_resolution->setValue(newValue); }
    void setDuration(int newValue) { m_ui.m_duration->setValue(newValue); }
    void setSendNoteOff(bool newValue) { m_ui.m_use_noteoff->setChecked(newValue); }
    void setWeakNote(int newValue);
    void setStrongNote(int newValue);
    void setInstrumentName(QString name);
    void setProgramName(QString name);
    void setBankName(QString name);
    void setDarkMode(bool mode) { m_ui.m_dark_mode->setChecked(mode); }
    void setInternalIcons(bool icons) { m_ui.m_internal_icons->setChecked(icons); }

public slots:
    void slotInstrumentChanged(int idx);
    void slotBankChanged(int idx);
    void slotProgramChanged(int idx);

private:
    Ui::KMetroPreferencesBase m_ui;
    InstrumentList* m_insList;
    Instrument m_ins;
};

#endif // KMETROPREFERENCES_H
