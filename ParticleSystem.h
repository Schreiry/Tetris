#pragma once
// ParticleSystem.h
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>

// Структура отдельной частицы
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;   // время жизни (в секундах)
    sf::Color color;
};

class ParticleSystem {
public:
    ParticleSystem();

    // Добавление частиц в заданной позиции, заданного цвета, count – число частиц
    void addParticles(sf::Vector2f pos, sf::Color color, int count);

    // Обновление состояния частиц
    void update(float dt);

    // Отрисовка частиц в окне
    void draw(sf::RenderWindow& window);

private:
    std::vector<Particle> particles;
};

#endif // PARTICLE_SYSTEM_H
