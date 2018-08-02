cmake_minimum_required(VERSION 3.6)
project(Simulator360)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        Project.cpp
	Simulator.cpp
        Project.h
        Robot.h
        Simulator.h
        Vector2D.h)

add_executable(Simulator360 ${SOURCE_FILES})