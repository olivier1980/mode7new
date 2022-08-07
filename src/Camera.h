#ifndef MODE7MYSDL_CAMERA_H
#define MODE7MYSDL_CAMERA_H

#include <SDL.h>

//float RTD = M_PI / 380;

struct Point {
    float x,y;
};

class Camera {
public:
    explicit Camera(SDL_Renderer *renderer);

    float x{};
    float y{};
    float w{200};
    float h{200};
    float angle{};
    float zoom{1};
    float skew{};
    float zoomSpeed{100.0f};
    int height = 20;
    Point points[4]{};

    float dt;

    float targetDegrees{};
    float timeElapsed{};
    float targetDegreesPerSecond{};
    bool targetRotateLeft = true;

    void moveLeft(float d);
    void moveRight(float d);
    void moveUp(float d);
    void moveDown(float d);

    void skewHorizontal(float angle);

    void ZoomIn();
    void ZoomOut();
    void rotateTo(float degrees, float sec);
    void changeHeight (float d);

    void Update();
    void rotate(float degrees);
    void debugDraw(float factor);
private:
    void Zoom(int zoomSpeed);
    SDL_Renderer *renderer;
};


#endif //MODE7MYSDL_CAMERA_H
