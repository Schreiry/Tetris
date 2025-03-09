#pragma once
// Debug.h
#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

// ����� Debug ��� ����� � ����������� ���������� ����������
class Debug {
public:
    // ����������� ��������� � ��������� ������
    static void Log(const std::string& message);

    // ���������� ���������� ������ (����������, ��������, ������ ����)
    static void Update();

    // ����� ����������������� ���������� � ������������������ � �������
    static void PrintPerformanceInfo();

    // ������� ��� ��������� ������������� ������ (� ������)
    static size_t GetMemoryUsage();

    // ������� ��� ��������� �������� ���������� (� ���������)
    // ���������� ������� �� �� � ����� ������� ������-��������.
    static float GetCPUUsage();

    // ������� ��� ��������� �������� FPS (���������� �� �������� �����)
    static void SetFPS(float fps);

private:
    // �������� ���������� �������� FPS
    static float currentFPS;
    // ����� ���������� ���������� (����� ������������ ��� ������� ����������)
    static std::chrono::steady_clock::time_point lastUpdate;
};

#endif // DEBUG_H
