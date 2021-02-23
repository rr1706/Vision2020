cmake_minimum_required(VERSION 3.0)
project( vision2020 )

option(HEAD "Run the program without the GUI" OFF)
option(NETWORK "Control networking on or off (uses Network Tables)" OFF)
#option(NETWORK "Control networking on or off" OFF)

if(CUDA_VERSION)
    add_definitions(-DWITH_CUDA)
endif(CUDA_VERSION)

if(HEAD)
    add_definitions(-DWITH_HEAD)
endif(HEAD)

if(NETWORK)
    add_definitions(-DWITH_NETWORK)
endif(NETWORK)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )
find_package( Ruby REQUIRED )
#find_package( wpilib REQUIRED )

file(GLOB srcs src/*.cpp src/headers/*.hpp)

add_executable( vision2020 ${srcs} )
target_include_directories( vision2020 PRIVATE ${RUBY_INCLUDE_DIRS} )
target_link_libraries( vision2020 ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ) #ntcore

#install(TARGETS vision2020 RUNTIME DESTINATION bin)
#install(FILES vision.service DESTINATION /lib/systemd/system)
