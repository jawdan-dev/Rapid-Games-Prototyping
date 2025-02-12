cmake_minimum_required(VERSION 3.10)
project(AVirtualBeing)

add_executable(AVirtualBeing ${CMAKE_CURRENT_SOURCE_DIR}/source/beingEngine/core/entry.cpp)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
target_sources(AVirtualBeing PRIVATE ${SRC_FILES})

set(TARGET_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries/desktop)
set_target_properties(
	AVirtualBeing
	PROPERTIES
	CXX_STANDARD 20
	RUNTIME_OUTPUT_DIRECTORY ${TARGET_OUTPUT_DIRECTORY}
)

target_include_directories(AVirtualBeing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Submodule validate function.
function(check_and_add_subdirectory dir)
	if(NOT EXISTS ${dir}/CMakeLists.txt)
		message(FATAL_ERROR "Fatal: Submodule " ${dir} " not found\n")
	endif()

	add_subdirectory(${dir})
endfunction()

# GLFW.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
check_and_add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/glfw)
target_include_directories(AVirtualBeing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modules/glfw/include)
target_link_libraries(AVirtualBeing PRIVATE glfw)

# # GLEW.
set(glew-cmake_BUILD_SHARED FALSE)
set(glew-cmake_BUILD_STATIC ON)
set(ONLY_LIBS ON)

check_and_add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/glew)
target_include_directories(AVirtualBeing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modules/glew/include)
target_link_libraries(AVirtualBeing PRIVATE libglew_static)

# # OpenGL.
find_package(OpenGL REQUIRED)
target_link_libraries(AVirtualBeing PRIVATE OpenGL::GL)

# # Strip.
target_link_libraries(AVirtualBeing PRIVATE -s)