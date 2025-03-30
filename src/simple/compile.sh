#!/bin/bash

echo "Compiling simplified TicTacTech game..."

GAME_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
echo "Game root directory: $GAME_ROOT"

# Use the local SFML copy
if [ -d "$GAME_ROOT/include/SFML" ]; then
    echo "Using local SFML installation"
    
    # Compile the game
    clang++ -std=c++17 TicTacTech.cpp -o TicTacTech \
        -I"$GAME_ROOT/include" \
        -L"$GAME_ROOT/lib" \
        -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
        -Wl,-rpath,"$GAME_ROOT/lib"
    
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
        echo "To run the game, use: ./TicTacTech"
    else
        echo "Compilation failed!"
    fi
# Check if SFML is installed via Homebrew as a fallback
elif [ -d "/usr/local/Cellar/sfml" ]; then
    # Get the installed SFML version (assuming only one version is installed)
    SFML_VERSION=$(ls -1 /usr/local/Cellar/sfml | head -1)
    SFML_PATH="/usr/local/Cellar/sfml/$SFML_VERSION"
    echo "Found SFML at $SFML_PATH"
    
    # Compile the game
    clang++ -std=c++17 TicTacTech.cpp -o TicTacTech \
        -I"$SFML_PATH/include" \
        -L"$SFML_PATH/lib" \
        -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
        -framework OpenGL -framework FLAC -framework ogg -framework vorbis -framework vorbisenc -framework vorbisfile
    
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
        echo "To run the game, use: ./TicTacTech"
    else
        echo "Compilation failed!"
    fi
else
    # Try alternative method if SFML is not installed via Homebrew
    echo "SFML not found. Trying to compile using system libraries..."
    
    clang++ -std=c++17 TicTacTech.cpp -o TicTacTech \
        -I/usr/local/include \
        -L/usr/local/lib \
        -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
        echo "To run the game, use: ./TicTacTech"
    else
        echo "Compilation failed. Please install SFML library."
        echo "For macOS, you can use: brew install sfml"
    fi
fi 