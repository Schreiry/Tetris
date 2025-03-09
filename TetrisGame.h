#pragma once
// TetrisGame.h
#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"
#include "Tetrsize.h"
#include "Constant.h"

struct LineClearAnimation {
    int row;            // индекс очищенной строки (0..gridHeight-1)
    float timer;        // накопленное время анимации (в секундах)
    static constexpr float duration = 2.0f; // общее время анимации (2 секунды)
};



class TetrisGame {

private:
    // ...
    std::vector<LineClearAnimation> lineClearAnimations;


public:
    TetrisGame();

    // Обновление логики игры (падение фигур и анимация частиц)
    void update(float dt);

    // Обработка ввода пользователя
    void handleInput(const sf::Event& event);

    // Отрисовка игрового поля, текущей фигуры, системы частиц и превью следующей фигуры
    void draw(sf::RenderWindow& window);

    // Геттеры для проверки состояния игры
    bool isGameOver() const { return gameOver; }
    int getScore() const { return score; }
    int getCombo() const { return combo; }

private:
    // Игровая сетка: gridHeight x gridWidth, -1 если ячейка пуста, иначе индекс фигуры для цвета
    std::vector<std::vector<int>> grid;

    int currentX;
    int currentY;
    int currentPiece;
    int currentRotation;
    sf::Color currentColor;

    // Для превью следующей фигуры
    int nextPiece;
    int nextRotation;
    sf::Color nextColor;

    float fallTimer;   // таймер падения
    float fallDelay;   // задержка между падениями
    int score;
    int combo;
    bool gameOver;

    ParticleSystem particleSystem;

    // Проверка возможности размещения фигуры в заданной позиции
    bool doesPieceFit(int piece, int rotation, int posX, int posY);

    // Фиксация фигуры в сетке
    void lockPiece();

    // Спавн новой фигуры с использованием превью следующей
    void spawnNewPiece();

    // Отрисовка превью следующей фигуры
    void drawNextPiece(sf::RenderWindow& window);
};

#endif // TETRIS_GAME_H
