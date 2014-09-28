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

#include <cmath>
#include <QEvent>
#include <QLabel>
#include <QStandardPaths>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QInputDialog>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QtDBus/QDBusConnection>

#include <QDebug>

#include "kmetronome.h"
#include "kmetropreferences.h"
#include "sequenceradapter.h"
#include "defs.h"
#include "drumgrid.h"
#include "drumgridmodel.h"
#include "instrument.h"
#include "about.h"
#include "kmetronome_adaptor.h"

static QDir dataDirectory()
{
    QDir test(QApplication::applicationDirPath() + "/../share/kmetronome/");
    if (test.exists())
        return test;
    QStringList candidates = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    foreach(const QString& d, candidates) {
        test = QDir(d);
        if (test.exists())
            return test;
    }
    return QDir();
}

static QDir localeDirectory()
{
    QDir data = dataDirectory();
    data.cd("locale");
    return data;
}

KMetronome::KMetronome(QWidget *parent) :
    QMainWindow(parent),
    m_patternMode(false),
    m_seq(0)
{
    new KmetronomeAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/", this);
    dbus.registerService("net.sourceforge.kmetronome");

    m_trq = new QTranslator(this);
    m_trp = new QTranslator(this);
    m_trq->load( "qt_" + configuredLanguage(), QLibraryInfo::location(QLibraryInfo::TranslationsPath) );
    m_trp->load( configuredLanguage(), localeDirectory().absolutePath() );
    QApplication::installTranslator(m_trq);
    QApplication::installTranslator(m_trp);

    m_ui.setupUi(this);
    m_ui.m_exitbtn->setFocusPolicy(Qt::NoFocus);
    m_ui.m_playbtn->setShortcut(Qt::Key_MediaPlay);
    m_ui.m_stopbtn->setShortcut(Qt::Key_MediaStop);

    /**
     * Tempo reference:
     * http://www.music.vt.edu/musicdictionary/appendix/tempo/tempo1.html
     */
    m_ui.m_air->addItem("Larghissimo", 20);
    m_ui.m_air->addItem("Largo",       40);
    m_ui.m_air->addItem("Larghetto",   60);
    m_ui.m_air->addItem("Adagio",      70);
    m_ui.m_air->addItem("Andante",     90);
    m_ui.m_air->addItem("Moderato",   110);
    m_ui.m_air->addItem("Allegro",    120);
    m_ui.m_air->addItem("Vivace",     160);
    m_ui.m_air->addItem("Presto",     170);
    m_ui.m_air->addItem("Prestissimo",200);
    m_ui.m_air->setCurrentIndex(4);

    connect( m_ui.m_exitbtn, SIGNAL(clicked()), SLOT(close()) );
    connect( m_ui.m_configbtn, SIGNAL(clicked()), SLOT(optionsPreferences()) );
    connect( m_ui.m_patternbtn, SIGNAL(clicked()), SLOT(editPatterns()) );
    connect( m_ui.m_playbtn, SIGNAL(clicked()), SLOT(play()) );
    connect( m_ui.m_stopbtn, SIGNAL(clicked()), SLOT(stop()) );
    connect( m_ui.m_beatsBar, SIGNAL(valueChanged(int)), SLOT(beatsBarChanged(int)) );
    connect( m_ui.m_figure, SIGNAL(activated(int)), SLOT(rhythmFigureChanged(int)) );
    connect( m_ui.m_tempo, SIGNAL(valueChanged(int)), SLOT(tempoChanged(int)) );
    connect( m_ui.m_dial1, SIGNAL(valueChanged(int)), SLOT(weakVeloChanged(int)) );
    connect( m_ui.m_dial2, SIGNAL(valueChanged(int)), SLOT(strongVeloChanged(int)) );
    connect( m_ui.m_dial3, SIGNAL(valueChanged(int)), SLOT(volumeChanged(int)) );
    connect( m_ui.m_dial4, SIGNAL(valueChanged(int)), SLOT(balanceChanged(int)) );
    connect( m_ui.m_air, SIGNAL(activated(int)), SLOT(tempoComboChanged(int)) );
    connect( m_ui.m_tempo, SIGNAL(valueChanged(int)), SLOT(displayTempo(int)) );
    connect( m_ui.m_pattern, SIGNAL(activated(int)), SLOT(patternChanged(int)) );

    m_model = new DrumGridModel(this);
    m_instrumentList = new InstrumentList;
    m_model->setInstrumentList(m_instrumentList);
    QString drums =  dataDirectory().filePath("drums.ins");
    if (!drums.isEmpty()) {
        m_instrumentList->load(drums);
    }
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
        setupActions();
        readConfiguration();
        createLanguageMenu();
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
    m_ui.actionAboutQt->setIcon(QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
    connect( m_ui.actionPlayStop, SIGNAL(triggered(bool)), SLOT(toggle(bool)) );
    connect( m_ui.actionImportPatterns, SIGNAL(triggered()), SLOT(importPatterns()) );
    connect( m_ui.actionExportPatterns, SIGNAL(triggered()), SLOT(exportPatterns()) );
    connect( m_ui.actionQuit, SIGNAL(triggered()), SLOT(close()) );
    connect( m_ui.actionEditPatterns, SIGNAL(triggered()), SLOT(editPatterns()) );
    connect( m_ui.actionShowActionButtons, SIGNAL(triggered(bool)), SLOT(displayFakeToolbar(bool)));
    connect( m_ui.actionConfiguration, SIGNAL(triggered()), SLOT(optionsPreferences()) );
    connect( m_ui.actionAbout, SIGNAL(triggered()), SLOT(about()) );
    connect( m_ui.actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
    connect( m_ui.actionHelp, SIGNAL(triggered()), SLOT(help()) );
}

void KMetronome::closeEvent(QCloseEvent *event)
{
    saveConfiguration();
    QMainWindow::closeEvent(event);
}

void KMetronome::about()
{
    About dlg(this);
    dlg.exec();
}

void KMetronome::help()
{
    QString hlpFile = QLatin1Literal("kmetronome.html");
    QDir data = dataDirectory();
    QFileInfo finfo(data.filePath(hlpFile));
    if (finfo.exists()) {
        QUrl url = QUrl::fromLocalFile(finfo.absoluteFilePath());
        QDesktopServices::openUrl(url);
    }
}

void KMetronome::saveConfiguration()
{
    QSettings settings;
    settings.beginGroup("Settings");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("language", m_language);
    if (m_seq != 0) {
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
        settings.setValue("fakeToolbar", m_ui.actionShowActionButtons->isChecked());
    }
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
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
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
    setBeatsBar(ts_num);
    setFigure(ts_div);
    setTempo(tempo);
    displayWeakVelocity(weakVel);
    displayStrongVelocity(strongVel);
    displayVolume(volume);
    displayBalance(balance);
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
    m_seq->sendInitialControls();
    bool fakeToolbar = settings.value("fakeToolbar", true).toBool();
    m_ui.actionShowActionButtons->setChecked(fakeToolbar);
    displayFakeToolbar(fakeToolbar);
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
    display(bar, beat);
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
    enableControls(false);
    m_ui.actionConfiguration->setEnabled(false);
    m_ui.actionPlayStop->setChecked(true);
    m_seq->metronome_start();
    updateDisplay(1, 0);
    m_ui.m_stopbtn->setFocus();
}

void KMetronome::stop()
{
    m_seq->metronome_stop();
    enableControls(true);
    m_ui.actionConfiguration->setEnabled(true);
    m_ui.actionPlayStop->setChecked(false);
    m_ui.m_playbtn->setFocus();
}

void KMetronome::cont()
{
    enableControls(false);
    m_ui.actionConfiguration->setEnabled(false);
    m_seq->metronome_continue();
}

void KMetronome::setTempo(int newTempo)
{
    if (newTempo < TEMPO_MIN || newTempo > TEMPO_MAX)
        return;
    m_ui.m_tempo->setValue(newTempo);
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

    setBeatsBar(numerator);
    setFigure(denominator);
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
    setPatterns(lst);
}

void KMetronome::readDrumGridPattern()
{
    if (m_drumgrid == NULL) {
        m_drumgrid = new DrumGrid(this);
        m_drumgrid->setModel(m_model);
        m_drumgrid->setSequencer(m_seq);
    }
    m_drumgrid->setInstrument(m_instrument);
    if (m_patternMode)
        m_drumgrid->readPattern(getSelectedPattern());
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
    tempoChanged(getTempo());
    setSelectedPattern(tmpPattern);
    m_seq->setPatternMode(patternMode());
}

void KMetronome::patternChanged(int idx)
{
    m_patternMode = (idx > 0);
    if (m_patternMode) {
        readDrumGridPattern();
    }
    m_seq->setPatternMode(m_patternMode);
    if (m_patternMode) {
        setBeatsBar(m_model->columnCount());
        setFigure(m_model->patternFigure());
    }
    m_ui.m_beatsBar->setEnabled(!m_patternMode);
    m_ui.m_figure->setEnabled(!m_patternMode);
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
    QString dirName = QStandardPaths::locate(QStandardPaths::DataLocation, "*.pat", QStandardPaths::LocateDirectory) ;
    QString path = QFileDialog::getOpenFileName(this, tr("Import Patterns"),dirName,tr("Pattern Files (*.pat)"));
    if (!path.isEmpty()) {
        importPatterns(path);
    }
}

void KMetronome::exportPatterns()
{
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(dirName);
    if (!dir.exists()) {
        dir.mkpath(dirName);
    }
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setWindowTitle(tr("Export Patterns"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Pattern Files (*.pat)"));
    dialog.setDirectory(dirName);
    dialog.setDefaultSuffix("pat");
    if (dialog.exec() == QDialog::Accepted) {
        QStringList files = dialog.selectedFiles();
        if (!files.isEmpty()) {
            QString path = files.at(0);
            if (!path.isEmpty()) {
                exportPatterns(path);
            }
        }
    }
}

void KMetronome::display(int bar, int beat)
{
    m_ui.m_measureLCD->display(QString("%1:%2").arg(bar,  6, 10, QChar(' '))
                                          .arg(beat, 2, 10, QChar('0')));
}

void KMetronome::setFigure(int newValue)
{
    int ts_dd;
    int x = newValue;
    for(ts_dd = 0; x > 1; x /= 2)
        ++ts_dd;
    m_ui.m_figure->setCurrentIndex(ts_dd);
}

void KMetronome::displayTempo(int newTempo)
{
    int i, j = 0;
    m_ui.m_tempoLCD->display(newTempo);
    for(i = 0; i < m_ui.m_air->count(); ++i) {
        if (m_ui.m_air->itemData(i).toInt() > newTempo) break;
        j = i;
    }
    m_ui.m_air->setCurrentIndex(j);
}

void KMetronome::enableControls(bool e)
{
    m_ui.m_playbtn->setEnabled(e);
    m_ui.m_stopbtn->setEnabled(!e);
    m_ui.m_configbtn->setEnabled(e);
    m_ui.m_patternbtn->setEnabled(e);
    m_ui.m_beatsBar->setEnabled(e & !m_patternMode);
    m_ui.m_figure->setEnabled(e & !m_patternMode);
    m_ui.m_pattern->setEnabled(e);
}

void KMetronome::mouseDoubleClickEvent(QMouseEvent *)
{
    bool ok = false;
    int newTempo = QInputDialog::getInt(this, tr("Tempo"), tr("Enter new Tempo:"),
                        m_ui.m_tempo->value(), TEMPO_MIN, TEMPO_MAX, 1, &ok );
    if (ok) {
        m_ui.m_tempo->setValue(newTempo);
    }
}

void KMetronome::tempoComboChanged(int v)
{
    m_ui.m_tempo->setValue(m_ui.m_air->itemData(v).toInt());
}

void KMetronome::setPatterns(const QStringList& patterns)
{
    m_ui.m_pattern->clear();
    m_ui.m_pattern->addItem(tr("Automatic", "the pattern is created automatically"));
    m_ui.m_pattern->addItems(patterns);
}

QString KMetronome::getSelectedPattern()
{
    return m_ui.m_pattern->currentText();
}

void KMetronome::setSelectedPattern(const QString& pattern)
{
    if (pattern.isEmpty()) {
        patternChanged(0);
    } else {
        m_ui.m_pattern->setCurrentText(pattern);
        patternChanged(m_ui.m_pattern->currentIndex());
    }
}


QString KMetronome::configuredLanguage()
{
    if (m_language.isEmpty()) {
        QSettings settings;
        QString defLang = QLocale::system().name();
        settings.beginGroup("Settings");
        m_language = settings.value("language", defLang).toString();
        settings.endGroup();
        //qDebug() << Q_FUNC_INFO << m_language;
    }
    return m_language;
}

void KMetronome::slotSwitchLanguage(QAction *action)
{
    QString lang = action->data().toString();
    QLocale qlocale(lang);
    QString localeName = QLocale::languageToString(qlocale.language());
    if ( QMessageBox::question (this, tr("Language Changed"),
            tr("The language for this application is going to change to %1. "
               "Do you want to continue?").arg(localeName),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes )
    {
        m_language = lang;
        retranslateUi();
    } else {
        m_currentLang->setChecked(true);
    }
}
void KMetronome::createLanguageMenu()
{
    QString currentLang = configuredLanguage();
    QActionGroup *languageGroup = new QActionGroup(this);
    connect(languageGroup, SIGNAL(triggered(QAction *)),
            SLOT(slotSwitchLanguage(QAction *)));
    QDir dir = localeDirectory();
    QStringList fileNames = dir.entryList(QStringList("*.qm"));
    QStringList locales;
    locales << "en";
    foreach (const QString& fileName, fileNames) {
        QString locale = fileName;
        locale.truncate(locale.lastIndexOf('.'));
        locales << locale;
    }
    locales.sort();
    foreach (const QString& loc, locales) {
        QLocale qlocale(loc);
        QString localeName = QLocale::languageToString(qlocale.language());
        QAction *action = new QAction(localeName, this);
        action->setCheckable(true);
        action->setData(loc);
        m_ui.menuLanguage->addAction(action);
        languageGroup->addAction(action);
        if (currentLang.startsWith(loc)) {
            action->setChecked(true);
            m_currentLang = action;
        }
    }
}

void KMetronome::retranslateUi()
{
    m_trq->load( "qt_" + configuredLanguage(), QLibraryInfo::location(QLibraryInfo::TranslationsPath) );
    m_trp->load( configuredLanguage(), localeDirectory().absolutePath() );
    m_ui.retranslateUi(this);
    m_ui.menuLanguage->clear();
    createLanguageMenu();
}
