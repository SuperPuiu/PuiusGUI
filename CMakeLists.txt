cmake_minimum_required(VERSION 3.5.0)
project(puiusgui)

file(GLOB files CONFIGURE_DEPENDS "src/*.c")
file(GLOB tests CONFIGURE_DEPENDS "src/test/*.c")
file(GLOB headers "src/include/*.h" "src/include/*.hpp")

add_library(${PROJECT_NAME} SHARED ${files})
add_executable(test ${tests})


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(test PUBLIC src/include)
target_link_libraries(test PUBLIC ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC src/include)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2 SDL2_image SDL2_ttf)
set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${headers}"
)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib/ PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})