#include "Camera.h"
#include "glm/glm.hpp"
//#include "Logger/Logger.h"

float DTR = M_PI/180.0f;
float RTD = 180.0f/M_PI;

void Camera::Reset() {
    x = 0;
    y = 0;
    w = 200;
    h = 200;
    angle = 0;
    zoom = 0;
    targetZoomPerSecond = 0;
    targetDegreesPerSecond = 0;

    zoomSpeed = 100.0f;
    height = 20;
}

//actually strafe
void Camera::moveLeft(float d) {
    float sideAngle = angle + 1.5*M_PI;

    float dx = cos(sideAngle) * d;
    float dy = sin(sideAngle) * d;
    x += dx;
    y += dy;
}
void Camera::moveRight(float d) {
    float sideAngle = angle + 0.5*M_PI;

    float dx = cos(sideAngle) * d;
    float dy = sin(sideAngle) * d;
    x += dx;
    y += dy;
}
void Camera::moveUp(float d) {
    float dx = cos(angle) * d;
    float dy = sin(angle) * d;
    x += dx;
    y += dy;
};
void Camera::moveDown(float d) {
    float dx = SDL_cosf(angle) * d;
    float dy = SDL_sinf(angle) * d;
    x -= dx;
    y -= dy;
};

void Camera::skewHorizontal(float angle) {
    skew+=angle;
    //auto t2 = 3.14/180;
    //auto t = SDL_tanf(angle);
    //auto t3 = tan(45.0);
//    points[1].y = SDL_tanf(skew) * w;
//    points[2].y = SDL_tanf(skew) * w + h;
};

void Camera::ZoomIn() {
    Zoom((int)(zoomSpeed*dt));
}

void Camera::ZoomOut() {
    Zoom((int)(-zoomSpeed*dt));
}

void Camera::Zoom(float z) {

    zoom += z;

    Logger::Log("Current zoom: " + std::to_string(zoom));
}

void Camera::Update() {
    if (targetDegreesPerSecond != 0) {

        if ((int)(angle*RTD) == (int)targetDegrees) {
            angle = targetDegrees*DTR;
            targetDegreesPerSecond = 0;
            return;
        }

        auto d = targetDegreesPerSecond * dt;

        rotate(d);
    }

    if (targetZoomPerSecond != 0) {

        //Logger::Log(std::to_string((int)zoom) +  " - " + std::to_string((int)targetZoom));
        if ((int)zoom >= (int)targetZoom) {
            //angle = targetDegrees*DTR;
            targetZoomPerSecond = 0;
            return;
        }

        auto d = targetZoomPerSecond * dt;

        Zoom(d);
    }

}

void Camera::zoomTo(float z, float sec) {
    targetZoom = z;
    targetZoomPerSecond = (z-zoom)/sec;
    //int remainder = targetZoomPerSecond % 2;
    //targetZoomPerSecond = targetZoomPerSecond - remainder;
}

void Camera::rotateTo(float degrees, float sec) {
    targetDegreesPerSecond = degrees/sec;
    targetDegrees = degrees;
}

void Camera::changeHeight (float d) {
    if (height + d > 0) {
        height += d;
    }
};

void Camera::rotate(float degrees) {

    //Logger::Log("degrees = " + std::to_string(degrees));
    float radials = degrees * DTR;
    //Logger::Log("radial = " + std::to_string(radials/M_PI));
    angle += radials;
    //Logger::Log("angle = " + std::to_string(angle/M_PI));

}


