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

#ifndef __KMETROIFACE_H__
#define __KMETROIFACE_H__
 
#include <dcopobject.h>
#include <qstring.h>
 
class KMetroIface : virtual public DCOPObject
{
    K_DCOP
public:	
	
k_dcop:
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void cont() = 0;
    virtual int setTempo(int newTempo) = 0;
    virtual int setTimeSignature(int numerator, int denominator) = 0;
};
 
#endif
