#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>

class Props{
    // Class Object to show on screen
    public:
    int x;
    int y;
    sf::RectangleShape shape;
    Props(int x, int y){
        this->x = x;
        this->y = y;
        shape.setSize({20, 20});
        shape.setPosition({(float)x, (float)y});
    }
};

class Item : public Props{
    // Utilities item for player
    public:
    sf::Color COLOR;
    std::string NAME;
    std::string DESCRIPTION;
    Item(int x, int y) : Props(x, y) {
    }
};

class Apple : public Item{
    // Apple : eat to grow
    public:
    const sf::Color COLOR = sf::Color::Red;
    const std::string NAME = "Apple";
    const std::string DESCRIPTION = "Gives you +1 level";

    Apple(int x, int y) : Item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class Boost : Item{
    // boost : eat to move faster
    public:
    const sf::Color COLOR = sf::Color::Blue;
    const std::string NAME = "Speed Boost";
    const std::string DESCRIPTION = "Gives you x1.5 movement speed";

    Boost(int x, int y) : Item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class SnakeQueue : Props{
    // snake queue block
    public:
    const sf::Color COLOR = sf::Color::Green;

    SnakeQueue(int x, int y) : Props(x, y) {
        shape.setFillColor(COLOR);
    }
};

class Snake{
    // snake playable
    private:
    std::vector<SnakeQueue> items_queue;

    void init_snake_queue(){
        for (int i=0; i < snakeSize; i++) {
            SnakeQueue newSnakeQueueBlock(1, 2);
            items_queue.push_back(newSnakeQueueBlock);
        }
    }

    public:
    int snakeSize = 5;
    int orientation = 90;
    int headPos = 0;

    void changeOrientation(int degree){
        orientation += degree;
    }

    Snake(){
        init_snake_queue();
    }
};

class Game{
    // game interface (main class)
    private:
    sf::RenderWindow* window;
    std::vector<sf::Vertex> lines;
    sf::Font font;
    sf::Text text;

    int frame = 0;
    Snake* snake;
    std::vector<Props> object_to_show;

    public:
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int BORDER_SIZE = 40;
    const int BLOC_SIZE = 20;

    // set colors
    const sf::Color BACKGROUND_COLOR = sf::Color::Black;
    const sf::Color GRID_COLOR = sf::Color::White;

    Game(){
        // create the window
        window = new sf::RenderWindow(sf::VideoMode({(unsigned int)WIDTH, (unsigned int)HEIGHT}), "Snake Game");
        // create the snake
        snake = new Snake();
    }

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

        // run the program as long as the window is open
        while (window->isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window->pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed) {
                    window->close();
                } else if (event.type == sf::Event::KeyPressed) {
                    // Handle key press event
                    if (event.key.code == sf::Keyboard::Space) {
                        // Do something when the space bar is pressed
                    } else if (event.key.code == sf::Keyboard::Left) {
                        // Do something when the left arrow key is pressed
                        snake->changeOrientation(45);
                    } else if (event.key.code == sf::Keyboard::Right) {
                        // Do something when the right arrow key is pressed
                        snake->changeOrientation(-45);
                    }
                }
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
        Apple new_item = Apple(randomX, randomY);
        printf("New Item: %s\n", new_item.NAME.c_str());
        object_to_show.push_back(new_item);
    }

    ~Game() {
        delete window;
        delete snake;
    }
};

int main()
{
    srand(time(NULL));  // Init random generator
    printf("Starting rendering: c_snake window\n");

    // Run game
    Game gameInstance = Game();
    gameInstance.start();

    printf("Thank you for playing!\n");
}
