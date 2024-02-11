cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(puiusgui)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

file(GLOB files CONFIGURE_DEPENDS "src/*.c")
file(GLOB tests CONFIGURE_DEPENDS "src/test/*.c")
file(GLOB headers "src/include/*.h")

add_library(${PROJECT_NAME} SHARED ${files} src/arial.ttf)
add_executable(test ${tests})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/src/include ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(test PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/include ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${headers}"
)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib/ PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})
