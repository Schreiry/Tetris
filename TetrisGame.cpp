// TetrisGame.cpp
#include "TetrisGame.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

TetrisGame::TetrisGame()
    : grid(gridHeight, std::vector<int>(gridWidth, -1)),
    currentX(gridWidth / 2 - 2),
    currentY(0),
    fallTimer(0),
    fallDelay(0.5f),
    score(0),
    combo(0),
    gameOver(false)
{
    srand(static_cast<unsigned>(time(nullptr)));
    // ������������� ��������� ������
    nextPiece = rand() % Tetromino::tetrominoCount;
    nextRotation = 0;
    nextColor = Tetromino::colors[nextPiece];
    spawnNewPiece();
}

bool TetrisGame::doesPieceFit(int piece, int rotation, int posX, int posY) {
    for (int px = 0; px < 4; ++px) {
        for (int py = 0; py < 4; ++py) {
            int index = Tetromino::rotate(px, py, rotation);
            if (Tetromino::shapes[piece][index] == 'X') {
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

void TetrisGame::lockPiece() {
    score += 5; // ��������� ���� �� ���������� ������
    for (int px = 0; px < 4; ++px) {
        for (int py = 0; py < 4; ++py) {
            int index = Tetromino::rotate(px, py, currentRotation);
            if (Tetromino::shapes[currentPiece][index] == 'X') {
                int gx = currentX + px;
                int gy = currentY + py;
                if (gy >= 0 && gy < gridHeight && gx >= 0 && gx < gridWidth)
                    grid[gy][gx] = currentPiece;
                    if (fullLine) {
                        // ��������� �������� ������ ��� ��������� ������
                        for (int x = 0; x < gridWidth; ++x) {
                            sf::Vector2f pos(fieldOffsetX + x * blockSize + blockSize / 2.f,
                                fieldOffsetY + y * blockSize + blockSize / 2.f);
                            particleSystem.addParticles(pos, Tetromino::colors[grid[y][x]], 20);
                        }
                        // ��������� ����� �������� ��� ���� ������
                        LineClearAnimation anim;
                        anim.row = y;
                        anim.timer = 0.0f;
                        lineClearAnimations.push_back(anim);

                        // �������� ������ ���� � ����������� ������� ��������� �����
                        for (int ty = y; ty > 0; --ty) {
                            grid[ty] = grid[ty - 1];
                        }
                        grid[0] = std::vector<int>(gridWidth, -1);
                        linesCleared++;
                    }


            }
        }
    }
    // ��������� ����������� ������ � ������� ��
    int linesCleared = 0;
    for (int y = 0; y < gridHeight; ++y) {
        bool fullLine = true;
        for (int x = 0; x < gridWidth; ++x) {
            if (grid[y][x] == -1) { fullLine = false; break; }
        }
        if (fullLine) {
            // ��������� �������� ������ ��� ��������� ������
            for (int x = 0; x < gridWidth; ++x) {
                sf::Vector2f pos(fieldOffsetX + x * blockSize + blockSize / 2.f,
                    fieldOffsetY + y * blockSize + blockSize / 2.f);
                particleSystem.addParticles(pos, Tetromino::colors[grid[y][x]], 20);
            }
            // ������� ������ � �������� ������� ������ ����
            for (int ty = y; ty > 0; --ty) {
                grid[ty] = grid[ty - 1];
            }
            grid[0] = std::vector<int>(gridWidth, -1);
            linesCleared++;
        }
    }
    if (linesCleared > 0) {
        combo++;
        score += 125 * linesCleared;
    }
    else {
        combo = 0;
    }
    spawnNewPiece();
}

void TetrisGame::spawnNewPiece() {
    if (gameOver) return;
    // ���������� ��������� ������� ��� �������
    currentPiece = nextPiece;
    currentRotation = nextRotation;
    // �������� ��������� ���� �� ������� ������
    currentColor = Tetromino::colors[rand() % Tetromino::colors.size()];
    currentX = gridWidth / 2 - 2;
    currentY = 0;
    // ���������� ����� "next" �������
    nextPiece = rand() % Tetromino::tetrominoCount;
    nextRotation = 0;
    // ��������� ������� �������� ���� ��� ��������� ������
    nextColor = Tetromino::colors[rand() % Tetromino::colors.size()];
    // ���� ����� ������ �� ���������� � ���� ��������
    if (!doesPieceFit(currentPiece, currentRotation, currentX, currentY)) {
        gameOver = true;
    }
}


void TetrisGame::update(float dt) {
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

    // ��������� �������� ������� �����
    for (auto it = lineClearAnimations.begin(); it != lineClearAnimations.end(); ) {
        it->timer += dt;
        if (it->timer >= LineClearAnimation::duration)
            it = lineClearAnimations.erase(it);
        else
            ++it;
    }

}

void TetrisGame::drawLineClearAnimations(sf::RenderWindow& window) {
    // �������� ��� ��������� ������ ������: ����� ����� / ����� ��������
    float interval = LineClearAnimation::duration / gridWidth;
    for (const auto& anim : lineClearAnimations) {
        for (int x = 0; x < gridWidth; ++x) {
            // ���������, ������ �� ����� ��� ��������� ������� �������
            float localTime = anim.timer - x * interval;
            // ���� ��� ������ ������ ������ ����� (� ��� �� ����� �� ��������� ��������)
            if (localTime >= 0 && localTime <= interval) {
                // ����� ������� ������ ���������: ��� ������ �� �������, ��� ������ �������
                float alpha = 255.0f;
                if (localTime > interval / 2) {
                    alpha = 255 * (1 - (localTime - interval / 2) / (interval / 2));
                }
                sf::RectangleShape rect(sf::Vector2f(blockSize, blockSize));
                // ������� ������ � ������� ����
                rect.setPosition(fieldOffsetX + x * blockSize, fieldOffsetY + anim.row * blockSize);
                // ����� ���� (��������, �����) � �������������
                rect.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
                window.draw(rect);
            }
        }
    }
}



void TetrisGame::handleInput(const sf::Event& event) {
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

void TetrisGame::draw(sf::RenderWindow& window) {
    sf::RectangleShape rect(sf::Vector2f(blockSize - 1, blockSize - 1));
    // ��������� ������� �����
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            rect.setPosition(fieldOffsetX + x * blockSize, fieldOffsetY + y * blockSize);
            if (grid[y][x] == -1) {
                rect.setFillColor(sf::Color(40, 40, 40));
            }
            else {
                rect.setFillColor(Tetromino::colors[grid[y][x]]);
            }
            window.draw(rect);
        }
    }
    // ��������� ������� ������
    for (int px = 0; px < 4; ++px) {
        for (int py = 0; py < 4; ++py) {
            int index = Tetromino::rotate(px, py, currentRotation);
            if (Tetromino::shapes[currentPiece][index] == 'X') {
                int gx = currentX + px;
                int gy = currentY + py;
                sf::RectangleShape block(sf::Vector2f(blockSize - 1, blockSize - 1));
                block.setPosition(fieldOffsetX + gx * blockSize, fieldOffsetY + gy * blockSize);
                block.setFillColor(currentColor);
                window.draw(block);
            }
        }
    }
    // ��������� ������� ������
    particleSystem.draw(window);
    // ��������� ������ ��������� ������
    drawNextPiece(window);


}

void TetrisGame::drawNextPiece(sf::RenderWindow& window) {
    int previewX = fieldOffsetX + gridWidth * blockSize + 50;
    int previewY = fieldOffsetY + 50;
    // ������ ������� ������ � ������
    sf::RectangleShape previewArea(sf::Vector2f(4 * blockSize, 4 * blockSize));
    previewArea.setPosition(previewX, previewY);
    previewArea.setFillColor(sf::Color(30, 30, 30));
    previewArea.setOutlineThickness(2);
    previewArea.setOutlineColor(sf::Color::White);
    window.draw(previewArea);

    // ������ ��������� ������ � ������� ������
    sf::RectangleShape previewBlock(sf::Vector2f(blockSize - 1, blockSize - 1));
    for (int px = 0; px < 4; ++px) {
        for (int py = 0; py < 4; ++py) {
            int index = Tetromino::rotate(px, py, nextRotation);
            if (Tetromino::shapes[nextPiece][index] == 'X') {
                previewBlock.setPosition(previewX + px * blockSize, previewY + py * blockSize);
                previewBlock.setFillColor(nextColor);
                window.draw(previewBlock);
            }
        }
    }
}
