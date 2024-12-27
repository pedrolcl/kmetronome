# Drumstick Metronome (a.k.a. kmetronome)

[![Build on Linux](https://github.com/pedrolcl/kmetronome/actions/workflows/linux-build.yml/badge.svg)](https://github.com/pedrolcl/kmetronome/actions/workflows/linux-build.yml)

[Drumstick Metronome](https://kmetronome.sourceforge.io/kmetronome.shtml) is a MIDI based 
metronome using the ALSA sequencer.

The intended audience are musicians and music students. Like any physical 
metronome, it is a tool to keep the rhythm while playing musical instruments.

It uses MIDI instead of digital audio, allowing low CPU usage and very accurate 
timing thanks to the ALSA sequencer. This means that you also need a MIDI 
synthesizer for sound generation, because the program does not produce any sound
by itself. The best results come from using a hardware synthesizer, but you may
also use software synthesizers instead.

For brief building instructions, see [INSTALL](INSTALL).

## Downloads

Sources: https://sourceforge.net/projects/kmetronome/files/

[![Download Drumstick Metronome](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/kmetronome/files/latest/download)

[<img width='240' alt='Download on Flathub' src='https://flathub.org/assets/badges/flathub-badge-en.png'/>](https://flathub.org/apps/details/net.sourceforge.kmetronome)

[![Packaging status](https://repology.org/badge/vertical-allrepos/kmetronome.svg)](https://repology.org/project/kmetronome/versions)

## Developers environment

You need the following software:

* CMake 3.16 or later
* C++17 compiler
* Qt libraries 6.2 or later (or Qt 5.12 or later)
* ALSA library 
* Drumstick libraries 2.10 or later

## Getting the development sources

Compiling and hacking the Git sources is a bit different compared to the
distribution tarball. You can get the latest sources either using a sourceforge
user account, or the anonymous user (with read only rights). The Git client  
documentation for SourceForge users is available at
https://sourceforge.net/p/forge/documentation/Git/

1. Clone the [drumstick Git repository](https://sourceforge.net/p/drumstick/git/ci/master/tree/).

There is also a [Git mirror at GitHub](https://github.com/pedrolcl/drumstick)

example:

~~~sh
    git clone git://git.code.sf.net/p/drumstick/git drumstick-git
~~~

The module 'drumstick' is also used by other projects in a similar way as an 
independent shared dynamic library.

You need to configure, compile and optionally install drumstick libraries.

2. Clone the [kmetronome Git repository](https://sourceforge.net/p/kmetronome/git/ci/master/tree/).

There is also a [Git mirror at GitHub](https://github.com/pedrolcl/kmetronome)

example:

~~~sh
    git clone git://git.code.sf.net/p/kmetronome/git kmetronome-git
~~~

4. Configure and compile

~~~sh
    cd kmetronome-git
    mkdir build
    cmake -S . -B build -DCMAKE_BUILD_TYPE=debug \
               -DCMAKE_PREFIX_PATH=$HOME/Qt/6.6.1/gcc_64/ \
               -DCMAKE_INSTALL_PREFIX=/usr/local/
    make VERBOSE=1
~~~

5. Hack and enjoy!
