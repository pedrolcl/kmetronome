set( TRANSLATIONS
     kmetronome_cs.ts
     kmetronome_de.ts
     kmetronome_es.ts
     kmetronome_fr.ts
     kmetronome_tr.ts )

qt5_add_translation(QM_FILES ${TRANSLATIONS})

add_custom_target(translations ALL DEPENDS ${QM_FILES})

install( FILES ${QM_FILES}
         DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/kmetronome/translations/ )
