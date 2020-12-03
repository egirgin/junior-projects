cmake_minimum_required(VERSION 2.8.4)
project(project2)

set(CMAKE_CXX_STANDARD 14)

add_executable(project2 main.cpp Passenger.h Passenger.cpp Simulator.cpp Simulator.h SecurityCounter.cpp SecurityCounter.h LuggageCounter.cpp LuggageCounter.h)