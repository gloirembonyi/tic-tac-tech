#!/usr/bin/env python3
"""
Sound Generator for TicTacTech Game

This script generates simple sound effects for the game using the synthesizer library.
It requires Python 3.6+ and the 'synthesizer' and 'numpy' libraries.

Installation:
    pip install synthesizer numpy

Usage:
    python generate_sounds.py
"""

import os
import numpy as np

try:
    from synthesizer import Player, Synthesizer, Waveform
except ImportError:
    print("Required libraries not found. Please install with:")
    print("pip install synthesizer numpy")
    exit(1)

def ensure_directory(directory):
    """Ensure the directory exists."""
    if not os.path.exists(directory):
        os.makedirs(directory)
        print(f"Created directory: {directory}")

def save_sound(data, filename):
    """Save the sound data to a WAV file."""
    try:
        import scipy.io.wavfile as wavfile
        wavfile.write(filename, 44100, data.astype(np.int16))
        print(f"Created {filename}")
    except ImportError:
        print("Could not save WAV file: scipy library is required.")
        print("Install with: pip install scipy")

def generate_button_click():
    """Generate a button click sound."""
    player = Player()
    player.open_stream()
    synthesizer = Synthesizer(osc1_waveform=Waveform.square, osc1_volume=1.0, use_osc2=False)
    
    # Generate a short click
    frames = 8000  # ~0.2 seconds
    data = np.zeros(frames, dtype=np.float64)
    
    # Add the click sound (higher pitch, rapid decay)
    for i in range(frames):
        t = i / 44100
        amp = np.exp(-30 * t)  # Exponential decay
        freq = 1200  # Higher frequency for click
        sample = synthesizer.generate_constant_wave(freq, amp)
        data[i] = sample
    
    # Normalize
    data = data / np.max(np.abs(data)) * 32767
    
    return data

def generate_button_hover():
    """Generate a button hover sound."""
    player = Player()
    player.open_stream()
    synthesizer = Synthesizer(osc1_waveform=Waveform.sine, osc1_volume=0.8, use_osc2=False)
    
    # Generate a soft hover sound
    frames = 5000  # ~0.1 seconds
    data = np.zeros(frames, dtype=np.float64)
    
    # Add the hover sound (soft, short)
    for i in range(frames):
        t = i / 44100
        amp = np.exp(-40 * t)  # Fast decay
        freq = 800  # Medium frequency
        sample = synthesizer.generate_constant_wave(freq, amp)
        data[i] = sample
    
    # Normalize
    data = data / np.max(np.abs(data)) * 32767 * 0.5  # Lower volume
    
    return data

def generate_move_sound(is_x=True):
    """Generate a sound for placing X or O."""
    player = Player()
    player.open_stream()
    
    # Different waveform for X and O
    waveform = Waveform.sawtooth if is_x else Waveform.sine
    synthesizer = Synthesizer(osc1_waveform=waveform, osc1_volume=1.0, use_osc2=False)
    
    # Generate a placement sound
    frames = 15000  # ~0.34 seconds
    data = np.zeros(frames, dtype=np.float64)
    
    # X has a descending tone, O has an ascending tone
    for i in range(frames):
        t = i / 44100
        amp = np.exp(-10 * t)  # Slower decay
        
        if is_x:
            # Descending tone for X
            freq = 600 - 200 * t
        else:
            # Ascending tone for O
            freq = 400 + 200 * t
            
        sample = synthesizer.generate_constant_wave(freq, amp)
        data[i] = sample
    
    # Normalize
    data = data / np.max(np.abs(data)) * 32767 * 0.7
    
    return data

def generate_win_sound():
    """Generate a victory sound."""
    player = Player()
    player.open_stream()
    synthesizer = Synthesizer(osc1_waveform=Waveform.sine, osc1_volume=0.8, use_osc2=True,
                             osc2_waveform=Waveform.square, osc2_volume=0.3)
    
    # Generate a victory fanfare
    frames = 44100  # ~1 second
    data = np.zeros(frames, dtype=np.float64)
    
    # Create a simple ascending arpeggio
    notes = [400, 500, 600, 800]  # Frequencies for the notes
    note_duration = frames // len(notes)
    
    for i, note in enumerate(notes):
        start = i * note_duration
        end = (i + 1) * note_duration
        
        for j in range(start, end):
            t = (j - start) / 44100
            amp = np.exp(-3 * t)  # Slower decay for sustained notes
            sample = synthesizer.generate_constant_wave(note, amp)
            data[j] = sample
    
    # Add a final chord
    for i in range(frames // 2, frames):
        t = (i - frames // 2) / 44100
        amp = np.exp(-2 * t)  # Slow decay for final chord
        
        # Generate a chord (multiple frequencies)
        sample = (synthesizer.generate_constant_wave(600, amp) +
                 synthesizer.generate_constant_wave(750, amp) +
                 synthesizer.generate_constant_wave(900, amp)) / 3
        
        data[i] += sample  # Add to existing data
    
    # Normalize
    data = data / np.max(np.abs(data)) * 32767 * 0.8
    
    return data

def generate_draw_sound():
    """Generate a sound for a draw game."""
    player = Player()
    player.open_stream()
    synthesizer = Synthesizer(osc1_waveform=Waveform.triangle, osc1_volume=0.7, use_osc2=True,
                             osc2_waveform=Waveform.sine, osc2_volume=0.3)
    
    # Generate a "neutral" sound for draws
    frames = 30000  # ~0.7 seconds
    data = np.zeros(frames, dtype=np.float64)
    
    # Create a wavering tone
    for i in range(frames):
        t = i / 44100
        amp = np.exp(-5 * t)  # Medium decay
        
        # Wavering frequency
        freq = 350 + 50 * np.sin(2 * np.pi * 3 * t)
        
        sample = synthesizer.generate_constant_wave(freq, amp)
        data[i] = sample
    
    # Normalize
    data = data / np.max(np.abs(data)) * 32767 * 0.7
    
    return data

def main():
    """Main function to generate all sound effects."""
    print("Generating sound effects for TicTacTech game...")
    
    # Ensure the sound directory exists
    sound_dir = "../resources/sounds"
    ensure_directory(sound_dir)
    
    # Generate and save all sounds
    sounds = {
        "button_click.wav": generate_button_click(),
        "button_hover.wav": generate_button_hover(),
        "x_move.wav": generate_move_sound(is_x=True),
        "o_move.wav": generate_move_sound(is_x=False),
        "win.wav": generate_win_sound(),
        "draw.wav": generate_draw_sound()
    }
    
    for filename, data in sounds.items():
        save_sound(data, os.path.join(sound_dir, filename))
    
    print("\nSound generation complete! Place these files in your resources/sounds directory.")
    print("For background music, you'll need to find or create OGG files named:")
    print("- background_music.ogg")
    print("- menu_music.ogg")

if __name__ == "__main__":
    main() 