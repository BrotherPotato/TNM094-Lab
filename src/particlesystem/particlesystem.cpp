#include <particlesystem/particlesystem.h>
#include <iostream>
//#include <chrono>

bool Particle::isAlive(double deltaTime) {
    lifetime -= deltaTime;
    if (0 > lifetime) {
        return false;
    } else {
        return true;
    }
}
void Particle::calcAcceleration() { acceleration = force / mass; }
void Particle::calcVelocity(double deltaTime) {
    velocity += glm::vec2((deltaTime * acceleration.x), (deltaTime * acceleration.y));
}
void Particle::calcPosition(double deltaTime){
    position += glm::vec2((deltaTime * velocity.x), (deltaTime * velocity.y));
};
void Particle::update(double deltaTime) {
    calcAcceleration();
    calcVelocity(deltaTime);
    calcPosition(deltaTime);
    calcLerp();
    //std::lerp(this->color.a, this->color.b, deltaTime);
};

Particle::Particle(particleInfo data)
    : position{data.position}
    , color{data.color}
    , startColor{data.startColor}
    , endColor{data.endColor}
    , velocity{data.velocity}
    , acceleration{data.acceleration}
    , force{data.force}
    , mass{data.mass}
    , radius{data.radius}
    , lifetime{data.lifetime}
    , totalLifetime{lifetime} {};
	
void Particle::calcLerp() { 
    float lerp = lifetime / totalLifetime;
    // lerp 1 -> 0
    color.r = endColor.r + lerp * (startColor.r - endColor.r);
    color.g = endColor.g + lerp * (startColor.g - endColor.g);
    color.b = endColor.b + lerp * (startColor.b - endColor.b);
    // endcolor + 1 (color - endcolor) = color
    // endcolor + 0 (color - endcolor) = endcolor
}
particleSystem::particleSystem() //size_t numParticles
    : prevTime{0.0} {
}