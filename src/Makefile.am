# set the include path for X, qt and KDE
INCLUDES = $(ALSA_CFLAGS) $(all_includes)

# these are the headers for your project
noinst_HEADERS = kmetronome.h sequencerthread.h kmetroiface.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kmetronome.pot

KDE_ICON = kmetronome

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kmetronome

# the application source, library search path, and link libraries
kmetronome_SOURCES = main.cpp kmetronome.cpp kmetronomeviewbase.ui kmetronomeview.cpp kmetronomeview.h f16.xpm f1.xpm f2.xpm f32.xpm f4.xpm f64.xpm f8.xpm kmetropreferences.cpp kmetropreferences.h sequencerthread.cpp kmetropreferencesbase.ui kmetroiface.skel
kmetronome_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kmetronome_LDADD = $(LIB_KDEUI) $(ALSA_LIBS)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = kmetronome.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kmetronome


