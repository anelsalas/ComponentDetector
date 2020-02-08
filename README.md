# ComponentDetector
 Component Detector from images for a pick and place

 VCPKG command: E:\Source\cpp\vcpkg>vcpkg install opencv4[qt,eigen,ffmpeg,jpeg,png,openmp,tbb]:x64-windows
 
 https://wiki.qt.io/OpenCV_with_Qt

 https://gist.github.com/UnaNancyOwen/e24e67966af5f1d5f5cce4a88af7b4a4

 https://www.youtube.com/playlist?list=PLeLcvrwLe187K3Ceoe3X3VMAa31591aO8

 https://www.youtube.com/watch?v=fYBdwGpHQGw

-- Configuring x64-windows
-- Building x64-windows-dbg
-- Building x64-windows-rel
-- Installing: E:/Source/cpp/vcpkg/packages/opencv4_x64-windows/share/opencv4/copyright
-- Performing post-build validation
-- Performing post-build validation done
Building package opencv4[core,dnn,eigen,ffmpeg,jpeg,opengl,openmp,png,qt,tbb,tiff,webp]:x64-windows... done
Installing package opencv4[core,dnn,eigen,ffmpeg,jpeg,opengl,openmp,png,qt,tbb,tiff,webp]:x64-windows...
Installing package opencv4[core,dnn,eigen,ffmpeg,jpeg,opengl,openmp,png,qt,tbb,tiff,webp]:x64-windows... done
Elapsed time for package opencv4:x64-windows: 21.51 min

Total elapsed time: 3.55 h

The package opencv4:x64-windows provides CMake targets:

    find_package(OpenCV CONFIG REQUIRED)
    # Note: 11 target(s) were omitted.
    target_link_libraries(main PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann)
