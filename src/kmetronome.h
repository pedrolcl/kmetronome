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

#ifndef KMETRONOME_H
#define KMETRONOME_H

#include <KDE/KXmlGuiWindow>
#include <QtCore/QPointer>

class KAction;
class KToggleAction;
class KmetronomeView;
class SequencerAdapter;
class DrumGrid;

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
    void editPatterns();
    void patternChanged(int);
    void updatePatterns();

protected Q_SLOTS:
    void optionsPreferences();
    void tempoChanged(int);
    void beatsBarChanged(int);
    void rhythmFigureChanged(int);
    void weakVeloChanged(int);
    void strongVeloChanged(int);
    void volumeChanged(int);
    void balanceChanged(int);
    void toggle(bool checked);

private:
    void setupAccel();
    void setupActions();
    void saveConfiguration();
    void readConfiguration();

    bool m_styledKnobs;
    KmetronomeView *m_view;
    SequencerAdapter *m_seq;
    QPointer<DrumGrid> m_drumgrid;

    KAction *m_prefs;
    KToggleAction* m_playStop;
    QString m_instrument;
    QString m_bank;
    QString m_program;
};

#endif // KMETRONOME_H
