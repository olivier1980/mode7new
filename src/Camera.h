#pragma once

#include <SDL.h>
#include "Logger/Logger.h"

extern float DTR;// = M_PI/180.0f;
extern float RTD;// = 180.0f/M_PI;

class Camera {
public:
    Camera() = default;

    int x{};
    int y{};
    int w{200};
    int h{200};

    float angle{};
    float zoom{};
    float skew{};
    float zoomSpeed{100.0f};
    int height = 20;

    float dt;

    float targetDegrees{};
    float targetDegreesPerSecond{};
    float targetZoom{};
    float targetZoomPerSecond{};


    void moveLeft(float d);
    void moveRight(float d);
    void moveUp(float d);
    void moveDown(float d);

    void skewHorizontal(float angle);
    void Reset();
    void Zoom(float zoomSpeed);
    void ZoomIn();
    void ZoomOut();
    void zoomTo(float z, float sec);
    void rotateTo(float degrees, float sec);
    void changeHeight (float d);

    void Update();
    void rotate(float degrees);

    static int closestInteger(int a, int b) {
        int c1 = a - (a % b);
        int c2 = (a + b) - (a % b);


        if (a - c1 > c2 - a) {
            return c2;
        } else {
            return c1;
        }
    }

    static float closestFloat(float a, float b) {
        float c1 = a - fmod(a, b);
        float c2 = (a + b) - fmod(a, b);

        if (a - c1 > c2 - a) {
            return c2;
        } else {
            return c1;
        }
    }

    [[nodiscard]] SDL_Rect getZoomedSDLRect() const {

        int localZoom2 = closestInteger(zoom, 2);
        int localZoom = closestFloat(zoom, 2.0f);

        Logger::Log(std::to_string(zoom)
                    +  " - " + std::to_string(localZoom2)
                   +  " - " + std::to_string(localZoom)
        );

        return SDL_Rect {
            .x = x + (int)(localZoom / 2.0f),
            .y = y + (int)(localZoom / 2.0f),
            .w = w - localZoom,
            .h = h - localZoom
        };
    }

};
