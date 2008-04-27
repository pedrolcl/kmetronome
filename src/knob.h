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

#ifndef __KNOB_H
#define __KNOB_H

#include <qdial.h>

class Knob : public QDial
{
	Q_OBJECT
	Q_PROPERTY( QColor knobColor READ getKnobColor WRITE setKnobColor )
	Q_PROPERTY( QColor meterColor READ getMeterColor WRITE setMeterColor )

public:

	// Constructor.
	Knob(QWidget *pParent = 0, const char *pszName = 0);
	// Destructor.
	virtual ~Knob();

	const QColor& getKnobColor()  const { return m_knobColor;  }
	const QColor& getMeterColor() const { return m_meterColor; }

public slots:

	// Set the colour of the knob
	void setKnobColor(const QColor &color);

	// Set the colour of the meter
	void setMeterColor(const QColor &color);

protected:

	void drawTick(QPainter &paint, double angle, int size, bool internal);
	virtual void repaintScreen(const QRect *cr = 0);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void wheelEvent(QWheelEvent *e);
	void valueChange();

	QColor m_knobColor;
	QColor m_meterColor;
	bool   m_buttonPressed;
	int    m_lastY;
	int    m_lastX;

//	struct CacheIndex
//	{
//		CacheIndex(int _s, int _kc, int _mc, int _a, int _n, int _c) :
//			size(_s), knobColor(_kc), meterColor(_mc),
//			angle(_a), numTicks(_n), centered(_c) {}
//
//		bool operator<(const CacheIndex &i) const {
//			// woo!
//			if (size < i.size) return true;
//			else if (size > i.size) return false;
//			else if (knobColor < i.knobColor) return true;
//			else if (knobColor > i.knobColor) return false;
//			else if (meterColor < i.meterColor) return true;
//			else if (meterColor > i.meterColor) return false;
//			else if (angle < i.angle) return true;
//			else if (angle > i.angle) return false;
//			else if (numTicks < i.numTicks) return true;
//			else if (numTicks > i.numTicks) return false;
//			else if (centered == i.centered) return false;
//			else if (!centered) return true;
//			return false;
//		}

//		int          size;
//		unsigned int knobColor;
//		unsigned int meterColor;
//		int          angle;
//		int          numTicks;
//		bool         centered;
//	};

//	typedef std::map<CacheIndex, QPixmap> PixmapCache;
//	static PixmapCache m_pixmaps;
};

#endif
