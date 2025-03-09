#pragma once
// ParticleSystem.h
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>

// ��������� ��������� �������
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;   // ����� ����� (� ��������)
    sf::Color color;
};

class ParticleSystem {
public:
    ParticleSystem();

    // ���������� ������ � �������� �������, ��������� �����, count � ����� ������
    void addParticles(sf::Vector2f pos, sf::Color color, int count);

    // ���������� ��������� ������
    void update(float dt);

    // ��������� ������ � ����
    void draw(sf::RenderWindow& window);

private:
    std::vector<Particle> particles;
};

#endif // PARTICLE_SYSTEM_H
