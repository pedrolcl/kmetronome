/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2009 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
 *                                                                         *
 *   This program is free software); you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation); either version 2 of the License, or    *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY); without even the implied warranty of       *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program); if not, write to the Free Software          *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA 02110-1301, USA                                                    *
 ***************************************************************************/

#ifndef DEFS_H
#define DEFS_H

static const int TEMPO_MIN(25);
static const int TEMPO_MAX(250);
static const int TEMPO_DEFAULT(100);
static const int NOTE_DURATION(10);

static const int RHYTHM_TS_NUM(4);
static const int RHYTHM_TS_DEN(4);

static const int METRONOME_CHANNEL(9);
static const int METRONOME_STRONG_NOTE(34);
static const int METRONOME_WEAK_NOTE(33);
static const int METRONOME_VELOCITY(100);
static const int METRONOME_PROGRAM(0);
static const int METRONOME_RESOLUTION(120);
static const int METRONOME_VOLUME(100);
static const int METRONOME_BALANCE(64);

static const int TRANSPORT_PLAY(0);
static const int TRANSPORT_STOP(1);
static const int TRANSPORT_CONT(2);

static const int VOLUME_CC(7);
static const int BALANCE_CC(10);

#endif /*DEFS_H*/
