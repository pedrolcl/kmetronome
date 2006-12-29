/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2006 Pedro Lopez-Cabanillas                        *
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

#ifndef KMETRONOMEVIEW_H
#define KMETRONOMEVIEW_H

#include <kcombobox.h>
#include <knuminput.h>
#include <qslider.h>

#include "kmetronomeviewbase.h"

/**
Central View Widget Implementation
@author Pedro Lopez-Cabanillas
*/
class KmetronomeView : public KmetronomeViewBase
{
Q_OBJECT
public:
    KmetronomeView(QWidget *parent = 0, const char *name = 0);
    ~KmetronomeView();
    void display(int, int);
    int getTempo() { return m_tempo->value(); }
    int getBeatsBar() { return m_beatsBar->value(); }
    int getFigure() { return m_figure->currentItem(); }
    void setTempo(int newValue) { m_tempo->setValue(newValue); }
    void setBeatsBar(int newValue) { m_beatsBar->setValue(newValue); }
    void setFigure(int newValue);
    void enableControls(bool);

protected:
    virtual void mouseDoubleClickEvent ( QMouseEvent * e );
        
public slots:
    virtual void m_tempo_valueChanged(int);
};

#endif
