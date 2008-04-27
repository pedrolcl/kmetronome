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

#ifndef DEFS_H_
#define DEFS_H_

#define TEMPO_MIN 25
#define TEMPO_MAX 250
#define TEMPO_DEFAULT 100
#define NOTE_DURATION 10 

#define RHYTHM_TS_NUM 4
#define RHYTHM_TS_DEN 4

#define METRONOME_CHANNEL 9
#define METRONOME_STRONG_NOTE 34
#define METRONOME_WEAK_NOTE 33
#define METRONOME_VELOCITY 100
#define METRONOME_PROGRAM 0
#define METRONOME_RESOLUTION 120

#define TRANSPORT_PLAY 0
#define TRANSPORT_STOP 1
#define TRANSPORT_CONT 2

#define VOLUME_CC  7
#define BALANCE_CC 10

#endif /*DEFS_H_*/
