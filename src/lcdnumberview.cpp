/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2006-2020, Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

#include <QDebug>
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
    loadRenderer();
}

QString LCDNumberView::number()
{
    return m_number;
}

void LCDNumberView::setNumber(const QString& number)
{
    if (number != m_number) {
        m_number = number;
        if (m_number.length() > m_digitsCount)
        {
            setDigitCount(m_number.length());
        }
        update();
    }
}

int LCDNumberView::digitCount()
{
    return m_digitsCount;
}

void LCDNumberView::setDigitCount(const int count)
{
    if (count != m_digitsCount) {
        m_digitsCount = count;
        scene()->clear();
        for ( int i = 0; i < count; ++i ) {
            QGraphicsSvgItem *itm = new QGraphicsSvgItem();
            itm->setSharedRenderer(&m_renderer);
            itm->setPos(i * m_digitWidth, 0);
            scene()->addItem(itm);
        }
        QRectF rect(0, 0, m_digitsCount * m_digitWidth, m_digitHeight);
        scene()->setSceneRect(rect);
    }
}

void LCDNumberView::update()
{
    int i;
    QList<QGraphicsItem *> lst = scene()->items(Qt::AscendingOrder);
    for (i = 0; i < lst.length(); ++i) {
        lst.at(i)->setVisible(false);
    }
    i = m_digitsCount - m_number.length();
    if (i >= lst.length())
        return;
    foreach(const QChar& ch, m_number) {
        QGraphicsSvgItem *itm = dynamic_cast<QGraphicsSvgItem*>(lst.at(i));
        if (itm != nullptr) {
            if(ch == ':') {
                itm->setElementId(QLatin1Literal("colon"));
                itm->setVisible(true);
            } else if (ch == '_') {
                itm->setElementId(QLatin1Literal("underscore"));
                itm->setVisible(true);
            } else {
                QString id = QString("d%0").arg(ch);
                if (m_renderer.elementExists(id)) {
                    itm->setElementId(id);
                    itm->setVisible(true);
                }
            }
        }
        i++;
        if (i > m_digitsCount)
            break;
    }
    rescale();
}

void LCDNumberView::rescale()
{
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void LCDNumberView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    rescale();
}

void LCDNumberView::loadRenderer()
{
    QString foreColor = palette().color(QPalette::Active, QPalette::Foreground).name(QColor::HexRgb);
    QString backColor = palette().color(QPalette::Active, QPalette::Background).name(QColor::HexRgb);
    QFile resource(QLatin1Literal(":/lcdnumbers.svg"));
    if (resource.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString rescontents = QString::fromUtf8(resource.readAll()).arg(foreColor).arg(backColor);
        resource.close();
        if(m_renderer.load(rescontents.toUtf8())) {
            m_digitWidth = m_renderer.boundsOnElement("d8").width();
            m_digitHeight = m_renderer.boundsOnElement("d8").height();
        } else
            qWarning() << "renderer failed to load the SVG resource";
    } else
        qWarning() << "failed to open" << resource.fileName();
}
