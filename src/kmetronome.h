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

#ifndef _KMETRONOME_H_
#define _KMETRONOME_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qevent.h>
#include <kmainwindow.h>
#include "kmetronomeview.h"
#include "sequencerthread.h"
#include "kmetroiface.h"

/**
 * @short Application Main Window
 * @author Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>
 * @version 0.1
 */
class KMetronome : public KMainWindow, virtual public KMetroIface
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    KMetronome();

    /**
     * Default Destructor
     */
    virtual ~KMetronome();
    
    bool queryExit();
    
    /**
     * DCOP interface (KMetroIface)
     */
    int  setTempo(int newTempo);
    int  setTimeSignature(int numerator, int denominator);
    
public slots:
    void play();
    void stop();
    void cont();
    void optionsPreferences();
    void tempoChanged(int);
    void beatsBarChanged(int);
    void rhythmFigureChanged(int);
    void volumeChanged(int);
    void balanceChanged(int);

protected:
    void customEvent( QCustomEvent * e );
	
private:
    void setupAccel();
    void setupActions();
    void saveConfiguration();
    void readConfiguration();
    void processMetronomeEvent( QCustomEvent * e );
    void processTransportEvent( QCustomEvent * e );
    void processNotationEvent( QCustomEvent * e );
    
    KmetronomeView *m_view;
    SequencerThread *m_thread;
    KAction *m_prefs;
};

#endif // _KMETRONOME_H_
