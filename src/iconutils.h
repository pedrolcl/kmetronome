/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2006-2024, Pedro Lopez-Cabanillas <plcl@users.sf.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ICONUTILS_H
#define ICONUTILS_H

#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QComboBox>

namespace  IconUtils
{
    void PaintPixmap(QPixmap &pixmap, const QColor& color);
    QPixmap GetPixmap(const QString& fileName);
    QIcon GetIcon(const QString& name, bool forcedTheme = false);
    void SetLabelIcon(QLabel *label, const QString& fileName);
    void SetupComboFigures(QComboBox *combo);
    void SetWindowIcon(QWidget *widget);
}

#endif // ICONUTILS_H
