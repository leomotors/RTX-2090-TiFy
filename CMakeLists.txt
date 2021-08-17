cmake_minimum_required(VERSION 3.16)
project(RTX-2090-TiFy VERSION 1.0.0)

# Find Required Package
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

# Compiling
file(GLOB APP_SRCFILE "src/*.cpp")

add_executable(main 
    ${APP_SRCFILE}
)

include_directories(inc)

# C++17
set_property(TARGET main PROPERTY CXX_STANDARD 17)

# Link Libraries
target_link_libraries(main ${wxWidgets_LIBRARIES})
target_link_libraries(main ${OpenCV_LIBS})

# Add Warning Flags
if(MSVC)
    add_compile_options(/Wall)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

# Moving Executable to Project Root
if(WIN32)
    if(MSVC)
    add_custom_command(TARGET main POST_BUILD
        COMMAND move Release\\main.exe ..
        COMMENT "Moving Executable to Project Root..."
    )
    else()
    add_custom_command(TARGET main POST_BUILD
        COMMAND move main.exe ..
        COMMENT "Moving Executable to Project Root..."
    )
    endif()
else()
add_custom_command(TARGET main POST_BUILD
    COMMAND mv main ..
    COMMENT "Moving Executable to Project Root..."
)
endif()
