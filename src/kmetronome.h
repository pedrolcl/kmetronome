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

#ifndef _KMETRONOME_H_
#define _KMETRONOME_H_

#include <kaction.h>
#include <kxmlguiwindow.h>
#include "kmetronomeview.h"
#include "sequenceradapter.h"

class KMetronome : public KXmlGuiWindow
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.sourceforge.kmetronome")

public:
    KMetronome(QWidget* parent=0);
    virtual ~KMetronome() {}
    bool queryExit();

public Q_SLOTS:
    void play();
    void stop();
    void cont();
    void setTempo(int newTempo);
    void setTimeSignature(int numerator, int denominator);
    void updateDisplay(int, int);

protected Q_SLOTS:
    void optionsPreferences();
    void tempoChanged(int);
    void beatsBarChanged(int);
    void rhythmFigureChanged(int);
    void weakVeloChanged(int);
    void strongVeloChanged(int);
    void volumeChanged(int);
    void balanceChanged(int);

private:
    void setupAccel();
    void setupActions();
    void saveConfiguration();
    void readConfiguration();

    KmetronomeView *m_view;
    KAction *m_prefs;
    SequencerAdapter *m_seq;
    bool m_styledKnobs;
};

#endif // _KMETRONOME_H_
