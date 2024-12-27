% Hilfeindex

# Einführung

Drumstick Metronome ist ein MIDI-basiertes Metronom, das den ALSA-Sequenzer verwendet.

Zielgruppe sind Musiker und Musikstudenten. Wie jedes körperliche
Metronom ist ein Werkzeug, um den Rhythmus beim Musizieren zu halten
Instrumente.

Es verwendet MIDI anstelle von digitalem Audio, was eine geringe CPU-Auslastung ermöglicht und sehr genau ist 
Timing dank ALSA-Sequenzer. Das bedeutet, dass Sie auch einen MIDI . benötigen
Synthesizer zur Tonerzeugung, da das Programm keinen Ton erzeugt
von selbst. Die besten Ergebnisse erzielen Sie mit einem Hardware-Synthesizer, aber Sie können
Verwenden Sie stattdessen auch Software-Synthesizer.

Fragen Sie nach Support, melden Sie Probleme, Fehler und Funktionsanfragen an das Tracking-System unter 
[SourceForge-Projektseite](https://sourceforge.net/p/kmetronome/support-requests/)

# Verwenden des Drumstick-Metronoms

## Merkmale

Sie müssen nur einige Steuerelemente anpassen, wie den Tempo-Schieberegler, die
Beats/Bar-Spin-Box und den Beat-Längen-Selektor. Drücken Sie die Play-Taste
anfangen. Verwenden Sie nach Belieben die Stopptaste.

Das Tempo kann mit dem Schieberegler von 25 bis 250 QPM eingestellt werden. Die Einheiten sind
Viertel pro Minute (Mälzel Metronom-Einheiten). Sie können auch doppelklicken
über dem Hauptfenster, um ein Dialogfeld zu öffnen, in dem Sie ein neues eingeben können
Tempo direkt mit der Tastatur. Es gibt auch eine Combobox zur Auswahl
und zeigen Sie das Tempo mit italienischen Musiknamen an.

Beats/Bar kann von 1 bis 32 Beats eingestellt werden. Das sind die Beats
auf jedem Takt oder Takt, und es ist der Zähler auf der Taktart als
es würde notiert werden.

Die Beatlänge ist der Nenner der Taktartangabe,
und repräsentiert die Dauer jedes Schlags. Ändern dieses Wertes nicht
ändern Sie die Bedeutung der Tempoeinheiten.

Muster ist eine Dropdown-Liste zur Auswahl einer Musterdefinition. Der Standard
Der Wert "Automatisch" bedeutet, dass das Programm Muster unter Verwendung der
im Konfigurationsdialog eingestellte Noten (stark/schwach) und den Rhythmus
Definition durch "Beats/Bar" und "Beat-Länge". Es enthält auch
die Namen von benutzerdefinierten Mustern.

## Einstieg

Dieses Programm verwendet das MIDI-Protokoll, daher ist es eine gute Idee,
einige grundlegende Begriffe zu MIDI, um die Konzepte vollständig zu verstehen
dahinter. Eine gute Einführung findest du hier:
[Was ist MIDI](https://www.midi.org/midi-articles/categories/MIDI%201.0).

Drumstick Metronome erzeugt MIDI-Events. Wenn Sie die Ereignisse hören möchten
in Klänge übersetzt, die Sie benötigen, um den MIDI OUT-Port davon zu verbinden
Programm an den MIDI IN-Port eines MIDI-Synthesizers. Es kann entweder ein
Hardware-MIDI-Synthesizer oder ein Software-Synthesizer. Wenn es ein externes ist
Hardware-Synthesizer benötigen Sie außerdem ein von ALSA unterstütztes MIDI-Interface
in Ihrem Computer installiert ist, und ein MIDI-Kabel, das an beide
MIDI-Schnittstelle des Computers und die MIDI IN-Buchse des Synthesizers.

Wenn Sie keinen externen MIDI-Synthesizer haben, können Sie trotzdem den
Programm mit einem MIDI-Software-Synthesizer, der den ALSA-Sequenzer unterstützt
Verbindungen, wie [QSynth](https://qsynth.sourceforge.io). Starte dein
Software-Synthesizer vor Drumstick Metronome, und stellen Sie dann die Verbindung her
zwischen den beiden Programmen, entweder im Konfigurationsdialog von Drumstick Metronome 
oder mit einem externen Verbindungsmanager wie 
[QJackCtl](https://qjackctl.sourceforge.io).

Vergessen Sie nicht, eine gute [Soundfont] zu installieren (https://en.wikipedia.org/wiki/SoundFont) 
in das Dialogfeld "Setup..." von QSynth.

## Aufbau

Drumstick Metronome verfügt über eingeschränkte Session-Management-Funktionen. Es kann sich erinnern
ein Anschluss für den ALSA-Ausgangsport und ein Anschluss für seinen
Eingangsport. Verbindungen werden beim Beenden des Programms gespeichert und gespeichert
am Anfang. Sie brauchen diese Funktion nicht, wenn Sie dies vorziehen
Verbindungen von Hand, mit aconnect oder einem anderen gleichwertigen Dienstprogramm, oder
wenn Sie einen externen Session-Manager wie die im Lieferumfang enthaltene Patchbay verwenden
Programm [QJackCtl](https://qjackctl.sourceforge.io).

Drumstick Metronome verwendet eine Instrumentendefinitionsdatei im .INS-Format
Format als Qtractor, TSE3, Cakewalk und Sonar. Das **Ausgabeinstrument**
Dropdown-Liste ermöglicht die Auswahl eines der Standard General MIDI,
Roland GS und Yamaha XG Drummaps. Sie können weitere Definitionen hinzufügen, indem Sie
eine Datei namens `drums.ins` unter `$HOME/.local/share/kmetronome.sourceforge.net`. Die
Inhalt der Dropdown-Listen **Bank**, **Programm**, **Schwach** und **Starke Note** 
hängen auch von dieser Definition ab.

**Kanal** ist normalerweise 10, d. h. der Percussion-Kanal eines General MIDI
Synthesizer. Es muss eine Zahl zwischen 1 und 16 sein.

**Auflösung** ist die Anzahl der Ticks (Zeiteinheiten) für jede Viertelnote.
Wertebereich von 48 bis 960. Standardeinstellung bis 120.

**Notendauer** ist die Länge (in Anzahl von Ticks) der Zeitspanne
zwischen einem NOTE ON und dem entsprechenden NOTE OFF-Ereignis. Diese Kontrolle ist
nur aktiviert, wenn **Sende NOTE OFF-Ereignisse** ebenfalls aktiviert ist. Sehr niedrige Werte
kann bei einigen Synthesizern gedämpfte Klicks verursachen.

Percussion-Sounds brauchen normalerweise keine NOTE OFF-Events, um danach gesendet zu werden
jede ANMERKUNG AN. Aktivieren Sie das Kontrollkästchen **ANMERKUNG AUS-Ereignisse senden** nur, wenn Sie
Synthesizer oder Instrument unterstützt oder erfordert diese Einstellung.

**Bank** und **Programm** wird verwendet, um das Schlagzeug für Instrumente zu ändern
unterstützt mehrere Einstellungen. Viele Synthesizer verstehen kein Programm
Änderungen für den Percussion-Kanal.

Im **Automatic**-Pattern-Modus wird der **Starke Note**-Sound als erster Beat gespielt 
in jedem Takt, während jeder andere Beat im gleichen Takt mit . gespielt wird 
der **schwache Ton**. Die Zahlenwerte 33 und 34 sind die GM2- und XG-Sounds
für Metronomklick bzw. Metronomglocke.

##Muster-Editor

In diesem Dialogfeld können Sie Muster bearbeiten, testen und auswählen. Erschaffen
neue Muster speichern Sie einfach die aktuelle Definition unter einem neuen Namen.
Muster werden durch eine Tabelle dargestellt. Die Zeilen in der Tabelle entsprechen
die Percussion-Klänge. Sie können Zeilen aus einer Liste von Sounds entfernen und hinzufügen
durch die Geräteeinstellungen im Konfigurationsdialog definiert. Die
Anzahl der Spalten in der Tabelle bestimmen die Länge des Musters,
zwischen 1 und 99 Elementen einer beliebigen Beatlänge.

Jede Tabellenzelle akzeptiert Werte zwischen N=1 und 9, entsprechend der
MIDI-Velocity (N*127/9) der Noten oder 0, um den Klang zu löschen. Gültig
Werte sind auch f (=forte) und p (=piano) entsprechend Variable
Geschwindigkeiten, die durch die Drehknöpfe (Strong/Weak) im Hauptfenster definiert werden.
Die Zellenwerte können entweder über die Tastatur ausgewählt und geändert werden
oder die Maus. Es ist nicht erforderlich, die Wiedergabe zu stoppen, bevor Sie die
Zellen.

# Befehlsreferenz

## Das Hauptfenster

### Das Dateimenü

**Datei → Muster importieren**

: Importiert Pattern-Definitionen in Drumstick Metronome

**Datei → Muster exportieren**

: Exportiert Pattern-Definitionen von Drumstick Metronome

**Datei → Wiedergabe/Stopp**

: Steuert die Pattern-Wiedergabe

**Datei → Beenden**

: Beendet Drumstick Metronom

### Das Bearbeiten-Menü

**Bearbeiten → Muster bearbeiten**

: Öffnet den Mustereditor

### Das Einstellungsmenü

**Einstellungen → Aktionsschaltflächen anzeigen**

: Zeigt die Aktionsschaltflächen an oder blendet sie aus. Für jede Schaltfläche gibt es äquivalente Menüpunkte.

**Einstellungen → Symbolleiste anzeigen**

: Blendet die Symbolleiste ein oder aus. Für jede Schaltfläche gibt es äquivalente Menüpunkte.

**Einstellungen → Konfiguration**

: Konfiguriert das Drumstick-Metronom

### Das Hilfemenü

**Hilfe → Hilfeinhalt**

: Öffnet das Hilfefenster

**Hilfe → Sprache**

: Öffnet ein Untermenü mit den verfügbaren Übersetzungen

**Hilfe → Über**

: Öffnet eine Infobox dieses Programms

**Hilfe → über Qt**

: Öffnet eine Infobox der Qt-Frameworks

# Externe Steuerung

Drumstick Metronom kann über seine D-Bus-Schnittstelle, System Realtime . gesteuert werden
und systemexklusive MIDI-Meldungen.

## D-Bus-Schnittstelle 

Drumstick Metronome bietet einige Funktionen über seine D-Bus-Schnittstelle. Du kannst
Verwenden Sie ein D-Bus-fähiges Programm, um das Drumstick-Metronom wie `qdbusviewer` oder das . zu steuern 
Kommandozeilen-Dienstprogramm `qdbus`.

Diese Befehle können beispielsweise von einer Shell-Eingabeaufforderung verwendet werden:

    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.play
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.stop
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.cont
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTempo 150
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTimeSignature 3 8

## Universelle systemexklusive Nachrichten

Drumstick Metronome versteht einige Universal System Exclusive-Meldungen. Weil
die Geräte-ID ist noch nicht implementiert, alle erkannten Nachrichten müssen
als Broadcast markiert werden (0x7F).

Echtzeitnachricht: Nachricht zur Änderung der Taktart

    Format: 0xF0 0x7F 0x7F 0x03 Befehlslänge Zähler Nenner ... 0xF7
                                0x02: Taktartänderung 
                                0x42: Taktartwechsel verzögert

Hinweise: Beide Befehle wirken sich nur auf die nächsten geplanten Muster nach . aus
den Befehl erhalten. Die Länge beträgt mindestens 2. Zusätzliche Bytes folgen
Zähler und Nenner werden ignoriert.

MMC-Nachrichten

    Format: 0xF0 0x7F 0x7F 0x06 Befehl 0xF7
                                0x01: Stopp
                                0x02: Abspielen
                                0x03: Aufgeschobenes Spiel

## MIDI-System-Echtzeitschnittstelle

Sie können ein MIDI-Gerät verwenden, das MIDI-System-Echtzeit-Meldungen erzeugt, um
Drumstick Metronom steuern. Akzeptierte System-Echtzeitnachrichten sind: Start (0xFA),
Stopp (0xFC) und Weiter (0xFB).

Viele Master-MIDI-Keyboards und MIDI-Tastengeräte bieten diese drei
MIDI-Transportsteuerung. Sie müssen Ihre externen MIDI-Geräte anschließen
über MIDI- (oder USB-)Kabel mit dem Computer und schließen Sie das
entsprechenden ALSA-Port mit dem Eingangsport des Drumstick-Metronoms. Diese Verbindung
muss von Hand erfolgen oder mit einer Sitzungsverwaltungssoftware wie
QJackCtl.

# Credits und Lizenz

Programm Copyright © 2005-2024 Pedro Lopez-Cabanillas

Dokumentation Copyright © 2005-2024 Pedro Lopez-Cabanillas

Dieses Programm ist freie Software; Sie können es weitergeben und/oder ändern
es unter den Bedingungen der GNU General Public License, wie veröffentlicht von
die Free Software Foundation; entweder Version 2 der Lizenz oder
(nach Ihrer Wahl) jede spätere Version.

Dieses Programm wird in der Hoffnung verteilt, dass es nützlich sein wird,
aber OHNE JEGLICHE GARANTIE; auch ohne die stillschweigende garantie von
MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Siehe die
GNU General Public License für weitere Details.
                                                                        
Sie sollten eine Kopie der GNU General Public License erhalten haben
zusammen mit diesem Programm. Falls nicht, siehe [https://www.gnu.org/licenses](https://www.gnu.org/licenses)

# Installation
 
## So erhalten Sie ein Drumstick-Metronom

Hier finden Sie die neueste Version: 
[Projektdateien](https://sourceforge.net/projects/kmetronome/files/kmetronome/)

## Anforderungen

Um Drumstick Metronom erfolgreich zu kompilieren und zu verwenden, benötigen Sie Qt 5 oder
später Drumstick 2, ALSA-Treiber und ALSA-Bibliothek.

Sie benötigen auch einen MIDI-Synthesizer, um Sound zu erzeugen.

Das Build-System erfordert [CMake](https://cmake.org) 3.14 oder neuer.

ALSA-Bibliothek, Treiber und Dienstprogramme finden Sie unter 
[ALSA-Homepage](https://www.alsa-project.org).

Drumstick::ALSA ist ein C++-Wrapper um die ALSA-Sequenzer-Schnittstelle mit Qt
Objekte, Idiome und Stile. Sie finden es bei der
[Drumstick-Startseite](https://drumstick.sourceforge.io).

Siehe auch die [Drumstick Metronome Homepage](https://kmetronome.sourceforge.io/kmetronome.shtml).

## Kompilierung und Installation

Um Drumstick Metronome auf Ihrem System zu kompilieren und zu installieren, geben Sie Folgendes ein:
im Basisverzeichnis der Drumstick Metronome Distribution:

    % cmake .
    % machen
    % sudo machen installieren

Da Drumstick Metronome `cmake` und `make` verwendet, sollten Sie keine Probleme haben
es kompilieren. Sollten Probleme auftreten, melden Sie diese bitte dem
Autor oder des Projekts 
[Fehlerverfolgungssystem](https://sourceforge.net/p/kmetronome/bugs/).

