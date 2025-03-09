#pragma once
// Tetrsize.h

#ifndef Tetrsize_H
#define Tetrsize_H

#include <array>
#include <string>
#include <SFML/Graphics.hpp>

using sf::Color;

namespace Tetromino {
    // ���������� ����� (���������� 7 ���������)
    const int tetrominoCount = 7;
    const int colorNum = 20;

    // ������������� ����� � ���� ������ 16 �������� (������� 4x4)
    // ������ 'X' � ����������� ������, '.' � ������.
    const std::array<std::string, tetrominoCount> shapes = {
         "....XXXX........",   // ������ A (��������, �����)
         ".X..XXX.........",   // ������ B (��������, J)
         "..X.XXX.........",   // ������ C (��������, L)
         ".XX.XX..........",    // ������ D (��������, ������� ��� O, ����� ������������)
         "..XX.XX.........",   // ������ E (��������, S)
         ".X..XX..X.......",   // ������ F (��������, T)
         "XX...XX........."    // ������ G (��������, Z)
    };

    // ������� ������ ��� �����
    const std::array<sf::Color, colorNum> colors = {
         Color(46, 199, 174),   // ���������
         Color(121, 160, 193),  // �����
         Color(255, 102, 1),    // ���������
         Color(255, 255, 0),    // �����
         Color(0, 255, 0),  // green
         Color(213, 188, 255),    // ����������
         Color(255, 50, 40),   // �������
         Color(213, 255,0),  //��������
         Color(0, 0,0),
         Color(156, 156,156),
         Color(228, 228,228),

    };

    // ������� �������� ������.
    // ���������� ������ ��� ������ (px, py) ����� �������� �� (r * 90) ��������.
    inline int rotate(int px, int py, int r) {
        switch (r % 4) {
        case 0: return py * 4 + px;             // 0�
        case 1: return 12 + py - (px * 4);        // 90�
        case 2: return 15 - (py * 4) - px;        // 180�
        case 3: return 3 - py + (px * 4);         // 270�
        default: return 0;
        }
    }
}

#endif // Tetrsize_H
