cmake_minimum_required(VERSION 3.0)
project( vision2020 )

option(HEAD "Run the program without the GUI" ON)
#currently does not work
option(CUDA_VERSION "Run using cuda acceleration" OFF)

option(NETWORK "Control networking on or off" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )
find_package( wpilib REQUIRED )

if(CUDA_VERSION)
    add_definitions(-DWITH_CUDA)
endif(CUDA_VERSION)

if(HEAD)
	add_definitions(-DWITH_HEAD)
endif(HEAD)

if(NETWORK)
    add_definitions(-DWITH_NETWORK)
endif(NETWORK)

file(GLOB srcs ./*.cpp ./*.h*)

add_executable( vision2020 ${srcs} )
target_link_libraries( vision2020 ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ntcore )

#install(TARGETS vision2020 RUNTIME DESTINATION bin)
#install(FILES vision.service DESTINATION /lib/systemd/system)
