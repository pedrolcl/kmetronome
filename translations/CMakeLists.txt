SET( TRANSLATIONS_FILES
     cs.ts
     de.ts
     es.ts
     fr.ts
     tr.ts )

QT5_ADD_TRANSLATION(QM_FILES ${TRANSLATIONS_FILES})
ADD_CUSTOM_TARGET(translations ALL DEPENDS ${QM_FILES})

INSTALL( FILES ${QM_FILES}
         DESTINATION share/kmetronome/locale )
