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
    src/Transform.cpp
    src/SGM.cpp
)

SET(HEADER_FILES
    include/FreeCV.h
    
    include/Core/Logger.h
    include/Core/Macros.h
    include/Core/Matrix.h
    include/Core/Vector.h
    include/Core/Rectangle.h
    
    include/Image/Image.h
    include/Image/ImageFileManager.h
    include/Image/Histogram.h
    include/Image/Filter.h
    include/Image/Transform.h
    
    include/Stereo/SGM.h
)


ADD_LIBRARY(FreeCV SHARED ${SOURCE_FILES} ${HEADER_FILES} )

INCLUDE_DIRECTORIES(FreeCV include)

SET(TARGET FreeCV PROPERTY CMAKE_C_FLAGS_DEBUG "-DDEBUG")


SET(TEST_SOURCE_FILES
    testMain.cpp
)

ADD_EXECUTABLE(FreeCVTest ${TEST_SOURCE_FILES})
TARGET_LINK_LIBRARIES(FreeCVTest FreeCV)


# copy all needed files to binary dir (when cmake)
file(GLOB files_needed

    TestData/hough.pgm
    
    TestData/lena_gray.pgm
    lTestData/lena_rgb.ppm
    
    TestData/stereo_disp_left.pgm
    TestData/stereo_disp_right.pgm
    TestData/stereo_left_gray.pgm
    TestData/stereo_right_gray.pgm
    TestData/stereo_left_rgb.ppm
    TestData/stereo_right_rgb.ppm
)
file(COPY ${files_needed} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


