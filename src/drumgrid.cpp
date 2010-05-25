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

#include "defs.h"
#include "drumgrid.h"
#include "drumgridmodel.h"
#include "sequenceradapter.h"
#include "ui_drumgrid.h"

#include <qmath.h>
#include <QtGui/QInputDialog>
#include <QtGui/QShortcut>
#include <QtGui/QToolTip>
#include <QtGui/QClipboard>
#include <QtCore/QSignalMapper>
#include <QtCore/QSettings>
#include <KDE/KConfigGroup>
#include <KDE/KIcon>
#include <KDE/KMessageBox>
#include <KDE/KInputDialog>
#include <KDE/KApplication>
#include <KDE/KDebug>

DrumGrid::DrumGrid(QWidget *parent)
    : KDialog(parent),
    m_ui(new Ui::DrumGrid),
    m_seq(NULL),
    m_model(NULL),
    m_figure(PATTERN_FIGURE),
    m_columns(PATTERN_COLUMNS)
{
    QWidget *widget = new QWidget(this);
    m_ui->setupUi(widget);
    setMainWidget( widget );
    setCaption( i18n("Pattern Editor") );
    setInitialSize( QSize(700,400) );
    m_ui->tableView->setSelectionMode(QTableView::ContiguousSelection);

    m_ui->startButton->setIcon(KIcon("media-playback-start"));
    m_ui->stopButton->setIcon(KIcon("media-playback-stop"));
    m_ui->saveButton->setIcon(KIcon("document-save"));
    m_ui->deleteButton->setIcon(KIcon("edit-delete"));
    m_ui->addButton->setIcon(KIcon("list-add"));
    m_ui->removeButton->setIcon(KIcon("list-remove"));
    m_ui->tempoSlider->setMaximum(TEMPO_MAX);
    m_ui->tempoSlider->setMinimum(TEMPO_MIN);

    m_popup = new QMenu(this);
    addEditAction(i18n("Cut"), SLOT(slotCut()), QKeySequence::Cut);
    addEditAction(i18n("Copy"), SLOT(slotCopy()), QKeySequence::Copy);
    addEditAction(i18n("Paste"), SLOT(slotPaste()), QKeySequence::Paste);
    m_popup->addSeparator();

    m_mapper = new QSignalMapper(this);
    addShortcut(QKeySequence("f"), "f");
    addShortcut(QKeySequence("p"), "p");
    addShortcut(QKeySequence("1"), "1");
    addShortcut(QKeySequence("2"), "2");
    addShortcut(QKeySequence("3"), "3");
    addShortcut(QKeySequence("4"), "4");
    addShortcut(QKeySequence("5"), "5");
    addShortcut(QKeySequence("6"), "6");
    addShortcut(QKeySequence("7"), "7");
    addShortcut(QKeySequence("8"), "8");
    addShortcut(QKeySequence("9"), "9");
    addShortcut(QKeySequence("0"), QString());
    addShortcut(QKeySequence::Delete, QString());

    connect( m_ui->startButton, SIGNAL(clicked()), SLOT(play()));
    connect( m_ui->stopButton, SIGNAL(clicked()), SLOT(stop()));
    connect( m_ui->tempoSlider, SIGNAL(valueChanged(int)), SLOT(slotTempoChanged(int)));
    connect( m_ui->gridColumns, SIGNAL(valueChanged(int)), SLOT(slotColumnsChanged(int)));
    connect( m_ui->figureCombo, SIGNAL(activated(int)), SLOT(slotFigureChanged(int)));
    connect( m_ui->patternCombo, SIGNAL(activated(int)), SLOT(patternChanged(int)));
    connect( m_ui->saveButton, SIGNAL(clicked()), SLOT(savePattern()));
    connect( m_ui->deleteButton, SIGNAL(clicked()), SLOT(removePattern()));
    connect( m_ui->addButton, SIGNAL(clicked()), SLOT(addRow()));
    connect( m_ui->removeButton, SIGNAL(clicked()), SLOT(removeRow()));
    connect( m_ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
             SLOT(gridContextMenu(const QPoint&)) );
    connect( m_mapper, SIGNAL(mapped(QString)), SLOT(shortcutPressed(QString)));
    connect( this, SIGNAL(signalUpdate(int,int)), SLOT(updateDisplay(int,int)));
}

DrumGrid::~DrumGrid()
{
    foreach(QShortcut* s, m_shortcuts)
        delete s;
    delete m_ui;
}

void DrumGrid::setModel(DrumGridModel* model)
{
    m_model = model;
    m_ui->tableView->setModel(m_model);
    connect ( m_ui->tableView, SIGNAL(doubleClicked(const QModelIndex&)),
              m_model, SLOT(changeCell(const QModelIndex &)) );
}

void DrumGrid::setSequencer(SequencerAdapter* seq)
{
    m_seq = seq;
    connect( m_seq, SIGNAL(signalUpdate(int,int)),
             SLOT(updateDisplay(int,int)), Qt::QueuedConnection);
}

void DrumGrid::setInstrument(const QString& instrument)
{
    int bank = m_seq->getBank();
    int patch = m_seq->getProgram();
    m_model->loadKeyNames(instrument, bank, patch);
}

void DrumGrid::updateView()
{
    m_ui->tableView->resizeColumnsToContents();
    m_ui->tableView->resizeRowsToContents();
}

void DrumGrid::enableWidgets(bool enable)
{
    m_ui->gridColumns->setEnabled(enable);
    m_ui->figureCombo->setEnabled(enable);
    m_ui->addButton->setEnabled(enable);
    m_ui->removeButton->setEnabled(enable);
    m_ui->saveButton->setEnabled(enable);
    m_ui->deleteButton->setEnabled(enable);
    m_ui->patternCombo->setEnabled(enable);
}

void DrumGrid::play()
{
    enableWidgets(false);
    m_seq->metronome_set_tempo();
    m_seq->metronome_start();
}

void DrumGrid::stop()
{
    m_seq->metronome_stop();
    enableWidgets(true);
}

void DrumGrid::slotTempoChanged(int newTempo)
{
    m_seq->setBpm(newTempo);
    m_seq->metronome_set_tempo();
    updateTempo(newTempo);
}

void DrumGrid::updateTempo(int newTempo)
{
    m_ui->tempoLabel->setNum(newTempo);
}

void DrumGrid::slotFigureChanged(int idx)
{
    m_figure = (int)pow(2, idx);
    m_model->setPatternFigure(m_figure);
}

void DrumGrid::setFigure(int newValue)
{
    int ts_dd;
    int x = m_figure = newValue;
    for (ts_dd = 0; x > 1; x /= 2)
        ++ts_dd;
    m_ui->figureCombo->setCurrentIndex(ts_dd);
    m_model->setPatternFigure(newValue);
}

void DrumGrid::slotColumnsChanged(int columns)
{
    m_columns = columns;
    m_model->updatePatternColumns(columns);
    updateView();
}

void DrumGrid::shortcutPressed(const QString& value)
{
    QModelIndex index = m_ui->tableView->currentIndex();
    m_model->changeCell(index, value);
}

void DrumGrid::addShortcut(const QKeySequence& key, const QString& value)
{
    QShortcut* shortcut = new QShortcut(key, m_ui->tableView);
    connect (shortcut, SIGNAL(activated()), m_mapper, SLOT(map()));
    m_mapper->setMapping(shortcut, value);
    m_shortcuts.append(shortcut);
    QAction* action;
    if (value.isEmpty())
        action = m_popup->addAction(key.toString(), m_mapper, SLOT(map()));
    else
        action = m_popup->addAction(value, m_mapper, SLOT(map()));
    m_mapper->setMapping(action, value);
}

void DrumGrid::addEditAction(const QString& name, const char* slot, const QKeySequence& key)
{
    QShortcut* shortcut = new QShortcut(key, m_ui->tableView);
    connect (shortcut, SIGNAL(activated()), this, slot);
    m_shortcuts.append(shortcut);
    m_popup->addAction(name, this, slot, key);
}

void DrumGrid::readPattern()
{
    KConfigGroup config = KGlobal::config()->group(QSTR_PATTERN+m_currentPattern);
    QStringList keys = config.keyList();
    if (!keys.empty()) {
        setFigure(config.readEntry(QSTR_FIGURE, PATTERN_FIGURE));
        int cols = config.readEntry(QSTR_BEATS, PATTERN_COLUMNS);
        m_ui->gridColumns->setValue(cols);
        slotColumnsChanged(cols);
        keys.removeOne(QSTR_FIGURE);
        keys.removeOne(QSTR_BEATS);
        keys.sort();
        m_model->clearPattern();
        foreach(const QString& key, keys) {
            bool isNumber;
            int k = key.toInt(&isNumber);
            if (isNumber) {
                QStringList row = config.readEntry(key, QStringList());
                m_model->addPatternData(k, row);
            }
        }
        m_model->endOfPattern();
    }
}

void DrumGrid::readPattern(const QString& name)
{
    m_currentPattern = name;
    readPattern();
}

void DrumGrid::writePattern()
{
    KConfigGroup config = KGlobal::config()->group(QSTR_PATTERN+m_currentPattern);
    config.deleteGroup();
    config.writeEntry( QSTR_FIGURE, m_figure );
    config.writeEntry( QSTR_BEATS, m_columns );
    for(int r = 0; r < m_model->rowCount(); ++r) {
        config.writeEntry( m_model->patternKey(r),
                           m_model->patternData(r) );
    }
    config.sync();
}

void DrumGrid::writePattern(const QString& name)
{
    m_currentPattern = name;
    writePattern();
}

void DrumGrid::removePattern(const QString& name)
{
    if ( KMessageBox::questionYesNo (this,
            i18n("Do you want to remove the current pattern?"),
            i18n("Remove Pattern"),
            KStandardGuiItem::yes(),
            KStandardGuiItem::no(),
            "removepattern") == KMessageBox::Yes) {
        KGlobal::config()->deleteGroup(QSTR_PATTERN+name);
        KGlobal::config()->sync();
    }
}

QStringList DrumGrid::patterns()
{
    QStringList lst;
    int n = QSTR_PATTERN.size();
    foreach(const QString& name, KGlobal::config()->groupList()) {
        if (name.startsWith(QSTR_PATTERN))
            lst += name.mid(n);
    }
    return lst;
}

void DrumGrid::patternChanged(int /*idx*/)
{
    readPattern(m_ui->patternCombo->currentText());
}

void DrumGrid::savePattern()
{
    QString newName = m_ui->patternCombo->currentText();
    writePattern(newName);
    m_ui->patternCombo->setCurrentItem(newName, true);
}

void DrumGrid::removePattern()
{
    int idx = m_ui->patternCombo->currentIndex();
    QString tmpPattern = m_ui->patternCombo->currentText();
    m_ui->patternCombo->removeItem(idx);
    removePattern(tmpPattern);
    m_currentPattern.clear();
    m_model->clearPattern();
    if (m_ui->patternCombo->count() > 0)
        m_ui->patternCombo->setCurrentItem(0);
}

void DrumGrid::showEvent(QShowEvent* /*event*/)
{
    m_ui->tempoSlider->setValue(m_seq->getBpm());
    updateTempo(m_seq->getBpm());
    m_seq->setPatternMode(true);
    m_ui->patternCombo->clear();
    m_ui->patternCombo->addItems(patterns());
    if (m_currentPattern.isEmpty())
        if (m_ui->patternCombo->count() == 0) {
            m_currentPattern = i18nc("name of an automatically created pattern",
                "Sample Pattern");
            m_model->fillSampleData();
            m_ui->patternCombo->setCurrentItem(m_currentPattern, true);
        } else {
            m_currentPattern = m_ui->patternCombo->currentText();
            readPattern();
        }
    else
        m_ui->patternCombo->setCurrentItem(m_currentPattern);
    updateView();
}

void DrumGrid::done(int r)
{
    stop();
    m_seq->setPatternMode(false);
    if (r == KDialog::Accepted && !m_currentPattern.isEmpty())
        writePattern();
    KDialog::done(r);
}

void DrumGrid::updateDisplay(int /*bar*/, int beat)
{
    if (m_ui->chkselbeat->isChecked())
        m_ui->tableView->selectColumn(beat-1);
    m_ui->beatNumber->display(beat);
}

void DrumGrid::addRow()
{
    QStringList keys = m_model->keyNames();
    QString name = KInputDialog::getItem(i18n("Insert Pattern Row"),
                      i18n("Drum Key:"), keys,  0, false, 0, this);
    if (!name.isEmpty())
        m_model->insertPatternRow(name);
}

void DrumGrid::removeRow()
{
    int row = -1;
    const QModelIndexList indexlist =
            m_ui->tableView->selectionModel()->selectedIndexes();
    if (indexlist.count() == m_columns) {
        foreach (const QModelIndex& idx, indexlist) {
            if (row < 0)
                row = idx.row();
            else if (row != idx.row()) {
                row = -1;
                break;
            }
        }
    }
    if (row < 0)
        KMessageBox::sorry(this, i18n("Please select one, and only one row"));
    else if ( KMessageBox::questionYesNo (this,
                i18n("Do you want to remove the selected pattern row?"),
                i18n("Remove Row"),
                KStandardGuiItem::yes(),
                KStandardGuiItem::no(),
                "removepatternrow") == KMessageBox::Yes)
            m_model->removePatternRow(row);
}

void DrumGrid::gridContextMenu(const QPoint&)
{
    if (m_popup != NULL)
        m_popup->exec(QCursor::pos());
}

void DrumGrid::slotCut()
{
    QStringList list;
    QItemSelectionModel* selModel = m_ui->tableView->selectionModel();
    QItemSelectionRange range = selModel->selection().first();
    QModelIndexList indexes = selModel->selectedIndexes();
    for (int row=range.top(); row<=range.bottom(); ++row) {
        QStringList rowList;
        foreach ( const QModelIndex& idx, indexes ) {
            if (idx.row() == row) {
                rowList << idx.data().toString();
                m_model->changeCell(idx, QString());
            }
        }
        list << rowList.join(QString(','));
    }
    kapp->clipboard()->setText( list.join(QString('\n')) ) ;
}

void DrumGrid::slotCopy()
{
    QStringList list;
    QItemSelectionModel* selModel = m_ui->tableView->selectionModel();
    QItemSelectionRange range = selModel->selection().first();
    QModelIndexList indexes = selModel->selectedIndexes();
    for (int row=range.top(); row<=range.bottom(); ++row) {
        QStringList rowList;
        foreach ( const QModelIndex& idx, indexes ) {
            if (idx.row() == row)
                rowList << idx.data().toString();
        }
        list << rowList.join(QString(','));
    }
    kapp->clipboard()->setText( list.join(QString('\n')) ) ;
}

void DrumGrid::slotPaste()
{
    QString clbrdText = kapp->clipboard()->text();
    if (!clbrdText.isEmpty()) {
        int x, y=0;
        QItemSelectionModel* selModel = m_ui->tableView->selectionModel();
        QItemSelectionRange range = selModel->selection().first();
        QStringList rows = clbrdText.split('\n');
        foreach( const QString& r, rows ) {
            QStringList list = r.split(',');
            for(x=0; x<list.size(); ++x) {
                QModelIndex idx = m_model->index(range.top()+y, range.left()+x);
                m_model->changeCell(idx, list.at(x));
            }
            y++;
        }
    }
}
