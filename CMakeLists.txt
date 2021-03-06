project(kmancala)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )

install( DIRECTORY themes DESTINATION  ${DATA_INSTALL_DIR}/kmancala )

SET(MAJOR_VERSION 1)
SET(MINOR_VERSION 0)
SET(PATCH_VERSION 0)

IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_SET_DESTDIR "on")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
SET(CPACK_GENERATOR "DEB")

SET(CPACK_PACKAGE_DESCRIPTION "Kalah board game from Mancala family games")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Kalah board game from Mancala family games")
SET(CPACK_PACKAGE_VENDOR "")
SET(CPACK_PACKAGE_CONTACT "Dalibor Horinek")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")

SET(CPACK_DEBIAN_PACKAGE_DEPENDS "kdebase-runtime (>= 4:4.4.1), kdelibs5 (>= 4:4.5.1), libc6 (>= 2.1.3), libgcc1 (>= 1:4.1.1), libqt4 (>= 4.5.0), libqtcore4 (>= 4.5.0), libqtgui4 (>= 4.5.0), libstdc++6 (>= 4.2.1)")

SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "games")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
INCLUDE(CPack)

ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
