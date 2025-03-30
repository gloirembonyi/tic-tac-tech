# Game Resources

This directory contains all the assets needed for the TicTacTech game.

## Directory Structure

- `images/`: Contains all game textures and images
  - Game markers (X and O) in different themes
  - UI elements such as buttons and backgrounds
- `fonts/`: Contains text fonts
  - Main font for UI text
  - Title font for headings
- `sounds/`: Contains audio files
  - Sound effects for game actions (moves, wins, menu interactions)
  - Background music tracks

## File Formats

- Images: PNG format (with transparency)
- Fonts: TTF format
- Sound Effects: WAV format
- Music: OGG format

## Adding Custom Resources

To add your own custom resources:

1. Place new files in the appropriate directory
2. Update the corresponding resource manager's loading function
   - For textures: Add to `ResourceManager::loadDefaultResources()`
   - For sounds: Add to `SoundManager::loadDefaultSounds()`
3. Use the resources in game via their respective managers

## Default Resources

The game includes the following default resources:

### Textures

- X and O markers in three themes (default, dark, colorful)
- Menu backgrounds
- Button states (normal, hover, pressed)

### Fonts

- Open Sans Regular
- Open Sans Bold

### Sounds

- Button click
- Button hover
- X move
- O move
- Win sound
- Draw sound
- Background music
- Menu music

## Note

Before running the game, ensure that all required resources are present. Missing resources will cause loading errors that are reported to the console.
