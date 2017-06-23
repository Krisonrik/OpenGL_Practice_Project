cmake_minimum_required(VERSION 3.7)
project(loadShader)

if(MSVC)
  set(DLLIMPORT "__declspec(dllimport)")
  set(DLLEXPORT "__declspec(dllexport)")
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/loadShader.cpp
)

set(PROJECT_INCS
${PROJECT_SOURCE_DIR}/include/hvr/loadShader/loadShader.hpp
)

set(PROJECT_SHAD
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader_excercise2.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader_excercise3.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader_texture.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader_transformations.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader_transformations_exercise_02.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader_coordinate-systems.glsl
${PROJECT_SOURCE_DIR}/include/hvr/glsl/vertex_shader_lighting_colors.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/fragment_shader.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/fragment_shader_excercise3.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/fragment_shader_texture.glsl
# ${PROJECT_SOURCE_DIR}/include/hvr/glsl/fragment_shader_texture_excercise4.glsl
${PROJECT_SOURCE_DIR}/include/hvr/glsl/fragment_shader_lighting_colors.glsl

)

source_group("include" FILES ${PROJECT_INCS})
source_group("src"     FILES ${PROJECT_SRCS})
source_group("shader" FILES ${PROJECT_SHAD})

find_package( OpenCV REQUIRED )
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${PROJECT_INCS} ${PROJECT_SHAD})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_compile_definitions(${PROJECT_NAME} INTERFACE
                          HVR_RICHARD_LOADSHADER_DLL=${DLLIMPORT})

target_compile_definitions(${PROJECT_NAME} PRIVATE
                          HVR_RICHARD_LOADSHADER_DLL=${DLLEXPORT})

target_include_directories(${PROJECT_NAME} PUBLIC
                          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${OpenCV_INCLUDE_DIRS}
                          #${GLEW_INCLUDE_PATH}
                          ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} glfw GLEW::glew ${OPENGL_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY
            FOLDER ${CMAKE_PROJECT_NAME}/modules)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES ${PROJECT_SOURCE_DIR}/include/loadShader.hpp
        DESTINATION include)

export(TARGETS ${PROJECT_NAME}
      APPEND FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)

file(APPEND
    ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    "set(HVR_SAMPLE_LIBRARIES \${HVR_SAMPLE_LIBRARIES} ${PROJECT_NAME})\n")

if(MSVC)
  add_custom_command(TARGET loadShader POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${PROJECT_SOURCE_DIR}/include/hvr/glsl/
              ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>)
elseif((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCC
    OR CMAKE_COMPILER_IS_GNUCXX)

  file(GLOB DENOISE_IMAGES ${PROJECT_SOURCE_DIR}/data/Denoise/*)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/denoisedata.cmake
    "file(COPY ${DENOISE_IMAGES} DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/data/Denoise
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_WRITE GROUP_READ WORLD_READ)")

  add_custom_command(TARGET DenoiseTest POST_BUILD COMMAND ${CMAKE_COMMAND}
                  -P ${CMAKE_CURRENT_BINARY_DIR}/denoisedata.cmake)
endif()
