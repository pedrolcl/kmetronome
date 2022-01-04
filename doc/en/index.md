% Help Index

# Introduction

Drumstick Metronome is a MIDI based metronome using the ALSA sequencer.

The intended audience are musicians and music students. Like any physical 
metronome it is a tool to keep the rhythm while playing musical
instruments.

It uses MIDI instead of digital audio, allowing low CPU usage and very accurate 
timing thanks to the ALSA sequencer. This means that you also need a MIDI 
synthesizer for sound generation, because the program does not produce any sound
by itself. The best results come from using a hardware synthesizer, but you may
also use software synthesizers instead.

Ask for support, report problems, bugs and feature requests to the tracking system at 
[SourceForge project site](https://sourceforge.net/p/kmetronome/support-requests/)

# Using Drumstick Metronome

## Features

You only need to adjust some controls, like the tempo slider, the
beats/bar spin box and the beat length selector. Press the play button
to start. Use the stop button at your convenience.

Tempo can be set from 25 to 250 QPM using the slider. The units are
quarters per minute (Mälzel Metronome units). You can also double click
over the main window to open a dialog box where you can enter a new
tempo directly with the keyboard. There is also a combo box to choose
and display the tempo using Italian musical names.

Beats/Bar can be set from 1 to 32 beats. These are the number of beats
on each measure or bar, and it is the numerator on the time signature as
it would be notated.

The beat length is the denominator on the time signature specification,
and represents the duration of each beat. Changing this value doesn't
change the meaning of the tempo units.

Pattern is a drop-down list to choose a pattern definition. The default
"Automatic" value means that the program generates patterns using the
notes set in the configuration dialog (Strong/Weak) and the rhythm
definition provided by "Beats/Bar" and "Beat length". It also contains
the names of user-defined patterns.

## Getting Started

This program uses the MIDI protocol, so it is a good idea to to have
some basic notions about MIDI in order to fully understand the concepts
behind it. You can find here a good introduction: 
[What is MIDI](https://www.midi.org/midi-articles/categories/MIDI%201.0).

Drumstick Metronome produces MIDI events. If you want to hear the events
translated into sounds you need to connect the MIDI OUT port from this
program to the MIDI IN port of a MIDI synthesizer. It can be either a
hardware MIDI synthesizer or a software one. If it is an external
hardware synthesizer, you also need an ALSA supported MIDI interface
installed in your computer, and a MIDI cable attached to both the
computer's MIDI interface, and the synthesizer MIDI IN socket.

If you don't have an external MIDI synthesizer, you can still use the
program with a MIDI software synthesizer supporting ALSA sequencer
connections, like [QSynth](https://qsynth.sourceforge.io). Start your
software synthesizer before Drumstick Metronome, and then make the connection
between the two programs, either on Drumstick Metronome's configuration dialog 
or using an external connection manager like 
[QJackCtl](https://qjackctl.sourceforge.io).

Don't forget to install a good [Sound Font](https://en.wikipedia.org/wiki/SoundFont) 
into QSynth's "Setup..." dialog.

## Configuration

Drumstick Metronome has limited session management capabilities. It can remember
one connection for the ALSA output port, and one connection for its
input port. Connections are stored when the program exits and remembered
at startup. You don't need this feature if you prefer to make such
connections by hand, using aconnect or any other equivalent utility, or
if you use an external session manager like the patchbay included in the
program [QJackCtl](https://qjackctl.sourceforge.io).

Drumstick Metronome uses an instrument definition file in .INS format, the same
format as Qtractor, TSE3, Cakewalk and Sonar. The **Output instrument**
drop-down list allows one to choose one among the standard General MIDI,
Roland GS and Yamaha XG drum maps. You can add more definitions creating
a file named `drums.ins` at `$HOME/.local/share/kmetronome.sourceforge.net`. The
contents of **Bank**, **Program**, **Weak** and **Strong note** drop-down lists 
also depend on this definition.

**Channel** is usually 10, meaning the percussion channel of a General MIDI
synthesizer. It must be a number between 1 and 16.

**Resolution** is the number of ticks (time units) for each quarter note.
Value range from 48 to 960. Defaults to 120.

**Note duration** is the length (in number of ticks) of the time span
between a NOTE ON and its corresponding NOTE OFF event. This control is
enabled only when **Send NOTE OFF events** is also enabled. Very low values
can cause muted clicks on some synthesizers.

Percussion sounds usually don't need NOTE OFF events to be sent after
every NOTE ON. Select the **Send NOTE OFF events** checkbox only if your
synthesizer or instrument supports or requires this setting.

**Bank** and **Program** is used to change the drum set for instruments
supporting several settings. Many synthesizers don't understand program
changes for the percussion channel.

In **Automatic** pattern mode, **Strong note** sound is played as the first beat 
in every measure, while any other beat in the same measure is played using 
the **Weak note** sound. The numeric values 33 and 34 are the GM2 and XG sounds
for metronome click and metronome bell respectively.

## Pattern Editor

Using this dialog box you may edit, test and select patterns. To create
new patterns, you simply save the current definition under a new name.
Patterns are represented by a table. The rows in the table correspond to
the percussion sounds. You can remove and add rows from a list of sounds
defined by the instrument settings in the configuration dialog. The
number of columns in the table determine the length of the pattern,
between 1 and 99 elements of any beat length.

Each table cell accepts values between N=1 and 9, corresponding to the
MIDI velocity (N*127/9) of the notes, or 0 to cancel the sound. Valid
values are also f (=forte) and p (=piano) corresponding to variable
velocities defined by the rotary knobs (Strong/Weak) in the main window.
The cell values can be selected and modified using either the keyboard
or the mouse. There is no need to stop the playback before modifying the
cells.

# Command Reference

## The main window

### The File Menu

**File → Import Patterns**

:   Imports pattern definitions into Drumstick Metronome

**File → Export Patterns**

:   Exports pattern definitions from Drumstick Metronome

**File → Play/Stop**

:   Controls pattern playback

**File → Quit**

:   Quits Drumstick Metronome

### The Edit Menu

**Edit → Edit Patterns**

:   Opens the pattern editor

### The Settings Menu

**Settings → Show Action Buttons**

:   Shows or hides the action buttons. There are equivalent menu items for every button.

**Settings → Show Toolbar**

:   Shows or hides the toolbar. There are equivalent menu items for every button.

**Settings → Configuration**

:   Configures Drumstick Metronome

### The Help Menu

**Help → Help Contents**

:   Opens the help window

**Help → Language**

:   Opens a sub-menu showing the available translations

**Help → about**

:   Opens an about box of this program

**Help → about Qt**

:   Opens an about box of the Qt frameworks

# External Control

Drumstick Metronome can be controlled using its D-Bus interface, System Realtime
and System Exclusive MIDI messages.

## D-Bus Interface 

Drumstick Metronome provides some functions through its D-Bus interface. You can
use a D-Bus enabled program to control Drumstick Metronome like `qdbusviewer` or the 
commandline utility `qdbus`.

For instance, these commands can be used from a shell prompt:

    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.play
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.stop
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.cont
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTempo 150
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTimeSignature 3 8

## Universal System Exclusive messages

Drumstick Metronome understands some Universal System Exclusive messages. Because
the device ID is not yet implemented, all the recognised messages must
be marked as broadcast (0x7F).

Realtime Message: Time Signature Change Message

    Format: 0xF0 0x7F 0x7F 0x03 Command Length Numerator Denominator ... 0xF7
                                0x02: Time Signature Change 
                                0x42: Time Signature Change Delayed

Notes: both commands affect only the next scheduled patterns after
receiving the command. Length is at least 2. Extra bytes following
Numerator and Denominator are ignored.

MMC Messages

    Format: 0xF0 0x7F 0x7F 0x06 Command 0xF7
                                0x01: Stop
                                0x02: Play
                                0x03: Deferred Play

## MIDI System Realtime Interface

You can use a MIDI device generating MIDI System Realtime messages to
control Drumstick Metronome. System Realtime messages accepted are: Start (0xFA),
Stop (0xFC) and Continue (0xFB).

Many master MIDI keyboards and MIDI buttons devices provide those three
MIDI transport controls. You need to connect your external MIDI devices
to the computer using MIDI (or USB) cables, and connect the
corresponding ALSA port to the input port of Drumstick Metronome. This connection
must be done by hand, or using a session management software like
QJackCtl.

# Credits and License

Program Copyright © 2005-2022 Pedro Lopez-Cabanillas

Documentation Copyright © 2005-2022 Pedro Lopez-Cabanillas

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
                                                                        
You should have received a copy of the GNU General Public License
along with this program. If not, see [https://www.gnu.org/licenses](https://www.gnu.org/licenses)

# Installation
 
## How to obtain Drumstick Metronome

Here you can find the latest version: 
[Project's files](https://sourceforge.net/projects/kmetronome/files/kmetronome/)

## Requirements

In order to successfully compile and use Drumstick Metronome, you need Qt 5 or
later, Drumstick 2, ALSA drivers and ALSA library.

You also need a MIDI synthesizer to produce sound.

The build system requires [CMake](https://cmake.org) 3.14 or newer.

ALSA library, drivers and utilities can be found at 
[ALSA home page](https://www.alsa-project.org).

Drumstick::ALSA is a C++ wrapper around the ALSA sequencer interface using Qt
objects, idioms and style. You can find it at the 
[Drumstick home page](https://drumstick.sourceforge.io).

See also the [Drumstick Metronome home page](https://kmetronome.sourceforge.io/kmetronome.shtml).

## Compilation and Installation

In order to compile and install Drumstick Metronome on your system, type the
following in the base directory of the Drumstick Metronome distribution:

    % cmake .
    % make
    % sudo make install

Since Drumstick Metronome uses `cmake` and `make` you should have no trouble
compiling it. Should you run into problems please report them to the
author or the project's 
[bug tracking system](https://sourceforge.net/p/kmetronome/bugs/).
