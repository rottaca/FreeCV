cmake_minimum_required (VERSION 2.6)
PROJECT(FreeCV)


SET(SOURCE_FILES 
    src/Logger.cpp
    src/Vector.cpp
    src/Matrix.cpp
    src/Image.cpp
    src/ImageFileManager.cpp
)

SET(HEADER_FILES
    include/Logger.h
    include/Macros.h
    include/Vector.h
    include/Matrix.h
    include/Image.h
    include/ImageFileManager.h
)

ADD_LIBRARY(FreeCV SHARED ${SOURCE_FILES} ${HEADER_FILES} )

INCLUDE_DIRECTORIES(FreeCV include)

SET(TARGET FreeCV PROPERTY CMAKE_C_FLAGS_DEBUG "-DDEBUG")


SET(TEST_SOURCE_FILES
    src/testMain.cpp
)

ADD_EXECUTABLE(FreeCVTest ${TEST_SOURCE_FILES})
TARGET_LINK_LIBRARIES(FreeCVTest FreeCV)



