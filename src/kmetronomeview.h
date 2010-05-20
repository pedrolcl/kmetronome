/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2010 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
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

#ifndef KMETRONOMEVIEW_H
#define KMETRONOMEVIEW_H

#include <QMouseEvent>
#include <QStyle>
#include <kcombobox.h>
#include <knuminput.h>

#include "ui_kmetronomeviewbase.h"

class KmetronomeView : public QWidget, Ui::KmetronomeViewBase
{
    Q_OBJECT
public:
    KmetronomeView(QWidget *parent = 0);
    virtual ~KmetronomeView() {}
    void display(int, int);
    int getTempo() { return m_tempo->value(); }
    int getBeatsBar() { return m_beatsBar->value(); }
    int getFigure() { return m_figure->currentIndex(); }
    void setTempo(int newValue) { m_tempo->setValue(newValue); }
    void setBeatsBar(int newValue) { m_beatsBar->setValue(newValue); }
    void setFigure(int newValue);
    void enableControls(bool e);
    void updateKnobs(bool styled);
    void setPatterns(const QStringList& patterns);
    bool patternMode() { return m_patternMode; }
    QString getSelectedPattern();
    void setSelectedPattern(const QString& pattern);

protected:
    virtual void mouseDoubleClickEvent ( QMouseEvent * e );

public Q_SLOTS:
    void displayTempo(int);
    void displayWeakVelocity(int v) { m_dial1->setValue(v); }
    void displayStrongVelocity(int v) { m_dial2->setValue(v); }
    void displayVolume(int v) { m_dial3->setValue(v); }
    void displayBalance(int v) { m_dial4->setValue(v); }
    void tempoComboChanged(int);
    void patternChanged(int);
    void play();
    void stop();

private:
    QStyle* m_dialStyle;
    bool m_patternMode;
};

#endif
