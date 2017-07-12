if(WIN32)
	if(MSVC)
		add_definitions( "-D_CRT_SECURE_NO_WARNINGS" )
	endif()
	message( STATUS "Squirrel headers: ${SQUIRREL_INCLUDE_DIR}" )	
	file( GLOB_RECURSE LHMP_CLIENT_SOURCES "src/*.cpp" )
	
	set(LHMP_CLIENT_SOURCES
		"src/AntiCheat.cpp"
		"src/CChat.cpp"
		"src/CColoredText.cpp"
		"src/CCore.cpp"
		"src/CCrashHandler.cpp"
		"src/CEngineStack.cpp"
		"src/CEntity.cpp"
		"src/CFileTransfer.cpp"
		"src/CFont.cpp"
		"src/CGame.cpp"
		"src/CGameSync.cpp"
		"src/CGraphics.cpp"
		"src/CHooks.cpp"
		"src/CIngameMenu.cpp"
		"src/CInterpolation.cpp"
		"src/CKeyboard.cpp"
		"src/CLocalPlayer.cpp"
		"src/CLog.cpp"
		"src/CNametags.cpp"
		"src/CNetwork.cpp"
		"src/CPed.cpp"
		"src/CPedPool.cpp"
		"src/CPickup.cpp"
		"src/CPickupPool.cpp"
		"src/CSQFonts.cpp"
		"src/CSQImages.cpp"
		"src/CSquirrel.cpp"
		"src/CVehicle.cpp"
		"src/CVehiclePool.cpp"
		"src/main.cpp"
	)
	
	add_library( lhmp_client SHARED "${LHMP_CLIENT_SOURCES}" )
	include_directories( SYSTEM "${CMAKE_SOURCE_DIR}/vendor/directx/include" ${SQUIRREL_INCLUDE_DIR} )
	target_link_libraries( lhmp_client raknet "${CMAKE_SOURCE_DIR}/vendor/directx/lib/x86/d3d8.lib" )
	target_include_directories( lhmp_client PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
else()
	message( STATUS "Client can't be build outside Windows environment." )	
endif()