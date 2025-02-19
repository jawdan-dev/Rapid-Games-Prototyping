cmake_minimum_required(VERSION 3.10)
set(PROJECT_TITLE SYSTEM_WR3CK)
project(${PROJECT_TITLE})

# Source
add_executable(${PROJECT_TITLE} ${CMAKE_CURRENT_SOURCE_DIR}/source/beingEngine/core/entry.cpp)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
target_sources(${PROJECT_TITLE} PRIVATE ${SRC_FILES})
target_include_directories(${PROJECT_TITLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

set_target_properties(${PROJECT_TITLE} PROPERTIES CXX_STANDARD 20)

# Submodule validate function.
function(check_and_add_subdirectory dir)
	if(NOT EXISTS ${dir}/CMakeLists.txt)
		message(FATAL_ERROR "Fatal: Submodule " ${dir} " not found\n")
	endif()

	add_subdirectory(${dir})
endfunction()

# Asset Watching
function(watch)
	set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ARGV})
	set_target_properties(${PROJECT_TITLE} PROPERTIES LINK_DEPENDS ${ARGV})
endfunction()

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/virtualGame/assets)
file(GLOB_RECURSE ASSET_FILES_RAW ${ASSETS_DIR}/**)

foreach(ASSET_FILE IN LISTS ASSET_FILES_RAW)
	watch(${ASSET_FILE})
endforeach()

# Handle platforms.
if(EMSCRIPTEN)
	set_target_properties(${PROJECT_TITLE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries/web)
	set(CMAKE_EXECUTABLE_SUFFIX .html)
	set(CMAKE_EXE_LINKER_FLAGS "-sMAX_WEBGL_VERSION=2 -sUSE_GLFW=3 -sNO_DISABLE_EXCEPTION_CATCHING")

	# Preload Assets.
	set(ASSET_FILES "")

	foreach(ASSET_FILE IN LISTS ASSET_FILES_RAW)
		FILE(RELATIVE_PATH ASSET_FILE_RELATIVE ${ASSETS_DIR} ${ASSET_FILE})
		list(APPEND ASSET_FILES "--preload-file=\"${ASSET_FILE}\"@\"assets/${ASSET_FILE_RELATIVE}\"")
	endforeach()

	string(REPLACE ";" " " ASSET_PRELOADS "${ASSET_FILES}")
	set_target_properties(${PROJECT_TITLE} PROPERTIES LINK_FLAGS "${ASSET_PRELOADS}")

else()
	set(TARGET_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries/desktop)
	set_target_properties(${PROJECT_TITLE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_OUTPUT_DIRECTORY})

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
	if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
	endif()

	# Move Assets to build.
	foreach(ASSET_FILE IN LISTS ASSET_FILES_RAW)
		FILE(RELATIVE_PATH ASSET_FILE_RELATIVE ${ASSETS_DIR} ${ASSET_FILE})
		add_custom_command(TARGET ${PROJECT_TITLE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSET_FILE} ${TARGET_OUTPUT_DIRECTORY}/assets/${ASSET_FILE_RELATIVE})
	endforeach()

	# GLFW.
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	check_and_add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/glfw)
	target_include_directories(${PROJECT_TITLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modules/glfw/include)
	target_link_libraries(${PROJECT_TITLE} PRIVATE glfw)

	# # GLEW.
	set(glew-cmake_BUILD_SHARED FALSE)
	set(glew-cmake_BUILD_STATIC ON)
	set(ONLY_LIBS ON)
	check_and_add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/glew)
	target_include_directories(${PROJECT_TITLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/modules/glew/include)
	target_link_libraries(${PROJECT_TITLE} PRIVATE libglew_static)

	# # OpenGL.
	find_package(OpenGL REQUIRED)
	target_link_libraries(${PROJECT_TITLE} PRIVATE OpenGL::GL)
endif()

# # Strip.
target_link_options(${PROJECT_TITLE} PRIVATE -s)