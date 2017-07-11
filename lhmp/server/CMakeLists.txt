file(GLOB_RECURSE LHMP_SERVER_SOURCES "src/*.cpp")

add_library(lhmp_server
	"${LHMP_SERVER_SOURCES}"
)

target_include_directories(lhmp_server
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)