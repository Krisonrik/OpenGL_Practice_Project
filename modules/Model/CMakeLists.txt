cmake_minimum_required(VERSION 3.7)
project(Model)

if(MSVC)
  set(DLLIMPORT "__declspec(dllimport)")
  set(DLLEXPORT "__declspec(dllexport)")
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/Model.cpp

)


set(PROJECT_INCS
${PROJECT_SOURCE_DIR}/include/hvr/Model/Model.hpp
)

source_group("include" FILES ${PROJECT_INCS})
source_group("src"     FILES ${PROJECT_SRCS})

find_package( OpenCV REQUIRED )
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${PROJECT_INCS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

if(MSVC)
target_compile_definitions(${PROJECT_NAME} PUBLIC
              _CRT_SECURE_NO_WARNINGS)
endif()


target_compile_definitions(${PROJECT_NAME} INTERFACE
                          HVR_RICHARD_MODEL_DLL=${DLLIMPORT})


target_compile_definitions(${PROJECT_NAME} PRIVATE
                          HVR_RICHARD_MODEL_DLL=${DLLEXPORT})

target_include_directories(${PROJECT_NAME} PUBLIC
                          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${OpenCV_INCLUDE_DIRS}
                          #${GLEW_INCLUDE_PATH}
                          ${ASSIMP_INCLUDE_DIR}
                          # ${ASSIMP_ROOT_DIR}/include
                          ${OPENGL_INCLUDE_DIR})
# set(ASSIMP_LIB 
#   C:/Libraries/vcpkg/packages/assimp_x64-windows/lib)
# message(${ASSIMP_LIB})
# C:/Libraries/vcpkg/packages/assimp_x64-windows/lib
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES} loadMesh loadImg loadShader glfw GLEW::glew ${OpenCV_LIBS} ${OPENGL_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY
            FOLDER ${CMAKE_PROJECT_NAME}/modules)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES ${PROJECT_SOURCE_DIR}/include/Model.hpp
        DESTINATION include)

export(TARGETS ${PROJECT_NAME}
      APPEND FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)

file(APPEND
    ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    "set(HVR_SAMPLE_LIBRARIES \${HVR_SAMPLE_LIBRARIES} ${PROJECT_NAME})\n")

