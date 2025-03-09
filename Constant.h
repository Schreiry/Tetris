#pragma once

// Constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

// Размер одного блока (в пикселях)
const int blockSize = 30;

// Размеры игрового поля (в блоках)
const int gridWidth = 15;
const int gridHeight = 25;

// Отступ игрового поля от краёв окна (в пикселях)
const int fieldOffsetX = 50;
const int fieldOffsetY = 50;

// Размеры окна: игровое поле + дополнительная область для отчётов и превью
const int windowWidth = fieldOffsetX + gridWidth * blockSize + 200;
const int windowHeight = fieldOffsetY + gridHeight * blockSize + 50;

#endif // CONSTANTS_H
