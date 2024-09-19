#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <span>
#include <iostream>
#include <random>
#include <cmath>
#include <string>

struct values {
    float speed = 1.0f;
    bool recreate = true;
    bool bounds = false;
    float boundsSize = 1.0f;
    glm::vec2 windVelocity = {0.0f, 0.0f};
    bool loop = false;
    float dragForce = 0.0f;
    bool bounce = false;
};

struct particleInfo {
    glm::vec2 position = glm::vec2 {0, 0};
    glm::vec4 color = glm::vec4{1, 0, 0, 1};
    glm::vec4 startColor = glm::vec4{1, 0, 0, 1};
    glm::vec4 endColor = glm::vec4{0, 1, 0, 1};
    glm::vec2 velocity = glm::vec2 {0, 0};
    glm::vec2 acceleration = glm::vec2{0, 0};
    glm::vec2 force = glm::vec2{0, 0};
    float mass = 1.0f;
    float radius = 5.0f;
    float lifetime = 200.0f;
    float totalLifetime = lifetime;
};



class Particle {

public:
    ///*particle(unsigned int seed);
    //particle(unsigned int seed, glm::vec2 inposition);*/
    Particle(particleInfo data);
    void calcLerp();
    bool isAlive(double time);
    void calcAcceleration();
    void calcVelocity(double deltaTime);
    void calcPosition(double deltaTime);
    void update(double deltaTime);

    const glm::vec2* getPosition() const { return &position; }
    const float* getRadius() const { return &radius; }
    const glm::vec4* getColor() const { return &color; }
    
    float getLifetime() { return lifetime; }
    float getMass() { return mass; }
    glm::vec2 getForce() { return force; }
    glm::vec2 getVelocity() { return velocity; }
    glm::vec2 getAcceleration() { return acceleration; }

    //glm::vec2 addWind(glm::vec2 windVelocity) { return velocity += windVelocity/mass; }
    glm::vec2 addForce(glm::vec2 newForce) { return force += newForce; }
    glm::vec2 addVelocity(glm::vec2 newVelocity) { return velocity += newVelocity; }
    void setPositionX(float x) { position.x = x; }
    void setPositionY(float y) { position.y = y; }
    void setVelocityX(float x) { velocity.x = x; }
    void setVelocityY(float y) { velocity.y = y; }
    

private:

    glm::vec2 position;
    glm::vec4 color;
    glm::vec4 startColor;
    glm::vec4 endColor;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    glm::vec2 force;
    float mass;
    float radius;
    float lifetime;
    float totalLifetime;
};

class Effect {
public:
    virtual ~Effect() = default;

    virtual void effectParticle(Particle& particle) = 0;

    virtual void adjustEffect(glm::vec2) = 0;

    virtual void adjustEffect(float) = 0;
};

class Wind : public Effect {
public:
    Wind(glm::vec2 inWind) : wind{inWind} {}
    /*Wind(glm::vec2 inWind) { 
        wind = inWind;
    }*/
    virtual void effectParticle(Particle& particle) override { 
        particle.addVelocity(wind * 0.5f); 
    }
    virtual void adjustEffect(glm::vec2 inWind) override {  // 
        wind = inWind;
    }
    virtual void adjustEffect(float) override {
        // lol
    }

private:
    glm::vec2 wind;
};

class Drag : public Effect {
public:
    Drag(float dragForce) : dragForce{dragForce} {}
    virtual void effectParticle(Particle& particle) override {
        float scaleFactor = 0.3f;
        float currentVelocity = std::sqrt(particle.getVelocity().x * particle.getVelocity().x + particle.getVelocity().y * particle.getVelocity().y);
        
        particle.addVelocity(dragForce * scaleFactor * (-1) * currentVelocity * particle.getVelocity()); 
	}
    virtual void adjustEffect(glm::vec2) override {
		// lol
	}

    virtual void adjustEffect(float inDragForce)override {
		dragForce = inDragForce;
	}

private:
	float dragForce;
};

class Emitter {
public:

    Emitter()
        : gen{[]() {
            std::random_device rd;  // Will be used to obtain a seed for the random number engine
            std::mt19937 gen;       // Standard mersenne_twister_engine
            return gen;
        }()}    
     {}
    virtual ~Emitter() = default;

    virtual Particle addParticle() = 0;

    glm::vec2 randPosition() { return {positionDist(gen), positionDist(gen)}; };
    float randSize() { return sizeDist(gen); };
    glm::vec4 randColor() { return {colorDist(gen), colorDist(gen), colorDist(gen), 0.5f}; };
    float randLifetime() { return lifetimeDist(gen); };

    //friend class ExplosiveEmitter;

protected:
    std::mt19937 gen;
    std::random_device rd;
    std::uniform_real_distribution<float> positionDist;
    std::uniform_real_distribution<float> sizeDist;
    std::uniform_real_distribution<float> colorDist;
    std::uniform_real_distribution<float> lifetimeDist;

};

class Explosion : public Emitter {
public:
    Explosion()// : Emitter()
    { 
       //positionDist = -1.0f, 1.0f} 
        positionDist = std::uniform_real_distribution<float>{-1.0f, 1.0f};
        sizeDist = std::uniform_real_distribution<float>{3.0f, 10.0f};
        colorDist = std::uniform_real_distribution<float>{0.0f, 1.0f};
        lifetimeDist = std::uniform_real_distribution<float>{2.5f, 20.5f};
    }

    virtual Particle addParticle() override {
        particleInfo testParticle;  
        testParticle.radius = 15; //randSize()
        testParticle.position = glm::vec2{0, 0};
        testParticle.velocity = randPosition();  // randPosition()
        testParticle.acceleration = randPosition();
        testParticle.lifetime = randLifetime();
        
        return Particle(testParticle);
    }
};

class Random : public Emitter {
public:
    Random() : Emitter() {
        // positionDist = -1.0f, 1.0f}
        positionDist = std::uniform_real_distribution<float>{-1.0f, 1.0f};
        sizeDist = std::uniform_real_distribution<float>{3.0f, 10.0f};
        colorDist = std::uniform_real_distribution<float>{0.0f, 1.0f};
        lifetimeDist = std::uniform_real_distribution<float>{2.5f, 20.5f};
    }

    virtual Particle addParticle() override {
        particleInfo testParticle;
        testParticle.radius = 15;  // randSize()
        testParticle.position = randPosition();
        testParticle.velocity = randPosition();  // randPosition()
        testParticle.acceleration = randPosition();
        testParticle.lifetime = randLifetime();

        return Particle(testParticle);
    }
};

class particleSystem {
public:
    particleSystem();  // constructor  size_t numParticles

    void addParticle() { 
        for (size_t i = 0; i < emitters.size(); i++) {
            particles.push_back((*emitters[i]).addParticle()); 
        }
    };

    void addParticle(std::shared_ptr<Emitter> emitter) {
        particles.push_back((*emitter).addParticle()); 
    }
    
    void removeParticle(size_t i) { 
        particles.erase(particles.begin() + i); 
    };
    void updateAll(double time, values s) {
        const double dt = (time - prevTime) * s.speed;
        prevTime = time;
        for (size_t i = 0; i < particles.size(); i++) {
            if (particles[i].isAlive(dt) == false) {
                removeParticle(i);
                if (s.recreate) {
                    addParticle(); 
                }
                continue;
            }
            
            if (s.bounds) {
                if ((particles[i].getPosition()->x > s.boundsSize) ||
                    (particles[i].getPosition()->x < (-s.boundsSize)) ||
                    (particles[i].getPosition()->y > s.boundsSize) ||
                    (particles[i].getPosition()->y < (-s.boundsSize))) {
					removeParticle(i);
					if (s.recreate) {
						addParticle();
					}
					continue;
				}
            }

            if (s.loop) {
                if (particles[i].getPosition()->x > s.boundsSize) {
                    particles[i].setPositionX(-s.boundsSize);
			    }
                else if (particles[i].getPosition()->x < (-s.boundsSize)) {
                    particles[i].setPositionX(s.boundsSize);
				}
                else if (particles[i].getPosition()->y > s.boundsSize) {
                    particles[i].setPositionY(-s.boundsSize);
				}
                else if (particles[i].getPosition()->y < (-s.boundsSize)) {
                    particles[i].setPositionX(s.boundsSize);
				}
            }

            if (s.bounce) {
                if ((particles[i].getPosition()->x >= s.boundsSize)
                   || (particles[i].getPosition()->x <= (-s.boundsSize))) {
				    particles[i].setVelocityX(-particles[i].getVelocity().x);
                }
                if ((particles[i].getPosition()->y >= s.boundsSize) ||
                    (particles[i].getPosition()->y <= (-s.boundsSize))) {
                    particles[i].setVelocityY(-particles[i].getVelocity().y);
                }
                    
            }
            
            for (size_t y = 0; y < effects.size(); y++) {
                effects[y]->effectParticle(particles[i]);
            }

            particles[i].update(dt);
        }
    };

    const glm::vec2* getPosition() {
        return particles[0].getPosition();    
    };
    const float* getSize() { 
        return particles[0].getRadius();
    };
    const glm::vec4* getColor() {
        return particles[0].getColor();
    };
	
    std::vector<Particle> getParticles() { 
        return particles;
    }

    void addEmitter(std::shared_ptr<Emitter> newEmitter) {
        emitters.push_back(newEmitter);
        std::cout << "Emitter added" << emitters.size() << std::endl;
    }

    void addEffect(std::shared_ptr<Effect> newEffect) { 
        effects.push_back(newEffect);
        std::cout << "Effect added" << effects.size() << std::endl;
    }

    std::shared_ptr<Emitter> getEmitter(std::string className) {
        for (size_t i = 0; i < emitters.size(); i++) {
            if (typeid(*emitters[i]).name() == ("class " + className)) {
                return emitters[i];
            }
        }
        return nullptr;
    }

    std::shared_ptr<Effect> getEffect(std::string className) {
        for (size_t i = 0; i < effects.size(); i++) {
            if (typeid(*effects[i]).name() == ("class " + className)) {
				return effects[i];
			}
		}
		return nullptr;
	}

    void removeEmitter(std::string name) {
        for (size_t i = 0; i < emitters.size(); i++) {
            if (typeid(*emitters[i]).name() == ("class " + name)) {
				emitters.erase(emitters.begin() + i);
				std::cout << "Emitter removed" << emitters.size() << std::endl;
			}
		}
	}

    int getEmitterSize() { return emitters.size(); 
    }

    void removeEffect(std::string name) {
        for (size_t i = 0; i < effects.size(); i++) {
            if (typeid(*effects[i]).name() == ("class " + name)) {
				effects.erase(effects.begin() + i);
				std::cout << "Effect removed" << effects.size() << std::endl;
			}
		}
	}

private:
    std::vector<std::shared_ptr<Emitter>> emitters;
    std::vector<std::shared_ptr<Effect>> effects;

    std::vector<Particle> particles;
    double prevTime;  // for delta time calculation
};
