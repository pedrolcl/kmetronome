/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2006-2023, Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

#ifndef DRUMGRID_H
#define DRUMGRID_H

#include "defs.h"
#include <QDialog>
#include <QMenu>
#include <QShortcut>
#include <QCloseEvent>
#include <QWidget>

namespace Ui
{
    class DrumGrid;
}

class SequencerAdapter;
class DrumGridModel;

class DrumGrid : public QDialog
{
    Q_OBJECT
public:
    DrumGrid(QWidget *parent = nullptr);
    virtual ~DrumGrid();
    void setSequencer(SequencerAdapter* seq);
    void setModel(DrumGridModel* model);

    void subscribe(const QString& portName);
    void addShortcut(const QKeySequence& key, const QString& value);
    void addEditAction(const QString& name, const char* slot, const QKeySequence& key);
    void readPattern();
    void readPattern(const QString& name);
    void writePattern();
    void writePattern(const QString& name);
    void removePattern(const QString& name);
    void updateTempo(int newTempo);
    void showEvent(QShowEvent* event) override;
    void done(int r) override;
    QStringList patterns();
    QString currentPattern() { return m_currentPattern; }
    void setFigure(int figure);
    void setInstrument(const QString& instrument);
    void enableWidgets(bool enable);
    void setIcons(bool internal);

public slots:
    void updateView();
    void play();
    void stop();
    void slotTempoChanged(int newTempo);
    void slotColumnsChanged(int columns);
    void slotFigureChanged(int idx);
    void shortcutPressed(const QString& value);
    void updateDisplay(int bar, int beat);
    void patternChanged(int idx);
    void savePattern();
    void removePattern();
    void addRow();
    void removeRow();
    void gridContextMenu( const QPoint& );
    void slotCut();
    void slotCopy();
    void slotPaste();

signals:
    void signalUpdate(int bar, int beat);

private:
    Ui::DrumGrid *m_ui;
    SequencerAdapter *m_seq;
    DrumGridModel* m_model;
    int m_figure;
    int m_columns;
    unsigned long m_tick;
    QVector<QShortcut*> m_shortcuts;
    QString m_currentPattern;
    QMenu* m_popup;
    bool m_internalIcons;
};

#endif // DRUMGRID_H
