cmake_minimum_required(VERSION 3.7)
project(Hello_Triangle)

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/Hello_Triangle.cpp
)

find_package( OpenCV REQUIRED )
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

source_group("src" FILES ${PROJECT_SRCS})

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${OpenCV_INCLUDE_DIRS}
                          ${GLEW_INCLUDE_PATH}
                          ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} glfw ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/apps)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION bin)
