cmake_minimum_required(VERSION 3.10)
project(AVirtualBeing)

add_executable(AVirtualBeing ${CMAKE_CURRENT_SOURCE_DIR}/source/beingEngine/core/entry.cpp)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
target_sources(AVirtualBeing PRIVATE ${SRC_FILES})

set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

set(TARGET_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries/desktop)
set_target_properties(
	AVirtualBeing
	PROPERTIES
	CXX_STANDARD 20
	RUNTIME_OUTPUT_DIRECTORY ${TARGET_OUTPUT_DIRECTORY}
)

target_include_directories(AVirtualBeing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)


file(GLOB_RECURSE ASSET_FILES_RAW ${CMAKE_CURRENT_SOURCE_DIR}/source/virtualGame/assets/**)
foreach(ASSET_FILE IN LISTS ASSET_FILES_RAW)
	FILE(RELATIVE_PATH ASSET_FILE_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/virtualGame/assets/ ${ASSET_FILE})
	add_custom_command(TARGET AVirtualBeing POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSET_FILE} ${TARGET_OUTPUT_DIRECTORY}/assets/${ASSET_FILE_RELATIVE})
endforeach()


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
target_link_options(AVirtualBeing PRIVATE -s)