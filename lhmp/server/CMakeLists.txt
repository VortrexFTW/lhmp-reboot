find_package(Squirrel REQUIRED)
find_package(RakNet REQUIRED)

if(MSVC)
	add_definitions( "-D_CRT_SECURE_NO_WARNINGS" )
endif()
message( STATUS "Squirrel headers: ${SQUIRREL_INCLUDE_DIR}" )	
file( GLOB_RECURSE LHMP_SERVER_SOURCES "src/*.cpp" )

set(LHMP_SERVER_SOURCES
	"src/CBanSystem.cpp"
	"src/CConfig.cpp"
	"src/CConsole.cpp"
	"src/CCore.cpp"
	"src/CDoorPool.cpp"
	"src/CEntity.cpp"
	"src/CFileTransfer.cpp"
	"src/CLog.cpp"
	"src/CMasterServer.cpp"
	"src/CNetworkManager.cpp"
	"src/CPlayer.cpp"
	"src/CPickup.cpp"
	"src/CPickupPool.cpp"
	"src/CQueryServer.cpp"
	"src/CScript.cpp"
	"src/CScripts.cpp"
	"src/CTCP.cpp"
	"src/CTickManager.cpp"
	"src/CTimer.cpp"
	"src/CTimerPool.cpp"
	"src/CVehicle.cpp"
	"src/CVehiclePool.cpp"
	"src/Main.cpp"
	"src/sq_funcdef.cpp"
	"src/sq_functions.cpp"
	"src/sq_utils.cpp"
	"src/squirrelheads.cpp"
)

add_library( lhmp_server SHARED "${LHMP_SERVER_SOURCES}" )
include_directories( SYSTEM ${SQUIRREL_INCLUDE_DIR} ${RAKNET_INCLUDE_DIR} )
target_link_libraries( lhmp_server raknet "${CMAKE_SOURCE_DIR}/vendor/directx/lib/x86/d3d8.lib" )
target_include_directories( lhmp_server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
