#!/bin/bash

# Navigate to the game root directory
cd "$(dirname "$0")"
GAME_ROOT="$(pwd)"
echo "Game root directory: $GAME_ROOT"

# Step 1: Generate resources if they don't exist
if [ ! -d "resources/images" ] || [ ! -d "resources/sounds" ]; then
    echo "Resources don't exist. Generating them..."
    
    # First, make sure the Python scripts are executable
    chmod +x tools/create_images.py tools/simple_sounds.py
    
    # Generate images
    echo "Generating images..."
    python3 tools/create_images.py
    
    # Generate sounds
    echo "Generating sounds..."
    python3 tools/simple_sounds.py
fi

# Step 2: Compile the game
echo "Compiling TicTacTech game..."

# Check for SFML installation
if [ -d "/usr/local/include/SFML" ]; then
    echo "Using system SFML installation"
    SFML_INCLUDE="/usr/local/include"
    SFML_LIB="/usr/local/lib"
elif [ -d "$GAME_ROOT/include/SFML" ]; then
    echo "Using local SFML installation"
    SFML_INCLUDE="$GAME_ROOT/include"
    SFML_LIB="$GAME_ROOT/lib"
else
    echo "SFML not found. Please install SFML library."
    echo "For macOS, you can use: brew install sfml"
    echo "For Linux: apt-get install libsfml-dev"
    echo "Or extract the provided SFML framework in the include/ and lib/ directories."
    exit 1
fi

# Compile the game
g++ -std=c++17 src/SimpleTicTacTech.cpp -o TicTacTech \
    -I"$SFML_INCLUDE" \
    -L"$SFML_LIB" \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    
    # Step 3: Set the library path
    if [ "$(uname)" == "Darwin" ]; then  # macOS
        export DYLD_LIBRARY_PATH="$SFML_LIB:$DYLD_LIBRARY_PATH"
        export DYLD_FRAMEWORK_PATH="$GAME_ROOT/Frameworks:$DYLD_FRAMEWORK_PATH"
    else  # Linux
        export LD_LIBRARY_PATH="$SFML_LIB:$LD_LIBRARY_PATH"
    fi
    
    # Step 4: Run the game
    echo "Starting TicTacTech game..."
    ./TicTacTech
else
    echo "Compilation failed. Please check errors above."
    exit 1
fi 