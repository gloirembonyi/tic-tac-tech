#!/bin/bash

# Step 1: Navigate to the game directory and check if the executable exists
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"
GAME_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

if [ ! -f "./TicTacTech" ]; then
    echo "Game executable not found. Trying to compile it first..."
    chmod +x ./compile.sh
    ./compile.sh
    
    if [ ! -f "./TicTacTech" ]; then
        echo "Failed to compile the game. Please check for errors above."
        exit 1
    fi
fi

# Step 2: Set the DYLD_LIBRARY_PATH to find the SFML libraries
if [ -d "$GAME_ROOT/lib" ]; then
    export DYLD_LIBRARY_PATH="$GAME_ROOT/lib:$DYLD_LIBRARY_PATH"
    echo "Using local SFML libraries"
elif [ -d "/usr/local/Cellar/sfml" ]; then
    SFML_VERSION=$(ls -1 /usr/local/Cellar/sfml | head -1)
    export DYLD_LIBRARY_PATH="/usr/local/Cellar/sfml/$SFML_VERSION/lib:$DYLD_LIBRARY_PATH"
    echo "Using SFML libraries from Homebrew installation"
else
    export DYLD_LIBRARY_PATH="/usr/local/lib:$DYLD_LIBRARY_PATH"
    echo "Using system SFML libraries"
fi

# Step 3: Run the game
echo "Starting TicTacTech game..."
./TicTacTech 