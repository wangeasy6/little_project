cmake_minimum_required(VERSION 2.5 FATAL_ERROR)
project(Server)
#message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
#message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})

##########################################
### Selectable scheme:
# sample
# thread
# fork
# poll
# epoll
# select
##########################################
set(SCH_DIR "thread")
message(STATUS "Selected scheme -- " ${SCH_DIR})

ADD_SUBDIRECTORY(libsdk)
ADD_SUBDIRECTORY(bin/)

include_directories(./libsdk/inc)
include_directories(./bin/${SCH_DIR}/inc)
add_executable(httpd_easyw ./main.c)
TARGET_LINK_LIBRARIES(httpd_easyw sch_api sdk)