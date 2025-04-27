cmake_minimum_required(VERSION 3.16)    # 限制最低CMake版本

# 设置交叉编译工具链的路径
set(CROSS_COMPILER_DIR /opt/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.3-1)
# C编译器
set(CMAKE_C_COMPILER ${CROSS_COMPILER_DIR}/bin/loongarch64-linux-gnu-gcc)
# C++编译器
set(CMAKE_CXX_COMPILER ${CROSS_COMPILER_DIR}/bin/loongarch64-linux-gnu-g++)

# 项目名称
project(ls2k300)

# 指定所用的源文件路径
aux_source_directory(${PROJECT_SOURCE_DIR}/ SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/Core/Src SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/User/Src SRC)

# 指定所用的头文件路径
include_directories(${PROJECT_SOURCE_DIR}/Core/Inc)
include_directories(${PROJECT_SOURCE_DIR}/User/Inc)

# 指定OpenCV所用头文件路径
include_directories(${PROJECT_SOURCE_DIR}/Third_Party/opencv/include/opencv4/)

# 由源文件生成一个可执行程序
add_executable(ls2k300 ${SRC})

# 连接OpenCV动态库
target_link_libraries(ls2k300 PUBLIC
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_core.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_imgproc.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_highgui.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_videoio.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_imgcodecs.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_calib3d.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_dnn.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_features2d.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_flann.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_gapi.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_objdetect.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_ml.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_photo.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_stitching.so
    ${PROJECT_SOURCE_DIR}/Third_Party/opencv/lib/libopencv_video.so
)

# 连接多线程pthread库
target_link_libraries(ls2k300 PUBLIC pthread)