#include "raycasting.h"
#include <math.h>

//extern void enableFpu(void);

#define CPACR (unsigned int *) 0xE000ED88

double sqrt_software(double x) {
    if (x < 0) {
        return -1; // Invalid input for negative numbers
    }

    // Handle small values
    if (x == 0) return 0;

    // Initial guess for the square root
    double guess = x / 2.0;
    double epsilon = 1e-3;  // Tolerance

    // Newton-Raphson iteration
    while (1) {
        double d0 = x / guess;
        double d1 = guess + d0;
        double d2 = d1 / 2.0;


        double d3 = 24.5 + 2.0;

        double d4 = 24.5;
        double d5 = 2.0;
        double d6 = 0;
        d6 = d4 + d5;

        double new_guess = (guess + x / guess) / 2.0;
        if (fabs(new_guess - guess) < epsilon) {
            return new_guess;
        }
        guess = new_guess;
    }
}

void main(void){
    //enableFpu();

    *CPACR |= 0xF00000; // Enable FPU, men verkar inte göra nån skillnad

    volatile double val = 47.0;
    volatile float val_f = (float) val; // Fungerar inte, val_f blir fel värde

    volatile float f_a = 2.0f;
    volatile float f_b = 3.0f;
    volatile float f_c = f_a * f_b; // Fungerar inte, f_c blir fel värde

    f_a = 24.5;
    f_b = 2.0;
    volatile float f_d = f_a + f_b;

    volatile double d_a = 2.0;
    volatile double d_b = 3.0;
    volatile double d_c = d_a * d_b;

    volatile float f0 = 49.0f;
    volatile float f1 = sqrtf(f0);

    volatile double d0 = 49.0;
    volatile double d1 = sqrt_software(d0);
    //volatile double d1 = sqrt(d0);

    double* distances = castRays(2, 2, 45.0f);
    double rayDistance0 = distances[0];
    double rayDistance1 = distances[1];
    double rayDistance2 = distances[2];
}