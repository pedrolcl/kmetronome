/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2006-2014, Pedro Lopez-Cabanillas <plcl@users.sf.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <QtGlobal>
#include <QGraphicsItem>
#include "lcdnumberview.h"

LCDNumberView::LCDNumberView(QWidget *parent) :
    QGraphicsView(parent),
    m_digitWidth(0),
    m_digitHeight(0),
    m_digitsCount(0)
{
    setScene(new QGraphicsScene(this));
    setTransformationAnchor(AnchorUnderMouse);
    setViewportUpdateMode(FullViewportUpdate);
    setBackgroundRole(QPalette::Background);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_renderer.load(QLatin1Literal(":/lcdnumbers.svg"));
    m_digitWidth = m_renderer.boundsOnElement("8").width();
    m_digitHeight = m_renderer.boundsOnElement("8").height();
}

QString LCDNumberView::number()
{
    return m_number;
}

void LCDNumberView::setNumber(const QString& number)
{
    if (number != m_number) {
        m_number = number;
        update();
    }
}

int LCDNumberView::digitCount()
{
    return m_digitsCount;
}

void LCDNumberView::setDigitCount(const int count)
{
    m_digitsCount = count;
    rescale();
}

void LCDNumberView::update()
{
    int count = 0;
    qreal x = 0;
    scene()->clear();
    foreach(const QChar& ch, m_number) {
        QGraphicsSvgItem *itm = new QGraphicsSvgItem();
        itm->setSharedRenderer(&m_renderer);
        if(ch == ':') {
            itm->setElementId(QLatin1Literal("colon"));
        } else if (ch == '_') {
            itm->setElementId(QLatin1Literal("underscore"));
        } else {
            QString id(ch);
            if (m_renderer.elementExists(id)) {
                itm->setElementId(id);
            } else {
                x += m_digitWidth;
                delete itm;
                continue;
            }
        }
        itm->setPos(x, 0);
        scene()->addItem(itm);
        x += m_digitWidth;
        count++;
    }
    if (count > 0) {
        if (count > m_digitsCount) {
            m_digitsCount = count;
        }
        rescale();
    }
}

void LCDNumberView::rescale()
{
    QRectF rect(0, 0, m_digitWidth * m_digitsCount, m_digitHeight);
    scene()->setSceneRect(rect);
    fitInView(rect, Qt::KeepAspectRatio);
}

void LCDNumberView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    rescale();
}
