# cmake_minimum_required(VERSION 3.0.0)
# project(HelloGL VERSION 0.1.0)

# # 使用 C++ 17 标准
# set(CMAKE_CXX_STANDARD 17)

# # 设置代码文件
# set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

# # 添加头文件
# set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include/)
# set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/)
# include_directories(${HEADER_DIR} ${LIB_DIR})

# # 添加目标链接
# set(GLFW_LINK ${LIB_DIR}libglfw.3.dylib)
# link_libraries(${GLFW_LINK})

# # 执行编译命令
# set(SOURCES ${SRC_DIR}glad.c ${SRC_DIR}main.cpp)
# add_executable(HelloGL ${SOURCES})

# # 链接系统的 OpenGL 框架
# if (APPLE)
#     target_link_libraries(HelloGL "-framework OpenGL")
# endif()

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

cmake_minimum_required(VERSION 3.0.0)
project(HelloGL VERSION 0.1.0)

set(CMAKE_OSX_ARCHITECTURES "arm64")

# 使用 C++ 17 标准
set(CMAKE_CXX_STANDARD 17)

# 设置代码文件
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

# 添加头文件
set(GLEW_H /opt/homebrew/Cellar/glew/2.2.0_1/include/) 
set(GLFW_H /opt/homebrew/Cellar/glfw/3.3.8/include/)
include_directories(${GLEW_H} ${GLFW_H})
include_directories("/usr/local/include/")

# 添加目标链接
set(GLEW_LINK /opt/homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib) 
set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.3.8/lib/libglfw.3.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

# 执行编译命令
set(SOURCES ${SRC_DIR}main.cpp)
add_executable(HelloGL ${SOURCES})

if (APPLE) 
    target_link_libraries(HelloGL "-framework OpenGL") 
    target_link_libraries(HelloGL "-framework GLUT") 
endif()
# target_link_libraries(${PROJECT_NAME}
# GLEW
# GLFW
# "-framework OpenGL"
# )

