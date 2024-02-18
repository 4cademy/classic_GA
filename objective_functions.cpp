//
// Created by Marcel Beyer on 12.01.2024.
//

#include <vector>
#include <cmath>

// local includes
#include "objective_functions.h"

// ACKLEY FUNCTION
float ackley_function(const std::vector<float>& solution, int DIM) {
    float result = 0;
    float sum1 = 0;
    float sum2 = 0;

    float a = 20;
    float b = 0.2;
    float c = 2*M_PI;

    for (int i = 0; i < DIM; i++) {
        sum1 += powf(solution[i], 2);
        sum2 += cosf(c*solution[i]);
    }

    result = -a * expf(-b*sqrtf(sum1/(float)DIM)) - expf(sum2/(float)DIM) + a + (float)M_E;

    return result;
}

// GRIEWANK FUNCTION
float griewank_function(const std::vector<float>& solution, int DIM) {
    float result = 0;
    float sum1 = 0;
    float sum2 = 1;

    for (int i = 0; i < DIM; i++) {
        sum1 += powf(solution[i], 2)/4000;
        sum2 *= cosf(solution[i]/sqrtf((float)(i+1)));
    }

    result = sum1 - sum2 + 1;

    return result;
}

// LEVY FUNCTION
float levy_function(const std::vector<float>& solution, int DIM) {
    float result = 0;
    float w = 1 + (solution[0] - 1)/4;

    float sum1 = powf(sinf(M_PI*w), 2);

    float sum2 = 0;

    for (int i = 0; i < DIM-1; i++) {
        w = 1 + (solution[i] - 1)/4;
        sum2 += powf(w - 1, 2)*(1 + 10*powf(sinf(M_PI*w + 1), 2));
    }

    w = 1 + (solution[DIM-1] - 1)/4;
    float sum3 = powf(w - 1, 2)*(1 + powf(sinf(2.0f*(float)M_PI*w), 2));

    result = sum1 + sum2 + sum3;

    return result;
}

// RASTRIGIN FUNCTION
float rastrigin_function(const std::vector<float>& solution, int DIM) {
    float result = 10.0f*(float)DIM;

    for (int i = 0; i < DIM; i++) {
        result += powf(solution[i], 2) - 10*cosf(2.0f*(float)M_PI*solution[i]);
    }


    return result;
}

// SCHWEFEL FUNCTION
float schwefel_function(const std::vector<float>& solution, int DIM) {
    float result = 418.9829f*(float)DIM;

    for (int i = 0; i < DIM; i++) {
        result += -solution[i]*sinf(sqrtf(fabsf(solution[i])));
    }

    return result;
}

// SPHERE FUNCTION
float sphere_function(const std::vector<float>& solution, int DIM) {
    float result = 0;

    for (int i = 0; i < DIM; i++) {
        result += powf(solution[i], 2);
    }
    return result;
}

// ROTATED HYPER-ELLIPSOID FUNCTION
float rotated_hyper_ellipsoid_function(const std::vector<float>& solution, int DIM) {       // very computationally expensive
    float result = 0;

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j <= i; j++) {
            result += powf(solution[j], 2);
        }
    }
    return result;
}

// SUM OF DIFFERENT POWERS FUNCTION
float sum_of_different_powers_function(const std::vector<float>& solution, int DIM) {
    float result = 0;

    for (int i = 0; i < DIM; i++) {
        result += powf(fabsf(solution[i]), (float)(i+2));
    }
    return result;
}

// SUM SQUARES FUNCTION
float sum_squares_function(const std::vector<float>& solution, int DIM) {
    float result = 0;

    for (int i = 0; i < DIM; i++) {
        result += (float)(i+1)*powf(solution[i], 2);
    }
    return result;
}

// ZAKHAROV FUNCTION
float zakharov_function(const std::vector<float>& solution, int DIM) {
    float result = 0;
    float sum1 = 0;
    float sum2 = 0;

    for (int i = 0; i < DIM; i++) {
        sum1 += powf(solution[i], 2);
        sum2 += 0.5f*(float)(i+1)*solution[i];
    }

    result = sum1 + sum2*sum2 + sum2*sum2*sum2*sum2;

    return result;
}

// ROSENBROCK FUNCTION
float rosenbrock_function(const std::vector<float>& solution, int DIM) {
    float result = 0;

    for (int i = 0; i < DIM-1; i++) {
        result += 100*powf((solution[i+1] - powf(solution[i], 2)), 2) + powf((solution[i] - 1), 2);
    }

    return result;
}

// Define the function pointer type
typedef float (*ObjectiveFunctionPointer)(const std::vector<float>&, int);

// Create a vector of function pointers and initialize it with a function
std::vector<ObjectiveFunctionPointer> objectiveFunctionVector {
    &ackley_function,
    &griewank_function,
    &levy_function,
    &rastrigin_function,
    &schwefel_function,
    &rotated_hyper_ellipsoid_function,
    &sphere_function,
    &sum_of_different_powers_function,
    &sum_squares_function,
    &zakharov_function,
    &rosenbrock_function
};

float objective_function_no(const std::vector<float>& solution, const int DIM, const int number){
    int index = number;
    return objectiveFunctionVector[index](solution, DIM);
}