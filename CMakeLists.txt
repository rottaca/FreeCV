cmake_minimum_required (VERSION 2.6)
PROJECT(FreeCV)


SET(SOURCE_FILES 
    src/Logger.cpp
    src/Vector.cpp
    src/Matrix.cpp
    src/Image.cpp
    src/ImageFileManager.cpp
    src/Filter.cpp
    src/Rectangle.cpp
    src/Histogram.cpp
)

SET(HEADER_FILES
    include/FreeCV.h
    
    include/Core/Logger.h
    include/Core/Macros.h
    include/Core/Matrix.h
    include/Core/Vector.h
    
    include/Geometry/Rectangle.h
    
    include/Image/Image.h
    include/Image/ImageFileManager.h
    include/Image/Histogram.h
    include/Image/Filter.h
)


ADD_LIBRARY(FreeCV SHARED ${SOURCE_FILES} ${HEADER_FILES} )

INCLUDE_DIRECTORIES(FreeCV include)

SET(TARGET FreeCV PROPERTY CMAKE_C_FLAGS_DEBUG "-DDEBUG")


SET(TEST_SOURCE_FILES
    testMain.cpp
)

ADD_EXECUTABLE(FreeCVTest ${TEST_SOURCE_FILES})
TARGET_LINK_LIBRARIES(FreeCVTest FreeCV)



