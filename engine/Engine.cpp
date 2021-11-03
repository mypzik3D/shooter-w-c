//
// Created by Иван Ильин on 14.01.2021.
//

#include <iostream>

#include "Engine.h"
#include "utils/Time.h"
#include "ResourceManager.h"
#include "animation/Timeline.h"
#include "SoundController.h"

Engine::Engine() {
    Time::init();
    Timeline::init();
    ResourceManager::init();
    SoundController::init();
}

void Engine::create(int screenWidth, int screenHeight, const std::string &name, bool verticalSync, sf::Color background,
                    sf::Uint32 style) {
    _name = name;
    screen->open(screenWidth, screenHeight, name, verticalSync, background, style);

    Log::log("Engine::create(): started engine (" + std::to_string(screenWidth) + "x" + std::to_string(screenHeight) +
             ") with title '" + name + "'.");
    Time::update();

    start();
    camera->init(screenWidth, screenHeight);

    while (screen->isOpen()) {
        // 'd' in the beginning of the name means debug.
        // While printing debug info we will take into account only timer names witch start with 'd '
        Time::startTimer("d all");

        screen->clear();

        Time::update();

        Time::startTimer("d game update");
        update();
        Time::stopTimer("d game update");

        // sometimes we dont need to update physics world
        // (for example in menu or while pause)
        // hence we can set '_updateWorld' equal to false in setUpdateWorld(bool):
        if (_updateWorld) {

            Time::startTimer("d animations");
            Timeline::update();
            Time::stopTimer("d animations");

            Time::startTimer("d collisions");
            world->update();
            Time::stopTimer("d collisions");

            Time::startTimer("d projections");
            if (_useOpenGL) {
                GLfloat *view = camera->glView();
                for (auto &it : *world) {
                    if (it.second->isVisible()) {
                        GLfloat *model = it.second->glModel();
                        GLfloat *geometry = Screen::glMeshToGLfloatArray(it.second, camera->position());
                        screen->glDrawMesh(geometry, view, model, 3 * it.second->triangles().size());
                        free(geometry);
                        free(model);
                    }
                }
                free(view);
            } else {
                // clear triangles from previous frame
                camera->clear();
                // project triangles to the camera plane
                for (auto &it : *world) {
                    camera->project(it.second);
                }
                // draw triangles on the screen
                for (auto &t : camera->sorted()) {
                    screen->drawTriangle(*t);
                }

                _triPerSec = camera->buffSize() * Time::fps();
            }
            Time::stopTimer("d projections");

            if (Consts::SHOW_FPS_COUNTER) {
                screen->drawText(std::to_string(Time::fps()) + " fps",
                                 Vec2D(static_cast<double>(screen->width()) - 100.0, 10.0), 25,
                                 sf::Color(100, 100, 100));
            }

            printDebugInfo();
            gui();
        }

        screen->display();

        Time::stopTimer("d all");
    }
}

void Engine::exit() {
    if (screen->isOpen()) {
        screen->close();
    }
    SoundController::free();
    ResourceManager::free();
    Timeline::free();
    Time::free();

    Log::log("Engine::exit(): exit engine (" + std::to_string(screen->width()) + "x" +
             std::to_string(screen->height()) + ") with title '" + screen->title() + "'.");
}

void Engine::printDebugInfo() const {

    if (_showDebugInfo) {
        // coordinates & fps:
        std::string text = _name + "\n\n X: " +
                           std::to_string((camera->position().x())) + "\n Y: " +
                           std::to_string((camera->position().y())) + "\n Z: " +
                           std::to_string((camera->position().z())) + "\n\n" +
                           std::to_string(screen->width()) + "x" +
                           std::to_string(screen->height()) + "\t" +
                           std::to_string(Time::fps()) + " fps";
        if (_useOpenGL) {
            text += "\n Using OpenGL acceleration";
        } else {
            text += "\n" + std::to_string(_triPerSec) + " tris/s";
        }

        sf::Text t;

        t.setFont(*ResourceManager::loadFont(Consts::THIN_FONT));
        t.setString(text);
        t.setCharacterSize(30);
        t.setFillColor(sf::Color::Black);
        t.setPosition(static_cast<float>(screen->width()) - 400.0f, 10.0f);

        screen->drawText(t);

        // timers:
        int timerWidth = screen->width() - 100;
        float xPos = 50;
        float yPos = 300;
        int height = 50;

        double totalTime = Time::elapsedTimerMilliseconds("d all");
        double timeSum = 0;
        int i = 0;
        for (auto &[timerName, timer] : Time::timers()) {
            int width = timerWidth * timer.elapsedMilliseconds() / totalTime;

            if (timerName == "d all" || timerName[0] != 'd') {
                continue;
            }

            screen->drawTetragon(Vec2D{xPos, yPos + height * i},
                                 Vec2D{xPos + width, yPos + height * i},
                                 Vec2D{xPos + width, yPos + height + height * i},
                                 Vec2D{xPos, yPos + height + height * i},
                                 {static_cast<sf::Uint8>(255 * width / timerWidth),
                                  static_cast<sf::Uint8>(255 * (1 - width / timerWidth)),
                                  0, 100});


            screen->drawText(
                    timerName.substr(2, timerName.size()) + " (" + std::to_string((int) (100 * timer.elapsedMilliseconds() / totalTime)) + "%)",
                    Vec2D{xPos + 10, yPos + height * i + 5}, 30,
                    sf::Color(0, 0, 0, 100));

            i++;
            timeSum += timer.elapsedMilliseconds();
        }

        int width = timerWidth * (totalTime - timeSum) / totalTime;
        screen->drawTetragon(Vec2D{xPos, yPos + height * i},
                             Vec2D{xPos + width, yPos + height * i},
                             Vec2D{xPos + width, yPos + height + height * i},
                             Vec2D{xPos, yPos + height + height * i},
                             {static_cast<sf::Uint8>(255 * width / timerWidth),
                              static_cast<sf::Uint8>(255 * (1 - width / timerWidth)),
                              0, 100});


        screen->drawText("other (" + std::to_string((int) (100 * (totalTime - timeSum) / totalTime)) + "%)",
                         Vec2D{xPos + 10, yPos + height * i + 5}, 30,
                         sf::Color(0, 0, 0, 100));

    }
}
