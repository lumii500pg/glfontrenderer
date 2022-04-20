cmake_minimum_required(VERSION 3.22)
project(GLFontRenderer)

set(CMAKE_CXX_STANDARD 23)

set(GLFONTRENDERER_DEPENDENCY_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
set(GLFONTRENDERER_INCLUDE_DIRECTORY ${GLFONTRENDERER_DEPENDENCY_DIRECTORY}/include)
set(GLFONTRENDERER_LIBRARY_DIRECTORY ${GLFONTRENDERER_DEPENDENCY_DIRECTORY}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(GLFONTRENDERER_LIBRARIES ${GLFONTRENDERER_LIBRARY_DIRECTORY}/SDL2.lib
        ${GLFONTRENDERER_LIBRARY_DIRECTORY}/SDL2main.lib
        ${GLFONTRENDERER_LIBRARY_DIRECTORY}/freetype.lib
        ${GLFONTRENDERER_LIBRARY_DIRECTORY}/glew32.lib)

include_directories(${GLFONTRENDERER_INCLUDE_DIRECTORY})

file(GLOB_RECURSE GLFONTRENDERER_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c**)
file(GLOB_RECURSE GLFONTRENDERER_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h**)

#add_library(GLFontRenderer SHARED ${GLFONTRENDERER_SOURCE_FILES} ${GLFONTRENDERER_HEADER_FILES})
add_executable(GLFontRenderer ${GLFONTRENDERER_SOURCE_FILES} ${GLFONTRENDERER_HEADER_FILES} src/main.cpp src/logger.cpp src/logger.h src/ttfloader.cpp src/ttfloader.h src/fileutils.cpp src/fileutils.h src/bitmap.cpp src/bitmap.h src/timeutils.h src/font.h src/render/bufferbuilder.cpp src/render/bufferbuilder.h src/render/fontmeshbuilder.h)
target_link_libraries(GLFontRenderer ${GLFONTRENDERER_LIBRARIES} opengl32.lib)
