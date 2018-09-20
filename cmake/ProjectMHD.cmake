include(ExternalProject)

set(MHD_CONFIG ./configure --enable-spdy=no --disable-curl --disable-messages --disable-postprocessor --enable-https=no)
set(MHD_BUILD make)

ExternalProject_Add(MHD
    PREFIX ${CMAKE_SOURCE_DIR}/deps
    DOWNLOAD_NAME libmicrohttpd-0.9.44.tar.gz
    DOWNLOAD_NO_PROGRESS 0
    URL https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-0.9.44.tar.gz
    URL_HASH SHA256=f2739cc05112dc00a5ebe1a470174970ca3a3fd71dcd67fb7539af9d83b8411e
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ${MHD_CONFIG}
    LOG_CONFIGURE 1
    BUILD_COMMAND ${TASSL_BUILD_COMMAND}
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(MHD SOURCE_DIR)
add_library(Mhd STATIC IMPORTED)
set(MHD_INCLUDE_DIR ${SOURCE_DIR}/src/include/)
set(MHD_LIBRARY ${SOURCE_DIR}/src/microhttpd/.libs/libmicrohttpd.a)