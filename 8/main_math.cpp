#include <iostream>
#include <ctime>

using namespace std;

#define TIMES 1e7

void print_time(clock_t time, const char* action)
{
    if (time >= 1000)
        cout << "\t" << action << ": " << ((double)time) / CLOCKS_PER_SEC << " s.";
    else
        cout << "\t" << action << ": " << ((double)time) << " ms.";
}


template <typename Type>
Type sum(Type a, Type b)
{
    Type result = 0;
    clock_t start_time, res_time = 0;

    for (int i = 0; i < TIMES; i++)
    {
        start_time = clock();
        result = a + b;
        res_time += clock() - start_time;
    }

    print_time(res_time, "Sum");

    return result;
}

template <typename Type>
Type mul(Type a, Type b)
{
    Type result = 0;
    clock_t start_time, res_time = 0;

    for (int i = 0; i < TIMES; i++)
    {
        start_time = clock();
        result = a * b;
        res_time += clock() - start_time;
    }

    print_time(res_time, "Mul");

    return result;
}

#ifdef ASM
template <typename Type>
Type sum_asm(Type a, Type b)
{
    Type result = 0;
    clock_t start_time, res_time = 0;

    for (int i = 0; i < TIMES; i++)
    {
        start_time = clock();
        __asm__(
            "fld %1\n"                  // загружаем a на вершину стека
            "fld %2\n"                  // загружаем b на вершину стека
            "faddp %%ST(1), %%ST(0)\n"  // складываем ST(1) и ST(0), сохраняем результат в ST(1)
                                        // и извлекаем из стека сопроцессора (поэтому 'p' в названии команды)
            "fstp %0\n"                 // извлекаем число из FPU в память, в данном случае из ST(0) в res
            : "=m"(result)              // res - выходной параметр
            : "m"(a),                   // a - входной параметр
              "m"(b)                    // b - входной параметр
        );
        res_time += clock() - start_time;
    }
    
    print_time(res_time, "Sum");
    
    return result;
}

template <typename Type>
Type mul_asm(Type a, Type b)
{
    Type result = 0;
    clock_t start_time, res_time = 0;

    for (int i = 0; i < TIMES; i++)
    {
        start_time = clock();
        __asm__
        (
            "fld %1\n"
            "fld %2\n"
            "fmulp %%ST(1), %%ST(0)\n"
            "fstp %0\n"
            :"=m"(result)
            : "m"(a),
            "m"(b)
        );
        res_time += clock() - start_time;
    }

    print_time(res_time, "Mul");

    return result;
}
#endif

template <typename Type>
void time_measure(Type a, Type b)
{
    #ifdef ASM
    sum_asm(a, b);
    mul_asm(a, b);
    #else
    sum(a, b);
    mul(a, b);
    #endif
}

int main()
{
    float f1 = 3.7f;
    float f2 = 2.3f;
    cout << "FLOAT:";
    time_measure(f1, f2);

    double d1 = 3.7;
    double d2 = 2.3;
    cout << "\nDOUBLE:";
    time_measure(d1, d2);

    cout << endl;
}