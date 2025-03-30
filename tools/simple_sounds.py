#!/usr/bin/env python3
"""
Simple Sound Generator for TicTacTech Game

This script generates very basic sound effect placeholder files.
It only requires the standard library.

Usage:
    python simple_sounds.py
"""

import os
import wave
import array
import math
import struct
import random
from pathlib import Path

def ensure_directory(directory):
    """Create directory if it doesn't exist"""
    if not os.path.exists(directory):
        os.makedirs(directory)
        print(f"Created directory: {directory}")

def create_simple_beep(filename, frequency=440, duration=0.3, volume=0.5, sample_rate=44100):
    """Create a simple beep sound effect"""
    # Calculate the number of frames required
    num_frames = int(duration * sample_rate)
    
    # Create a wave file
    wavefile = wave.open(filename, 'w')
    wavefile.setparams((1, 2, sample_rate, num_frames, 'NONE', 'not compressed'))
    
    # Generate a sine wave
    max_amplitude = 32767.0 * volume
    samples = array.array('h')
    
    for i in range(num_frames):
        time_point = float(i) / sample_rate
        value = int(max_amplitude * math.sin(2.0 * math.pi * frequency * time_point))
        samples.append(value)
    
    # Fade out at the end
    fade_length = min(int(num_frames * 0.2), 4000)
    for i in range(fade_length):
        samples[num_frames - fade_length + i] = int(samples[num_frames - fade_length + i] * 
                                                   (1.0 - float(i) / fade_length))
    
    # Write the samples to the wave file
    wavefile.writeframes(samples.tobytes())
    wavefile.close()
    
    print(f"Created {filename}")

def create_button_click(filename, sample_rate=44100):
    """Create a button click sound effect"""
    duration = 0.15
    num_frames = int(duration * sample_rate)
    
    wavefile = wave.open(filename, 'w')
    wavefile.setparams((1, 2, sample_rate, num_frames, 'NONE', 'not compressed'))
    
    max_amplitude = 32767.0 * 0.7
    samples = array.array('h')
    
    # Quick attack, longer decay
    attack = int(num_frames * 0.05)
    decay = num_frames - attack
    
    # Attack phase
    for i in range(attack):
        time_point = float(i) / sample_rate
        value = int(max_amplitude * (float(i) / attack) * 
                   math.sin(2.0 * math.pi * 1200 * time_point))
        samples.append(value)
    
    # Decay phase
    for i in range(decay):
        time_point = float(i + attack) / sample_rate
        decay_factor = 1.0 - float(i) / decay
        value = int(max_amplitude * decay_factor * 
                   math.sin(2.0 * math.pi * 800 * time_point))
        samples.append(value)
    
    wavefile.writeframes(samples.tobytes())
    wavefile.close()
    
    print(f"Created {filename}")

def create_button_hover(filename, sample_rate=44100):
    """Create a button hover sound effect"""
    duration = 0.1
    num_frames = int(duration * sample_rate)
    
    wavefile = wave.open(filename, 'w')
    wavefile.setparams((1, 2, sample_rate, num_frames, 'NONE', 'not compressed'))
    
    max_amplitude = 32767.0 * 0.3
    samples = array.array('h')
    
    # Generate a soft sweeping sound
    for i in range(num_frames):
        time_point = float(i) / sample_rate
        # Sweep from higher to lower frequency
        frequency = 2000 - (1000 * time_point / duration)
        value = int(max_amplitude * (1.0 - float(i) / num_frames) * 
                   math.sin(2.0 * math.pi * frequency * time_point))
        samples.append(value)
    
    wavefile.writeframes(samples.tobytes())
    wavefile.close()
    
    print(f"Created {filename}")

def create_move_sound(filename, is_x=True, sample_rate=44100):
    """Create a move sound for X or O"""
    duration = 0.2
    num_frames = int(duration * sample_rate)
    
    wavefile = wave.open(filename, 'w')
    wavefile.setparams((1, 2, sample_rate, num_frames, 'NONE', 'not compressed'))
    
    max_amplitude = 32767.0 * 0.6
    samples = array.array('h')
    
    # Different frequency characteristics for X and O
    base_freq = 600 if is_x else 400
    freq_mod = 200 if is_x else 100
    
    for i in range(num_frames):
        time_point = float(i) / sample_rate
        progress = float(i) / num_frames
        
        # Modulate frequency
        frequency = base_freq + freq_mod * (1.0 - progress)
        
        # Apply envelope
        envelope = math.sin(math.pi * progress) if progress < 0.8 else (1.0 - (progress - 0.8) / 0.2)
        
        value = int(max_amplitude * envelope * 
                   math.sin(2.0 * math.pi * frequency * time_point))
        samples.append(value)
    
    wavefile.writeframes(samples.tobytes())
    wavefile.close()
    
    print(f"Created {filename}")

def create_win_sound(filename, sample_rate=44100):
    """Create a winning sound effect"""
    duration = 1.0
    num_frames = int(duration * sample_rate)
    
    wavefile = wave.open(filename, 'w')
    wavefile.setparams((1, 2, sample_rate, num_frames, 'NONE', 'not compressed'))
    
    max_amplitude = 32767.0 * 0.7  # Reduced volume to prevent overflow
    samples = array.array('h')
    
    # Create a cheerful arpeggio
    frequencies = [523.25, 659.26, 783.99, 1046.5]  # C5, E5, G5, C6
    
    for i in range(num_frames):
        time_point = float(i) / sample_rate
        progress = float(i) / num_frames
        
        # Choose frequency based on time
        freq_idx = min(int(progress * 16) % 8, 3)
        frequency = frequencies[freq_idx]
        
        # Apply envelope
        envelope = min(1.0, 10.0 * progress) if progress < 0.1 else max(0.0, 1.0 - (progress - 0.6) / 0.4)
        
        value = int(max_amplitude * envelope * 
                   math.sin(2.0 * math.pi * frequency * time_point))
        # Clamp value to prevent overflow
        value = max(min(value, 32767), -32768)
        samples.append(value)
    
    wavefile.writeframes(samples.tobytes())
    wavefile.close()
    
    print(f"Created {filename}")

def create_draw_sound(filename, sample_rate=44100):
    """Create a draw sound effect"""
    duration = 0.8
    num_frames = int(duration * sample_rate)
    
    wavefile = wave.open(filename, 'w')
    wavefile.setparams((1, 2, sample_rate, num_frames, 'NONE', 'not compressed'))
    
    max_amplitude = 32767.0 * 0.6  # Reduced volume to prevent overflow
    samples = array.array('h')
    
    # Create a neutral, slightly descending sound
    for i in range(num_frames):
        time_point = float(i) / sample_rate
        progress = float(i) / num_frames
        
        # Descending frequency
        frequency = 400 - 100 * progress
        
        # Apply envelope
        envelope = min(1.0, 5.0 * progress) if progress < 0.2 else max(0.0, 1.0 - (progress - 0.5) / 0.5)
        
        value = int(max_amplitude * envelope * 
                   math.sin(2.0 * math.pi * frequency * time_point))
        # Clamp value to prevent overflow
        value = max(min(value, 32767), -32768)
        samples.append(value)
    
    wavefile.writeframes(samples.tobytes())
    wavefile.close()
    
    print(f"Created {filename}")

def create_empty_ogg(filename):
    """Create an empty OGG file as a placeholder"""
    with open(filename, 'wb') as f:
        f.write(b'')
    print(f"Created placeholder {filename}")

def generate_all_sounds():
    """Generate all sound effects for the game"""
    print("Generating simple sound effects for TicTacTech game...")
    
    # Ensure sounds directory exists
    sounds_dir = "../resources/sounds"
    ensure_directory(sounds_dir)
    
    # Create UI sounds
    create_button_click(f"{sounds_dir}/button_click.wav")
    create_button_hover(f"{sounds_dir}/button_hover.wav")
    
    # Create game sounds
    create_move_sound(f"{sounds_dir}/x_move.wav", is_x=True)
    create_move_sound(f"{sounds_dir}/o_move.wav", is_x=False)
    create_win_sound(f"{sounds_dir}/win.wav")
    create_draw_sound(f"{sounds_dir}/draw.wav")
    
    # Create empty OGG placeholders for music
    create_empty_ogg(f"{sounds_dir}/background_music.ogg")
    create_empty_ogg(f"{sounds_dir}/menu_music.ogg")
    
    print("\nSound generation complete! Placeholder sound files have been created.")
    print("Note: The OGG music files are empty placeholders.")

if __name__ == "__main__":
    generate_all_sounds() 