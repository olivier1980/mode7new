#ifndef MODE7MYSDL_FUNCTION_H
#define MODE7MYSDL_FUNCTION_H

#include <SDL.h>

struct Point {
    float x,y;
};

class Camera {
public:
    explicit Camera(SDL_Renderer *renderer) : renderer(renderer) {
        points[0].x = 0;
        points[0].y = 0;

        points[1].x = w;
        points[1].y = 0;

        points[2].x = w;
        points[2].y = h;

        points[3].x = 0;
        points[3].y = h;
    }

    float x{};
    float y{};
    float w{200};
    float h{200};
    float angle{};
    float zoom{1};
    float skew{};

    Point points[4]{};

    void moveLeft(float d) {
        x-=d;
        for (int i = 0; i<4; ++i) {
            points[i].x -= d;
        }
    }
    void moveRight(float d) {
        x+=d;
        for (int i = 0; i<4; ++i) {
            points[i].x += d;
        }
    }
    void moveUp(float d) {
        y -= d;
        for (int i = 0; i<4; ++i) {
            points[i].y -= d;
        }
    };
    void moveDown(float d) {
        y += d;
        for (int i = 0; i<4; ++i) {
            points[i].y += d;
        }
    };

    void skewHorizontal(float angle) {
        skew+=angle;
        //auto t2 = 3.14/180;
        //auto t = SDL_tanf(angle);
        //auto t3 = tan(45.0);
        points[1].y = SDL_tanf(skew) * w;
        points[2].y = SDL_tanf(skew) * w + h;
    };

    void Zoom(float z) {
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

        for (int i = 0; i<4; ++i) {

            points[i].x -= (w/2) + x;
            points[i].y -= h/2  + y;

            float rotated_x = points[i].x * cos (angle) - points[i].y * sin (angle);
            float rotated_y = points[i].x * sin (angle) + points[i].y * cos (angle);

            rotated_x += (w/2) + x;
            rotated_y += h/2 + y;

            points[i].x = rotated_x;
            points[i].y = rotated_y;
        }
    }

    void rotate(float degrees) {
        angle += degrees;
        for (int i = 0; i<4; ++i) {

            points[i].x -= (w/2) + x;
            points[i].y -= h/2  + y;

            float rotated_x = points[i].x * cos (degrees) - points[i].y * sin (degrees);
            float rotated_y = points[i].x * sin (degrees) + points[i].y * cos (degrees);

            rotated_x += (w/2) + x;
            rotated_y += h/2 + y;

            points[i].x = rotated_x;
            points[i].y = rotated_y;
        }

    }

    void debugDraw(float factor)
    {
        for (int i = 0; i<4; ++i) {
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
private:
    SDL_Renderer *renderer;
};

//void shrinkRect(SDL_Rect &input, const float v = 2, bool fromCenter = true)
//{
//    if (fromCenter) {
//        input.x += v/2;
//        input.y += v/2;
//    }
//    input.w-=v;
//    input.h-=v;
//}
//void growRect(SDL_Rect &input, const int v = 2, bool fromCenter = true)
//{
//    if (fromCenter) {
//        input.x -= v/2;
//        input.y -= v/2;
//    }
//    input.w+=v;
//    input.h+=v;
//}
//
//SDL_Rect scaleRect(const SDL_Rect &input, const float &factor = 1.0f)
//{
//    int newX = input.x * factor;
//    int newY = input.y * factor;
//    int newW = input.w * factor;
//    int newH = input.h * factor;
//    return SDL_Rect {
//            .x = newX,
//            .y = newY,
//            .w = newW,
//            .h = newH
//    };
//}

// get_pixel: Acquires a 32-bit pixel from a surface at given coordinates
Uint32 get_pixel( SDL_Surface* surface, int x, int y )
{
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *) p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *) p;
            break;

        default:
            return 0;
    }
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32*)surface->pixels;

    //Get the requested pixelSDL_SRCCOLORKEY
    return pixels[ ( y * surface->w ) + x ];
}

// put_pixel: Drops a 32-bit pixel onto a surface at given coordinates.
void put_pixel( SDL_Surface *surface, int x, int y, Uint32 c )
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = c;
            break;

        case 2:
            *(Uint16 *) p = c;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (c >> 16) & 0xff;
                p[1] = (c >> 8) & 0xff;
                p[2] = c & 0xff;
            } else {
                p[0] = c & 0xff;
                p[1] = (c >> 8) & 0xff;
                p[2] = (c >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *) p = c;
            break;
    }
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = c;
}

#endif //MODE7MYSDL_FUNCTION_H
