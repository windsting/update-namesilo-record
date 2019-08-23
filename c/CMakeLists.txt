cmake_minimum_required(VERSION 3.11)

project(update_namesilo_record
	VERSION 0.1
	DESCRIPTION "A simple toy to update ip address for a domain record registered in namesilo.com"
	LANGUAGES C)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src)

