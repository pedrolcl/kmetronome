set( TRANSLATIONS
     cs.ts
     de.ts
     es.ts
     fr.ts
     tr.ts )

qt5_add_translation(QM_FILES ${TRANSLATIONS})

add_custom_target(translations ALL DEPENDS ${QM_FILES} ${APP_RES} ${QT_RES})

foreach(QMF ${QM_FILES})
    get_filename_component(LANG ${QMF} NAME_WE)
    install( FILES ${QMF}
             DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/kmetronome/translations/
             RENAME "kmetronome_${LANG}.qm" )
endforeach()
