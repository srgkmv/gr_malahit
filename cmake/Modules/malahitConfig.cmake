if(NOT PKG_CONFIG_FOUND)
    INCLUDE(FindPkgConfig)
endif()
PKG_CHECK_MODULES(PC_MALAHIT malahit)

FIND_PATH(
    MALAHIT_INCLUDE_DIRS
    NAMES malahit/api.h
    HINTS $ENV{MALAHIT_DIR}/include
        ${PC_MALAHIT_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    MALAHIT_LIBRARIES
    NAMES gnuradio-malahit
    HINTS $ENV{MALAHIT_DIR}/lib
        ${PC_MALAHIT_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/malahitTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MALAHIT DEFAULT_MSG MALAHIT_LIBRARIES MALAHIT_INCLUDE_DIRS)
MARK_AS_ADVANCED(MALAHIT_LIBRARIES MALAHIT_INCLUDE_DIRS)
