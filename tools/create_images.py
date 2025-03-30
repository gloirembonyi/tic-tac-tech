#!/usr/bin/env python3
"""
Modern Image Generator for TicTacTech Game
"""

import os
import sys
from math import sin, cos, pi, sqrt

print("Starting modern image generation script")

try:
    from PIL import Image, ImageDraw, ImageFilter, ImageChops
    print("Successfully imported PIL")
except ImportError:
    print("ERROR: PIL (Pillow) not found.")
    sys.exit(1)

# Create resources/images directory if it doesn't exist
images_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "resources", "images")
print(f"Images directory path: {images_dir}")

if not os.path.exists(images_dir):
    os.makedirs(images_dir)
    print(f"Created directory: {images_dir}")
else:
    print(f"Directory already exists: {images_dir}")

def add_glow(img, color, blur=3):
    """Add glow effect to an image"""
    glow = img.filter(ImageFilter.GaussianBlur(blur))
    result = Image.new('RGBA', img.size, (0, 0, 0, 0))
    result.paste(img, (0, 0), img)
    return result

def create_x_marker(filename, size=100, color=(235, 65, 65), bg_color=(0, 0, 0, 0)):
    """Create a modern X marker"""
    img = Image.new('RGBA', (size, size), bg_color)
    draw = ImageDraw.Draw(img)
    
    line_width = max(5, int(size / 12))
    padding = int(size * 0.2)
    
    # Draw X with rounded ends
    draw.line((padding, padding, size - padding, size - padding), 
              fill=color, width=line_width, joint="curve")
    draw.line((size - padding, padding, padding, size - padding), 
              fill=color, width=line_width, joint="curve")
    
    # Add glow effect
    glow = img.filter(ImageFilter.GaussianBlur(3))
    
    final_img = Image.new('RGBA', (size, size), bg_color)
    final_img.paste(glow, (0, 0), glow)
    final_img.paste(img, (0, 0), img)
    
    final_img.save(filename)
    print(f"Created {filename}")

def create_o_marker(filename, size=100, color=(65, 105, 225), bg_color=(0, 0, 0, 0)):
    """Create a modern O marker"""
    img = Image.new('RGBA', (size, size), bg_color)
    draw = ImageDraw.Draw(img)
    
    line_width = max(5, int(size / 12))
    padding = int(size * 0.15)
    
    # Draw O
    draw.ellipse((padding, padding, size - padding, size - padding), 
                 outline=color, width=line_width)
    
    # Add glow effect
    glow = img.filter(ImageFilter.GaussianBlur(3))
    
    final_img = Image.new('RGBA', (size, size), bg_color)
    final_img.paste(glow, (0, 0), glow)
    final_img.paste(img, (0, 0), img)
    
    final_img.save(filename)
    print(f"Created {filename}")

def create_background(filename, width=800, height=600):
    """Create a modern background"""
    img = Image.new('RGB', (width, height), (15, 15, 25))
    draw = ImageDraw.Draw(img)
    
    # Create elegant dark gradient
    for y in range(height):
        for x in range(width):
            # Calculate distance from center
            dx = 2 * (x - width/2) / width
            dy = 2 * (y - height/2) / height
            distance = sqrt(dx*dx + dy*dy)
            
            # Apply radial gradient
            r = int(25 - 10 * distance)
            g = int(25 - 10 * distance)
            b = int(40 - 15 * distance)
            
            # Ensure values are in valid range
            r = max(10, min(40, r))
            g = max(10, min(40, g))
            b = max(25, min(60, b))
            
            img.putpixel((x, y), (r, g, b))

    # Add subtle grid
    for x in range(0, width, 20):
        for y in range(height):
            r, g, b = img.getpixel((x, y))
            img.putpixel((x, y), (min(r+8, 255), min(g+8, 255), min(b+12, 255)))
    
    for y in range(0, height, 20):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            img.putpixel((x, y), (min(r+8, 255), min(g+8, 255), min(b+12, 255)))
    
    # Apply vignette effect
    for y in range(height):
        for x in range(width):
            # Calculate distance from center
            dx = 2 * (x - width/2) / width
            dy = 2 * (y - height/2) / height
            distance = sqrt(dx*dx + dy*dy)
            
            # Apply vignette if near edges
            if distance > 0.7:
                factor = max(0, min(1, (distance - 0.7) / 0.3))
                r, g, b = img.getpixel((x, y))
                img.putpixel((x, y), (
                    int(r * (1 - factor * 0.7)),
                    int(g * (1 - factor * 0.7)),
                    int(b * (1 - factor * 0.7))
                ))
    
    img.save(filename)
    print(f"Created {filename}")

def create_button(filename, width=200, height=50, color=(80, 80, 100), 
                outline_color=(120, 120, 150), bg_color=(0, 0, 0, 0)):
    """Create a modern button"""
    img = Image.new('RGBA', (width, height), bg_color)
    draw = ImageDraw.Draw(img)
    
    # Define corner radius
    corner_radius = int(height / 3)
    
    # Draw rounded rectangle base
    draw.rounded_rectangle(
        [(0, 0), (width-1, height-1)], 
        corner_radius, 
        fill=color,
        outline=outline_color,
        width=2
    )
    
    # Add highlight at the top
    highlight_height = int(height * 0.5)
    for y in range(highlight_height):
        alpha = int(100 * (1 - y / highlight_height))
        if alpha <= 0:
            continue
            
        if y < corner_radius:
            # Calculate width at this y position for rounded corners
            dx = corner_radius - int(sqrt(corner_radius**2 - (corner_radius - y)**2))
            draw.line(
                [(dx, y), (width - dx, y)], 
                fill=(255, 255, 255, alpha)
            )
        else:
            draw.line(
                [(0, y), (width, y)], 
                fill=(255, 255, 255, alpha)
            )
    
    # Apply slight blur for smoothness
    img = img.filter(ImageFilter.GaussianBlur(radius=0.5))
    
    img.save(filename)
    print(f"Created {filename}")

# Create all the required images
print("Creating modern game images...")

# X markers
create_x_marker(os.path.join(images_dir, "x.png"), color=(235, 65, 65))
create_x_marker(os.path.join(images_dir, "x_dark.png"), color=(180, 50, 50))
create_x_marker(os.path.join(images_dir, "x_color.png"), color=(255, 40, 80))

# O markers
create_o_marker(os.path.join(images_dir, "o.png"), color=(65, 105, 225))
create_o_marker(os.path.join(images_dir, "o_dark.png"), color=(50, 80, 180))
create_o_marker(os.path.join(images_dir, "o_color.png"), color=(40, 120, 255))

# Background
create_background(os.path.join(images_dir, "background.png"))

# Buttons
create_button(os.path.join(images_dir, "button.png"), color=(80, 80, 100), outline_color=(120, 120, 150))
create_button(os.path.join(images_dir, "button_hover.png"), color=(100, 100, 130), outline_color=(160, 160, 200))
create_button(os.path.join(images_dir, "button_pressed.png"), color=(60, 60, 80), outline_color=(100, 100, 130))

print("Modern image generation complete!") 