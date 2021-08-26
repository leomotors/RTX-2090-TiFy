cmake_minimum_required(VERSION 3.16)
project(RTX-2090-TiFy VERSION 1.0.69.1)

set(APP_VERSION "${CMAKE_PROJECT_VERSION}")
set(VERSION_BRANCH "Release Preview")
configure_file(config/AppProps.h.in ${CMAKE_CURRENT_BINARY_DIR}/gen/AppProps.h)

set(EXECUTABLE_NAME "RTX2090TiFy")

# Find Required Package
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Source Files
file(GLOB APP_SRCFILE "src/*.cpp")

# Compiling
if(WIN32)
    add_executable(${EXECUTABLE_NAME} WIN32 
        ${APP_SRCFILE}
    )
else()
    add_executable(${EXECUTABLE_NAME} 
        ${APP_SRCFILE}
    )
endif()

include_directories(config)
include_directories(inc)

# Link Libraries
target_link_libraries(${EXECUTABLE_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${OpenCV_LIBS})
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gen)

# C++20
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

# Add Warning Flags
if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4)
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-result -Wno-unused-parameter -Wno-sign-compare -Wno-reorder -O3")
endif()

# Making App Ready
if(WIN32)
    if(NOT MSVC)
        # For Windows with MinGW
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND move ${EXECUTABLE_NAME}.exe ..
            COMMENT "Moving Executable to Project Root..."
        )
    else()
        # For MSVC, Please use Visual Studio and Target x64-Release
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${PROJECT_SOURCE_DIR}\\scripts\\packapp.bat ${PROJECT_SOURCE_DIR} ${APP_VERSION}
            COMMENT "Packing app with packapp.bat"
        )
    endif()
else()
    # For Linux
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND mv ${EXECUTABLE_NAME} ..
        COMMENT "Moving Executable to Project Root..."
)
endif()
