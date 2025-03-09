#pragma once
// Debug.h
#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

// Класс Debug для сбора и отображения отладочной информации
class Debug {
public:
    // Логирование сообщения с временной меткой
    static void Log(const std::string& message);

    // Обновление внутренних метрик (вызывается, например, каждый кадр)
    static void Update();

    // Вывод отформатированной информации о производительности в консоль
    static void PrintPerformanceInfo();

    // Функция для получения использования памяти (в байтах)
    static size_t GetMemoryUsage();

    // Функция для получения загрузки процессора (в процентах)
    // Реализация зависит от ОС – здесь приведён пример-заглушка.
    static float GetCPUUsage();

    // Функция для установки текущего FPS (вызывается из главного цикла)
    static void SetFPS(float fps);

private:
    // Хранение последнего значения FPS
    static float currentFPS;
    // Время последнего обновления (можно использовать для расчёта интервалов)
    static std::chrono::steady_clock::time_point lastUpdate;
};

#endif // DEBUG_H
