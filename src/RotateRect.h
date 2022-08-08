#pragma once
#include <SDL.h>

struct Point {
    float x,y;
};

class RotateRect {

public:
    Point points[4]{};

    RotateRect(SDL_Rect rect, float angle = 0.0f) {

        points[0].x = rect.x;
        points[0].y = rect.y;

        points[1].x = rect.x+rect.w;
        points[1].y = rect.y;

        points[2].x = rect.x+rect.w;
        points[2].y = rect.y+rect.h;

        points[3].x = rect.x;
        points[3].y = rect.y+rect.h;

        //apply rotation
        //if (angle != 0) {
            for (int i = 0; i < 4; ++i) {

                points[i].x -= (rect.w / 2) + rect.x;
                points[i].y -= rect.h / 2 + rect.y;

                float rotated_x = points[i].x * SDL_cosf(angle) - points[i].y * SDL_sinf(angle);
                float rotated_y = points[i].x * SDL_sinf(angle) + points[i].y * SDL_cosf(angle);

                rotated_x += (rect.w / 2) + rect.x;
                rotated_y += rect.h / 2 + rect.y;

                points[i].x = rotated_x;
                points[i].y = rotated_y;
            }
        //}

    }

    void DrawRect(SDL_Renderer *r, float factor = 1.0f) {

        for (int i = 0; i<4; ++i) {

            SDL_SetRenderDrawColor(r, 0,255,0,155);
            if (i==1) {
                SDL_SetRenderDrawColor(r, 255,0,0,155);

            }
            if (i == 3) {
                //Logger::Log((int)(points[i].x * factor) & (int)599);
                //Logger::Log((int)(points[i].x* factor));

                SDL_RenderDrawLine(r,
                                   (int)(points[i].x * factor),
                                   (int)(points[i].y * factor),
                                   (int)(points[0].x * factor),
                                   (int)(points[0].y * factor)
                );
                return;
            }
            SDL_RenderDrawLine(r,
                               (int)(points[i].x*factor) ,
                               (int)(points[i].y*factor) ,
                               (int)(points[i+1].x*factor),
                               (int)(points[i+1].y*factor));
        }


    }


};

