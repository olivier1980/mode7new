#ifndef MODE7_NEW_ROTATERECT_H
#define MODE7_NEW_ROTATERECT_H
#include <SDL.h>

struct Point {
    float x,y;
};

class RotateRect {

public:
    Point points[4]{};

    RotateRect(float x, float y, float w, float h, int zoom = 0, float angle = 0.0f) {

        //apply zoom
        if ((int)zoom != 0) {
            if ((int)zoom % 2 == 0) {
                x += zoom / 2.0f;
                y += zoom / 2.0f;
                w -= zoom ;
                h -= zoom ;
            }
        }

        points[0].x = x;
        points[0].y = y;

        points[1].x = x+w;
        points[1].y = y;

        points[2].x = x+w;
        points[2].y = y+h;

        points[3].x = x;
        points[3].y = y+h;

        //apply rotation
        if (angle != 0) {
            for (int i = 0; i < 4; ++i) {

                points[i].x -= (w / 2) + x;
                points[i].y -= h / 2 + y;

                float rotated_x = points[i].x * SDL_cosf(angle) - points[i].y * SDL_sinf(angle);
                float rotated_y = points[i].x * SDL_sinf(angle) + points[i].y * SDL_cosf(angle);

                rotated_x += (w / 2) + x;
                rotated_y += h / 2 + y;

                points[i].x = rotated_x;
                points[i].y = rotated_y;
            }
        }

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


#endif //MODE7_NEW_ROTATERECT_H
