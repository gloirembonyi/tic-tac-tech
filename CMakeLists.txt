cmake_minimum_required(VERSION 3.14)
project(TicTacTech VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML components
# Try to use Homebrew on macOS first
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/Cellar/sfml")
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/sfml")
endif()

find_package(SFML 2.5 COMPONENTS graphics audio system window REQUIRED)

# Set include directories
include_directories(include)

# Set sources
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Board.cpp
    src/UI/Button.cpp
    src/UI/Menu.cpp
    src/UI/AnimationManager.cpp
    src/UI/ThemeManager.cpp
    src/Resources/ResourceManager.cpp
    src/Resources/SoundManager.cpp
)

# Define executable
add_executable(TicTacTech ${SOURCES})

# Link SFML libraries
target_link_libraries(TicTacTech
    sfml-graphics
    sfml-audio
    sfml-system
    sfml-window
)

# Copy resources folder to build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}) 