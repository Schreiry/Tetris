#pragma once

//Directive
#ifndef CONSTANTS_H
#define CONSTANTS_H

// Size of one block (in pixels)
const int blockSize = 30;

// Playing field dimensions (in blocks)
const int gridWidth = 15;
const int gridHeight = 25;

// Indentation of the playing field from the window edges (in pixels)
const int fieldOffsetX = 50;
const int fieldOffsetY = 50;

// Window size: playing field + additional area for reports and previews
const int windowWidth = fieldOffsetX + gridWidth * blockSize + 200;
const int windowHeight = fieldOffsetY + gridHeight * blockSize + 50;

#endif // CONSTANTS_H
