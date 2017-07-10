#	For use with ezraknet (https://github.com/PerikiyoXD/ezraknet)
#
#	This is based of 
#	https://gist.githubusercontent.com/davedissian/df6342d9e67de5e1d838/raw/f315526f0e30ce23cec862b9520255f0298ff097/FindRakNet.cmake"
#
#	RAKNET_FOUND - If RakNet has been found somewhere across the system.
#	RAKNET_INCLUDE_DIR - Include directories.
#	RAKNET_LIBRARIES - Libraries to link against.

find_path(RAKNET_INCLUDE_DIR
    NAME RakPeer.h
    PATHS
    ${RAKNET_DIR}/include
    ENV CPATH
    /usr/local/include
    /usr/include
    /sw/include
    /opt/local/include
    /opt/csw/include
    /opt/include
)

# Find the debug library
find_library(RAKNET_LIBRARY_DEBUG
    NAMES RakNetLibStaticDebug
    PATHS
    ${RAKNET_DIR}/lib
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
    /usr/freeware/lib64
)

# Find the optimized library
find_library(RAKNET_LIBRARY_RELEASE
    NAMES RakNetLibStatic
    PATHS
    ${RAKNET_DIR}/lib
    ENV LD_LIBRARY_PATH
    ENV LIBRARY_PATH
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
    /usr/freeware/lib64
)

find_package(OpenSSL)
set(RAKNET_INCLUDE_DIR ${RAKNET_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
if (WIN32)
    set(RAKNET_LIBRARIES
        optimized ${RAKNET_LIBRARY_RELEASE}
        debug ${RAKNET_LIBRARY_DEBUG})
    set(RAKNET_LIBRARIES ${RAKNET_LIBRARIES} ws2_32.lib) # Raknet depends on Winsock 2
else()
    set(RAKNET_LIBRARIES ${RAKNET_LIBRARY_RELEASE})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RakNet DEFAULT_MSG RAKNET_LIBRARIES RAKNET_INCLUDE_DIR)
mark_as_advanced(RAKNET_LIBRARY_RELEASE RAKNET_LIBRARY_DEBUG)
