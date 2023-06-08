#include <iostream>
#include <cmath>

using namespace std;

double sin_asm()
{
    double result = 0.0;
    __asm__(
        "fldpi\n"
        "fsin\n"
        "fstp %0\n"
        :"=m"(result)
    );
    return result;
}

double sin_half_asm()
{
    double result = 0.0;
    double divide = 2.0;
    __asm__(
        "fldpi\n"
        "fld %1\n"
        "fdiv %%ST(1), %%ST(0)\n"
        "fsin\n"
        "fstp %0\n"
        : "=m"(result)
        : "m"(divide)
    );
    return result;
}


int main()
{
    cout << "SIN:" << endl;
    printf("sin(3.14)\t%.20f\n", sin(3.14));
    printf("sin(3.141596)\t%.20f\n", sin(3.141596));
    printf("sin(M_PI)\t%.20f\n", sin(M_PI));
    printf("asm sin(pi)\t%.20f\n", sin_asm());
    cout << endl;
    printf("sin(3.14 / 2)\t\t%.20f\n", sin(3.14 / 2.0));
    printf("sin(3.141596 / 2)\t%.20f\n", sin(3.141596 / 2.0));
    printf("sin(M_PI / 2.0)\t\t%.20f\n", sin(M_PI / 2.0));
    printf("asm sin(pi / 2)\t\t%.20f\n", sin_half_asm());
}