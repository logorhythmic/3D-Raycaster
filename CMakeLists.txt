cmake_minimum_required(VERSION 3.25...4.3)

project(raycaster-3D)

add_executable(${PROJECT_NAME} 
  src/Main.c
  src/boundaries.c
  src/framebuffer_3D.c
  src/player.c
  src/raycast_2D.c
  src/raycast_3D.c
  src/render_scene.c)

set(CMAKE_C_FLAGS_RELEASE "-O3")

target_include_directories( ${PROJECT_NAME} PRIVATE include)
  
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${MATH_LIBRARY})
endif()

find_package(SDL3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
	"$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)


add_custom_target(run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
