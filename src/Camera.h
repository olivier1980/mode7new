#ifndef MODE7MYSDL_CAMERA_H
#define MODE7MYSDL_CAMERA_H



#include <SDL.h>

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
    int height = 20;
    Point points[4]{};

    void moveLeft(float d);
    void moveRight(float d);
    void moveUp(float d);
    void moveDown(float d);

    void skewHorizontal(float angle);

    void Zoom(float z);

    void changeHeight (float d);

    void rotate(float degrees);

    void debugDraw(float factor);
private:
    SDL_Renderer *renderer;
};


#endif //MODE7MYSDL_CAMERA_H
