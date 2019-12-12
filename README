For brief building instructions, see INSTALL.

Developers environment
======================

You need the following software:

* CMake
    Debian: "cmake"
    http://packages.debian.org/sid/cmake

* Qt5 libraries
    Debian: "qtbase5-dev"
    https://packages.debian.org/sid/qtbase5-dev-tools

* ALSA library
    Debian package: "libasound2-dev"
    http://packages.debian.org/sid/libasound2-dev

* Drumstick libraries
    http://drumstick.sourceforge.net

Getting the development sources
===============================

Compiling and hacking the SVN sources is a bit different compared to the
distribution tarball. You can get the latest sources either using a sourceforge
user account, or the anonymous user (with read only rights). The SVN client  
documentation for SourceForge users is available at
https://sourceforge.net/p/forge/documentation/SVN%20Overview/

1. Check out the module drumstick from the SVN repository.
        Host: svn.code.sf.net
        Path: /p/drumstick/code/trunk
        Module: drumstick

example:
$ svn checkout https://svn.code.sf.net/p/drumstick/code/trunk drumstick

The module 'drumstick' is also used by other projects in a similar way as an 
independent shared dynamic library.

You need to configure, compile and optionally install drumstick libraries.

2. Check out the module kmetronome from the SVN repository.
        Host: svn.code.sf.net
        Path: /p/kmetronome/code/trunk
        Module: kmetronome

example:
$ svn co  https://svn.code.sf.net/p/kmetronome/code/trunk kmetronome 

4. Configure and compile

$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=debug \
           -DCMAKE_PREFIX_PATH=$HOME/Qt/5.12.5/gcc_64/ \
           -DCMAKE_INSTALL_PREFIX=/usr/local/

$ make VERBOSE=1

5. Hack and enjoy!
