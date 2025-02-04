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

#include <drumstick/alsaclient.h>
#include "about.h"
#include "iconutils.h"

const QString PGM_VERSION(QT_STRINGIFY(VERSION));
const QString BLD_DATE(__DATE__);
const QString BLD_TIME(__TIME__);
#if defined(Q_CC_GNU) || defined(Q_CC_GCCE)
const QString CMP_VERSION(__VERSION__);
#elif defined(Q_CC_MSVC)
const QString CMP_VERSION(QString::number(_MSC_VER/100.0,'g',2));
#else
const QString CMP_VERSION(QString());
#endif

About::About(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    retranslateUi();
    IconUtils::SetLabelIcon(ui.labelIcon, ":/icons/midi/icon128.png");
    IconUtils::SetWindowIcon(this);
#if defined(SMALL_SCREEN)
    setWindowState(Qt::WindowActive | Qt::WindowMaximized);
#else
    setMinimumSize(550,580);
#endif
}

void About::retranslateUi()
{
    QString strver = PGM_VERSION;
#ifdef REVISION
    strver.append(" r");
    strver.append(QT_STRINGIFY(REVISION));
#endif

    ui.retranslateUi(this);
    ui.labelVersion->setText(tr("<html>"
            "<head>"
              "<meta name=\"qrichtext\" content=\"1\" />"
              "<style type=\"text/css\">\np, li { white-space: pre-wrap; }</style>"
            "</head>"
            "<body style=\"font-family:'Sans Serif'; font-size:12pt; font-style:normal;\">"
              "<p style=\"margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                "Version: %1<br/>"
                "Qt version: %5<br/>"
                "Drumstick version: %6<br/>"
                "Build date: %2<br/>"
                "Build time: %3<br/>"
                "Compiler: %4"
              "</p>"
            "</body>"
            "</html>").arg(strver, BLD_DATE, BLD_TIME, CMP_VERSION,
                           qVersion(), drumstick::ALSA::getDrumstickLibraryVersion()));
}
