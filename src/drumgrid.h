/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2006-2010, Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

#ifndef DRUMGRID_H
#define DRUMGRID_H

#include <QtGui/QDialog>
#include <QtGui/QShortcut>
#include <QtGui/QCloseEvent>
#include <QtCore/QSignalMapper>
#include "defs.h"

const QString QSTR_PATTERN("Pattern");

namespace Ui
{
    class DrumGrid;
}

namespace drumstick
{
    class MidiClient;
    class MidiPort;
    class MidiQueue;
    class SequencerEvent;
}

class SequencerAdapter;
class DrumGridModel;

using namespace drumstick;

class DrumGrid : public QDialog
{
    Q_OBJECT

public:
    DrumGrid(QWidget *parent = 0);
    virtual ~DrumGrid();
    void setSequencer(SequencerAdapter* seq);

    void subscribe(const QString& portName);
    void addShortcut(const QKeySequence& key, const QString& value);
    void readSettings();
    void writeSettings();
    void updateTempo(int newTempo);
    void showEvent(QShowEvent* event);
    void done(int r);

public slots:
    void updateView();
    void play();
    void stop();
    void tempoChange(int newTempo);
    void gridColumns(int columns);
    void shortcutPressed(const QString& value);
    void updateDisplay(int bar, int beat);

signals:
    void signalUpdate(int bar, int beat);

private:
    Ui::DrumGrid *m_ui;
    SequencerAdapter *m_seq;
    DrumGridModel* m_model;
    int m_figure;
    unsigned long m_tick;
    QSignalMapper* m_mapper;
    QVector<QShortcut*> m_shortcuts;
};

#endif // DRUMGRID_H
