#include <catch2/catch_all.hpp>
#include <glm/glm.hpp>

#include <particlesystem/particlesystem.h>

#include <rendering/window.h>
#include <example/randomsystem.h>
#include <particlesystem/particlesystem.h>

#include <fmt/format.h>

#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

/* Unit tests using the catch2 framework
 * Homepage: https://github.com/catchorg/Catch2
 * Tutorial: https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
 * Docs: https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md
 */

TEST_CASE("Empty system", "[particleSystem]") {

    particleSystem system{};

    REQUIRE(system.getParticles().size() == 0);
}

// Tests using the GIVEN/WHEN/THEN pattern
/*
SCENARIO("New Emitter", "[particleSystem]") {
    GIVEN("A system with with new emitter") {
        particleSystem system{};

        std::shared_ptr<Emitter> explosion = std::make_shared<Explosion>();
        system.addEmitter(explosion);

        THEN("The initial size of emitters should be 1") {
            REQUIRE(system.getEmitterSize() == 1);
            REQUIRE(system.getEmitter("Explosion"));
        }

        WHEN("Add a particle with the new emitter") {
            system.addParticle(system.getEmitter("Explosion"));

            THEN("The initial size should be 1") { REQUIRE(system.getParticles().size() == 1); }
        }

        WHEN("Add a particle with the new emitter with the alternative method") {
            system.addParticle();

            THEN("The initial size should be 1") { REQUIRE(system.getParticles().size() == 1); }
        }

        // all when are isolated
        WHEN("Add 100 more particles with the given emitter") {
            for (int i = 0; i < 100; i++) {
                system.addParticle(system.getEmitter("Explosion"));
            }

            THEN("The new size should be 100") { REQUIRE(system.getParticles().size() == 100); }
        }
    }
}
*/

class Shooter : public Emitter {
public:
    Shooter() {
        // positionDist = -1.0f, 1.0f}
        positionDist = std::uniform_real_distribution<float>{-1.0f, 1.0f};
        sizeDist = std::uniform_real_distribution<float>{3.0f, 10.0f};
        colorDist = std::uniform_real_distribution<float>{0.0f, 1.0f};
        lifetimeDist = std::uniform_real_distribution<float>{2.5f, 20.5f};
    }

    virtual Particle addParticle() override {
        particleInfo testParticle;
        testParticle.radius = 15;  // randSize()
        testParticle.position = glm::vec2{0, 0};
        testParticle.velocity = glm::vec2{0, -1};  // randPosition()
        testParticle.acceleration = randPosition();
        testParticle.lifetime = randLifetime();

        return Particle(testParticle);
    }
};

SCENARIO("Newest Emitter", "[particleSystem]") {
    GIVEN("A system with with new emitter") {
        particleSystem system{};

        

        std::shared_ptr<Emitter> shooter = std::make_shared<Shooter>();
        system.addEmitter(shooter);

        THEN("The initial size of emitters should be 1") {
            REQUIRE(system.getEmitterSize() == 1);
            REQUIRE(system.getEmitter("Shooter"));
        }

        WHEN("Add a particle with the new emitter") {
            system.addParticle(system.getEmitter("Shooter"));

            THEN("The initial size should be 1") { REQUIRE(system.getParticles().size() == 1); }
        }

        WHEN("Add a particle with the new emitter with the alternative method") {
            system.addParticle();

            THEN("The initial size should be 1") { REQUIRE(system.getParticles().size() == 1); }
        }

        // all when are isolated
        WHEN("Add 100 more particles with the given emitter") {
            for (int i = 0; i < 100; i++) {
                system.addParticle(system.getEmitter("Shooter"));
            }

            THEN("The new size should be 100") { REQUIRE(system.getParticles().size() == 100); }
        }
    }
}