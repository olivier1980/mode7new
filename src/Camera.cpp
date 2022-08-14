#include "Camera.h"
#include "glm/glm.hpp"

//#include "Logger/Logger.h"

float DTR = M_PI/180.0f;
float RTD = 180.0f/M_PI;

void Camera::Reset() {
    x = 100;
    y = 100;
    w = 200;
    h = 200;
    angle = 0;
    zoom = -20;
    targetZoomPerSecond = 0;
    targetDegreesPerSecond = 0;

    //zoomSpeed = 100.0f;
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

    //Logger::Log("Current zoom: " + std::to_string(zoom));
}

void Camera::Update() {

    if (!animatelist.empty()) {
        auto el = animatelist.front();

        //trying to copy, but its unique so by definition cant be copied
//        for (auto action : el.actions) {
//        }
        for (auto &action : el.actions) {
            //static_cast<Type> //allows cast to primitive types
            //dynamic_cast (checks at runtime for which object is in ptr)
            //const_cast adds/removes const - code smell
            //reinterpret_cast  -doesnt check anything, quake3 sqrt faster hack

            //use run action to switch instead action->run();

            //dynamic cast has higher cost
            if (holds_alternative<ZoomAction>(action)) {
                auto zoom = get<ZoomAction>(action);
                //targetZoomPerSecond = zoom.targetZoom;
            }
            if (holds_alternative<TranslateAction>(action)) {
                auto zoom = get<TranslateAction>(action);
            }

        }

        //reset pointer in loop  .reset() = sets to null, and cleans up
        //remove pointer when done
        animatelist.pop();
    }

    if (targetZoomPerSecond != 0) {

        //Logger::Log(std::to_string((int)zoom) +  " - " + std::to_string((int)targetZoom));
        if ((targetZoomIn && (int)zoom > (int)targetZoom) ||
            (!targetZoomIn && (int)zoom < (int)targetZoom)
        ) {
            //angle = targetDegrees*DTR;
            targetZoomPerSecond = 0;
            //targetCallback(*this);
            return;
        }

        auto d = targetZoomPerSecond * dt;

        Zoom(d);
    }

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

ZoomAction::ZoomAction(int targetZoom) : targetZoom(targetZoom) {}
TranslateAction::TranslateAction(int targetX, int targetY) : targetX(targetX), targetY(targetY) {}
TurnAction::TurnAction(float targetAngle) : targetAngle(targetAngle) {}
