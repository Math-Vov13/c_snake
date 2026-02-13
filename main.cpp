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
#include <optional>

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
    public:
    sf::Color COLOR;
    string DESCRIPTION;
    virtual string getName() const = 0;
    Item(int x, int y) : Props(x, y) {
        shape.setSize({15, 15});
    }
};

class Apple : public Item{
    public:
    const sf::Color COLOR = sf::Color::Red;
    const string DESCRIPTION = "Gives you +1 level";

    virtual string getName() const override { return "Apple"; }

    Apple(int x, int y) : Item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class Boost : public Item{
    public:
    const sf::Color COLOR = sf::Color::Blue;
    const string DESCRIPTION = "Gives you x1.5 movement speed";

    virtual string getName() const override { return "Speed Boost"; }

    Boost(int x, int y) : Item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class RottenApple : public Item{
    public:
    const sf::Color COLOR = sf::Color::Yellow;
    const string DESCRIPTION = "Kills you";

    virtual string getName() const override { return "Rotten Apple"; }

    RottenApple(int x, int y) : Item(x, y) {
        shape.setFillColor(COLOR);
    }
};

class SnakeTail : public Props{
    // snake queue block
    public:
    const sf::Color COLOR = sf::Color::Green;

    SnakeTail(int x, int y) : Props(x, y) {
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
        auto newSnakeHead = SnakeTail(newPos[0], newPos[1]);
        items_tail.push_back(newSnakeHead); // Add new head
    }

    bool isHeadTouchingTail(){
        return false;
    }

    optional<Item*> getEatingItem(){
        float maxDist = 8.0f;

        for (auto i = 0; i < (int) items_on_board->size(); i++) {
            float dx = (*items_on_board)[i]->x - headPos[0];
            float dy = (*items_on_board)[i]->y - headPos[1];
            float dist = sqrt(dx*dx + dy*dy);

            if (dist <= maxDist) {
                Item* item = (*items_on_board)[i];
                items_on_board->erase(items_on_board->begin() + i);
                if (item->getName() == "Apple")
                    growth(1);
                if (item->getName() == "Speed Boost")
                    growth(5);
                if (item->getName() == "Rotten Apple")
                    alive = false;
                return item;
            }
        }

        return nullopt;
    }

    public:
    bool alive = true;
    int blocSize = 15;
    int orientation = 0;
    array<float, 2> headPos;
    deque<SnakeTail> items_tail;
    vector<Item*>* items_on_board;

    void move(){
        auto newPos = calculNextPos();
        addNewStage(newPos); // Add new head
        getEatingItem(); // Eat item in front of the snake head
        items_tail.pop_front(); // Remove back
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
    sf::Text textFrames;
    sf::Text textScore;

    int frame = 0;
    Snake* snake;
    vector<Item*> object_to_show;

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

        snake->items_on_board = &object_to_show;
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
            textFrames = sf::Text("Frames: 0", font); // Initialize with default text
            textFrames.setCharacterSize(24);
            textFrames.setFillColor(sf::Color::Red);
            textFrames.setPosition(10, 10);

            textScore = sf::Text("Score: 0", font); // Initialize with default text
            textScore.setCharacterSize(24);
            textScore.setFillColor(sf::Color::Red);
            textScore.setPosition(400, 10);
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
                    return;
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

            if (snake->alive == false) {
                window->close();
                printf("Sorry your snake died! With a score of %d\n\n", (int)snake->items_tail.size());
                return;
            }

            if (frame % 1500 == 0) {
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
            window->draw((*object_to_show[i]).shape);
        }
        // 2. snake
        for (auto i = 0; i < (int)snake->items_tail.size(); i++) {
            window->draw(snake->items_tail.at(i).shape);
        }

        // second layer: grid
        window->draw(&lines[0], lines.size(), sf::Lines);
        // third layer: texts & scores
        textFrames.setString("Frames: " + std::to_string(++frame));
        textScore.setString("Score: " + std::to_string(snake->items_tail.size() - 5));
        window->draw(textFrames);
        window->draw(textScore);

        // end the current frame
        window->display();
    }

    void generate_random_item(){
        int randomX = rand() % (WIDTH - (BORDER_SIZE * 2) - 10) + BORDER_SIZE;
        int randomY = rand() % (HEIGHT - (BORDER_SIZE * 2) - 10) + BORDER_SIZE;

        int roll = rand() % 100;
        Item* item;

        if (roll < 70) {
            item = new Apple(randomX, randomY);
        } else if (roll < 90) {
            item = new Boost(randomX, randomY);
        } else {
            item = new RottenApple(randomX, randomY);
        }

        object_to_show.push_back(item);
        printf("New Item: %s\n", item->getName().c_str());
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
