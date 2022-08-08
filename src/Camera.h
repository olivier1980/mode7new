#ifndef MODE7MYSDL_CAMERA_H
#define MODE7MYSDL_CAMERA_H

#include <SDL.h>

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

    SDL_Rect getSDLRect() {
        return SDL_Rect {
            .x = x,
            .y = y,
            .w = w,
            .h = h
        };
    }

};


#endif //MODE7MYSDL_CAMERA_H
