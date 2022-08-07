cmake_minimum_required(VERSION 3.23)
project(mode7mysdl)

set(CMAKE_CXX_STANDARD 20)

include_directories(I:/SDL2/include lib)
link_directories(I:/SDL2/lib/x64 lib)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME}  ${SRC_FILES} src/function.h src/Camera.cpp src/Camera.h)

#looks for a file with .lib extension
target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf SDL2_Image)

#Assets
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        )
add_dependencies(${PROJECT_NAME} copy_assets)