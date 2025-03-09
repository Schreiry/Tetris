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
    int row;            // ������ ��������� ������ (0..gridHeight-1)
    float timer;        // ����������� ����� �������� (� ��������)
    static constexpr float duration = 2.0f; // ����� ����� �������� (2 �������)
};



class TetrisGame {

private:
    // ...
    std::vector<LineClearAnimation> lineClearAnimations;


public:
    TetrisGame();

    // ���������� ������ ���� (������� ����� � �������� ������)
    void update(float dt);

    // ��������� ����� ������������
    void handleInput(const sf::Event& event);

    // ��������� �������� ����, ������� ������, ������� ������ � ������ ��������� ������
    void draw(sf::RenderWindow& window);

    // ������� ��� �������� ��������� ����
    bool isGameOver() const { return gameOver; }
    int getScore() const { return score; }
    int getCombo() const { return combo; }

private:
    // ������� �����: gridHeight x gridWidth, -1 ���� ������ �����, ����� ������ ������ ��� �����
    std::vector<std::vector<int>> grid;

    int currentX;
    int currentY;
    int currentPiece;
    int currentRotation;
    sf::Color currentColor;

    // ��� ������ ��������� ������
    int nextPiece;
    int nextRotation;
    sf::Color nextColor;

    float fallTimer;   // ������ �������
    float fallDelay;   // �������� ����� ���������
    int score;
    int combo;
    bool gameOver;

    ParticleSystem particleSystem;

    // �������� ����������� ���������� ������ � �������� �������
    bool doesPieceFit(int piece, int rotation, int posX, int posY);

    // �������� ������ � �����
    void lockPiece();

    // ����� ����� ������ � �������������� ������ ���������
    void spawnNewPiece();

    // ��������� ������ ��������� ������
    void drawNextPiece(sf::RenderWindow& window);
};

#endif // TETRIS_GAME_H
