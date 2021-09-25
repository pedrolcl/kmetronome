foreach(S IN ITEMS 48 64 128 256 512)
    install (FILES kmetronome_${S}.png
             DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/${S}x${S}/apps
             RENAME kmetronome.png)
endforeach()

install ( FILES kmetronome.svgz
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps )
