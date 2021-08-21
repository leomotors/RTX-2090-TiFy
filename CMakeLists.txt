cmake_minimum_required(VERSION 3.16)
project(RTX-2090-TiFy VERSION 1.0.52.1)
set(APP_VERSION "${CMAKE_PROJECT_VERSION}")
configure_file(config/AppProps.h.in ${CMAKE_CURRENT_BINARY_DIR}/gen/AppProps.h)

# Find Required Package
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Compiling
file(GLOB APP_SRCFILE "src/*.cpp")

add_executable(RTX2090TiFy 
    ${APP_SRCFILE}
)

include_directories(inc)

# Link Libraries
target_link_libraries(RTX2090TiFy ${wxWidgets_LIBRARIES})
target_link_libraries(RTX2090TiFy ${OpenCV_LIBS})
target_include_directories(RTX2090TiFy PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gen)

# C++20
set_property(TARGET RTX2090TiFy PROPERTY CXX_STANDARD 20)

# Add Warning Flags
if(MSVC)
    add_compile_options(/Wall)
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-sign-compare -Wno-reorder -O3")
endif()

# Moving Executable to Project Root (! Maybe changed)
if(WIN32)
    if(MSVC)
    add_custom_command(TARGET RTX2090TiFy POST_BUILD
        COMMAND move Release\\RTX2090TiFy.exe ..
        COMMENT "Moving Executable to Project Root..."
    )
    else()
    add_custom_command(TARGET RTX2090TiFy POST_BUILD
        COMMAND move RTX2090TiFy.exe ..
        COMMENT "Moving Executable to Project Root..."
    )
    endif()
else()
add_custom_command(TARGET RTX2090TiFy POST_BUILD
    COMMAND mv RTX2090TiFy ..
    COMMENT "Moving Executable to Project Root..."
)
endif()
