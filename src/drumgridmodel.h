/*
    KMetronome - ALSA Sequencer based MIDI metronome
    Copyright (C) 2006-2011, Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

#ifndef DRUMGRIDMODEL_H
#define DRUMGRIDMODEL_H

#include "defs.h"
#include <QtCore/QAbstractTableModel>
#include <QtCore/QStringList>

class InstrumentList;

class DrumGridModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    DrumGridModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setInstrumentList(InstrumentList* instruments);
    void loadKeyNames(const QString& ins, int bank, int patch);
    void fillSampleData();
    void clearPattern();
    void addPatternData(int key, const QStringList& row);
    void endOfPattern();
    QStringList patternData(int row);
    QString patternKey(int row);
    QString patternHit(int row, int col);
    void updatePatternColumns(int columns);
    void setPatternFigure(int figure) { m_figure = figure; }
    int patternFigure() { return m_figure; }
    void insertPatternRow(const QString& name);
    void removePatternRow(int row);
    QStringList keyNames();

public slots:
    void changeCell(const QModelIndex &index);
    void changeCell(const QModelIndex &index, const QString& newValue);

private:
    int m_columns;
    int m_figure;
    QString m_lastValue;
    InstrumentList* m_insList;
    QMap<int,QString> m_keyNames;
    QList<QStringList> m_modelData;
    QList<QStringList> m_tempData;
    QList<int> m_keys;
    QList<int> m_tempKeys;
};

#endif /* DRUMGRIDMODEL_H */
