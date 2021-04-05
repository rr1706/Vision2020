cmake_minimum_required(VERSION 3.0)
project( vision2020 )

###############################################################################

option(HEAD "Run the program without the GUI" OFF)
option(NETWORK "Control networking on or off (uses Network Tables)" OFF)
option(SERVICE "Adds and install target to the Makefile to run on start up" OFF)

if(CUDA_VERSION) # Required by Cuda don't worry about it
    add_definitions(-DWITH_CUDA)
endif(CUDA_VERSION)

if(HEAD)
    add_definitions(-DWITH_HEAD)
endif(HEAD)

if(NETWORK)
    add_definitions(-DWITH_NETWORK)
endif(NETWORK)

if(SERVICE)
    add_definitions(-DWITH_SERVICE)
endif(SERVICE)

###############################################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###############################################################################

find_package( Poco REQUIRED Foundation Net CONFIG )
find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )
if(WITH_NETWORK)
    find_package( wpilib REQUIRED )
endif()

###############################################################################

file(GLOB srcs src/vision2020/*.cpp)
add_executable( vision2020 ${srcs} )

target_include_directories( vision2020 PRIVATE src/vision2020/include/ )
if(WITH_NETWORK)
	target_link_libraries( vision2020 ${OpenCV_LIBS} Poco::Foundation Poco::Net ${CMAKE_THREAD_LIBS_INIT} ntcore )
else()
    target_link_libraries( vision2020 ${OpenCV_LIBS} Poco::Foundation Poco::Net ${CMAKE_THREAD_LIBS_INIT} )
endif()

###############################################################################

if(WITH_SERVICE)
    install(TARGETS vision2020 RUNTIME DESTINATION bin)
    install(FILES vision.service DESTINATION /lib/systemd/system)
endif()
