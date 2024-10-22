cmake_minimum_required(VERSION 3.29)
project(3DViewer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the correct Qt installation path
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/mingw_64")

# Find the necessary Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)
find_package(OpenGL REQUIRED)  # Ensure OpenGL is found

# Find Assimp
find_package(Assimp REQUIRED)

# Include Assimp headers
include_directories(${ASSIMP_INCLUDE_DIRS})

# Include necessary directories for the project
include_directories(${CMAKE_SOURCE_DIR}/lib)  # For additional libs if needed in the future

# Define the executable, excluding MorphIO and HDF5 for now
add_executable(3DViewer
        main.cpp
        ViewerWidget.cpp
)

# Link against the necessary Qt, OpenGL, and Assimp libraries
target_link_libraries(3DViewer
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGLWidgets
        OpenGL::GL
        ${ASSIMP_LIBRARIES}  # Assimp library for STL loading
)

# Optionally, set the output directory for the executable
set_target_properties(3DViewer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
