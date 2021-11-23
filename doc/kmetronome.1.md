% KMETRONOME(1) kmetronome 0.0.0 | Drumstick MIDI Metronome
% Pedro López-Cabanillas <plcl@users.sf.net>

# NAME

**kmetronome** - Drumstick MIDI metronome using ALSA sequencer and QT user interface

# SYNOPSIS

| **kmetronome** \[standard options]

# DESCRIPTION

Drumstick Metronome is a MIDI metronome with QT interface based on the
ALSA sequencer. It is a tool for musicians and music students to keep
the rhythm while playing musical instruments. It uses MIDI for sound
generation instead of digital audio, allowing low CPU usage, and very
accurate timing thanks to the ALSA sequencer.

## Standard Options

The following options apply to all Qt5 applications.

`-style=` style / `-style` style

:   Set the application GUI style. Possible values depend on the system
    configuration. If Qt is compiled with additional styles or has
    additional styles as plugins these will be available to the `-style`
    command line option.

`-stylesheet=` stylesheet / `-stylesheet` stylesheet

:   Set the application styleSheet. The value must be a path to a file
    that contains the Style Sheet.

`-widgetcount`

:   Print debug message at the end about number of widgets left
    undestroyed and maximum number of widgets existed at the same time.

`-reverse`

:   Set the application's layout direction to Qt::RightToLeft. This
    option is intended to aid debugging and should not be used in
    production. The default value is automatically detected from the
    user's locale (see also QLocale::textDirection()).

`-platform` platformName\[:options\]

:   Specify the Qt Platform Abstraction (QPA) plugin.

`-platformpluginpath` path

:   Specify the path to platform plugins.

`-platformtheme` platformTheme

:   Specify the platform theme.

`-plugin` plugin

:   Specify additional plugins to load. The argument may appear multiple
    times.

`-qwindowgeometry` geometry

:   Specify the window geometry for the main window using the
    X11-syntax. For example: -qwindowgeometry 100x100+50+50

`-qwindowicon` icon

:   Set the default window icon.

`-qwindowtitle` title

:   Set the title of the first window.

`-session` session

:   Restore the application from an earlier session.

`-display` hostname:screen\_number

:   Switch displays on X11. Overrides the DISPLAY environment variable.

`-geometry` geometry

:   Specify the window geometry for the main window on X11. For example:
    -geometry 100x100+50+50

`-dialogs=` \[xp|none\]

:   Only available for the Windows platform. XP uses native style
    dialogs and none disables them.

`-fontengine=` freetype

:   Use the FreeType font engine.

# BUGS

See Tickets at Sourceforge <https://sourceforge.net/p/kmetronome/>

# SEE ALSO

**qt5options (7)**

# LICENSE

This manual page was written by Pedro Lopez-Cabanillas
<plcl@users.sourceforge.net> Permission is granted to copy, distribute
and/or modify this document under the terms of the GNU General Public
License, Version 2 or any later version published by the Free Software
Foundation; considering as source code all the file that enable the
production of this manpage.
