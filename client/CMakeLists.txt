file(GLOB_RECURSE LHMP_CLIENT_SOURCES "src/*.cpp")

add_library(lhmp_client
	"${LHMP_CLIENT_SOURCES}"
)

target_include_directories(lhmp_client
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)