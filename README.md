# C++ Snake Game with SFML

A simple snake game built with C++ and [SFML](https://www.sfml-dev.org/tutorials/3.0/window/window/) (Simple and Fast Multimedia Library).

## Features

- nothing more than a **simple snake game made in C++ by me!** :)

## Requirements

- C++17 compiler (g++, clang++, etc.)
- SFML 2.5 or later

## Installation

### Linux (Debian/Ubuntu)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install g++ libsfml-dev git

# Clone and build
git clone git@github.com:Math-Vov13/c_snake.git
cd c_snake
make
```

### Linux (Fedora/RHEL)

```bash
# Install dependencies
sudo dnf install gcc-c++ SFML-devel git

# Clone and build
git clone git@github.com:Math-Vov13/c_snake.git
cd c_snake
make
```

### Manual Compilation

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o snake_game -lsfml-graphics -lsfml-window -lsfml-system
```

## Running the Game

```bash
./snake_game
```

## Controls

- Close the window to exit the game
- `left-arrow` and `right-arrow` to change snake's orientation

## Development

### Project Structure

```
c_snake/
├── main.cpp          # Main game code with SFML
├── .github/workflows/ # GitHub Actions CI configuration
├── README.md         # Project documentation
└── LICENSE           # License
```

### Building with Make

Create a simple Makefile:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
TARGET = snake_game

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
```

## Troubleshooting

### Font Issues

If you see font loading errors, you can:

1. Place a font file (e.g., `arial.ttf` or `sansation.ttf`) in the project `./fonts/` directory
2. Install system fonts:
   ```bash
   # On Debian/Ubuntu
   sudo apt-get install fonts-liberation ttf-mscorefonts-installer
   
   # On Fedora
   sudo dnf install liberation-fonts mscore-fonts-all
   ```

### SFML Installation Issues

If SFML is not found during compilation:

```bash
# Check if SFML is installed
pkg-config --modversion sfml-all

# If not installed, install it:
# Debian/Ubuntu
sudo apt-get install libsfml-dev

# Fedora
sudo dnf install SFML-devel
```

### Common Compilation Errors

If you get errors about missing SFML libraries, make sure you have all the required SFML components:

```bash
sudo apt-get install libsfml-dev libsfml-graphics libsfml-window libsfml-system
```

## License

[License](LICENSE)

## Author

Mathéo VOVARD
