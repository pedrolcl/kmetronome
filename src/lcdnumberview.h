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

#ifndef LCDNUMBERVIEW_H
#define LCDNUMBERVIEW_H

#include <QGraphicsView>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QResizeEvent>

class LCDNumberView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LCDNumberView(QWidget *parent = nullptr);
    QString number();
    int digitCount();
    void setDigitCount(const int count);
    void loadRenderer();
    void update();
    void rescale();
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void setNumber(const QString& number);

private:
    QSvgRenderer m_renderer;
    QString m_number;
    qreal m_digitWidth;
    qreal m_digitHeight;
    int m_digitsCount;
};

#endif // LCDNUMBERVIEW_H
