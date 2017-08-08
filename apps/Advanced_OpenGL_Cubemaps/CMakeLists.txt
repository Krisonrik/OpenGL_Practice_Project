cmake_minimum_required(VERSION 3.7)
project(Advanced_OpenGL_Cubemaps)

if(MSVC)
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/Advanced_OpenGL_Cubemaps.cpp
)

find_package(OpenCV REQUIRED )
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

source_group("src" FILES ${PROJECT_SRCS})

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

if(MSVC)
target_compile_definitions(${PROJECT_NAME} PUBLIC
							_CRT_SECURE_NO_WARNINGS)
endif()


target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${OpenCV_INCLUDE_DIRS}
                          # ${GLEW_INCLUDE_PATH}
                          ${ASSIMP_INCLUDE_DIR}
                          # ${ASSIMP_ROOT_DIR}/include
                          ${OPENGL_INCLUDE_DIR}
                          # ${TINYXML2_INCLUDE_DIRS}
                          )

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS}  ${ASSIMP_LIBRARIES} glm loadMesh Model loadImg loadShader loadAssetDir cameraEuler glfw GLEW::glew tinyxml2 ${OPENGL_LIBRARIES} )

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/apps)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION bin)
