
# FreeCV
This is an implementation of my own computervision library.
The library will be extended according to my projects.

## Features:
 - Logger class that allows logging of text messages.
    - It includes formatting.
    - Distinguishes between debug and release
      - Debug-Messages will be removed in release mode.
      - Log massages contain file name and line number in debug mode.
  - Contains Matrix and Vector class implementation with compile time and runtime fixed size (@see TODO's).
  - Contains StopWatch class to check performance for example,...
  - Contains Linux compartible VideoCapture implementation using video4linux2 API.
  - Contains an ExposureControl class to control linux webcams.
  - Contains Image class that supports common formats (RGB888, Grayscale8, YUYV, FLOAT32)
  - Contains Histogram calculation (normal and cumulative, only grayscale), its possible to specify a region of interest where the histogram should be calculated
  - Contains conversion from histogram to image (to display histogram)
  - Contains HistogramEqualisation algorithm
  - Contains image load/save functionality for (PGM and PPM, both ascii and binary)
  - Contains ColorConvert implementaion
  - Contains ImageFilters (currently: MedianFilter, GausFilter, generic ApplyKernel function)
  - Contains an implementation of the stereo matching algorithm "semi global matching (SGM)". It currently evaluates 8 instead of 16 paths but produces pretty results.
  - Contains a point cloud creator that converts disparity maps into 3D coordinates.
  - Point cloud creator can load and save .ply files that contains point coordinates and color.
  - Added optional Qt Interface to convert images into QImages
  - Added OpenMP support to SGM
  - LU-Decomposition with column pivoing implemented.
  - Cholesky-Decomposition implemented.
  - Matrix invert with LU decomposition implemented.

  
## TODO's:
  - Implement Matrix Determinant
  - Implement ExposureControl for ManualCtrl (User supplies goal luminance and algorithm adjusts exposure).
  - Implement ImageTransform (Rotate, Scale, Translate)
  - Implement ImageHoughTransform
  - Implement CalibLoader, UndistortAndRectify, ProjectPoint, UnprojectPoint,...
  - Cuda Implementation of most features: ColorConvert, HoughTransform, Histogram calculation, Filtering, SGM,...
  - Add data structures like Kd-trees,...
  - Add openCV Interface to convert matrices and images into cv::mat objects.
