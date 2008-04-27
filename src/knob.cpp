/****************************************************************************
   This widget is based on a design by Thorsten Wilms, 
   implemented by Chris Cannam in Rosegarden,
   adapted by Pedro Lopez-Cabanillas

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*****************************************************************************/

#include <cmath>
#include <qpainter.h>
#include <qpixmap.h>
#include <qimage.h>

#include "knob.h"

#define QKNOB_MIN (0.25 * M_PI)
#define QKNOB_MAX (1.75 * M_PI)
#define QKNOB_RANGE (QKNOB_MAX - QKNOB_MIN)

//Knob::PixmapCache Knob::m_pixmaps;

// Constructor.
Knob::Knob ( QWidget *pParent, const char *pszName )
	: QDial(pParent, pszName),
	m_knobColor(Qt::black), m_meterColor(Qt::white),
        m_buttonPressed(false), m_lastY(0), m_lastX(0)
{ 
}

// Destructor.
Knob::~Knob (void)
{
}


void Knob::repaintScreen ( const QRect * /*cr*/ )
{
	QPainter paint;

	double angle = QKNOB_MIN // offset
		+ (QKNOB_RANGE *
			(double(value() - minValue()) /
			(double(maxValue()) - double(minValue()))));
	int degrees = int(angle * 180.0 / M_PI);

	int ns = notchSize();
	int numTicks = 1 + (maxValue() + ns - minValue()) / ns;
	
	QColor knobColor(m_knobColor);
	if (knobColor == Qt::black)
		knobColor = colorGroup().mid();

	QColor meterColor(m_meterColor);
	if (!isEnabled())
		meterColor = colorGroup().mid();
	else if (m_meterColor == Qt::white)
		meterColor = colorGroup().highlight();

	int m_size = width() < height() ? width() : height();
	
//	CacheIndex index(m_size, knobColor.pixel(), meterColor.pixel(),
//		degrees, numTicks, false);
//
//	if (m_pixmaps.find(index) != m_pixmaps.end()) {
//		paint.begin(this);
//		paint.drawPixmap(0, 0, m_pixmaps[index]);
//		paint.end();
//		return;
//	}

	int scale = 4;
	int width = m_size * scale;
	QPixmap map(width, width);
	map.fill(paletteBackgroundColor());
	paint.begin(&map);

	QPen pen;
	QColor c;

	// Knob body and face...

	c = knobColor;
	pen.setColor(knobColor);
	pen.setWidth(scale);
	
	paint.setPen(pen);
	paint.setBrush(c);

	int indent = (int)(width * 0.15 + 1);

	paint.drawEllipse(indent, indent, width-2*indent, width-2*indent);

	pen.setWidth(2 * scale);
	int pos = indent + (width-2*indent) / 12;
	int darkWidth = (width-2*indent) * 3 / 5;
	while (darkWidth) {
		c = c.light(101);
		pen.setColor(c);
		paint.setPen(pen);
		paint.drawEllipse(pos, pos, darkWidth, darkWidth);
		if (!--darkWidth) break;
		paint.drawEllipse(pos, pos, darkWidth, darkWidth);
		if (!--darkWidth) break;
		paint.drawEllipse(pos, pos, darkWidth, darkWidth);
		++pos; --darkWidth;
	}

	// Now the bright metering bit...

	c = meterColor;
	pen.setColor(c);
	pen.setWidth(indent);
	paint.setPen(pen);

	paint.drawArc(indent/2, indent/2,
		width-indent, width-indent, (180 + 45) * 16, -(degrees - 45) * 16);

	// Tick notches...

	paint.setBrush(QBrush::NoBrush);

	if ( notchesVisible() ) {
		pen.setColor(colorGroup().dark());
		pen.setWidth(scale);
		paint.setPen(pen);
		for (int i = 0; i < numTicks; ++i) {
			int div = numTicks;
			if (div > 1) --div;
			drawTick(paint, QKNOB_MIN + (QKNOB_MAX - QKNOB_MIN) * i / div,
				width, i != 0 && i != numTicks-1 );
		}
	}

	// Shadowing...

	pen.setWidth(scale);
	paint.setPen(pen);

	int shadowAngle = -720;
	c = colorGroup().dark();
	for (int arc = 120; arc < 2880; arc += 240) {
		pen.setColor(c);
		paint.setPen(pen);
		paint.drawArc(indent, indent,
			width-2*indent, width-2*indent, shadowAngle + arc, 240);
		paint.drawArc(indent, indent,
			width-2*indent, width-2*indent, shadowAngle - arc, 240);
		c = c.light(110);
	}

	shadowAngle = 2160;
	c = colorGroup().dark();
	for (int arc = 120; arc < 2880; arc += 240) {
		pen.setColor(c);
		paint.setPen(pen);
		paint.drawArc(scale/2, scale/2,
			width-scale, width-scale, shadowAngle + arc, 240);
		paint.drawArc(scale/2, scale/2,
			width-scale, width-scale, shadowAngle - arc, 240);
		c = c.light(101);
	}

	// And undraw the bottom part...

	pen.setColor(paletteBackgroundColor());
	paint.setPen(pen);
	paint.drawArc(scale/2, scale/2,
		width-scale, width-scale, -45 * 16, -90 * 16);

	double hyp = double(width) / 2.0;
	double len = hyp - indent;
	--len;

	double x0 = hyp;
	double y0 = hyp;

	double x = hyp - len * sin(angle);
	double y = hyp + len * cos(angle);

	c = colorGroup().dark();
	pen.setColor(isEnabled() ? c : c.light(140));
	pen.setWidth(scale * 2);
	paint.setPen(pen);
	paint.drawLine(int(x0), int(y0), int(x), int(y));
	paint.end();

	// Image rendering...

//	QImage img = map.convertToImage().smoothScale(m_size, m_size);
//	m_pixmaps[index] = QPixmap(img);
	paint.begin(this);
//	paint.drawPixmap(0, 0, m_pixmaps[index]);
	paint.drawPixmap(0,0,map.convertToImage().smoothScale(m_size, m_size));
	paint.end();
}


void Knob::drawTick ( QPainter &paint,
	double angle, int size, bool internal )
{
	double hyp = double(size) / 2.0;
	double x0 = hyp - (hyp - 1) * sin(angle);
	double y0 = hyp + (hyp - 1) * cos(angle);

	if (internal) {

		double len = hyp / 4;
		double x1 = hyp - (hyp - len) * sin(angle);
		double y1 = hyp + (hyp - len) * cos(angle);
		
		paint.drawLine(int(x0), int(y0), int(x1), int(y1));

	} else {

		double len = hyp / 4;
		double x1 = hyp - (hyp + len) * sin(angle);
		double y1 = hyp + (hyp + len) * cos(angle);

		paint.drawLine(int(x0), int(y0), int(x1), int(y1));
	}
}


void Knob::setKnobColor ( const QColor& color )
{
	m_knobColor = color;
	repaint();
}


void Knob::setMeterColor ( const QColor& color )
{
	m_meterColor = color;
	repaint();
}

void Knob::mousePressEvent(QMouseEvent *e)
{
        if (e->button() == LeftButton) {
                m_buttonPressed = true;
                m_lastY = e->y();
                m_lastX = e->x();
                emit dialPressed();
        } else 
        if (e->button() == MidButton) {
                setValue ( (maxValue() + minValue()) / 2 );
        }
}

void Knob::mouseReleaseEvent(QMouseEvent *e)
{
        if ( e->button() == LeftButton ) {
                m_buttonPressed = false;
                m_lastY = 0;
                m_lastX = 0;
                emit dialReleased();
        }
}

void Knob::mouseMoveEvent(QMouseEvent *e)
{
        if ( m_buttonPressed ) {
                // Dragging by x or y axis when clicked modifies value
                //
                int newValue = value() +
                        (m_lastY - e->y() + e->x() - m_lastX) * lineStep();

                if (newValue > maxValue())
                        newValue = maxValue();
                else
                if (newValue < minValue())
                        newValue = minValue();

                m_lastY = e->y();
                m_lastX = e->x();
        
                setValue( newValue );
                emit dialMoved( value() );
        }
}

void Knob::wheelEvent(QWheelEvent *e)
{
        int newValue = value();
        if (e->delta() > 0)
                newValue -= pageStep();
        else 
                newValue += pageStep();

        if (newValue > maxValue())
                newValue = maxValue();

        if (newValue < minValue())
                newValue = minValue();

        setValue( newValue );
}

void Knob::valueChange()
{
        repaintScreen();
        emit valueChanged( value() );
#if defined(QT_ACCESSIBILITY_SUPPORT)
        QAccessible::updateAccessibility( this, 0, QAccessible::ValueChanged );
#endif
}

#include "knob.moc"
