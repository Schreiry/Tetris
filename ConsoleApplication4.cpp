// Directive Space : 
#include <SFML/Graphics.hpp>
#include "TetrisGame.h"
#include "Constant.h"
#include "Debug.h"
#include <sstream>

// <#><#><#><#><#><#><#><#><#><#><#><#>



int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tetris with SFML", sf::Style::Close);
    window.setFramerateLimit(90);

    // Допустим, используем шрифт для игрового интерфейса
    //sf::Font font;
    //if (!font.loadFromFile("sansation.ttf"))
    //    return -1;

    TetrisGame game;

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(fieldOffsetX + gridWidth * blockSize + 50, fieldOffsetY + 200);

    float gameOverTimer = 0.f;
    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            game.handleInput(event);
        }

        if (!game.isGameOver())
            game.update(dt);
        else
            gameOverTimer += dt;

        // Обновляем текст счета и комбо
        std::stringstream ss;
        ss << "Score: " << game.getScore() << "\nLines Combo: " << game.getCombo();
        scoreText.setString(ss.str());

        window.clear(sf::Color(20, 20, 20));
        game.draw(window);
        window.draw(scoreText);

        // Обновление дебаг информации: считаем FPS
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            Debug::SetFPS(static_cast<float>(frameCount));
            Debug::PrintPerformanceInfo(); // вывод в консоль
            frameCount = 0;
            fpsClock.restart();
        }

        window.display();
    }
    return 0;
}
