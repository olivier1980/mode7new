#pragma once

#include <SDL.h>
#include <functional>
#include <utility>
#include "Logger/Logger.h"
#include <queue>
#include <memory>
#include <variant>
#include <math.h>

extern float DTR;// = M_PI/180.0f;
extern float RTD;// = 180.0f/M_PI;

struct Mode7Param {
    float fFoVDivider{5.0f};
    float sampleHeight{200.0f};
    float sampleWidth{200.0f};
    float horizon = 5.0f; //5
    int winHeight = 600; //animate this
    int winYPos = 300;
};


struct IAnimateAction {
    //virtual void test() {}

    //runtime type information
    //virtual table pointer first before fields
    virtual ~IAnimateAction() = default;

    virtual void run() {
        std::cout << "IAnimateAction";
    };
};

struct AnimateParams{

    std::unique_ptr<IAnimateAction> action;
    //callback std::function
    //msec
};



struct ZoomAction : public IAnimateAction {
    explicit ZoomAction(int targetZoom);
    int targetZoom;

    //override not required but good practice
    //
    void run() override {
        std::cout << "ZoomAction" << targetZoom;
    };
    //virtual void run() {};
};
struct TranslateAction : public IAnimateAction{
    TranslateAction(int targetX, int targetY);
    int targetX;
    int targetY;

    void run() override {
        std::cout << "TranslateAction";
    };
};
struct TurnAction: public IAnimateAction {
    explicit TurnAction(float targetAngle);
    float targetAngle; //radials
};

struct AnimateActions {
    int mSec; //define in action itself?
    //CALLBACK PARAMETER
    //std::vector<std::unique_ptr<IAnimateAction>> actions;
    std::vector<std::variant<ZoomAction, TranslateAction>> actions;
    //std::vector<IAnimateAction> actions;
};



class Camera {
public:
    Camera() = default;
    Mode7Param mode7Params;
    bool CameraTopDownMode = true;
    float x{300};
    float y{300};
    int w{200};
    int h{200};


    std::queue<AnimateActions> animatelist;

    float angle{35*DTR};
    float zoom{};
    float skew{};
    float zoomSpeed{100.0f};
    int height = 20;

    float dt;

    float targetDegrees{};
    float targetDegreesPerSecond{};

    float targetZoom{};
    float targetZoomPerSecond{};
    float targetZoomIn = true;

    //performance
    std::function<void(Camera &t)> targetCallback;

    void moveLeft(float d);
    void moveRight(float d);
    void moveUp(float d);
    void moveDown(float d);

    void skewHorizontal(float angle);
    void Reset();
    void Zoom(float zoomSpeed);
    void ZoomIn();
    void ZoomOut();

    int getWindowHeight() const
    {
        return CameraTopDownMode ? h : mode7Params.winHeight;
    }

    int getWindowHeightOffset() const
    {
        return CameraTopDownMode ? 0 : mode7Params.winYPos;
    }

    void zoomTo(float z, float sec, bool zoomIn = true) {
        targetZoom = z;
        targetZoomPerSecond = (z-zoom)/sec;
        targetZoomIn = zoomIn;
    }

    template<typename F>
    void zoomTo(float z, float sec ,F &func) {
        targetZoom = z;
        targetZoomPerSecond = (z-zoom)/sec;
        targetCallback = std::move(func);
    }

    void rotateTo(float degrees, float sec);
    void animateHeight() {
        //mode7Params.winHeight = 600;
        //mode7Params.winYPos = 0;
        //mode7Params.horizon = 5;
        //CameraTopDownMode = false;

    }


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

//        Logger::Log(std::to_string(zoom)
//                    +  " - " + std::to_string(localZoom2)
//                   +  " - " + std::to_string(localZoom)
//        );

        return SDL_Rect {
            .x = (int)(x + (localZoom / 2.0f)),
            .y = (int) (y + (localZoom / 2.0f)),
            .w = w - localZoom,
            .h = h - localZoom
        };
    }

};


//void Camera::zoomTo(float z, float sec, std::function<void()> func ) {
//
//    //int remainder = targetZoomPerSecond % 2;
//    //targetZoomPerSecond = targetZoomPerSecond - remainder;
//}
