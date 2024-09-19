#include <rendering/window.h>
#include <example/randomsystem.h>
#include <particlesystem/particlesystem.h>

#include <fmt/format.h>

#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

/*
 * This application represents the "Client" the uses your particle system
 */
int main() try {
    rendering::Window window("Particle System v0.0.2 pre-release alpha", 850, 850);

    // Creates an example system with a 1000 particles.
    // This should be replaced with your new particle system
    //const size_t numParticles = 1000;
    //example::RandomSystem system{numParticles};
    particleSystem system{}; // numParticles
    /*
    Wind wind{glm::vec2{2, 3}};
    system.addEffect(&wind);
    */

    
    
    
    bool running = true;
    
    values UIParam;
    bool windOn = false;
    bool dragOn = false;
    bool explosionOn = false;
    bool randomOn = false;
    int explosionSpawn = 100;
    int explosionSpawn2 = 100;
    
    while (running) {
        window.beginFrame();
        //std::cout << system.getParticles().size();
        // Update the example system.
        // This should be replaced with a call to the new particle system
        //system.update(window.time(), speed);
        system.updateAll(window.time(), UIParam);  // window.time()
        // Clear screen with color
        window.clear({0, 0, 0, 1});
        if (system.getParticles().size() != 0) {
            // Draw particles
            //  This should also be adjusted to work with the new particle system
            window.drawPoints(system.getPosition(), system.getSize(), system.getColor(),
                              system.getParticles().size(), sizeof(Particle));
        
        }
        
        // Example user interface, extend for your needs.
        // Might add controls for effects and emitters etc.
        {
            window.beginGuiWindow("UI");
            window.text("I'm text!");
            window.sliderFloat("Speed", UIParam.speed, 0.001f, 10.0f);
            
            
            if (window.checkbox("Wind", windOn)) {
                if (windOn) {
                    std::shared_ptr<Effect> wind = std::make_shared<Wind>(UIParam.windVelocity);
                    system.addEffect(wind);
                } else {
                    system.removeEffect("Wind");
                }
            }
            if (windOn) {
                window.sliderFloat("Wind x", UIParam.windVelocity.x, -0.05f, 0.05f);
                window.sliderFloat("Wind y", UIParam.windVelocity.y, -0.05f, 0.05f);
                system.getEffect("Wind")->adjustEffect(UIParam.windVelocity);
            } else {
                UIParam.windVelocity = glm::vec2(0.0f, 0.0f);
            }
            
            if (window.checkbox("Drag", dragOn)) {
                if (dragOn) {
                    UIParam.dragForce = 0.01f;
					std::shared_ptr<Effect> drag = std::make_shared<Drag>(UIParam.dragForce);
					system.addEffect(drag);
                }
                else {
					system.removeEffect("Drag");
				}   
            }
            if (dragOn) {
				window.sliderFloat("Drag", UIParam.dragForce, 0.0f, 2.5f);
				system.getEffect("Drag")->adjustEffect(UIParam.dragForce);
            }
            else {
                UIParam.dragForce = 0.0f;
			}
            
            window.checkbox("Recreate particles", UIParam.recreate);

            window.text("I'm text!");

            if (window.checkbox("Kill out of bounds", UIParam.bounds)) {
                if (UIParam.bounds) {
					UIParam.loop = false;
                    UIParam.bounce = false;
                }
			}
            
            if (window.checkbox("Bounds loop", UIParam.loop)) {
                if (UIParam.loop) {
                    UIParam.bounds = false;
					UIParam.bounce = false;
                }
            }

            if (window.checkbox("Bounds bounce", UIParam.bounce)) {
                if (UIParam.bounce) {
					UIParam.bounds = false;
                    UIParam.loop = false;
                }
            }
            
            if (UIParam.bounds) {
				window.sliderFloat("Bounds size", UIParam.boundsSize, 0.5f, 2.0f);
            }
            else {
				UIParam.boundsSize = 1.0f;
			}

            if (window.checkbox("Explosion Emitter", explosionOn)) {
                if (explosionOn) {
					std::shared_ptr<Emitter> explosion = std::make_shared<Explosion>();
					system.addEmitter(explosion);
                    
                }
                else {
					system.removeEmitter("Explosion");
				}   
            }
			
            if (explosionOn) {
                window.sliderInt("Spawn explosion", explosionSpawn, 1, 1000);
			    if (window.button("Spawn an explosion of particles")) {
                    for (int i = 0; i < explosionSpawn; i++) {
                        system.addParticle(system.getEmitter("Explosion"));
                    }
                }
            }

			if (window.checkbox("Random Emitter", randomOn)) {
                if (randomOn) {
                    std::shared_ptr<Emitter> random = std::make_shared<Random>();
                    system.addEmitter(random);

                } else {
                    system.removeEmitter("Random");
                }
            }

            if (randomOn) {
                window.sliderInt("Spawn random explosion", explosionSpawn2, 1, 1000);
                if (window.button("Spawn a random explosion of particles")) {
                    for (int i = 0; i < explosionSpawn2; i++) {
                        system.addParticle(system.getEmitter("Random"));
                    }
                }
            }

            if (window.button("Close application")) {
                running = false;
            }
            

            
            window.text("Time Alive: " + fmt::to_string(window.time()));
            window.text("RecreateBool: " + fmt::to_string(UIParam.recreate));
            window.text("BoundsBool: " + fmt::to_string(UIParam.bounds));
            window.text("WindBool: " + fmt::to_string(windOn));
            window.text("DragBool: " + fmt::to_string(dragOn));
            window.text("Number of particles: " + fmt::to_string(system.getParticles().size()));
            //window.text("Number: " + fmt::to_string(system.getRand()));
            window.endGuiWindow();
        }
        
        window.endFrame();
        running = running && !window.shouldClose();
    }

    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    fmt::print("{}", e.what());
    return EXIT_FAILURE;
}
