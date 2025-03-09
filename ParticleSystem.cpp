// ParticleSystem.cpp
#include "ParticleSystem.h"
#include <cstdlib>
#include <cmath>

ParticleSystem::ParticleSystem() {
    // ƒополнительна€ инициализаци€, если потребуетс€
}

void ParticleSystem::addParticles(sf::Vector2f pos, sf::Color color, int count) {
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.position = pos;
        float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float speed = (rand() % 50 + 50) / 100.f; // скорость от 0.5 до 1.0
        p.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        p.lifetime = 1.0f + (rand() % 100) / 100.f; // врем€ жизни от 1 до 2 секунд
        p.color = color;
        particles.push_back(p);
    }
}

void ParticleSystem::update(float dt) {
    for (auto it = particles.begin(); it != particles.end(); ) {
        it->lifetime -= dt;
        if (it->lifetime <= 0)
            it = particles.erase(it);
        else {
            it->velocity.y += 9.8f * dt; // эффект гравитации
            it->position += it->velocity;
            ++it;
        }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    sf::CircleShape circle;
    circle.setRadius(2.0f);
    circle.setOrigin(2.0f, 2.0f);
    for (auto& p : particles) {
        circle.setPosition(p.position);
        sf::Color col = p.color;
        col.a = static_cast<sf::Uint8>(255 * (p.lifetime / 2.0f)); // плавное исчезновение
        circle.setFillColor(col);
        window.draw(circle);
    }
}
