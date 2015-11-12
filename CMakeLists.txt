cmake_minimum_required (VERSION 2.6)
PROJECT(FreeCV)

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

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
    src/VideoCapture.cpp
    src/ColorConvert.cpp
    src/PointCloudCreator.cpp
    src/UndistortAndRectify.cpp
    src/CalibLoader.cpp
)

SET(HEADER_FILES
    include/FreeCV/FreeCV.h
    
    include/FreeCV/Core/Logger.h
    include/FreeCV/Core/StopWatch.h
    include/FreeCV/Core/Macros.h
    include/FreeCV/Core/Matrix.h
    include/FreeCV/Core/Vector.h
    include/FreeCV/Core/Rectangle.h
    
    include/FreeCV/Image/Image.h
    include/FreeCV/Image/ImageFileManager.h
    include/FreeCV/Image/Histogram.h
    include/FreeCV/Image/Filter.h
    include/FreeCV/Image/Transform.h
    include/FreeCV/Image/ColorConvert.h
    
    include/FreeCV/Stereo/SGM.h
    include/FreeCV/Stereo/PointCloudCreator.h
    include/FreeCV/Stereo/UndistortAndRectify.h
    include/FreeCV/Stereo/CalibLoader.h
    
    include/FreeCV/IO/VideoCapture.h
)


ADD_LIBRARY(FreeCV ${SOURCE_FILES} ${HEADER_FILES} )

target_include_directories(FreeCV PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    # Headers used from source/build location:
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    # Headers used from the installed location:
    "$<INSTALL_INTERFACE:include>"
)

SET(TARGET FreeCV PROPERTY CMAKE_C_FLAGS_DEBUG "-DDEBUG")
SET(TARGET FreeCV PROPERTY CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")

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


