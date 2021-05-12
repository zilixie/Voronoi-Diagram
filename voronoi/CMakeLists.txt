cmake_minimum_required(VERSION 3.5)
project(voronoi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(lib/nanogui)
include_directories(lib/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
include_directories(include)
add_definitions(${NANOGUI_EXTRA_DEFS})

file(GLOB SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} nanogui ${NANOGUI_EXTRA_LIBS})