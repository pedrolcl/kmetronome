# Drumstick Metronome (a.k.a. kmetronome)

[Drumstick Metronome](https://kmetronome.sourceforge.io) is a MIDI based 
metronome using the ALSA sequencer.

The intended audience are musicians and music students. Like any physical 
metronome, it is a tool to keep the rhythm while playing musical instruments.

It uses MIDI instead of digital audio, allowing low CPU usage and very accurate 
timing thanks to the ALSA sequencer. This means that you also need a MIDI 
synthesizer for sound generation, because the program does not produce any sound
by itself. The best results come from using a hardware synthesizer, but you may
also use software synthesizers instead.

For brief building instructions, see INSTALL.

## Downloads

Sources: https://sourceforge.net/projects/kmetronome/files/

[![Download Drumstick Metronome](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/kmetronome/files/latest/download)

[<img width='240' alt='Download on Flathub' src='https://flathub.org/assets/badges/flathub-badge-en.png'/>](https://flathub.org/apps/details/net.sourceforge.kmetronome)

[![Packaging status](https://repology.org/badge/vertical-allrepos/kmetronome.svg)](https://repology.org/project/kmetronome/versions)

## Developers environment

You need the following software:

* CMake 3.14 or later
* Qt libraries 5.12 or later
* ALSA library 
* Drumstick libraries 2.0 or later

## Getting the development sources

Compiling and hacking the SVN sources is a bit different compared to the
distribution tarball. You can get the latest sources either using a sourceforge
user account, or the anonymous user (with read only rights). The SVN client  
documentation for SourceForge users is available at
https://sourceforge.net/p/forge/documentation/SVN%20Overview/

1. Check out the module drumstick from the SVN repository.
        Host: svn.code.sf.net
        Path: /p/drumstick/code/trunk
        Module: drumstick

There is also a [Git mirror at GitHub](https://github.com/pedrolcl/drumstick)

example:

~~~
$ svn checkout https://svn.code.sf.net/p/drumstick/code/trunk drumstick
~~~

The module 'drumstick' is also used by other projects in a similar way as an 
independent shared dynamic library.

You need to configure, compile and optionally install drumstick libraries.

2. Check out the module kmetronome from the SVN repository.
        Host: svn.code.sf.net
        Path: /p/kmetronome/code/trunk
        Module: kmetronome

There is also a [Git mirror at GitHub](https://github.com/pedrolcl/kmetronome)

example:

~~~
$ svn checkout https://svn.code.sf.net/p/kmetronome/code/trunk kmetronome 
~~~

4. Configure and compile

~~~
$ cd kmetronome
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=debug \
           -DCMAKE_PREFIX_PATH=$HOME/Qt/5.12.11/gcc_64/ \
           -DCMAKE_INSTALL_PREFIX=/usr/local/
$ make VERBOSE=1
~~~

5. Hack and enjoy!
