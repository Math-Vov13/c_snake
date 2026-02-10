#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>

class object{
    // Class Object to show on screen
    public:
    int x;
    int y;
    sf::RectangleShape shape;
    object(int x, int y){
        this->x = x;
        this->y = y;
        shape.setSize({20, 20});
        shape.setPosition({(float)x, (float)y});
    }

    ~object(){
    }
};

class item : public object{
    // Utilities item for player
    public:
    sf::Color COLOR;
    std::string NAME;
    std::string DESCRIPTION;
    item(int x, int y) : object(x, y) {
    }
};

class apple : public item{
    // Apple : eat to grow
    public:
    const sf::Color COLOR = sf::Color::Red;
    const std::string NAME = "Apple";
    const std::string DESCRIPTION = "Gives you +1 level";

    apple(int x, int y) : item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class boost : item{
    // boost : eat to move faster
    public:
    const sf::Color COLOR = sf::Color::Blue;
    const std::string NAME = "Speed Boost";
    const std::string DESCRIPTION = "Gives you x1.5 movement speed";

    boost(int x, int y) : item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class snake_queue : object{
    // snake queue block
};

class snake{
    // snake playable
};

class game{
    // game interface (main class)
    private:
    sf::RenderWindow* window;
    std::vector<sf::Vertex> lines;
    sf::Font font;
    sf::Text text;

    int frame = 0;
    snake Snake;
    std::vector<object> object_to_show;

    public:
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int BORDER_SIZE = 40;
    const int BLOC_SIZE = 20;

    // set colors
    const sf::Color BACKGROUND_COLOR = sf::Color::Black;
    const sf::Color GRID_COLOR = sf::Color::White;

    void start(){
        // Create lines on the grid
        for (int x = BORDER_SIZE; x <= WIDTH - BORDER_SIZE; x += BLOC_SIZE) {
            lines.push_back(sf::Vertex(sf::Vector2f(x, BORDER_SIZE), GRID_COLOR));
            lines.push_back(sf::Vertex(sf::Vector2f(x, HEIGHT - BORDER_SIZE), GRID_COLOR));
        }
        for (int y = BORDER_SIZE; y <= HEIGHT - BORDER_SIZE; y += BLOC_SIZE) {
            lines.push_back(sf::Vertex(sf::Vector2f(BORDER_SIZE, y), GRID_COLOR));
            lines.push_back(sf::Vertex(sf::Vector2f(WIDTH - BORDER_SIZE, y), GRID_COLOR));
        }
        printf("Taille %d", (int)lines.size());

        // Try to load font
        if (!font.loadFromFile("fonts/arial.ttf")) {
            printf("Failed to load arial.ttf, please verifiy that it is correctly installed locally..\n");
        }

        if (font.getInfo().family != "") { // Check if font was loaded successfully
            text = sf::Text("Tick: 0", font); // Initialize with default text
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Red);
            text.setPosition(10, 10);
        }

        // create the window
        window = new sf::RenderWindow(sf::VideoMode({(unsigned int)WIDTH, (unsigned int)HEIGHT}), "Snake Game");

        // run the program as long as the window is open
        while (window->isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window->pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window->close();
            }

            // Génération aléatoire (0.1% de chance par frame)
            if (rand() % 100000 == 0)
                generate_random_item();

            render_screen();
        }
    }

    void render_screen(){
        // clear the window with black color
        window->clear(BACKGROUND_COLOR);

        // draw everything here...
        // first layer: items
        for (int i = 0; i < object_to_show.end() - object_to_show.begin(); i++) {
            window->draw(object_to_show[i].shape);
        }
        // second layer: grid
        window->draw(&lines[0], lines.size(), sf::Lines);
        // third layer: texts & scores
        text.setString("frames: " + std::to_string(++frame));
        window->draw(text);

        // end the current frame
        window->display();
    }

    void generate_random_item(){
        // generate only apple for the moment! :(
        printf("spawning random item...");
        int randomX = rand() % (WIDTH - (BORDER_SIZE * 2) - 10) + BORDER_SIZE;
        int randomY = rand() % (HEIGHT - (BORDER_SIZE * 2) - 10) + BORDER_SIZE;
        apple new_item = apple(randomX, randomY);
        printf("New Item: %s\n", new_item.NAME.c_str());
        object_to_show.push_back(new_item);
    }

    ~game() {
        delete window;
    }
};

int main()
{
    srand(time(NULL));  // Initialise le générateur aléatoire
    printf("Starting rendering: c_snake window\n");

    // Run game
    game Game = game();
    Game.start();

    printf("Thank you for playing!\n");
}
