#include "Camera.h"
#include "Logger/Logger.h"

float RTD = M_PI / 180;

Camera::Camera(SDL_Renderer *renderer) : renderer(renderer) {
    points[0].x = 0;
    points[0].y = 0;

    points[1].x = w;
    points[1].y = 0;

    points[2].x = w;
    points[2].y = h;

    points[3].x = 0;
    points[3].y = h;
}

void Camera::moveLeft(float d) {
    x-=d;
    for (int i = 0; i<4; ++i) {
        points[i].x -= d;
    }
}
void Camera::moveRight(float d) {
    x+=d;
    for (int i = 0; i<4; ++i) {
        points[i].x += d;
    }
}
void Camera::moveUp(float d) {
    y -= d;
    for (int i = 0; i<4; ++i) {
        points[i].y -= d;
    }
};
void Camera::moveDown(float d) {
    y += d;
    for (int i = 0; i<4; ++i) {
        points[i].y += d;
    }
};

void Camera::skewHorizontal(float angle) {
    skew+=angle;
    //auto t2 = 3.14/180;
    //auto t = SDL_tanf(angle);
    //auto t3 = tan(45.0);
    points[1].y = SDL_tanf(skew) * w;
    points[2].y = SDL_tanf(skew) * w + h;
};

void Camera::ZoomIn() {
    Zoom((int)(zoomSpeed*dt));
}

void Camera::ZoomOut() {
    Zoom((int)(-zoomSpeed*dt));
}

void Camera::Zoom(int z) {

    zoom += z;
    x += z/2;
    y += z/2;
    w -= z;
    h -= z;

    points[0].x = x;
    points[0].y = y;

    points[1].x = x+w;
    points[1].y = y;

    points[2].x = x+w;
    points[2].y = y+h;

    points[3].x = x;
    points[3].y = y+h;

    if (angle) {
        for (int i = 0; i < 4; ++i) {

            points[i].x -= (w / 2) + x;
            points[i].y -= h / 2 + y;

            float rotated_x = points[i].x * cos(angle) - points[i].y * sin(angle);
            float rotated_y = points[i].x * sin(angle) + points[i].y * cos(angle);

            rotated_x += (w / 2) + x;
            rotated_y += h / 2 + y;

            points[i].x = rotated_x;
            points[i].y = rotated_y;
        }
    }
}

void Camera::Update() {
    if (targetDegreesPerSecond != 0) {

        if ((angle*180)/M_PI < targetDegrees) {
            angle = targetDegrees*RTD;
            targetDegreesPerSecond = 0;
            return;
        }

        auto d = targetDegreesPerSecond * dt;
        if (targetRotateLeft) {
            rotate(d);
        }
    }
}

void Camera::rotateTo(float degrees, float sec) {

    targetDegreesPerSecond = degrees/sec;
    targetDegrees = degrees;

}

void Camera::changeHeight (float d) {
    height += d;
};

void Camera::rotate(float degrees) {

    Logger::Log("degrees = " + std::to_string(degrees));
    degrees = RTD*degrees;
    Logger::Log("radial = " + std::to_string(degrees));
    angle += degrees;
    Logger::Log("angle = " + std::to_string(angle));

    for (int i = 0; i<4; ++i) {

        points[i].x -= (w/2) + x;
        points[i].y -= h/2  + y;

        float rotated_x = points[i].x * SDL_cosf (-degrees) - points[i].y * SDL_sinf (-degrees);
        float rotated_y = points[i].x * SDL_sinf (-degrees) + points[i].y * SDL_cosf (-degrees);

        rotated_x += (w/2) + x;
        rotated_y += h/2 + y;

        points[i].x = rotated_x;
        points[i].y = rotated_y;
    }

}

void Camera::debugDraw(float factor)
    {
        SDL_SetRenderDrawColor(renderer, 255,0,0,155);

        for (int i = 0; i<4; ++i) {
            if (i>0) {
                SDL_SetRenderDrawColor(renderer, 0,255,0,155);
            }
            if (i == 3) {
                SDL_RenderDrawLine(renderer,
                                   points[i].x*factor,
                                   points[i].y*factor,
                                   points[0].x*factor,
                                   points[0].y*factor);
                return;
            }
            SDL_RenderDrawLine(renderer,
                               points[i].x*factor,
                               points[i].y*factor,
                               points[i+1].x*factor,
                               points[i+1].y*factor);
        }

    }

