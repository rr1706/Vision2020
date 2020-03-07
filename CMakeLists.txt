cmake_minimum_required(VERSION 3.0)
project( vision2020 )

option(HEAD "Run the program without the GUI" ON)
option(NETWORK "Control networking on or off (uses Network Tables)" OFF)
option(NETWORK_UDP "Control networking on or off (uses UDP)" OFF)
option(WEBSTREAM "opens up a webstream that can be accesed at {unknown}" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OpenCV_DIR = "/usr/local/include/opencv4")

find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )
find_package( wpilib REQUIRED )

if(CUDA_VERSION)
    add_definitions(-DWITH_CUDA)
endif(CUDA_VERSION)

if(HEAD)
	add_definitions(-DWITH_HEAD)
endif(HEAD)

if(NETWORK_UDP)
    add_definitions(-DWITH_NETWORK_UDP)
endif(NETWORK_UDP)

if(WEBSTREAM)
    add_definitions(-DWITH_WEBSTREAM)
endif(WEBSTREAM)

if(NETWORK)
    add_definitions(-DWITH_NETWORK)
endif(NETWORK)

file(GLOB srcs ./*.cpp ./*.h*)

add_executable( vision2020 ${srcs} )
target_link_libraries( vision2020 ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ntcore )

install(TARGETS vision2020 RUNTIME DESTINATION bin)
install(FILES vision.service DESTINATION /lib/systemd/system)
