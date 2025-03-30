#!/usr/bin/env python3
"""
SVG to PNG Converter for TicTacTech Game

This script converts SVG images to PNG for compatibility with systems
that don't support SVG rendering in SFML.
It requires Python 3.6+ and the 'cairosvg' library.

Installation:
    pip install cairosvg

Usage:
    python convert_svg_to_png.py
"""

import os
import glob

try:
    import cairosvg
except ImportError:
    print("Required library not found. Please install with:")
    print("pip install cairosvg")
    exit(1)

def ensure_directory(directory):
    """Ensure the directory exists."""
    if not os.path.exists(directory):
        os.makedirs(directory)
        print(f"Created directory: {directory}")

def convert_svg_to_png(svg_file, png_file):
    """Convert SVG file to PNG."""
    try:
        cairosvg.svg2png(url=svg_file, write_to=png_file)
        print(f"Converted {svg_file} to {png_file}")
    except Exception as e:
        print(f"Error converting {svg_file}: {e}")

def main():
    """Main function to convert all SVG files to PNG."""
    print("Converting SVG images to PNG for TicTacTech game...")
    
    # Get the path to the images directory
    images_dir = "../resources/images"
    ensure_directory(images_dir)
    
    # Find all SVG files
    svg_files = glob.glob(os.path.join(images_dir, "*.svg"))
    
    if not svg_files:
        print(f"No SVG files found in {images_dir}")
        return
    
    print(f"Found {len(svg_files)} SVG files to convert")
    
    # Convert each SVG file to PNG
    for svg_file in svg_files:
        png_file = svg_file.replace(".svg", ".png")
        convert_svg_to_png(svg_file, png_file)
    
    print("\nConversion complete! PNG files are now available in the resources/images directory.")
    print("Both SVG and PNG versions are kept, so the game can use the appropriate format based on compatibility.")

if __name__ == "__main__":
    main() 