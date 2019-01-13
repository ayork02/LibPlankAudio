CMake_minimum_required(VERSION 3.5)

project(plankaudio)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(plankaudio_VERSION_MAJOR 0)
set(plankaudio_VERSION_MINOR 9)
set(plankaudio_VERSION_PATCH 2)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(source_dir "${PROJECT_SOURCE_DIR}/src/")

file(GLOB source_files "${source_dir}/*.cpp")

find_package(Threads REQUIRED)
find_package(ALSA REQUIRED)
find_package(PortAudio REQUIRED)
find_package(Boost 1.58 REQUIRED COMPONENTS thread)

add_library(plankaudio SHARED ${source_files})
 
set_target_properties(plankaudio PROPERTIES 
	PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include/plankaudio.h
)

target_include_directories(plankaudio
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sndfile/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${ALSA_INCLUDE_DIRS}
		${PORTAUDIO_INCLUDE_DIRS}
		${Boost_INCLUDE_DIRS}
		${LIBSNDFILE_INCLUDE_DIRS}
)

target_link_libraries(plankaudio
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sndfile/lib/libsndfile.so.1.0.28
		${CMAKE_THREAD_LIBS_INIT}
		${ALSA_LIBRARIES}
		${PORTAUDIO_LIBRARIES}
		${LIBSNDFILE_LIBS}
		Boost::boost
		Boost::thread
)

install(TARGETS plankaudio
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
