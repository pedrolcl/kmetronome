/***************************************************************************
 *   KMetronome - ALSA Sequencer based MIDI metronome                      *
 *   Copyright (C) 2005-2014 Pedro Lopez-Cabanillas <plcl@users.sf.net>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA 02110-1301, USA                                                    *
 ***************************************************************************/

#include "kmetronome.h"
#include "kmetronomeview.h"
#include "kmetropreferences.h"
#include "sequenceradapter.h"
//#include "kmetronomeadaptor.h"
#include "defs.h"
#include "drumgrid.h"
#include "drumgridmodel.h"
#include "instrument.h"

#include <cmath>
#include <QEvent>
#include <QLabel>
#include <QStandardPaths>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>

//#include <QtDBus/QDBusConnection>

KMetronome::KMetronome(QWidget *parent) :
    QMainWindow(parent),
    m_styledKnobs(true),
    m_view(0),
    m_seq(0)
{
    //new KmetronomeAdaptor(this);
    //QDBusConnection dbus = QDBusConnection::sessionBus();
    //dbus.registerObject("/", this);
    m_view = new KmetronomeView(this);
    m_model = new DrumGridModel(this);
    m_instrumentList = new InstrumentList;
    m_model->setInstrumentList(m_instrumentList);
    QString drums =  QStandardPaths::locate(QStandardPaths::DataLocation, "drums.ins");
    if (!drums.isEmpty())
        m_instrumentList->load(drums);
    try {
        m_seq = new SequencerAdapter(this);
        m_seq->setModel(m_model);
        connect(m_seq, SIGNAL(signalUpdate(int,int)),
                SLOT(updateDisplay(int,int)), Qt::QueuedConnection);
        connect(m_seq, SIGNAL(signalPlay()), SLOT(play()), Qt::QueuedConnection);
        connect(m_seq, SIGNAL(signalStop()), SLOT(stop()), Qt::QueuedConnection);
        connect(m_seq, SIGNAL(signalCont()), SLOT(cont()), Qt::QueuedConnection);
        connect(m_seq, SIGNAL(signalNotation(int,int)),
                SLOT(setTimeSignature(int,int)), Qt::QueuedConnection);
        setCentralWidget(m_view);
        setupActions();
        //setAutoSaveSettings();
        //setupPlaces();
        readConfiguration();
    } catch (SequencerError& ex) {
        QString errorstr = tr("Fatal error from the ALSA sequencer. "
            "This usually happens when the kernel doesn't have ALSA support, "
            "or the device node (/dev/snd/seq) doesn't exists, "
            "or the kernel module (snd_seq) is not loaded. "
            "Please check your ALSA/MIDI configuration. Returned error was: %1")
            .arg(ex.qstrError());
        QMessageBox::critical(0, tr("Error"), errorstr);
        close();
    }
}

KMetronome::~KMetronome()
{
    delete m_instrumentList;
}

void KMetronome::setupActions()
{
    //QAction *quit = new QAction(this);

//    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
//    m_prefs = KStandardAction::preferences(this, SLOT(optionsPreferences()),
//                                           actionCollection());
//    KStandardAction::configureToolbars(this, SLOT(configureToolbars()),
//            actionCollection());
//    KStandardAction::keyBindings(guiFactory(), SLOT(configureShortcuts()),
//                                 actionCollection());

    m_playStop = new QAction(this);
    m_playStop->setText(tr("Play/Stop"));
    m_playStop->setCheckable(true);
    m_playStop->setIcon(QIcon::fromTheme("media-playback-start"));
    //actionCollection()->addAction("play_stop", m_playStop);
    connect(m_playStop, SIGNAL(triggered(bool)), SLOT(toggle(bool)));

    m_fakeToolbar = new QAction(this);
    m_fakeToolbar->setCheckable(true);
    m_fakeToolbar->setText(tr("Show Action Buttons"));
    m_fakeToolbar->setChecked(true);
    //actionCollection()->addAction("show_faketoolbar", m_fakeToolbar);
    connect(m_fakeToolbar, SIGNAL(triggered(bool)), m_view,
            SLOT(displayFakeToolbar(bool)));

    m_editPatterns = new QAction(QIcon::fromTheme("document-edit"), tr("Patterns"), this);
    //actionCollection()->addAction("edit_patterns", m_editPatterns);
    connect(m_editPatterns, SIGNAL(triggered()), SLOT(editPatterns()));

    QAction* imp = new QAction(QIcon::fromTheme("document-import"),tr("Import Patterns"), this);
    //actionCollection()->addAction("import_patterns", imp);
    connect(imp, SIGNAL(triggered()), SLOT(importPatterns()));

    QAction* exp = new QAction( QIcon::fromTheme("document-export"),tr("Export Patterns"), this);
    //actionCollection()->addAction("export_patterns", exp);
    connect(exp, SIGNAL(triggered()), SLOT(exportPatterns()));

    //setStandardToolBarMenuEnabled(true);
    //createGUI();
}

bool KMetronome::queryExit()
{
    saveConfiguration();
    return true;
}

void KMetronome::saveConfiguration()
{
    QSettings settings;
    settings.beginGroup("Settings");
    if (m_seq == NULL)
        return;
    settings.setValue("instrument", m_instrument);
    settings.setValue("bank", m_bank);
    settings.setValue("program", m_program);
    settings.setValue("weakNote", m_seq->getWeakNote());
    settings.setValue("strongNote", m_seq->getStrongNote());
    settings.setValue("channel", m_seq->getChannel());
    settings.setValue("weakVelocity", m_seq->getWeakVelocity());
    settings.setValue("strongVelocity", m_seq->getStrongVelocity());
    settings.setValue("volume", m_seq->getVolume());
    settings.setValue("balance", m_seq->getBalance());
    settings.setValue("resolution", m_seq->getResolution());
    settings.setValue("sendNoteOff", m_seq->getSendNoteOff());
    settings.setValue("duration", m_seq->getNoteDuration());
    settings.setValue("tempo", m_seq->getBpm());
    settings.setValue("rhythmNumerator", m_seq->getRhythmNumerator());
    settings.setValue("rhythmDenominator", m_seq->getRhythmDenominator());
    settings.setValue("autoconnect", m_seq->getAutoConnect());
    settings.setValue("outputConn", m_seq->getOutputConn());
    settings.setValue("inputConn", m_seq->getInputConn());
    settings.setValue("styledKnobs", m_styledKnobs);
    settings.setValue("fakeToolbar", m_fakeToolbar->isChecked());
    settings.endGroup();
    settings.sync();
}

void KMetronome::applyInstrumentSettings()
{
    Instrument ins = m_instrumentList->value(m_instrument);
    m_seq->setBankSelMethod(ins.bankSelMethod());
    InstrumentPatches banks = ins.patches();
    InstrumentPatches::ConstIterator j;
    for( j = banks.constBegin(); j != banks.constEnd(); ++j ) {
        InstrumentData bank = j.value();
        if (bank.name() == m_bank) {
            int ibank = j.key();
            InstrumentData::ConstIterator k;
            for( k = bank.constBegin(); k != bank.constEnd(); ++k ) {
                if (k.value() == m_program) {
                    int iprogram = k.key();
                    m_seq->setBank(ibank);
                    m_seq->setProgram(iprogram);
                    return;
                }
            }
        }
    }
}

void KMetronome::readConfiguration()
{
    QSettings settings;
    settings.beginGroup("Settings");
    m_instrument = settings.value("instrument", QString()).toString();
    m_bank = settings.value("bank", QString()).toString();
    m_program = settings.value("program", QString()).toString();
    applyInstrumentSettings();
    m_seq->setChannel(settings.value("channel", METRONOME_CHANNEL).toInt());
    m_seq->setWeakNote(settings.value("weakNote", METRONOME_WEAK_NOTE).toInt());
    m_seq->setStrongNote(settings.value("strongNote", METRONOME_STRONG_NOTE).toInt());
    m_seq->setResolution(settings.value("resolution", METRONOME_RESOLUTION).toInt());
    int volume = settings.value("volume", METRONOME_VOLUME).toInt();
    int balance = settings.value("balance", METRONOME_PAN).toInt();
    int weakVel = settings.value("weakVelocity", METRONOME_VELOCITY).toInt();
    int strongVel = settings.value("strongVelocity", METRONOME_VELOCITY).toInt();
    int tempo = settings.value("tempo", 100).toInt();
    int ts_num = settings.value("rhythmNumerator", 4).toInt();
    int ts_div = settings.value("rhythmDenominator", 4).toInt();
    m_seq->setVolume(volume);
    m_seq->setBalance(balance);
    m_seq->setWeakVelocity(weakVel);
    m_seq->setStrongVelocity(strongVel);
    m_seq->setBpm(tempo);
    m_seq->setRhythmNumerator(ts_num);
    m_seq->setRhythmDenominator(ts_div);
    m_view->setBeatsBar(ts_num);
    m_view->setFigure(ts_div);
    m_view->setTempo(tempo);
    m_view->displayWeakVelocity(weakVel);
    m_view->displayStrongVelocity(strongVel);
    m_view->displayVolume(volume);
    m_view->displayBalance(balance);
    int duration = settings.value("duration", NOTE_DURATION).toInt();
    m_seq->setNoteDuration(duration);
    bool sendNoteOff = settings.value("sendNoteOff", true).toBool();
    m_seq->setSendNoteOff(sendNoteOff);
    bool autoconn = settings.value("autoconnect", false).toBool();
    m_seq->setAutoConnect(autoconn);
    if(autoconn) {
        m_seq->setOutputConn(settings.value("outputConn", "20:0").toString());
        m_seq->setInputConn(settings.value("inputConn", "20:0").toString());
        m_seq->connect_output();
        m_seq->connect_input();
    }
    m_styledKnobs = settings.value("styledKnobs", true).toBool();
    m_seq->sendInitialControls();
    //m_view->updateKnobs(m_styledKnobs);
    bool fakeToolbar = settings.value("fakeToolbar", true).toBool();
    m_fakeToolbar->setChecked(fakeToolbar);
    m_view->displayFakeToolbar(fakeToolbar);
    updatePatterns();
}

void KMetronome::optionsPreferences()
{
    QPointer<KMetroPreferences> dlg = new KMetroPreferences(this);
    dlg->fillOutputConnections(m_seq->outputConnections());
    dlg->fillInputConnections(m_seq->inputConnections());
    dlg->fillInstruments(m_instrumentList);
    dlg->setAutoConnect(m_seq->getAutoConnect());
    QString conn = m_seq->getOutputConn();
    if (conn != NULL && !conn.isEmpty())
        dlg->setOutputConnection(conn);
    conn = m_seq->getInputConn();
    if (conn != NULL && !conn.isEmpty())
        dlg->setInputConnection(conn);
    dlg->setInstrumentName(m_instrument);
    dlg->setBankName(m_bank);
    dlg->setProgramName(m_program);
    dlg->setWeakNote(m_seq->getWeakNote());
    dlg->setStrongNote(m_seq->getStrongNote());
    dlg->setChannel(m_seq->getChannel());
    dlg->setResolution(m_seq->getResolution());
    dlg->setSendNoteOff(m_seq->getSendNoteOff());
    dlg->setDuration(m_seq->getNoteDuration());
    if (dlg->exec() == QDialog::Accepted) {
        m_seq->disconnect_output();
        m_seq->disconnect_input();
        if (dlg != NULL) {
            m_seq->setAutoConnect(dlg->getAutoConnect());
            m_seq->setOutputConn(dlg->getOutputConnection());
            m_seq->setInputConn(dlg->getInputConnection());
            m_instrument = dlg->getInstrumentName();
            m_bank = dlg->getBankName();
            m_program = dlg->getProgramName();
            applyInstrumentSettings();
            m_seq->setWeakNote(dlg->getWeakNote());
            m_seq->setStrongNote(dlg->getStrongNote());
            m_seq->setResolution(dlg->getResolution());
            m_seq->setChannel(dlg->getChannel());
            m_seq->setSendNoteOff(dlg->getSendNoteOff());
            m_seq->setNoteDuration(dlg->getDuration());
            m_seq->connect_output();
            m_seq->connect_input();
            m_seq->sendInitialControls();
        }
    }
    delete dlg;
}

void KMetronome::updateDisplay(int bar, int beat)
{
    m_view->display(bar, beat);
}

void KMetronome::tempoChanged(int newTempo)
{
    m_seq->setBpm(newTempo);
    m_seq->metronome_set_tempo();
}

void KMetronome::beatsBarChanged(int beats)
{
    m_seq->setRhythmNumerator(beats);
}

void KMetronome::rhythmFigureChanged(int figure)
{
    m_seq->setRhythmDenominator((int)pow(2, figure));
}

void KMetronome::weakVeloChanged(int vel)
{
    m_seq->setWeakVelocity(vel);
}

void KMetronome::strongVeloChanged(int vel)
{
    m_seq->setStrongVelocity(vel);
}

void KMetronome::volumeChanged(int vol)
{
    m_seq->setVolume(vol);
	m_seq->sendControlChange(VOLUME_CC, vol);
}

void KMetronome::balanceChanged(int bal)
{
    m_seq->setBalance(bal);
	m_seq->sendControlChange(PAN_CC, bal);
}

void KMetronome::toggle(bool checked)
{
    if (checked) play();
    else stop();
}

/**
 * Public D-Bus Interface Slots
 */

void KMetronome::play()
{
    m_view->enableControls(false);
    m_prefs->setEnabled(false);
    m_playStop->setChecked(true);
    m_seq->metronome_start();
    updateDisplay(1, 0);
}

void KMetronome::stop()
{
    m_seq->metronome_stop();
    m_view->enableControls(true);
    m_prefs->setEnabled(true);
    m_playStop->setChecked(false);
}

void KMetronome::cont()
{
    m_view->enableControls(false);
    m_prefs->setEnabled(false);
    m_seq->metronome_continue();
}

void KMetronome::setTempo(int newTempo)
{
    if (newTempo < TEMPO_MIN || newTempo > TEMPO_MAX)
        return;
    m_view->setTempo(newTempo);
}

void KMetronome::setTimeSignature(int numerator, int denominator)
{
    static const int valids[] = {1, 2, 4, 8, 16, 32, 64};
    bool invalid = true;
    for(int i=0; i<7; ++i) {
        if (denominator == valids[i]) {
            invalid = false;
            break;
        }
    }
    if (m_seq->isPlaying() ||  numerator < 1 || numerator > 32 || invalid)
        return;

    m_view->setBeatsBar(numerator);
    m_view->setFigure(denominator);
    m_seq->setRhythmDenominator(denominator);
}

/**
 * Patterns stuff
 */

void KMetronome::updatePatterns()
{
    const int n(QSTR_PATTERN.size());
    QStringList lst;
    QSettings settings;
    foreach(const QString& name, settings.childGroups()) {
        if (name.startsWith(QSTR_PATTERN))
            lst += name.mid(n);
    }
    m_view->setPatterns(lst);
}

void KMetronome::readDrumGridPattern()
{
    if (m_drumgrid == NULL) {
        m_drumgrid = new DrumGrid(this);
        m_drumgrid->setModel(m_model);
        m_drumgrid->setSequencer(m_seq);
    }
    m_drumgrid->setInstrument(m_instrument);
    if (m_view->patternMode())
        m_drumgrid->readPattern(m_view->getSelectedPattern());
}

void KMetronome::editPatterns()
{
    int res;
    QString tmpPattern;
    readDrumGridPattern();
    res = m_drumgrid->exec();
    updatePatterns();
    if (res == QDialog::Accepted && m_drumgrid != NULL)
        tmpPattern = m_drumgrid->currentPattern();
    tempoChanged(m_view->getTempo());
    m_view->setSelectedPattern(tmpPattern);
    m_seq->setPatternMode(m_view->patternMode());
}

void KMetronome::patternChanged(int /*idx*/)
{
    if (m_view->patternMode())
        readDrumGridPattern();
    m_seq->setPatternMode(m_view->patternMode());
    if (m_view->patternMode()) {
        m_view->setBeatsBar(m_model->columnCount());
        m_view->setFigure(m_model->patternFigure());
    }
}

void KMetronome::importPatterns(const QString& path)
{
    QSettings settings;
    QSettings input(path, QSettings::IniFormat, this);
    foreach(const QString& name, input.childGroups()) {
        input.beginGroup(name);
        settings.beginGroup(QSTR_PATTERN+name);
        settings.remove("");
        foreach(const QString& key, input.childKeys()) {
            settings.setValue(key, input.value(key));
        }
        settings.endGroup();
        input.endGroup();
    }
    settings.sync();
    updatePatterns();
}

void KMetronome::exportPatterns(const QString& path)
{
    QSettings settings;
    QSettings output(path, QSettings::IniFormat, this);
    const int n(QSTR_PATTERN.size());
    QStringList lst;
    foreach(const QString& name, settings.childGroups()) {
        if (name.startsWith(QSTR_PATTERN)) {
            lst += name.mid(n);
        }
    }
    if (!lst.isEmpty()) {
        foreach(const QString& name, lst) {
            settings.beginGroup(QSTR_PATTERN+name);
            output.beginGroup(name);
            output.remove("");
            foreach(const QString& key, settings.childKeys()) {
                output.setValue(key, settings.value(key));
            }
            output.endGroup();
            settings.endGroup();
        }
        output.sync();
    }
}

void KMetronome::importPatterns()
{
    QUrl u = QFileDialog::getOpenFileUrl(this, tr("Import Patterns"),
                QUrl("kfiledialog:///KMetronome"),
                tr("*.pat|Pattern Files (*.pat)"));
    if (!u.isEmpty()) {
        QString path = u.toLocalFile();
        if (!path.isNull())
            importPatterns(path);
    }
}

void KMetronome::exportPatterns()
{
    QUrl u = QFileDialog::getSaveFileUrl(this, tr("Export Patterns"),
                QUrl("kfiledialog:///KMetronome"),
                tr("*.pat|Pattern Files (*.pat)"));
    if (!u.isEmpty()) {
        QString path = u.toLocalFile();
        if (!path.isNull())
            exportPatterns(path);
    }
}

/*void KMetronome::setupPlaces()
{
    KFilePlacesModel *placesModel = new KFilePlacesModel;
    QString drums =  KStandardDirs::locate("appdata", "drums.ins");
    if (!drums.isEmpty()) {
        QFileInfo info(drums);
        KUrl samples(info.absolutePath());
        if (placesModel->url(placesModel->closestItem(samples)) != samples) {
            placesModel->addPlace(tr("Patterns"), samples,
                KApplication::applicationName(), KApplication::applicationName());
        }
    }
    delete placesModel;
}*/
