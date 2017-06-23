cmake_minimum_required(VERSION 3.7)
project(cameraEuler)

if(MSVC)
  set(DLLIMPORT "__declspec(dllimport)")
  set(DLLEXPORT "__declspec(dllexport)")
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/cameraEuler.cpp
${PROJECT_SOURCE_DIR}/src/cameraEuler_fps.cpp
)

set(PROJECT_INCS
${PROJECT_SOURCE_DIR}/include/hvr/cameraEuler/cameraEuler.hpp
${PROJECT_SOURCE_DIR}/include/hvr/cameraEuler/cameraEuler_fps.hpp
)


source_group("include" FILES ${PROJECT_INCS})
source_group("src"     FILES ${PROJECT_SRCS})


find_package( OpenCV REQUIRED )
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)


add_library(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${PROJECT_INCS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_compile_definitions(${PROJECT_NAME} INTERFACE
                          HVR_RICHARD_CAMERAEULER_DLL=${DLLIMPORT}
                          HVR_RICHARD_CAMERAEULERFPS_DLL=${DLLIMPORT})


target_compile_definitions(${PROJECT_NAME} PRIVATE
                          HVR_RICHARD_CAMERAEULER_DLL=${DLLEXPORT}
                          HVR_RICHARD_CAMERAEULERFPS_DLL=${DLLEXPORT})


target_include_directories(${PROJECT_NAME} PUBLIC
                          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${OpenCV_INCLUDE_DIRS}
                          # ${GLEW_INCLUDE_PATH}
                          ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} glm glfw GLEW::glew ${OPENGL_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY
            FOLDER ${CMAKE_PROJECT_NAME}/modules)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES ${PROJECT_SOURCE_DIR}/include/cameraEuler.hpp
        DESTINATION include)

export(TARGETS ${PROJECT_NAME}
      APPEND FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)

file(APPEND
    ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    "set(HVR_SAMPLE_LIBRARIES \${HVR_SAMPLE_LIBRARIES} ${PROJECT_NAME})\n")

