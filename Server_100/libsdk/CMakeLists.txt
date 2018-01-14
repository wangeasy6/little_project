SET(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)

include_directories(./inc)
#LINK_LIBRARIES(lua pcre xml2 pthread)
AUX_SOURCE_DIRECTORY(./src LIBSDK_SRCS_DIR)
ADD_LIBRARY(sdk SHARED ${LIBSDK_SRCS_DIR})

#AStyle.exe --style=allman --indent=spaces=4 -HpU --recursive *