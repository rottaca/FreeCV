cmake_minimum_required (VERSION 2.6)
PROJECT(FreeCV)

# Define software switches
# Avaialble switches are
# - WithOpenMP (default: ON)
# - WithQt (default: ON)
# - WithCuda (default: ON)

######################################################################
## Define Options
######################################################################
option(OPTION_WithOpenMP "WithOpenMP" ON)
option(OPTION_WithQt "WithQt" ON)
option(OPTION_WithCuda "WithCuda" ON)

######################################################################
## Check options
######################################################################
if(OPTION_WithOpenMP)
    FIND_PACKAGE(OpenMP)
    if(OPENMP_FOUND)
        message("OpenMP found. Multithreading enabled.") 
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        LIST(APPEND ADDITIONAL_LIBS "gomp")
        add_definitions(-DWITH_OPENMP)
    endif()
endif()

if(OPTION_WithQt)
    find_package(Qt5Widgets)
    if(Qt5Widgets_FOUND)
        message(STATUS "QT found! Qt Interface available!")
        add_definitions(-DWITH_QT)
        
        
    else()
        message(WARNING "QT not found! Qt Interface not available!")
    endif()
endif()
    
if(OPTION_WithCuda)
    find_package(CUDA)
    if(CUDA_FOUND)
        message(STATUS "Cuda found! GPU-Support available!")
        add_definitions(-DWITH_CUDA)
        
    else()
        message(WARNING "Cuda not found! GPU-Support not available!")
    endif()
endif()
    
######################################################################
## Library source files
######################################################################
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
    src/ColorConvert.cpp
    src/PointCloudCreator.cpp
    src/UndistortAndRectify.cpp
    src/CalibLoader.cpp
    src/Math.cpp
)

######################################################################
## Library header files
######################################################################
SET(HEADER_FILES
    include/FreeCV/FreeCV.h
    
    include/FreeCV/Core/Logger.h
    include/FreeCV/Core/StopWatch.h
    include/FreeCV/Core/Macros.h
    include/FreeCV/Core/Matrix.h
    include/FreeCV/Core/Vector.h
    include/FreeCV/Core/Rectangle.h
    include/FreeCV/Core/Math.h
    
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
    
)

######################################################################
## Add source files according to operating system
######################################################################
# Linux
if (UNIX)
    MESSAGE(STATUS "Linux detected! Enable v4l2 grabber and exposure controller.")
    LIST( APPEND HEADER_FILES
        include/FreeCV/Linux/VideoCapture.h
        include/FreeCV/Linux/ExposureController.h
        )
        
    LIST( APPEND SOURCE_FILES
        src/ExposureController.cpp
        src/VideoCapture.cpp
        )
endif (UNIX)
# Windows
if (WIN32) 
    MESSAGE(STATUS "Windows detected.! Disable v4l2 grabber and exposure controller.")

    LIST( APPEND HEADER_FILES
        include/FreeCV/Windows/Time.h
        include/FreeCV/Windows/Times.h
        )
        
    LIST( APPEND SOURCE_FILES
        src/Times.cpp
        )
endif (WIN32)

######################################################################
## Add source files according to additional interfaces
######################################################################
if(Qt5Widgets_FOUND)
    LIST( APPEND SOURCE_FILES
        src/QtInterface.cpp
    )
endif()

if(CUDA_FOUND)
    SET(CUDA_SOURCE_FILES 
        src/GPUData.cpp
    )
    SET(CUDA_HEADER_FILES 
        include/FreeCV/Cuda/Cuda.h
        include/FreeCV/Cuda/CudaError.h
        include/FreeCV/Cuda/GPUData.h
    )
    SET(CUDA_CU_FILES 

    )
endif()


######################################################################
## Add and link Library
######################################################################
##### FreeCV
if(CUDA_FOUND)
    CUDA_ADD_LIBRARY(FreeCV ${SOURCE_FILES} ${HEADER_FILES} ${CUDA_HEADER_FILES} ${CUDA_SOURCE_FILES} ${CUDA_CU_FILES})
else()
    ADD_LIBRARY(FreeCV ${SOURCE_FILES} ${HEADER_FILES})
endif()

target_link_libraries(FreeCV  ${ADDITIONAL_LIBS})

target_include_directories(FreeCV PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    # Headers used from source/build location:
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    # Headers used from the installed location:
    "$<INSTALL_INTERFACE:include>"
)
##### QT
if(Qt5Widgets_FOUND)
    qt5_use_modules(FreeCV Widgets)
endif()

##### Cuda
if(CUDA_FOUND)
    
endif()

######################################################################
## Print Build Summary
######################################################################
macro(PrintOptionResult optName optEnabled optFound)
    if(${optEnabled})
        if(${optFound})
          message(STATUS ${optName} ": ON - FOUND")
        else()
          message(STATUS ${optName} ": ON - NOT FOUND!")
        endif()
    else()
      message(STATUS ${optName} ": OFF")
    endif()
endmacro()

# Print summary
message(STATUS "")
message(STATUS "")
message(STATUS "-----------Summaray of FreeCV-Build-----------")
PrintOptionResult("OpenMP" OPTION_WithOpenMP OPENMP_FOUND)
PrintOptionResult("Qt" OPTION_WithQt Qt5Widgets_FOUND)
PrintOptionResult("Cuda" OPTION_WithCuda CUDA_FOUND)
message(STATUS "")
message(STATUS "")


#######################################################################
#######################################################################
### TestSoftware
#######################################################################
#######################################################################
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

message(STATUS "Copy test data to binariy dir...")
file(COPY ${files_needed} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


