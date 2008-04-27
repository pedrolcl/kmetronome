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

#include <kapplication.h>
#include <qlcdnumber.h>
#include <kpushbutton.h>
#include <qpixmap.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kinputdialog.h>

#include "kmetronomeview.h"
#include "defs.h"
#include "f1.xpm"
#include "f2.xpm"
#include "f4.xpm"
#include "f8.xpm"
#include "f16.xpm"
#include "f32.xpm"
#include "f64.xpm"

KmetronomeView::KmetronomeView(QWidget *parent, const char *name)
 : KmetronomeViewBase(parent, name)
{
    m_exitbtn->setIconSet(SmallIconSet("exit"));
    m_configbtn->setIconSet(SmallIconSet("configure"));
    m_playbtn->setIconSet(SmallIconSet("player_play"));
    m_stopbtn->setIconSet(SmallIconSet("player_stop"));
    
    connect( m_exitbtn, SIGNAL(clicked()), kapp, SLOT(quit()) );
    connect( m_configbtn, SIGNAL(clicked()),
             parent, SLOT(optionsPreferences()) );
    connect( m_playbtn, SIGNAL(clicked()), parent, SLOT(play()) );
    connect( m_stopbtn, SIGNAL(clicked()), parent, SLOT(stop()) );
    connect( m_beatsBar, SIGNAL(valueChanged(int)), parent, SLOT(beatsBarChanged(int)) );
    connect( m_figure, SIGNAL(activated(int)), parent, SLOT(rhythmFigureChanged(int)) );
    connect( m_tempo, SIGNAL(valueChanged(int)), parent, SLOT(tempoChanged(int)) );

    m_figure->insertItem(QPixmap(f1_xpm), i18n(" - Whole"));
    m_figure->insertItem(QPixmap(f2_xpm), i18n(" - Half"));
    m_figure->insertItem(QPixmap(f4_xpm), i18n(" - Quarter"));
    m_figure->insertItem(QPixmap(f8_xpm), i18n(" - Eighth"));
    m_figure->insertItem(QPixmap(f16_xpm), i18n(" - Sixteenth"));
    m_figure->insertItem(QPixmap(f32_xpm), i18n(" - Thirty-Second"));
    m_figure->insertItem(QPixmap(f64_xpm), i18n(" - Sixty-Fourth"));
    m_figure->setCurrentItem(2);
}

KmetronomeView::~KmetronomeView()
{
    qWarning("KmetronomeView Destroyed");
}

void KmetronomeView::display(int bar, int beat) 
{
    m_measureLCD->display(bar);
    m_beatLCD->display(beat);
}

void KmetronomeView::setFigure(int newValue)
{
    int ts_dd;
    int x = newValue;
    for(ts_dd = 0; x > 1; x /= 2)
	++ts_dd;
    m_figure->setCurrentItem(ts_dd);
}

void KmetronomeView::m_tempo_valueChanged(int newTempo)
{
    m_tempoLCD->display(newTempo);
}

void KmetronomeView::enableControls(bool e)
{
    m_playbtn->setEnabled(e);
    m_stopbtn->setEnabled(!e);
    m_configbtn->setEnabled(e);
    m_beatsBar->setEnabled(e);
    m_figure->setEnabled(e);
}

void KmetronomeView::mouseDoubleClickEvent(QMouseEvent *)
{
    bool ok = false;
    int newTempo = KInputDialog::getInteger( 
    					i18n("Tempo"), i18n("Enter new Tempo:"),
    					m_tempo->value(), TEMPO_MIN, TEMPO_MAX, 1, 10,
					    &ok, this );
    if (ok) {
    	m_tempo->setValue(newTempo);
    }
}

#include "kmetronomeview.moc"
