IF(NOT KMETRONOME_SOURCE_DIR)
    MESSAGE(FATAL_ERROR "You need to run cmake or ccmake at the root directory")
ENDIF(NOT KMETRONOME_SOURCE_DIR)

IF(USE_PCH)
    ADD_QTKDE_PRECOMPILED_HEADER(precompiled_headers qt_kde.h)
ENDIF(USE_PCH)

LINK_DIRECTORIES(
    ${KDE3_LIB_DIR} 
    ${ALSA_LIB_DIR}
)

INCLUDE_DIRECTORIES(AFTER
    ${QT_INCLUDE_DIR} 
    ${KDE3_INCLUDE_DIR} 
    ${ALSA_INC_DIR}
)

SET(kmetronome_SRCS 
	f16.xpm
	f1.xpm
	f2.xpm
	f32.xpm
	f4.xpm
	f64.xpm
	f8.xpm
	kmetroiface.h
	kmetronome.cpp
	kmetronome.h
	kmetronomeview.cpp
	kmetronomeview.h
	kmetropreferences.cpp
	kmetropreferences.h
	main.cpp
	qt_kde.h
	defs.h
	sequencerthread.cpp
	sequencerthread.h )

SET(kmetronome_UI 
	kmetronomeviewbase.ui
	kmetropreferencesbase.ui )

SET(kmetronome_DCOP 
	kmetroiface.h
)

KDE3_ADD_DCOP_SKELS(kmetronome_SRCS ${kmetronome_DCOP})
KDE3_ADD_DCOP_STUBS(kmetronome_SRCS ${kmetronome_DCOP}) 
KDE3_ADD_UI_FILES(kmetronome_SRCS ${kmetronome_UI})
KDE3_AUTOMOC(${kmetronome_SRCS})
KDE3_ADD_EXECUTABLE(kmetronome ${kmetronome_SRCS})

IF(USE_PCH)
    ADD_DEPENDENCIES(kmetronome precompiled_headers)
ENDIF(USE_PCH)

TARGET_LINK_LIBRARIES( kmetronome
    ${QT_AND_KDECORE_LIBS} 
    ${ALSA_LIBS} 
    kdeui
    kio
)

# Install targets
INSTALL( TARGETS kmetronome
         RUNTIME DESTINATION bin )

# Install GUI resource files
#FILE(GLOB RC_FILES *.rc)
#INSTALL( FILES ${RC_FILES}
#         DESTINATION share/apps/kmetronome )

# Install desktop file
INSTALL( FILES kmetronome.desktop
         DESTINATION share/applications )

# Icons
INSTALL( FILES hi16-app-kmetronome.png
         DESTINATION share/icons/hicolor/16x16/apps
         RENAME kmetronome.png )

INSTALL( FILES hi32-app-kmetronome.png
         DESTINATION share/icons/hicolor/32x32/apps
         RENAME kmetronome.png )
