# TicTacTech - Advanced Tic Tac Toe Game

A visually stunning and feature-rich implementation of the classic Tic Tac Toe game, using modern C++ and SFML.

## Features

- **Polished User Interface**: Clean, modern UI with smooth animations and transitions
- **Multiple Themes**: Choose from different visual themes (Classic, Dark, Colorful)
- **Sound Effects**: Immersive audio experience with unique sounds for different actions
- **Responsive Design**: Adapts to different window sizes for optimal viewing
- **Two-Player Mode**: Local multiplayer gameplay

## Screenshots

[Screenshots will be added here]

## Requirements

- C++17 or newer
- SFML 2.5 or newer
- CMake 3.14 or newer (for CMake build method)

## Building and Running

### Quick Start (Using Direct Compilation)

```bash
# Clone the repository
git clone https://github.com/gloirembonyi/tic-tac-tech.git
cd tic-tac-tech

# Make the run script executable
chmod +x run_game.sh

# Compile and run the game
./run_game.sh
```

### Alternative: Using CMake

```bash
# Clone the repository
git clone https://github.com/gloirembonyi/tic-tac-tech.git
cd tic-tac-tech

# Create build directory
mkdir build
cd build

# Generate build files and compile
cmake ..
make

# Run the game
./TicTacTech
```

## Helper Tools

This project includes helpful utility scripts in the `tools/` directory:

1. **Sound Generator** (`tools/simple_sounds.py`):

   - Generates sample sound effects for the game
   - Requirements: Python 3.6+, standard library only
   - Run with: `python tools/simple_sounds.py`

2. **Image Generator** (`tools/create_images.py`):
   - Creates PNG images for the game
   - Requirements: Python 3.6+, `pillow` library
   - Install with: `pip install pillow`
   - Run with: `python tools/create_images.py`

## Controls

- **Mouse**: Click on cells to place your marker (X or O)
- **UI Interaction**: Click on buttons to navigate menus and change settings
- **ESC Key**: Exit fullscreen mode

## Project Structure

- `src/`: Source code files
  - `SimpleTicTacTech.cpp`: Main game implementation with SFML
- `resources/`: Game assets
  - `images/`: Textures and images
  - `fonts/`: Text fonts
  - `sounds/`: Audio files
- `tools/`: Helper scripts for resource generation
- `include/`: SFML header files
- `lib/`: SFML library files

## Customization

You can easily customize the game by:

1. Adding new themes in `ThemeManager.cpp`
2. Creating new marker designs in the resources folder
3. Modifying sound effects and music

## Future Enhancements

- AI opponent with multiple difficulty levels
- Online multiplayer
- Game statistics and achievements
- Custom board sizes beyond 3x3

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- SFML development team for the excellent multimedia library
- [Add other acknowledgments as needed]

## Contact

[Your contact information here]
