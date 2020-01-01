/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2020 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
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

#include "kmetropreferences.h"
#include "iconutils.h"

KMetroPreferences::KMetroPreferences(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setWindowTitle(tr("Preferences"));
    connect( m_ui.m_instrument, SIGNAL(currentIndexChanged(int)),
             SLOT(slotInstrumentChanged(int)));
    connect( m_ui.m_bank, SIGNAL(currentIndexChanged(int)),
             SLOT(slotBankChanged(int)));
    connect( m_ui.m_program, SIGNAL(currentIndexChanged(int)),
             SLOT(slotProgramChanged(int)));
    IconUtils::SetWindowIcon(this);
}

KMetroPreferences::~KMetroPreferences()
{}

void KMetroPreferences::setOutputConnection(QString newValue) 
{
    int index = m_ui.m_out_connection->findText(newValue);
    if (index >= 0) {
        m_ui.m_out_connection->setCurrentIndex(index);
    }
}

void KMetroPreferences::setInputConnection(QString newValue) 
{ 
    int index = m_ui.m_in_connection->findText(newValue);
    if (index >= 0) {
        m_ui.m_in_connection->setCurrentIndex(index);
    }
}

void KMetroPreferences::fillInstruments(InstrumentList* instruments)
{
    m_insList = instruments;
    InstrumentList::ConstIterator it;
    for(it = m_insList->constBegin(); it != m_insList->constEnd(); ++it) {
        //kDebug() << it.key();
        m_ui.m_instrument->addItem(it.key());
    }
}

void KMetroPreferences::slotInstrumentChanged(int /*idx*/)
{
    QString name = m_ui.m_instrument->currentText();
    m_ins = m_insList->value(name);
    const InstrumentPatches& patches = m_ins.patches();
    InstrumentPatches::ConstIterator j;
    m_ui.m_bank->clear();
    m_ui.m_program->clear();
    m_ui.m_weak_note->clear();
    m_ui.m_strong_note->clear();
    for( j = patches.constBegin(); j != patches.constEnd(); ++j ) {
        InstrumentData patch = j.value();
        m_ui.m_bank->addItem(patch.name(), j.key());
    }
}

void KMetroPreferences::slotBankChanged(int idx)
{
    m_ui.m_program->clear();
    m_ui.m_weak_note->clear();
    m_ui.m_strong_note->clear();
    if (idx < 0)
        return;
    int bank = m_ui.m_bank->itemData(idx).toInt();
    const InstrumentData& patch = m_ins.patch(bank);
    InstrumentData::ConstIterator k;
    for( k = patch.constBegin(); k != patch.constEnd(); ++k ) {
        m_ui.m_program->addItem(k.value(), k.key());
    }
}

void KMetroPreferences::slotProgramChanged(int idx)
{
    if (idx < 0)
        return;
    int bank = m_ui.m_bank->itemData(m_ui.m_bank->currentIndex()).toInt();
    int patch = m_ui.m_program->itemData(idx).toInt();
    m_ui.m_weak_note->clear();
    m_ui.m_strong_note->clear();
    const InstrumentData& notes = m_ins.notes(bank, patch);
    InstrumentData::ConstIterator k;
    for( k = notes.constBegin(); k != notes.constEnd(); ++k ) {
        m_ui.m_weak_note->addItem(k.value(), k.key());
        m_ui.m_strong_note->addItem(k.value(), k.key());
    }
}

int KMetroPreferences::getWeakNote()
{
    return m_ui.m_weak_note->itemData( m_ui.m_weak_note->currentIndex() ).toInt();
}

int KMetroPreferences::getStrongNote()
{
    return m_ui.m_strong_note->itemData( m_ui.m_strong_note->currentIndex() ).toInt();
}

void KMetroPreferences::setWeakNote(int newValue)
{
    QString test = QString::number(newValue);
    int idx = m_ui.m_weak_note->findData(test);
    m_ui.m_weak_note->setCurrentIndex(idx);
}

void KMetroPreferences::setStrongNote(int newValue)
{
    QString test = QString::number(newValue);
    int idx = m_ui.m_strong_note->findData(test);
    m_ui.m_strong_note->setCurrentIndex(idx);
}

QString KMetroPreferences::getInstrumentName()
{
    return m_ui.m_instrument->currentText();
}

QString KMetroPreferences::getProgramName()
{
    return m_ui.m_program->currentText();
}

QString KMetroPreferences::getBankName()
{
    return m_ui.m_bank->currentText();
}

void KMetroPreferences::setInstrumentName(QString name)
{
    m_ui.m_instrument->setCurrentText(name);
}

void KMetroPreferences::setProgramName(QString name)
{
    m_ui.m_program->setCurrentText(name);
}

void KMetroPreferences::setBankName(QString name)
{
    m_ui.m_bank->setCurrentText(name);
}
