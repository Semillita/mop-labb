#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "raycasting.h"
#include "graphics.h"
#include "keyb.h"
#include "ports.h"
#include "gameloop.h"
#include "fpu.h"

//extern void enableFpu(void);

void floatTest(void){
    //*CPACR |= 0x00F00000; // Sätt igång FPU, gör ingen skillnad
    enable_fpu();

    float a = 2.0f;
    float b = 3.0f;
    float c = a * b; // Fungerar inte, c blir inf
    float d = a + b; // Fungerar bra

    double e = 2.0;
    double f = 3.0;
    double g = e * f; // Fungerar bra
    double h = e + f; // Fungerar inte, h blir 2

    float i = 49.0f;
    float j = sqrtf(i);

    double k = 49.0;
    //double l = sqrt(k); // Kraschar simserver

    double m = 47.0;
    float n = (float) m; // Fungerar inte, n blir inf

    double* distances = castRays(2, 2, 45.0f);
    double rayDistance0 = distances[0];
    double rayDistance1 = distances[1];
    double rayDistance2 = distances[2];
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int bresenham(int x0, int y0, int x1, int y1) {
    int steep = 1;
    if(abs(y1 - y0) > abs(x1 - x0)) {
        steep = 1;
    } else {
        steep = 0;
    }
    if(steep) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    if(x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int error = 0;
    int y = y0;
    int ystep = 0;
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    for (int x = x0; x < x1; x++) {
        if(steep) {
            graphic_pixel_set(y, x);
        } else {
            graphic_pixel_set(x, y);
        }
        error += dy;
        if(2*error >= dx) {
            y += ystep;
            error -= dx;
        }
    }
    return 0;
}

int keys[16];

int px = 20;
int py = 20;

int prev_px = 0;
int prev_py = 0;

const int player_1_x = 20;
const int player_2_x = 108;

int player_1_y = 20;
int player_2_y = 20;

int ball_x = 64;
int ball_y = 32;

int ball_speed_x = 1;
int ball_speed_y = 1;

void key_handler() {
    *EXTI_IMR = 0x000;

    unsigned char key = keyb();
    if (key != 0xFF) {
        if(key == 1) px -= 10;
        if(key == 2) px += 10;
    }

    *EXTI_PR = 0xF00; 
    *GPIO_D_ODRHIGH = 0xF0;
    *EXTI_IMR = 0xF00;
}

void update_player_positions(struct Input input) {
    if (input.player_1.up) {
        player_1_y -= 1;
        player_1_y = (player_1_y < 0) ? 0 : player_1_y;
    }

    if (input.player_1.down) {
        player_1_y += 1;
        player_1_y = (player_1_y > 128) ? 128 : player_1_y;
    }

    if (input.player_2.up) {
        player_2_y -= 1;
        player_2_y = (player_2_y < 0) ? 0 : player_2_y;
    }

    if (input.player_2.down) {
        player_2_y += 1;
        player_2_y = (player_2_y > 128) ? 128 : player_2_y;
    }
}

void update_ball_position() {
    ball_x += ball_speed_x;
    ball_y += ball_speed_y;

    if (ball_y < 0) {
        ball_y *= -1;
        ball_speed_y *= -1;
    } else if (ball_y >= 64) {
        ball_y = 64 - (ball_y - 64);
        ball_speed_y *= -1;
    }

    if (ball_x < 0) {
        ball_x *= -1;
        ball_speed_x *= -1;
    } else if (ball_x >= 128) {
        ball_x = 128 - (ball_x - 128);
        ball_speed_x *= -1;
    }
}

void draw_ball() {
    //bresenham(ball_x - 1, ball_y - 1, ball_x + 1, ball_y + 1);
    //bresenham(ball_x - 1, ball_y + 1, ball_x + 1, ball_y - 1);

    unsigned int ball_size = 5;
    for (int x = ball_x - (ball_size / 2); x < ball_x + (ball_size / 2); x++) {
        for (int y = ball_y - (ball_size / 2); y < ball_y + (ball_size / 2); y++) {
            if (x >= 0 && x < 128 && y >= 0 && y <= 64) {
                graphic_pixel_set(x, y);
            }
        }
    }
}

void update() {
    struct Input input;
    input = get_input();
    update_player_positions(input);

    update_ball_position();

    graphic_clear_screen();
    draw_ball();
}

void main(void) {
    enable_fpu();
    graphic_initialize();
    InitKeyboard(key_handler);
    graphic_clear_screen();
    bresenham(128/2, 64/2, 0, 0);
    bresenham(128/2, 64/2, 128, 64);
    bresenham(128/2, 64/2, 0, 64);
    bresenham(128/2, 64/2, 128, 0);

    start_game_loop(update, 80);

    while (1) {
        /*if ((prev_px != px) || (prev_py != py)) {
            graphic_clear_screen();
            prev_px = px;
            prev_py = py;
            bresenham(px, py, px + 30, py + 30);
        }*/
    }

    float a = 2.0f;
    float b = 3.0f;
    float c = a * b; // Fungerar inte, c blir inf
    float d = a + b; // Fungerar bra

    double e = 2.0;
    double f = 3.0;
    double g = e * f; // Fungerar bra
    double h = e + f; // Fungerar inte, h blir 2

    float i = 49.0f;
    float j = sqrtf(i);

    double k = 49.0;
    //double l = sqrt(k); // Kraschar simserver

    double m = 47.0;
    float n = (float) m; // Fungerar inte, n blir inf

    double* distances = castRays(2, 2, 45.0f);
    double rayDistance0 = distances[0];
    double rayDistance1 = distances[1];
    double rayDistance2 = distances[2];

    while(1) {

    }
}