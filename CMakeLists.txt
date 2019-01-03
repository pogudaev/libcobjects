cmake_minimum_required(VERSION 2.8)
project(cobjects C)

#============================== VERSION ================================
set(LIB_VERSION_MAJOR 0)
set(LIB_VERSION_MINOR 0)
set(LIB_VERSION_PATCH 0)
#=======================================================================
option(BUILD_STATIC_LIBS "Build the static library" OFF)
option(BUILD_SHARED_LIBS "Build the shared library" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_COMPILER "gcc")

set(LIB_VERSION_STRING ${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH})

set(SOURCE_LIB 
	"${CMAKE_SOURCE_DIR}/src/co_buffer.c"
	"${CMAKE_SOURCE_DIR}/src/co_list.c"
	"${CMAKE_SOURCE_DIR}/src/co_primitives.c"
	"${CMAKE_SOURCE_DIR}/src/co_string.c"
)

set(PUBLIC_HEADERS 
	"${CMAKE_SOURCE_DIR}/include/co_buffer.h"
	"${CMAKE_SOURCE_DIR}/include/co_list.h"
	"${CMAKE_SOURCE_DIR}/include/co_macros.h"
	"${CMAKE_SOURCE_DIR}/include/co_primitives.h"
	"${CMAKE_SOURCE_DIR}/include/co_status_code.h"
	"${CMAKE_SOURCE_DIR}/include/co_string.h"
)

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include)

add_custom_target(uninstall
	#TODO for not unix platform
	COMMAND xargs rm < install_manifest.txt
)

if (BUILD_STATIC_LIBS OR BUILD_SHARED_LIBS)

	if (BUILD_STATIC_LIBS)
		add_library(${PROJECT_NAME}_static STATIC ${SOURCE_LIB})
		install(TARGETS ${PROJECT_NAME}_static DESTINATION lib)
		set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LIB_VERSION_STRING} SOVERSION ${LIB_VERSION_MAJOR})
	endif (BUILD_STATIC_LIBS)

	if (BUILD_SHARED_LIBS)
		add_library(${PROJECT_NAME} SHARED ${SOURCE_LIB})
		install(TARGETS ${PROJECT_NAME} DESTINATION lib)
		set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LIB_VERSION_STRING} SOVERSION ${LIB_VERSION_MAJOR})
	endif (BUILD_SHARED_LIBS)


	add_subdirectory(test)
        add_subdirectory(example)
	install(FILES ${PUBLIC_HEADERS} DESTINATION include/cobjects)

else (BUILD_STATIC_LIBS OR BUILD_SHARED_LIBS)

	message(FATAL_ERROR "no target for build!")

endif (BUILD_STATIC_LIBS OR BUILD_SHARED_LIBS)




