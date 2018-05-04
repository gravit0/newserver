cmake_minimum_required(VERSION 3.1)

project(sourceofpackage)
include_directories(libns/include)
add_subdirectory( libns libns )
add_subdirectory( newserver newserver )
add_subdirectory( newserverinteractive nsi )
