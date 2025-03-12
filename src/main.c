#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "raycasting.h"
#include "graphics.h"
#include "keyb.h"
#include "ports.h"
#include "gameloop.h"
#include "fpu.h"

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

const int player_1_x = 20;
const int player_2_x = 108;

int player_1_y = 20;
int player_2_y = 20;

int ball_x = 64;
int ball_y = 32;

int ball_speed_x = 5;
int ball_speed_y = 5;

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

void draw_pixel(int x, int y) {
    if (x >= 1 && x < 128 && y >= 1 && y <= 64) {
        graphic_pixel_set(x, y);
    }
}

void draw_ball() {
    for (int x = 0; x < 128; x++) {
        graphic_pixel_set(x, 1);
    }

    for (int x = 0; x < 128; x++) {
        graphic_pixel_set(x, 64);
    }

    for (int y = 0; y < 64; y++) {
        graphic_pixel_set(1, y);
    }

    for (int y = 0; y < 64; y++) {
        graphic_pixel_set(127, y);
    }

    for (int x = ball_x - 2; x <= ball_x + 2; x++) {
        for (int y = ball_y - 2; y <= ball_y + 2; y++) {
            if (x >= 1 && x < 128 && y >= 1 && y <= 64) {
                graphic_pixel_set(x, y);
            }
        }
    }
}

void draw_player(int player_x, int player_y) {
    for (int x = player_x - 2; x <= player_x + 2; x++) {
        for (int y = player_y; y < player_y + 20; y++) {
            draw_pixel(x, y);
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
    draw_player(player_1_x, player_1_y);
    draw_player(player_2_x, player_2_y);
}

void main(void) {
    enable_fpu();
    graphic_initialize();
    init_keyboard();
    graphic_clear_screen();
    bresenham(128/2, 64/2, 0, 0);
    bresenham(128/2, 64/2, 128, 64);
    bresenham(128/2, 64/2, 0, 64);
    bresenham(128/2, 64/2, 128, 0);

    start_game_loop(update, 80);

    while (1) {}
}