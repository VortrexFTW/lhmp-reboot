#	Lost Heaven Multiplayer - Reboot Edition

cmake_minimum_required(VERSION 2.6)

project(lhmp)

#	Build RakNet (use raknet as library name, not ezraknet

add_subdirectory(lhmp/client)
#add_subdirectory(server)
#add_subdirectory(installer)