#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 1000000
#define N 4

void copy(float *dst, const float *src) 
{
    for (size_t i = 0; i < N; ++i)
        dst[i] = src[i];
}

void scalarMul(const float *V1, const float *V2)
{
    float res;

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) 
    {
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

void add(const float *V1, const float *V2) 
{
    float *VRes = malloc(N * sizeof(float));

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) 
    {
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

void sub(const float *V1, const float *V2) 
{
    float *VRes = malloc(N * sizeof(float));

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) 
    {
        start = clock();
        for (size_t j = 0; j < N; ++j)
            VRes[j] = V1[j] - V2[j];
        end = clock() - start;

        time += end;
    }

    printf("Time for SUB: %lu ms\n", time);
    printf("Result: [");
    for (size_t i = 0; i < N - 1; ++i)
        printf("%f; ", VRes[i]);
    printf("%f]\n", VRes[N - 1]);

    free(VRes);
}

void mul(const float *V1, float num) 
{
    float *VRes = malloc(N * sizeof(float));

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) 
    {
        start = clock();
        for (size_t j = 0; j < N; ++j)
            VRes[j] = V1[j] * num;
        end = clock() - start;

        time += end;
    }

    printf("Time for MUL: %lu ms\n", time);
    printf("Result: [");
    for (size_t i = 0; i < N - 1; ++i)
        printf("%f; ", VRes[i]);
    printf("%f]\n", VRes[N - 1]);

    free(VRes);
}

void scalarMulAsm(const float *V1, const float *V2) 
{
    float res;
    float V1Copy[N];
    float V2Copy[N];

    copy(V1Copy, V1);
    copy(V2Copy, V2);

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) 
    {
        res = 0;
        start = clock();

        __asm__ __volatile__(
            // Загрузка значения V1Copy в регистр xmm0
            "movaps %1, %%xmm0\n"    

            // Загрузка значения V2Copy в регистр xmm1
            "movaps %2, %%xmm1\n"    

            // Умножение xmm0 на xmm1 (V1Copy * V2Copy)
            "mulps %%xmm1, %%xmm0\n" 

            // Перемещение половины xmm0 в xmm1 (нижние 2 значения xmm0 в верхнюю часть xmm1)
            "movhlps %%xmm0, %%xmm1\n" 

            // Сложение xmm1 с xmm0 (верхняя часть xmm1 + xmm0)
            "addps %%xmm1, %%xmm0\n" 

            // Копирование значения xmm0 в xmm1
            "movaps %%xmm0, %%xmm1\n" 

            // Перестановка элементов xmm0 (меняются местами значения xmm0)
            "shufps $1, %%xmm0, %%xmm0\n" 

            // Сложение xmm1 с xmm0 (значения xmm1 + xmm0)
            "addps %%xmm1, %%xmm0\n" 

            // Загрузка значения xmm0 в переменную res
            "movss %%xmm0, %0\n"     

            : "=m"(VRes)                    // Выходной операнд - переменная VRes
            : "m"(V1Copy), "m"(V2Copy)      // Входные операнды - переменные V1Copy и num
            : "%xmm0", "%xmm1"          
            // Регистры xmm0 и xmm1 указаны в секции Clobber, чтобы компилятор знал, что эти регистры будут изменены в asm
        );

        end = clock() - start;
        time += end;
    }
    printf("Time for SCALAR: %lu ms\n", time);
    printf("Result: %f\n", res);
}

void addAsm(const float *V1, const float *V2) 
{
    float VRes[N];
    float V1Copy[N];
    float V2Copy[N];

    copy(V1Copy, V1);
    copy(V2Copy, V2);

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) 
    {
        start = clock();

        __asm__ __volatile__(
            // Загрузка значения V1Copy в регистр xmm0
            "movaps %1, %%xmm0\n"    

            // Загрузка значения V2Copy в регистр xmm1
            "movaps %2, %%xmm1\n"    

            // Сложение xmm1 и xmm0 (V1Copy + V2Copy)
            "addps %%xmm1, %%xmm0\n" 

            // Загрузка значения xmm0 в переменную VRes
            "movaps %%xmm0, %0\n"    

            : "=m"(VRes)                    // Выходной операнд - переменная VRes
            : "m"(V1Copy), "m"(V2Copy)      // Входные операнды - переменные V1Copy и num
            : "%xmm0", "%xmm1"          
            // Регистры xmm0 и xmm1 указаны в секции Clobber, чтобы компилятор знал, что эти регистры будут изменены в asm
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

void subAsm(const float *V1, const float *V2) 
{
    float VRes[N];
    float V1Copy[N];
    float V2Copy[N];

    copy(V1Copy, V1);
    copy(V2Copy, V2);

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; ++i) 
    {
        start = clock();

        __asm__ __volatile__(
            "movaps %1, %%xmm0\n"
            "movaps %2, %%xmm1\n"
            "subps %%xmm1, %%xmm0\n"
            "movaps %%xmm0, %0\n"
            : "=m"(VRes)
            : "m"(V1Copy), "m"(V2Copy)
            : "%xmm0", "%xmm1"
        );

        end = clock() - start;
        time += end;
    }

    printf("Time for SUB: %lu ms\n", time);
    printf("Result: [");
    for (size_t i = 0; i < N - 1; ++i)
        printf("%f; ", VRes[i]);
    printf("%f]\n", VRes[N - 1]);
}

void mulAsm(const float *V1, float num) 
{
    float VRes[N];
    float V1Copy[N];

    copy(V1Copy, V1);

    clock_t time = 0, start = 0, end = 0;
    for (size_t i = 0; i < COUNT; i++) 
    {
        start = clock();

        __asm__ (
            // Загрузка значения V1Copy в регистр xmm0
            "movaps %1, %%xmm0\n"    

            // Загрузка значения num в регистр xmm1
            "movaps %2, %%xmm1\n"     

            // Применение шаффлера для дублирования значения num в xmm1
            // "shufps $0, %%xmm1, %%xmm1\n" 

            // Умножение xmm0 на xmm1 (V1Copy * num)
            //так как числа имеют 32-разрядность, распределены 4 цифры * 8 бит(1 байт),
            //то следует выполнить команду 4 раза для перемножения всех чисел из 4 разряда
            "mulps %%xmm1, %%xmm0\n" 

            // Загрузка значения xmm0 в переменную VRes 
            "movaps %%xmm0, %0\n"    

            : "=m"(VRes)                 // Выходной операнд - переменная VRes
            : "m"(V1Copy), "m"(num)      // Входные операнды - переменные V1Copy и num
            : "%xmm0", "%xmm1"           // Регистры xmm0 и xmm1
        );


        end = clock() - start;
        time += end;
    }

    printf("Time for MUL: %lu ms\n", time);
    printf("Result: [");
    for (size_t i = 0; i < N - 1; ++i)
        printf("%f; ", VRes[i]);
    printf("%f]\n", VRes[N - 1]);
}

int main() 
{
    float V1[N] = {1e-4, 1.244e-3, -5666.3552, -99.934e-4};
    float V2[N] = {-1.1111, 2013, 43.3443, .178987};

    printf("\n_____________C OPERATIONS_____________\n");
    printf("_________SCALAR MULTIPLICATION________\n");
    scalarMul(V1, V2);

    printf("\n_______________ADDITION_______________\n");
    add(V1, V2);

    printf("\n____________SUBSTITUTION____________\n");
    sub(V1, V2);

    printf("\n____________MULTIPLICATION____________\n");
    mul(V1, 4.f);

    printf("\n_____________ASM INSERTION____________\n");
    printf("_________SCALAR MULTIPLICATION________\n");
    scalarMulAsm(V1, V2);

    printf("\n_______________ADDITION_______________\n");
    addAsm(V1, V2);

    printf("\n____________SUBSTITUTION____________\n");
    subAsm(V1, V2);

    // printf("\n____________MULTIPLICATION____________\n");
    // mulAsm(V1, 4);

    return 0;
}
