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
#include <iostream>  // For std::cerr

// ----             ----
using std::vector;
using std::array;
using std::string;
using sf::Color;
using sf::RenderWindow;
using sf::Vector2f;
// ----             ----

//  -------------------------
// |     Global constants    |
//  -------------------------
const int blockSize = 30;         // size of one block (in pixels) ;
const int gridWidth = 15;         // width of the playing field (in blocks) ;
const int gridHeight = 25;        // height of the playing field (in blocks) ;
const int fieldOffsetX = 50;      // indent of the playing field from the left edge ; 
const int fieldOffsetY = 50;      // indent of the playing field from the top edge ;

// Window size: playing field + report and preview area .
const int windowWidth = fieldOffsetX + gridWidth * blockSize + 200;
const int windowHeight = fieldOffsetY + gridHeight * blockSize + 50;

// ---------------------------
//Definition of Tetris pieces (4x4) ; 
// 'X' - filled block, '.' - empty cell ; 
// Figures: [ I ] , [ J ] , [ L ] , [ O ] , [ S ] , [ T ], [ Z ] . 
// ---------------------------
array<std::string, 8> tetromino =
{
    "....XXXX........", // A ; 
    ".X..XXX.........", // B ; 
    "..X.XXX.........", // C ; 
    //"...." ".XX." ".XX.", // D 
    ".XX.XX..........", // E ; 
    "..XX.XX.........", // F ; 
    ".X..XX..X.......", // G ; 
    "XX...XX.........",  // H .
};

// Палитра ярких цветов для фигур
array<Color, 8> tetroColors =
{
    Color(31, 111, 235),    // A – Blue (rgb(31, 111, 235)) ; 
    Color(121, 160, 193),   // B – сизый(dove-colored) (rgb(121, 160, 193)) ; 
    Color(255, 165, 0),     // C – Orange ; 
    //Color(0, 0, 0),       // D – Black ; 
    Color(255, 255, 0),     // O – Yellow ; 
    Color(38, 166, 65),     // S – Lime (rgb(38, 166, 65)) ; 
    Color(148, 0, 211),     // T – Violet ; 
    Color(243, 75, 125)     // Z – Red (rgb(243, 75, 125)) .
};

// ---------------------------
// Rotate figure function .
// Calculates the index (0..15) for a block in a 4x4 grid after rotation .
// ---------------------------
int rotate(int px, int py, int r) {
    switch (r % 4) {
    case 0: return py * 4 + px;           // 0° degrees
    case 1: return 12 + py - (px * 4);      // 90° degrees
    case 2: return 15 - (py * 4) - px;      // 180° degrees
    case 3: return 3 - py + (px * 4);       // 270° degrees
    }
    return 0;
}

// ---------------------------
// Structure for a particle system (comet tail effect)
// ---------------------------
struct Particle {
    Vector2f position;
    Vector2f velocity;
    float lifetime;   // lifetime (in seconds)
    Color color;
};

class ParticleSystem {
public:
    vector<Particle> particles;

// Add particles at position pos, given color, count - number of particles
    void addParticles(Vector2f pos, Color color, int count) {
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.position = pos;
            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
            float speed = (rand() % 50 + 50) / 100.f; // от 0.5 до 1.0
            p.velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);
            p.lifetime = 1.0f + (rand() % 100) / 100.f; // от 1 до 2 секунд
            p.color = color;
            particles.push_back(p);
        }
    }

    // Обновление частиц
    void update(float dt) {
        for (auto it = particles.begin(); it != particles.end(); ) {
            it->lifetime -= dt;
            if (it->lifetime <= 0)
                it = particles.erase(it);
            else {
                it->velocity.y += 9.8f * dt; // влияние гравитации
                it->position += it->velocity;
                ++it;
            }
        }
    }

    // Отрисовка частиц
    void draw(RenderWindow& window) {
        sf::CircleShape circle;
        circle.setRadius(2.0f);
        circle.setOrigin(2.0f, 2.0f);
        for (auto& p : particles) {
            circle.setPosition(p.position);
            Color col = p.color;
            col.a = static_cast<sf::Uint8>(255 * (p.lifetime / 2.0f));
            circle.setFillColor(col);
            window.draw(circle);
        }
    }
};

// ---------------------------
// Класс игры Tetris
// ---------------------------
class TetrisGame {
public:
    TetrisGame() : grid(gridHeight, vector<int>(gridWidth, -1)) {
        srand(static_cast<unsigned>(time(nullptr)));
        // Инициализируем следующий элемент
        nextPiece = rand() % 7;
        nextRotation = 0;
        nextColor = tetroColors[nextPiece];
        spawnNewPiece();
        fallTimer = 0;
        fallDelay = 0.5f; // время между падениями
        score = 0;
        combo = 0;
        gameOver = false;
    }

    // Игровая сетка: -1, если клетка пуста; иначе хранится индекс фигуры (для цвета)
    vector<vector<int>> grid;
    int currentX = gridWidth / 2 - 2;
    int currentY = 0;
    int currentPiece = 0; // индекс текущей фигуры [0,6]
    int currentRotation = 0;
    Color currentColor;

    // Для превью следующей фигуры
    int nextPiece;
    int nextRotation;
    Color nextColor;

    float fallTimer;   // таймер падения
    float fallDelay;   // задержка между падениями

    int score;
    int combo;

    bool gameOver;     // флаг окончания игры

    ParticleSystem particleSystem; // система частиц

    // Проверка, помещается ли фигура в указанной позиции
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

    // Фиксируем фигуру и объединяем её с сеткой
    void lockPiece() {
        // За размещение фигуры начисляем 5 очков
        score += 5;
        for (int px = 0; px < 4; ++px) {
            for (int py = 0; py < 4; ++py) {
                int index = rotate(px, py, currentRotation);
                if (tetromino[currentPiece][index] == 'X') {
                    int gx = currentX + px;
                    int gy = currentY + py;
                    if (gy >= 0 && gy < gridHeight && gx >= 0 && gx < gridWidth)
                        grid[gy][gx] = currentPiece; // сохраняем индекс для цвета
                }
            }
        }
        // Проверяем заполненные строки
        int linesCleared = 0;
        for (int y = 0; y < gridHeight; ++y) {
            bool line = true;
            for (int x = 0; x < gridWidth; ++x) {
                if (grid[y][x] == -1) { line = false; break; }
            }
            if (line) {
                // Эффект частиц для очищаемой строки
                for (int x = 0; x < gridWidth; ++x) {
                    Vector2f pos(fieldOffsetX + x * blockSize + blockSize / 2.f,
                        fieldOffsetY + y * blockSize + blockSize / 2.f);
                    particleSystem.addParticles(pos, tetroColors[grid[y][x]], 20);
                }
                // Удаляем строку и сдвигаем всё вниз
                for (int ty = y; ty > 0; --ty) {
                    grid[ty] = grid[ty - 1];
                }
                grid[0] = vector<int>(gridWidth, -1);
                linesCleared++;
            }
        }
        if (linesCleared > 0) {
            combo++;
            score += 125 * linesCleared; // 125 очков за каждую очищенную строку
        }
        else {
            combo = 0;
        }
        spawnNewPiece();
    }

    // Спавн новой фигуры с использованием превью следующей
    void spawnNewPiece() {
        if (gameOver) return;
        // Берём следующий элемент как текущий
        currentPiece = nextPiece;
        currentRotation = nextRotation;
        currentColor = nextColor;
        currentX = gridWidth / 2 - 2;
        currentY = 0;
        // Генерируем новый "next" элемент
        nextPiece = rand() % 7;
        nextRotation = 0;
        nextColor = tetroColors[nextPiece];
        // Если новая фигура не помещается – игра окончена
        if (!doesPieceFit(currentPiece, currentRotation, currentX, currentY)) {
            gameOver = true;
        }
    }

    // Обновление логики игры (падение фигур и система частиц)
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

    // Обработка ввода: движение, поворот, ускоренное падение
    void handleInput(const sf::Event& event) {
        if (gameOver) return;
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                if (doesPieceFit(currentPiece, currentRotation, currentX - 1, currentY))
                    currentX--;
            }
            else if (event.key.code == sf::Keyboard::Right) {
                if (doesPieceFit(currentPiece, currentRotation, currentX + 1, currentY))
                    currentX++;
            }
            else if (event.key.code == sf::Keyboard::Up) {
                int newRotation = (currentRotation + 1) % 4;
                if (doesPieceFit(currentPiece, newRotation, currentX, currentY))
                    currentRotation = newRotation;
            }
            else if (event.key.code == sf::Keyboard::Down) {
                if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1))
                    currentY++;
            }
        }
    }

    // Отрисовка игрового поля, текущей фигуры и системы частиц
    void draw(RenderWindow& window) {
        sf::RectangleShape rect(Vector2f(blockSize - 1, blockSize - 1));
        // Отрисовка сетки
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
        // Отрисовка текущей фигуры
        for (int px = 0; px < 4; ++px) {
            for (int py = 0; py < 4; ++py) {
                int index = rotate(px, py, currentRotation);
                if (tetromino[currentPiece][index] == 'X') {
                    int gx = currentX + px;
                    int gy = currentY + py;
                    sf::RectangleShape block(Vector2f(blockSize - 1, blockSize - 1));
                    block.setPosition(fieldOffsetX + gx * blockSize, fieldOffsetY + gy * blockSize);
                    block.setFillColor(currentColor);
                    window.draw(block);
                }
            }
        }
        // Отрисовка системы частиц
        particleSystem.draw(window);
        // Отрисовка превью следующей фигуры
        drawNextPiece(window);
    }

    // Отрисовка превью следующей фигуры в области справа
    void drawNextPiece(RenderWindow& window) {
        int previewX = fieldOffsetX + gridWidth * blockSize + 50;
        int previewY = fieldOffsetY + 50;
        // Рисуем область превью с рамкой
        sf::RectangleShape previewArea(Vector2f(4 * blockSize, 4 * blockSize));
        previewArea.setPosition(previewX, previewY);
        previewArea.setFillColor(Color(30, 30, 30));
        previewArea.setOutlineThickness(2);
        previewArea.setOutlineColor(Color::White);
        window.draw(previewArea);
        // Рисуем следующую фигуру внутри области
        sf::RectangleShape previewBlock(Vector2f(blockSize - 1, blockSize - 1));
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
        // Здесь можно добавить надпись "Next", если требуется
    }
};

// ---------------------------
// Главная функция
// ---------------------------
int main() {
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tetris with SFML", sf::Style::Close);
    window.setFramerateLimit(60);

    TetrisGame game;

    // Загрузка шрифта
    sf::Font font;
    // Пытаемся загрузить шрифт из файла "sansation.ttf"
    if (!font.loadFromFile("sansation.ttf")) {
        // Если не удалось, используем запасной путь в зависимости от платформы
#ifdef _WIN32
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Ошибка: не удалось загрузить шрифт sansation.ttf или Arial!" << std::endl;
        }
#elif defined(__linux__)
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            std::cerr << "Ошибка: не удалось загрузить шрифт sansation.ttf или DejaVuSans!" << std::endl;
        }
#else
        std::cerr << "Ошибка: не удалось загрузить шрифт sansation.ttf!" << std::endl;
#endif
    }

    // Настройка текста для отображения счета и комбо
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(fieldOffsetX + gridWidth * blockSize + 50, fieldOffsetY + 200);

    float gameOverTimer = 0; // для анимации надписи "GAME OVER"

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            game.handleInput(event);
        }

        if (!game.gameOver)
            game.update(dt);
        else
            gameOverTimer += dt;

        // Обновляем текст счета и комбо
        std::stringstream ss;
        ss << "Score: " << game.score << "\nLines Combo: " << game.combo;
        scoreText.setString(ss.str());

        window.clear(Color(20, 20, 20));
        game.draw(window);
        window.draw(scoreText);

        // Если игра окончена, отображаем анимированное сообщение "GAME OVER"
        if (game.gameOver && font.getInfo().family != "") {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(60);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setStyle(sf::Text::Bold);
            // Центрируем текст
            sf::FloatRect textRect = gameOverText.getLocalBounds();
            gameOverText.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            gameOverText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);
            // Анимация пульсации
            float scale = 1.0f + 0.2f * sin(5 * gameOverTimer);
            gameOverText.setScale(scale, scale);
            window.draw(gameOverText);
        }

        window.display();
    }
    return 0;
}
