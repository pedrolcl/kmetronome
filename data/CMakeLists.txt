FILE( GLOB SAMPLES *.pat )
INSTALL( FILES drums.ins ${SAMPLES} 
         DESTINATION ${DATA_INSTALL_DIR}/kmetronome )
