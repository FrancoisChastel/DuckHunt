cmake_minimum_required(VERSION 3.3)
project(TheMatrix1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Action.hpp Constants.hpp Client.hpp Client.cpp Deadline.hpp main.cpp Player.cpp Player.hpp GameServer.hpp
        GameServer.cpp GameState.hpp Player.hpp Model.cpp Model.hpp HMM.hpp)
add_executable(TheMatrix1 ${SOURCE_FILES})