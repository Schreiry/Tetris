#pragma once

// Constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

// ������ ������ ����� (� ��������)
const int blockSize = 30;

// ������� �������� ���� (� ������)
const int gridWidth = 15;
const int gridHeight = 25;

// ������ �������� ���� �� ���� ���� (� ��������)
const int fieldOffsetX = 50;
const int fieldOffsetY = 50;

// ������� ����: ������� ���� + �������������� ������� ��� ������� � ������
const int windowWidth = fieldOffsetX + gridWidth * blockSize + 200;
const int windowHeight = fieldOffsetY + gridHeight * blockSize + 50;

#endif // CONSTANTS_H
