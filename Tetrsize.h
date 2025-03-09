#pragma once
// Tetrsize.h

#ifndef Tetrsize_H
#define Tetrsize_H

#include <array>
#include <string>
#include <SFML/Graphics.hpp>

using sf::Color;

namespace Tetromino {
    // Количество фигур (стандартно 7 тетрамино)
    const int tetrominoCount = 7;
    const int colorNum = 20;

    // Представление фигур в виде строки 16 символов (матрица 4x4)
    // Символ 'X' – заполненная ячейка, '.' – пустая.
    const std::array<std::string, tetrominoCount> shapes = {
         "....XXXX........",   // Фигура A (например, линия)
         ".X..XXX.........",   // Фигура B (например, J)
         "..X.XXX.........",   // Фигура C (например, L)
         ".XX.XX..........",    // Фигура D (например, квадрат или O, можно адаптировать)
         "..XX.XX.........",   // Фигура E (например, S)
         ".X..XX..X.......",   // Фигура F (например, T)
         "XX...XX........."    // Фигура G (например, Z)
    };

    // Палитра цветов для фигур
    const std::array<sf::Color, colorNum> colors = {
         Color(46, 199, 174),   // бирюзовый
         Color(121, 160, 193),  // сизый
         Color(255, 102, 1),    // оранжевый
         Color(255, 255, 0),    // жёлтый
         Color(0, 255, 0),  // green
         Color(213, 188, 255),    // фиолетовый
         Color(255, 50, 40),   // красный
         Color(213, 255,0),  //лаймовый
         Color(0, 0,0),
         Color(156, 156,156),
         Color(228, 228,228),

    };

    // Функция поворота фигуры.
    // Возвращает индекс для ячейки (px, py) после поворота на (r * 90) градусов.
    inline int rotate(int px, int py, int r) {
        switch (r % 4) {
        case 0: return py * 4 + px;             // 0°
        case 1: return 12 + py - (px * 4);        // 90°
        case 2: return 15 - (py * 4) - px;        // 180°
        case 3: return 3 - py + (px * 4);         // 270°
        default: return 0;
        }
    }
}

#endif // Tetrsize_H
