/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2006-2022, Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

#include <QApplication>
#include <QPainter>
#include <QFileInfo>
#include "iconutils.h"

namespace IconUtils
{
    void PaintPixmap(QPixmap &pixmap, const QColor& color)
    {
        if (!pixmap.isNull()) {
            QPainter painter(&pixmap);
            painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(pixmap.rect(), color);
        }
    }

    QPixmap GetPixmap(const QString& fileName)
    {
        QPixmap pixmap;
        if (QFileInfo::exists(fileName)) {
            QImage image(fileName);
            pixmap = QPixmap::fromImage(image);
            if (image.allGray()) {
                PaintPixmap(pixmap, qApp->palette().color(QPalette::Active, QPalette::WindowText));
            }
        }
        return pixmap;
    }

    QIcon GetIcon(const QString& name, bool forcedIconTheme)
    {
        QIcon icon;
        if (!forcedIconTheme && QIcon::hasThemeIcon(name)) {
            icon = QIcon::fromTheme(name);
        }
        if (icon.isNull() || forcedIconTheme) {
            QString iconName = QString(":/icons/%1.png").arg(name);
            icon = QIcon(GetPixmap(iconName));
        }
        return icon;
    }

    void SetLabelIcon(QLabel *label, const QString& fileName)
    {
        label->setPixmap(GetPixmap(fileName));
    }

    void SetupComboFigures(QComboBox *combo)
    {
        QList< QPair<QString,int> > elements = {
            {QApplication::tr("Whole"),          1},
            {QApplication::tr("Half"),           2},
            {QApplication::tr("Quarter"),        4},
            {QApplication::tr("Eight"),          8},
            {QApplication::tr("Sixteenth"),     16},
            {QApplication::tr("Thirty-Second"), 32},
            {QApplication::tr("Sixty-Fourth"),  64}
        };
        combo->clear();
        for(const auto &p : elements) {
            QString iconName = QStringLiteral(":/icons/notevalues/%1.png").arg(p.second);
            combo->addItem(QIcon(GetPixmap(iconName)), p.first, p.second);
        }
    }

    void SetWindowIcon(QWidget *widget)
    {
        widget->setWindowIcon(QIcon(GetPixmap(":/icons/midi/icon64.png")));
    }
}
