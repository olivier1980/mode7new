#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Logger/Logger.h"
#include <filesystem>
#include "function.h"
#include "Camera.h"
#define MOVE_SPEED 5

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;
/*
 * TOP DOWN
 * left = move camera left
 * zoom = grow or shrink camera
 * no FOV
 *
 * FP
 * left = rotate to left
 * no zoom
 * FOV = angle
 */

bool CameraTopDownMode = true;
SDL_Event ev;


TTF_Font* tmpfont;
void loadfont() {
    tmpfont = TTF_OpenFont("I:\\fonts\\FreeSans.ttf", 22);
    if (tmpfont == nullptr){
        //printf("Unable to load font: %s %s \n", file, TTF_GetError());
        exit(-1);
    }
}


SDL_Window *window;
SDL_Renderer *renderer;
//SDL_Rect camera {
//        .x = 0,
//        .y = 0,
//        .w = 200,
//        .h = 200
//};


bool zoomIn, zoomOut;
bool rotateLeft, rotateRight;
bool moveLeft, moveRight, moveUp, moveDown;
bool flyUp, flyDown;
bool skew;
//float cameraRotation{0.0f};

SDL_Rect rectLeft;
SDL_Rect rectRight;

void init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

        exit(-1);
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    window = SDL_CreateWindow(
            "Game",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1200,
            600,
            SDL_WINDOW_BORDERLESS|SDL_WINDOW_OPENGL
    );
    if (!window) {

        exit(-1);
    }
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        exit(-1);
    }
}

bool switchView{};
int millisecsPreviousFrame = 0;

int main(int argc, char *argv[]) {

    init();
    Camera c(renderer);
    std::string filePath = "./assets/base.png";
    if (!std::filesystem::exists(filePath)) {
        Logger::Err("Texture not found: " + filePath);
        exit(-1);
    }


    SDL_Surface *surface = IMG_Load(filePath.c_str());
    SDL_Surface *convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);


    rectLeft.x = 0;
    rectLeft.y=  0;
    rectLeft.w = 600;
    rectLeft.h = 600;

    rectRight.x = 600;
    rectRight.y=  0;
    rectRight.w = 600;
    rectRight.h = 600;

    bool running = true;

    int horizon = 50;
    int sampleHeight = 200;

    while(running) {
        int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks64() - millisecsPreviousFrame);
        if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
            SDL_Delay(timeToWait);
        }
        double deltaTime = (SDL_GetTicks64() - millisecsPreviousFrame) / 1000.0;
        //std::cout << deltaTime << std::endl;
        // Store the "previous" frame time
        millisecsPreviousFrame = SDL_GetTicks64();
        //Logger::Log(switchView);

        c.dt = deltaTime;

        if (moveUp) {
            c.moveUp(MOVE_SPEED);
        }

        if (moveDown) {
            c.moveDown(MOVE_SPEED);
        }

        if (moveLeft) {
            c.moveLeft(MOVE_SPEED);
           // camera.x -= MOVE_SPEED;
        }
        if (moveRight) {
            c.moveRight(MOVE_SPEED);
          //  camera.x += MOVE_SPEED;
        }
        if (rotateLeft) {
            c.rotate(1.0f);
           // cameraRotation-=.01f;
        }
        if (rotateRight) {
            c.rotate(-1.0f); //clockwise rotation is negative
           // cameraRotation+=.01f;
        }
        if (zoomIn) {
            c.ZoomIn();
            //shrinkRect(camera);
        }
        if (zoomOut) {
            c.ZoomOut();
            //growRect(camera);
        }

        if (flyUp) c.changeHeight(1);
        if (flyDown) c.changeHeight(-1);

        if (skew) {
            c.skewHorizontal(0.01f);
        }

        //Clear
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, nullptr,&rectLeft);

        //camera
        SDL_SetRenderDrawColor(renderer, 0,255,0,155);

        //Texture is twice the size in left panel, to compensate the camera should be drawn halved
        float factor = (float)rectLeft.w / convertedSurface->w;
        //SDL_Rect scaledCamera = scaleRect(camera, factor);
        //SDL_RenderDrawRect(renderer, &scaledCamera);
        //SDL_RenderDrawRect(renderer, &camera);
        c.debugDraw(factor);

        SDL_Surface  *sur = SDL_CreateRGBSurface (0, c.w, c.h, 32,0,0,0,0);
        SDL_Surface  *newSurface = SDL_ConvertSurfaceFormat(sur, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(sur);

        int x_mask = convertedSurface->w - 1;
        int y_mask = convertedSurface->h - 1;

        // current position in the source bitmap
        float src_x, src_y;
        float start_x = c.x;
        float start_y = c.y;

        float dest_x;
        float dest_y;
        float rotated_x, rotated_y;

        if (switchView) {

            if (CameraTopDownMode) {
                if (c.zoom < 130) {
                    Logger::Log(c.zoom);
                    c.zoomSpeed = 160;
                    //zoomIn = true;
                    //rotateLeft = true;
                    c.rotateTo(-90, 5);
                    switchView = false;
                    //c.Zoom(2 * deltaTime);
                } else {

                    zoomIn =false;
                    //CameraTopDownMode = false;
                }
            } else {



            }


        }
        //Logger::Log(c.zoom);
        if (CameraTopDownMode) {


            for (int y = 0; y < c.h; y++) {

                // set the position in the source bitmap to the
                // beginning of this line
                src_x = start_x;
                src_y = start_y;

                for (int x = 0; x < c.w; x++) {
                    dest_x = src_x - (c.w / 2.0f) - c.x;
                    dest_y = src_y - (c.h / 2.0f) - c.y;

                    rotated_x = dest_x * cos(c.angle) - dest_y * sin(c.angle);
                    rotated_y = dest_x * sin(c.angle) + dest_y * cos(c.angle);

                    dest_x = rotated_x + (c.w / 2.0f) + c.x;
                    dest_y = rotated_y + (c.h / 2.0f) + c.y;

                    Uint32 pixel = get_pixel(convertedSurface,
                                             (int) dest_x & x_mask,
                                             (int) dest_y & y_mask);

                    put_pixel(newSurface, x, y, pixel);

                    // advance the position in the source bitmap
                    src_x++;
                }

                // for the next line we have a different starting position
                start_y++;
            }

        } else {

            // the distance and horizontal scale of the line we are drawing

            float fFoVHalf = M_PI / 4;

            for (int y = 0; y < newSurface->h/2; y++) {
                float angle = -c.angle-0.5*M_PI;
                float distance = c.height * sampleHeight / (y + horizon);
                float fStartX = -c.x + (cosf(angle + fFoVHalf) * distance);
                float fStartY = -c.y - (sinf(angle + fFoVHalf) * distance);
                float fEndX = -c.x + (cosf(angle - fFoVHalf) * distance);
                float fEndY = -c.y - (sinf(angle - fFoVHalf) * distance);

//                Logger::Log("first: " + std::to_string(first) + ", second " + std::to_string(second) + ", final: " + std::to_string(distance));
//                //huh++;


                //SDL_RenderDrawLine(renderer, spacex, spacey, spacex  * 200, spacey * 200);
                // go through all points in this screen line
                for (int x = 0; x < newSurface->w; x++) {
                    float fSampleWidth = (float)x / 200;
                    float fSampleX = fStartX + ((fEndX - fStartX) * fSampleWidth);
                    float fSampleY = fStartY + ((fEndY - fStartY) * fSampleWidth);

                    auto pixel = get_pixel(convertedSurface,
                                           (int) fSampleX & x_mask,
                                           (int) fSampleY & y_mask);

                    put_pixel(newSurface, (int) x, (int) y + newSurface->h/2, pixel);
                }
            }
        }

        auto tmpTexture = SDL_CreateTextureFromSurface(renderer, newSurface);

        //Rotation only useful from top-down
        SDL_RenderCopyEx(renderer, tmpTexture , nullptr, &rectRight, 0, nullptr, SDL_FLIP_NONE);

        SDL_FreeSurface(newSurface);

        //SDL_RenderCopy(renderer, texture , &camera, &rectRight);

        SDL_DestroyTexture(tmpTexture);

        SDL_RenderPresent(renderer);

        c.Update();

        while (SDL_PollEvent(&ev)) {
            const Uint8 *keys = SDL_GetKeyboardState(nullptr);
            zoomIn = false;
            zoomOut = false;
            rotateLeft = false;
            rotateRight = false;
            moveLeft = false;
            moveRight = false;
            moveUp = false;
            moveDown = false;
            skew = false;
            flyDown = false;
            flyUp = false;

            //TODO normalize movement
            if (keys[SDL_SCANCODE_LEFT]) {
                rotateLeft = true;
            }
            if (keys[SDL_SCANCODE_RIGHT]) {
                rotateRight = true;
            }
            if (keys[SDL_SCANCODE_W]) {
                moveUp = true;
            }
            if (keys[SDL_SCANCODE_S]) {
                moveDown = true;
            }
            if (keys[SDL_SCANCODE_A]) {
                moveLeft = true;
            }
            if (keys[SDL_SCANCODE_D]) {
                moveRight = true;
            }

            if(keys[SDL_SCANCODE_PAGEUP]) flyUp = true;
            if(keys[SDL_SCANCODE_PAGEDOWN]) flyDown = true;

            if (CameraTopDownMode) {
                if (keys[SDL_SCANCODE_Z]) {
                    zoomIn = true;
                }
                if (keys[SDL_SCANCODE_X]) {
                    zoomOut = true;
                }


                if (keys[SDL_SCANCODE_T]) {
                    skew = true;
                }
            }

            switch (ev.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if (ev.key.keysym.sym == SDLK_ESCAPE) running = false;

                    if (ev.key.keysym.sym == SDLK_g) switchView = !switchView;


                    if (ev.key.keysym.sym == SDLK_o) horizon++;
                    if (ev.key.keysym.sym == SDLK_p) horizon !=1 ? horizon-- : 1;
                    if (ev.key.keysym.sym == SDLK_k) sampleHeight++;
                    if (ev.key.keysym.sym == SDLK_l) sampleHeight--;
                    break;


            }
        }

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

