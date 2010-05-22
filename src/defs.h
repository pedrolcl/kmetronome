/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2010 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
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

#include <QtCore/QString>

#define CALC_LSB(x) (x % 0x80)
#define CALC_MSB(x) (x / 0x80)

const int TEMPO_MIN(25);
const int TEMPO_MAX(250);
const int TEMPO_DEFAULT(100);
const int NOTE_DURATION(10);

const int RHYTHM_TS_NUM(4);
const int RHYTHM_TS_DEN(4);

const int METRONOME_INSTRUMENT(0);
const int METRONOME_BANK(0);
const int METRONOME_PROGRAM(0);
const int METRONOME_STRONG_NOTE(34);
const int METRONOME_WEAK_NOTE(33);

const int METRONOME_CHANNEL(9);
const int METRONOME_VELOCITY(100);
const int METRONOME_RESOLUTION(120);
const int METRONOME_VOLUME(100);
const int METRONOME_PAN(64);

const int TRANSPORT_PLAY(0);
const int TRANSPORT_STOP(1);
const int TRANSPORT_CONT(2);

const int VOLUME_CC(7);
const int PAN_CC(10);
const int MSB_CC(0);
const int LSB_CC(0x20);

const int PATTERN_FIGURE(16);
const int PATTERN_COLUMNS(16);

const QString QSTR_PATTERN("Pattern_");
const QString QSTR_FIGURE("Figure");
const QString QSTR_BEATS("Beats");

#endif /*DEFS_H*/
