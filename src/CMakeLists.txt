set(kmancala_SRCS 
   kmancala.cpp
   main.cpp
   kmancalamain.cpp
   kmancalaboardgap.cpp
   kmancalaboard.cpp
   kmancalarenderer.cpp
   kmancalaplayersettingsdialog.cpp
   kmancalahistory.cpp 
   kmancalahistoryitem.cpp
   ai.cpp
   board.cpp
   controller.cpp
   player.cpp
 )

kde4_add_kcfg_files(kmancala_SRCS kmancalaprefs.kcfgc )
kde4_add_executable(kmancala ${kmancala_SRCS})

target_link_libraries(kmancala ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} kdegames)
install(TARGETS kmancala ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( FILES kmancala.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kmancala.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kmancalaui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kmancala )
