// -=-=-=-  Directive Sapce: TetrisGame -=-=-=-


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <array>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cmath>

// ----             ----
using namespace std;
using namespace sf;
// ----             ----

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//  -------------------------
// |     Global constants    |
//  -------------------------
const int blockSize = 30;         // size of one block (in pixels)
const int gridWidth = 15;         // playing field width (in blocks)
const int gridHeight = 25;         // playing field height (in blocks)
const int fieldOffsetX = 50;         // indentation of the playing field from the left edge
const int fieldOffsetY = 50;         // margin of the playing field from the top edge

// Window size: playing field + report and preview area
const int windowWidth = fieldOffsetX + gridWidth * blockSize + 200;
const int windowHeight = fieldOffsetY + gridHeight * blockSize + 50;

// ---------------------------
// Definition of Tetris figures (4x4) . 
// 'X' - filled block, '.' - empty cell . 
// Figures: I, J, L, O, S, T, Z . 
// ---------------------------
array<string, 8> tetromino =
{
    "....XXXX........", // A
    ".X..XXX.........", // B
    "..X.XXX.........", // C
    //"...." ".XX." ".XX.",//D
    ".XX.XX..........", // E
    "..XX.XX.........", // F
    ".X..XX..X.......", // G
    "XX...XX.........",  // H
};

// Vivid color palette for figures
array<Color, 8> tetroColors =
{
    Color(31, 111, 235),    // A – blue   rgb(31, 111, 235)
    Color(121, 160, 193),       // B – сизый   rgb(121, 160, 193)
    Color(255, 165, 0),     // C – оранжевый
	//Color(0, 0, 0),	        // D – черный
    Color(255, 255, 0),     // O – жёлтый
    Color(38, 166, 65),     // S – лаймовый  rgb(38, 166, 65)
    Color(148, 0, 211),     // T – фиолетовый
    Color(243, 75, 125)      // Z – красный  rgb(243, 75, 125)
};

// ---------------------------
// Shape rotation function
// Calculates index (0..15) for a block in 4x4 grid after rotation
// ---------------------------
int rotate(int px, int py, int r) {
	
    switch (r % 4) {
	    
    case 0: return py * 4 + px;           // 0 degrees
    case 1: return 12 + py - (px * 4);      // 90 degrees
    case 2: return 15 - (py * 4) - px;      // 180 degrees
    case 3: return 3 - py + (px * 4);       // 270 degrees
	    
    }

	
    return 0;
}

// ---------------------------
// Structure for particle system (comet tail effect)
// ---------------------------

struct Particle {

    Vector2f position;
    Vector2f velocity;
    float lifetime;   // lifespan in seconds
    Color color;

};

class ParticleSystem {

public:

    vector<Particle> particles;

    // Adding particles from the position pos, colored - @color, count - particle number.
    void addParticles(Vector2f pos, Color color, int count) {
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.position = pos;
            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
            float speed = (rand() % 50 + 50) / 100.f; // 0.5 to 1.0.
            p.velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);
            p.lifetime = 1.0f + (rand() % 100) / 100.f; // 1 to 2 seconds.
            p.color = color;
            particles.push_back(p);
		
        }
    }

    // Renewing the particles.
    void update(float dt) {
	    
        for (auto it = particles.begin(); it != particles.end(); ) {
		
            it->lifetime -= dt;
            if (it->lifetime <= 0)
                it = particles.erase(it);
		    
            else {
                it->velocity.y += 9.8f * dt; // gravity.
                it->position += it->velocity;
                ++it;
            }
        }
    }

    // Draw the particles.
    void draw(RenderWindow& window) {
	    
        CircleShape circle;
        circle.setRadius(2.0f);
        circle.setOrigin(2.0f, 2.0f);
	    
        for (auto& p : particles) {
            circle.setPosition(p.position);
            Color col = p.color;
            col.a = static_cast<Uint8>(255 * (p.lifetime / 2.0f));
            circle.setFillColor(col);
            window.draw(circle);
        }
    }
};

// ---------------------------
// Tetris Game Class.
// ---------------------------

class TetrisGame {

public:
    TetrisGame() : grid(gridHeight, vector<int>(gridWidth, -1)) {
	    
        srand(static_cast<unsigned>(time(nullptr)));
        // Инициализируем следующий элемент.
	    
        nextPiece = rand() % 7;
        nextRotation = 0;
        nextColor = tetroColors[nextPiece];
        spawnNewPiece();
        fallTimer = 0;
        fallDelay = 0.5f; // time between falls.
        score = 0;
        combo = 0;
        gameOver = false;
	    
    }

    // Game grid: -1 if the cell is empty; otherwise figure index (for color).
    vector<vector<int>> grid;
    int currentX = gridWidth / 2 - 2;
    int currentY = 0;
    int currentPiece = 0; // index of the current figure [0,6].
    int currentRotation = 0;
    Color currentColor;

    // To preview the following figure.
    int nextPiece;
    int nextRotation;
    Color nextColor;

    float fallTimer;   
    float fallDelay;   

    int score;
    int combo;

    bool gameOver;     // a losing flag that doesn't work yet,
		//like a lot of things that canon says should be and work ) .


    ParticleSystem particleSystem; // particle system.

    // Check if the figure fits in the specified position.
    bool doesPieceFit(int piece, int rotation, int posX, int posY) {
	    
        for (int px = 0; px < 4; ++px) {
		
            for (int py = 0; py < 4; ++py) {
		    
                int index = rotate(px, py, rotation);
                if (tetromino[piece][index] == 'X') {
			
                    int gx = posX + px;
                    int gy = posY + py;
                    if (gx < 0 || gx >= gridWidth || gy >= gridHeight)
                        return false;
                    if (gy >= 0 && grid[gy][gx] != -1)
                        return false;
                }
            }
        }
        return true;
    }

    // Fix the shape and merge it with the grid.
    void lockPiece() {
	    
        // You get 5 points for placing the figure.
        score += 5;
        for (int px = 0; px < 4; ++px) {
		
            for (int py = 0; py < 4; ++py) {
		    
                int index = rotate(px, py, currentRotation);
                if (tetromino[currentPiece][index] == 'X') {
			
                    int gx = currentX + px;
                    int gy = currentY + py;
                    if (gy >= 0 && gy < gridHeight && gx >= 0 && gx < gridWidth)
                        grid[gy][gx] = currentPiece; // save the index for the color.
                }
            }
        }
        // Checking the filled lines.
        int linesCleared = 0;
        for (int y = 0; y < gridHeight; ++y) {
		
            bool line = true;
            for (int x = 0; x < gridWidth; ++x) {
		    
                if (grid[y][x] == -1) { line = false; break; }
            }
            if (line) {
		    
                // Particle effect for the line to be cleaned.
                for (int x = 0; x < gridWidth; ++x) {
			
                    Vector2f pos(fieldOffsetX + x * blockSize + blockSize / 2.f,
                        fieldOffsetY + y * blockSize + blockSize / 2.f);
                    particleSystem.addParticles(pos, tetroColors[grid[y][x]], 20);
                }
                // We delete the line and move everything down.
                for (int ty = y; ty > 0; --ty) {
                    grid[ty] = grid[ty - 1];
                }
                grid[0] = vector<int>(gridWidth, -1);
                linesCleared++;
            }
        }
        if (linesCleared > 0) {
            combo++;
            score += 125 * linesCleared; // 125 points for each line cleared.
        }
		
        else {
            combo = 0;
        }
        spawnNewPiece();
    }

    // Spavn a new shape using preview of the next one
    void spawnNewPiece( ) {
        // If the game is already over, do not continue.
        if (gameOver) return;
        // Take the next element as the current element
        currentPiece = nextPiece;
        currentRotation = nextRotation; // обычно 0
        currentColor = nextColor;
        currentX = gridWidth / 2 - 2;
        currentY = 0;
        // Generate a new ‘next’ element
        nextPiece = rand() % 7;
        nextRotation = 0;
        nextColor = tetroColors[nextPiece];
	    
        // If a new piece does not fit - game over
        if (!doesPieceFit(currentPiece, currentRotation, currentX, currentY)) {
            gameOver = true;
        }
    }

    // Updating the game logic (falling pieces and particle system)
    void update(float dt) {
	    
        if (!gameOver) {
            fallTimer += dt;
		
            if (fallTimer >= fallDelay) {
                if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) {
                    currentY++;
                }
			
                else {
                    lockPiece();
                }
                fallTimer = 0;
            }
        }
        particleSystem.update(dt);
    }

    // Input processing: motion, rotation, accelerated drop
    void handleInput(const Event& event) {
	    
        if (gameOver) return; // If the game is over, do not process the input
	    
        if (event.type == Event::KeyPressed) {
		
            if (event.key.code == Keyboard::Left) {
		    
                if (doesPieceFit(currentPiece, currentRotation, currentX - 1, currentY))
                    currentX--;
            }
            else if (event.key.code == Keyboard::Right) {
		    
                if (doesPieceFit(currentPiece, currentRotation, currentX + 1, currentY))
                    currentX++;
            }
            else if (event.key.code == Keyboard::Up) {
                int newRotation = (currentRotation + 1) % 4;
                if (doesPieceFit(currentPiece, newRotation, currentX, currentY))
                    currentRotation = newRotation;
            }
            else if (event.key.code == Keyboard::Down) {
                if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1))
                    currentY++;
            }
        }
    }

    // Drawing of the playing field, current figure and particle system
    void draw(RenderWindow& window) {
	    
        RectangleShape rect(Vector2f(blockSize - 1, blockSize - 1));
        // Drawing the grid
        for (int y = 0; y < gridHeight; ++y) {
		
            for (int x = 0; x < gridWidth; ++x) {
		    
                rect.setPosition(fieldOffsetX + x * blockSize, fieldOffsetY + y * blockSize);
                if (grid[y][x] == -1) {
                    rect.setFillColor(Color(40, 40, 40));
                }
                else {
                    rect.setFillColor(tetroColors[grid[y][x]]);
                }
                window.draw(rect);
            }
        }
        // Draw the current figure
        for (int px = 0; px < 4; ++px) {
		
            for (int py = 0; py < 4; ++py) {
		    
                int index = rotate(px, py, currentRotation);
		    
                if (tetromino[currentPiece][index] == 'X') {
                    int gx = currentX + px;
                    int gy = currentY + py;
                    RectangleShape block(Vector2f(blockSize - 1, blockSize - 1));
                    block.setPosition(fieldOffsetX + gx * blockSize, fieldOffsetY + gy * blockSize);
                    block.setFillColor(currentColor);
                    window.draw(block);
                }
            }
        }
        // Drawing of the particle system
        particleSystem.draw(window);
        // Draw a preview of the next shape
        drawNextPiece(window);
    }

    // Draw a preview of the next figure in a special window on the right side of the screen
    void drawNextPiece(RenderWindow& window) {
	    
        int previewX = fieldOffsetX + gridWidth * blockSize + 50;
        int previewY = fieldOffsetY + 50;
        // Рисуем область превью с рамкой
        RectangleShape previewArea(Vector2f(4 * blockSize, 4 * blockSize));
        previewArea.setPosition(previewX, previewY);
        previewArea.setFillColor(Color(30, 30, 30));
        previewArea.setOutlineThickness(2);
        previewArea.setOutlineColor(Color::White);
        window.draw(previewArea);
        // Рисуем следующую фигуру внутри области
        RectangleShape previewBlock(Vector2f(blockSize - 1, blockSize - 1));
        for (int px = 0; px < 4; ++px) {
		
            for (int py = 0; py < 4; ++py) {
		    
                int index = rotate(px, py, nextRotation);
                if (tetromino[nextPiece][index] == 'X') {
                    previewBlock.setPosition(previewX + px * blockSize, previewY + py * blockSize);
                    previewBlock.setFillColor(nextColor);
                    window.draw(previewBlock);
                }
            }
        }
    }
};

// ---------------------------
// Main function
// ---------------------------
int main() {
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Tetris with SFML", Style::Close);
    window.setFramerateLimit(60);

    TetrisGame game;

    Font font;
    if (!font.loadFromFile("sansation.ttf")) {
    }
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(fieldOffsetX + gridWidth * blockSize + 50, fieldOffsetY + 200);

    float gameOverTimer = 0; 

    Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        Event event;
        while (window.pollEvent(event)) {
		
            if (event.type == Event::Closed)
                window.close();
            game.handleInput(event);
        }

        if (!game.gameOver)
            game.update(dt);
		
        else
            gameOverTimer += dt;

        // Update the text of the bill and combo - doesn't work 
        stringstream ss;
        ss << "Score: " << game.score << "\nLines Combo: " << game.combo;
        scoreText.setString(ss.str());

        window.clear(Color(20, 20, 20));
        game.draw(window);
        window.draw(scoreText);

        // If the game is over, display an animated Game Over message - does not work 
	    
        if (game.gameOver && font.getInfo().family != "") {
		
            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(60);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setStyle(Text::Bold);
            // Text centring - doesn't work
            FloatRect textRect = gameOverText.getLocalBounds();
            gameOverText.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            gameOverText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);
            // Animation: ripple - not working
            float scale = 1.0f + 0.2f * sin(5 * gameOverTimer);
            gameOverText.setScale(scale, scale);
            window.draw(gameOverText);
        }

        window.display();
    }

	
    return 0;
}
