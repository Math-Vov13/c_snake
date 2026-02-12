#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <cmath>
#include <cstdio>
#include <deque>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

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
    string NAME;
    string DESCRIPTION;
    Item(int x, int y) : Props(x, y) {
    }
};

class Apple : public Item{
    // Apple : eat to grow
    public:
    const sf::Color COLOR = sf::Color::Red;
    const string NAME = "Apple";
    const string DESCRIPTION = "Gives you +1 level";

    Apple(int x, int y) : Item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class Boost : Item{
    // boost : eat to move faster
    public:
    const sf::Color COLOR = sf::Color::Blue;
    const string NAME = "Speed Boost";
    const string DESCRIPTION = "Gives you x1.5 movement speed";

    Boost(int x, int y) : Item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class SnakeQueue : public Props{
    // snake queue block
    public:
    const sf::Color COLOR = sf::Color::Green;

    SnakeQueue(int x, int y) : Props(x, y) {
        shape.setFillColor(COLOR);
        shape.setSize({15, 15});
    }
};

class Snake{
    // snake playable
    private:
    array<float, 2> calculNextPos(){
        // Calculate next position using trigonometric circle
        // Convert orientation to radians (degrees to radians)
        float radians = orientation * M_PI / 180.0f;

        auto nextPos = headPos;
        // Calculate new position based on orientation
        nextPos[0] += blocSize * sin(radians);  // X coordinate
        nextPos[1] -= blocSize * cos(radians);  // Y coordinate (negative because Y axis is inverted)

        // (HARD CODED) grid limits => tp snake
        if (nextPos[0] <= (20 + 20) )
            nextPos[0] = 800 - 50;
        if (nextPos[0] >= (800 - 45) )
            nextPos[0] = 20 + 20;
        if (nextPos[1] <= (20 + 20) )
            nextPos[1] = 600 - 50;
        if (nextPos[1] >= (600 - 45) )
            nextPos[1] = 20 + 20;

        return nextPos;
    }

    void addNewStage(array<float, 2> newPos) {
        headPos = newPos;
        auto newSnakeHead = SnakeQueue(newPos[0], newPos[1]);
        items_queue.push_back(newSnakeHead); // Add new head
    }

    public:
    int blocSize = 15;
    int orientation = 0;
    array<float, 2> headPos;
    deque<SnakeQueue> items_queue;

    void move(){
        auto newPos = calculNextPos();
        addNewStage(newPos); // Add new head
        items_queue.pop_front(); // Remove back
    }

    void growth(int addSize = 1) {
        for (int i=0; i < addSize; i++) {
            auto newPos = calculNextPos();
            addNewStage(newPos); // Add new head
        }
    }

    void changeOrientation(int degree){
        // Orientation:
        // 0° / 360° => up
        // 90° => left
        // 270° => right
        // 180° => down
        orientation += degree;
    }

    Snake(array<float, 2> startPos){
        headPos = startPos;
        growth(5);
    }
};

class Game{
    // game interface (main class)
    private:
    sf::RenderWindow* window;
    vector<sf::Vertex> lines;
    sf::Font font;
    sf::Text text;

    int frame = 0;
    Snake* snake;
    vector<Props> object_to_show;

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
        snake = new Snake({(float) WIDTH / 2 - BORDER_SIZE, (float) HEIGHT / 2 - BORDER_SIZE + 120});
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
                        snake->changeOrientation(-90);
                    } else if (event.key.code == sf::Keyboard::Right) {
                        // Do something when the right arrow key is pressed
                        snake->changeOrientation(90);
                    }
                }
            }

            if (frame % 2500 == 0) {
                snake->move();
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
        // 1. usable
        for (auto i = 0; i < object_to_show.end() - object_to_show.begin(); i++) {
            window->draw(object_to_show[i].shape);
        }
        // 2. snake
        for (auto i = 0; i < (int)snake->items_queue.size(); i++) {
            window->draw(snake->items_queue.at(i).shape);
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
    srand(time(nullptr));  // Init random generator
    printf("Starting rendering: c_snake window\n");

    // Run game
    auto gameInstance = Game();
    gameInstance.start();

    printf("Thank you for playing!\n");
}
