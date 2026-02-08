#include <SFML/Graphics.hpp>
#include <cstdio>
#include <vector>

int main()
{
    printf("Starting rendering: c_snake window\n");

    // set colors
    const sf::Color backgroundColor = sf::Color::Black;
    const sf::Color lineColor = sf::Color::White;

    const int width = 800;
    const int height = 600;
    const int bloc_size = 20;

    int frame = 0;

    // Create lines on the grid
    std::vector<sf::Vertex> lines;
    for (int x = 0; x <= width; x += bloc_size) {
        lines.push_back(sf::Vertex(sf::Vector2f(x, 0), lineColor));
        lines.push_back(sf::Vertex(sf::Vector2f(x, height), lineColor));
    }
    for (int y = 0; y <= height; y += bloc_size) {
        lines.push_back(sf::Vertex(sf::Vector2f(0, y), lineColor));
        lines.push_back(sf::Vertex(sf::Vector2f(width, y), lineColor));
    }

    // Try to load font
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        printf("Failed to load arial.ttf, please verifiy that it is correctly installed locally..\n");
    }

    sf::Text text;
    if (font.getInfo().family != "") { // Check if font was loaded successfully
        text = sf::Text("Tick: 0", font); // Initialize with default text
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setPosition(10, 10);
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode({width, height}), "Snake Game");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(&lines[0], lines.size(), sf::Lines);
        text.setString("frames: " + std::to_string(++frame));
        window.draw(text);

        // end the current frame
        window.display();
    }
    printf("Thank you for playing!\n");
}
