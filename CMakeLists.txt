cmake_minimum_required(VERSION 3.16)
project(RTX-2090-TiFy)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main src/main.cpp)
target_link_libraries(main ${OpenCV_LIBS})

set_property(TARGET main PROPERTY CXX_STANDARD 17)

if(MSVC)
    add_compile_options(/Wall)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

if(WIN32)
add_custom_command(TARGET main POST_BUILD
    COMMAND move Release\\main.exe ..
    COMMENT "Moving Executable to Project Root..."
)
else()
add_custom_command(TARGET main POST_BUILD
    COMMAND mv main ..
    COMMENT "Moving Executable to Project Root..."
)
endif()
