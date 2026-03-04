# Prompter

A C++ teleprompter application that automatically scrolls text with adjustable speed. The window stays on top of other applications, making it perfect for presentations and video recordings.

## Features

- **Load Text Files**: Open and display any text file
- **Auto-Scroll**: Automatically scroll text at a configurable speed
- **Speed Control**: Adjust scrolling speed from 0.1 to 2.0 pixels per interval with smooth fractional scrolling
- **Font Size Control**: Adjust font size from 12pt to 22pt in real-time
- **Settings Persistence**: Automatically saves and restores last file, speed, font size, window size, and position
- **Always On Top**: Window stays above all other applications
- **Large Text Display**: Adjustable font size, center-aligned for easy reading
- **Smooth Scrolling**: Enhanced animation at 50fps for fluid movement
- **Simple Controls**: Start, Stop, Reset, and Load buttons for easy operation

## Requirements

- C++17 compatible compiler
- CMake 3.16 or higher
- Qt6 (Core and Widgets modules)

### Installing Qt6 on Linux

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install qt6-base-dev build-essential cmake
```

#### Fedora:
```bash
sudo dnf install qt6-qtbase-devel cmake gcc-c++
```

#### Arch Linux:
```bash
sudo pacman -S qt6-base cmake gcc
```

## Building the Application

1. Clone or navigate to the project directory:
```bash
cd /home/akomarovskyi/Projects/prompter
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure with CMake:
```bash
cmake ..
```

4. Build the project (use ninja or make depending on your system):
```bash
ninja
# or if you have make instead of ninja:
# make
```

5. Run the application:
```bash
./prompter
```

## Installation (Optional)

To install the application system-wide:

```bash
sudo ninja install
# or: sudo make install
```

This will install the `prompter` executable to `/usr/local/bin`.

## Usage

1. **Launch the application**:
   ```bash
   ./prompter
   ```
   The app will automatically restore your last session (file, speed, font size, and window position).

2. **Load a text file** (if not auto-loaded):
   - Click the "Load File" button
   - Select a text file (.txt or any text format)

3. **Adjust scroll speed** (optional):
   - Use the speed slider to set your preferred scrolling speed
   - Values range from 0.1 (slowest) to 2.0 (fastest)
   - Default is 0.5

4. **Adjust font size** (optional):
   - Use the font slider to set your preferred text size
   - Values range from 12pt to 22pt
   - Default is 16pt

5. **Start scrolling**:
   - Click the "Start" button to begin auto-scrolling
   - Click "Stop" to pause at any time

6. **Reset**:
   - Click "Reset" to return to the top of the text

## Controls

- **Load File**: Opens a file dialog to select a text file
- **Start/Stop**: Toggles automatic scrolling
- **Reset**: Returns scroll position to the beginning
- **Speed Slider**: Adjusts scrolling speed in real-time (0.1-2.0 pixels)
- **Font Slider**: Adjusts text size in real-time (12-22pt)

## Settings Persistence

The application automatically saves:
- Last opened file path
- Scroll speed setting
- Font size setting
- Window size and position

These settings are restored when you launch the app again, so you can continue right where you left off.

## Window Behavior

The prompter window is set to always stay on top of other windows, ensuring your script remains visible during presentations or recordings.

## Tips

- For best results, use plain text files (.txt)
- The comfortable reading speed range is typically 0.3-0.8 pixels per interval
- Adjust the font size based on your distance from the screen
- The application stops automatically when reaching the end of the text
- You can change both speed and font size while scrolling is active
- Settings are automatically saved, so your preferences persist between sessions
- The last file you opened will automatically load on startup

## Project Structure

```
prompter/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main.cpp
    ├── MainWindow.h
    └── MainWindow.cpp
```

## License

This project is free to use and modify.
