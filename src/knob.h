/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2005-2010, Pedro Lopez-Cabanillas <plcl@users.sf.net>

    For this file, the following copyright notice is also applicable:
    Copyright (C) 2005-2010, rncbc aka Rui Nuno Capela. All rights reserved.
    See http://qsynth.sourceforge.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along 
    with this program; If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KNOB_H
#define KNOB_H

#include <QDial>
#include <QMap>


//-------------------------------------------------------------------------
// Knob - A better QDial for QSynth.

class Knob : public QDial
{
	Q_OBJECT
	Q_PROPERTY( int defaultValue READ getDefaultValue WRITE setDefaultValue )
	Q_PROPERTY( DialMode dialMode READ getDialMode WRITE setDialMode )
	Q_ENUMS(DialMode)

public:

	// Constructor.
	Knob(QWidget *pParent = 0);
	// Destructor.
	~Knob();

	int getDefaultValue() const { return m_iDefaultValue; }

	// Knob dial mode behavior:
	// QDialMode   - Old QDial BEHAVIOUR.
	// AngularMode - Angularly relative to widget center.
	// LinearMode  - Proportionally to distance in one ortogonal axis.

	enum DialMode {	QDialMode, AngularMode, LinearMode };

	DialMode getDialMode() const { return m_dialMode; }

public slots:

	// Set default (mid) value.
	void setDefaultValue(int iDefaultValue);

	// Set knob dial mode behavior.
	void setDialMode(DialMode dialMode);

	void setValue(int val);	
	
protected:

	// Mouse angle determination.
	double mouseAngle(const QPoint& pos);

	// Alternate mouse behavior event handlers.
	virtual void mousePressEvent(QMouseEvent *pMouseEvent);
	virtual void mouseMoveEvent(QMouseEvent *pMouseEvent);
	virtual void mouseReleaseEvent(QMouseEvent *pMouseEvent);
	virtual void wheelEvent(QWheelEvent *pWheelEvent);

private:

	// Default (mid) value.
	int m_iDefaultValue;

	// Knob dial mode behavior.
	DialMode m_dialMode;

	// Alternate mouse behavior tracking.
	bool   m_bMousePressed;
	QPoint m_posMouse;

	// Just for more precission on the movement
	double m_lastDragValue;
};


#endif  // KNOB_H
