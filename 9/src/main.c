#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 1000000
#define N 4

void copy(float *dst, const float *src) {
    for (size_t i = 0; i < N; ++i)
        dst[i] = src[i];
}

void scalarMul(const float *V1, const float *V2) {
    float res;

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) {
        res = 0;

        start = clock();
        for (size_t j = 0; j < N; ++j)
            res += V1[j] * V2[j];
        end = clock() - start;

        time += end;
    }


    printf("Time for SCALAR: %lu ms\n", time);
    printf("Result: %f\n", res);
}

void add(const float *V1, const float *V2) {
    float *VRes = malloc(N * sizeof(float));

    clock_t time = 0, start, end;
    for (size_t i = 0; i < COUNT; ++i) {
        start = clock();
        for (size_t j = 0; j < N; ++j)
            VRes[j] = V1[j] + V2[j];
        end = clock() - start;

        time += end;
    }

    printf("Time for ADD: %lu ms\n", time);
    printf("Result: [");
    for (size_t i = 0; i < N - 1; ++i)
        printf("%f; ", VRes[i]);
    printf("%f]\n", VRes[N - 1]);

    free(VRes);
}


void scalarMulAsm(const float *V1, const float *V2) {
    float res;
    float V1Copy[N];
    float V2Copy[N];

    copy(V1Copy, V1);
    copy(V2Copy, V2);

    clock_t time = 0, start, end;
    for (size_t i = 0; i < COUNT; ++i) {
        res = 0;
        start = clock();

        __asm__ __volatile__(
                "movaps %1, %%xmm0\n"
                "movaps %2, %%xmm1\n"
                "mulps %%xmm1, %%xmm0\n"
                "movhlps %%xmm0, %%xmm1\n"  //  копирует более старшие половины значений из регистра
                                            // xmm0 в более младшие половины значений регистра xmm1
                "addps %%xmm1, %%xmm0\n"  // поэлементное сложение
                "movaps %%xmm0, %%xmm1\n"
                "shufps $1, %%xmm0, %%xmm0\n" // маске в нулевой - первый
                "addps %%xmm1, %%xmm0\n"
                "movss %%xmm0, %0\n"
                : "=m"(res)
                : "m"(V1Copy), "m"(V2Copy)
                : "%xmm0", "%xmm1"
                );

        end = clock() - start;
        time += end;
    }
    printf("Time for SCALAR: %lu ms\n", time);
    printf("Result: %f\n", res);
}

void addAsm(const float *V1, const float *V2) {
    float VRes[N];
    float V1Copy[N];
    float V2Copy[N];

    copy(V1Copy, V1);
    copy(V2Copy, V2);

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) {
        start = clock();

        __asm__ __volatile__(
                "movaps %1, %%xmm0\n"
                "movaps %2, %%xmm1\n"
                "addps %%xmm1, %%xmm0\n"
                "movaps %%xmm0, %0\n"
                : "=m"(VRes)
                : "m"(V1Copy), "m"(V2Copy)
                : "%xmm0", "%xmm1"
                );

        end = clock() - start;
        time += end;
    }

    printf("Time for ADD: %lu ms\n", time);
    printf("Result: [");
    for (size_t i = 0; i < N - 1; ++i)
        printf("%f; ", VRes[i]);
    printf("%f]\n", VRes[N - 1]);
}


int main() {
    float V1[N] = {1e-4, 1.244e-3, -5666.3552, -99.934e-4};
    float V2[N] = {-1.1111, 2013, 43.3443, .178987};

    printf("\n_____C_____\n");
    printf("SCALAR MULTIPLICATION\t\n");
    scalarMul(V1, V2);

    printf("\nADDITION\n");
    add(V1, V2);

    printf("\n_____ASM_____\n");
    printf("SCALAR MULTIPLICATION\n");
    scalarMulAsm(V1, V2);

    printf("\nADDITION\n");
    addAsm(V1, V2);

    return 0;
}
