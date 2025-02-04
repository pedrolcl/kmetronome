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

#ifndef ABOUT_H
#define ABOUT_H

#include "ui_about.h"
#include <QDialog>

class About : public QDialog
{
    Q_OBJECT

public:
    About(QWidget *parent = nullptr);
    void retranslateUi();

private:
    Ui::AboutClass ui;
};

#endif // ABOUT_H
