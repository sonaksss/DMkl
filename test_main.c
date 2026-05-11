
#include "include_DM.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ======================================== */
/* ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ */
/* ======================================== */

double measure_time(clock_t start, clock_t end)
{
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

NUMBN makeN(const char* str)
{
    NUMBN n;

    n.n = 0;
    while (str[n.n] != '\0')
        n.n++;

    n.A = (int*)malloc(sizeof(int) * n.n);

    for (int i = 0; i < n.n; i++)
        n.A[i] = str[n.n - 1 - i] - '0';

    return n;
}

NUMBZ makeZ(const char* str)
{
    NUMBZ z;

    if (str[0] == '-')
    {
        z.b = 1;
        str++;
    }
    else
    {
        z.b = 0;
    }

    z.n = 0;
    while (str[z.n] != '\0')
        z.n++;

    z.A = (int*)malloc(sizeof(int) * z.n);

    for (int i = 0; i < z.n; i++)
        z.A[i] = str[z.n - 1 - i] - '0';

    return z;
}

void printNt(NUMBN* n)
{
    for (int i = n->n - 1; i >= 0; i--)
        printf("%d", n->A[i]);
}

void printZt(NUMBZ* z)
{
    if (z->b)
        printf("-");

    for (int i = z->n - 1; i >= 0; i--)
        printf("%d", z->A[i]);
}

void freeNt(NUMBN* n)
{
    free(n->A);
}

void freeZt(NUMBZ* z)
{
    free(z->A);
}

/* ======================================== */
/* ОСНОВНАЯ ПРОГРАММА */
/* ======================================== */

int main()
{
    clock_t start, end;

    printf("========================================\n");
    printf(" АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ МОДУЛЕЙ\n");
    printf("========================================\n\n");

    /* ======================================== */
    /* НАТУРАЛЬНЫЕ ЧИСЛА */
    /* ======================================== */

    printf("--- Натуральные числа ---\n");

    NUMBN n1 = makeN("12345");
    NUMBN n2 = makeN("6789");

    start = clock();
    int cmp = COM_NN_D(&n1, &n2);
    end = clock();

    printf("COM_NN_D(12345, 6789) = %d, время: %.3f ms\n",
           cmp,
           measure_time(start, end));

    start = clock();
    NUMBN* addNN = ADD_NN_N(&n1, &n2);
    end = clock();

    printf("ADD_NN_N = ");
    printNt(addNN);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBN* subNN = SUB_NN_N(&n1, &n2);
    end = clock();

    printf("SUB_NN_N = ");
    printNt(subNN);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBN* mulNN = MUL_NN_N(&n1, &n2);
    end = clock();

    printf("MUL_NN_N = ");
    printNt(mulNN);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBN* divNN = DIV_NN_N(&n1, &n2);
    end = clock();

    printf("DIV_NN_N = ");
    printNt(divNN);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBN* modNN = MOD_NN_N(&n1, &n2);
    end = clock();

    printf("MOD_NN_N = ");
    printNt(modNN);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBN* gcfNN = GCF_NN_N(&n1, &n2);
    end = clock();

    printf("GCF_NN_N = ");
    printNt(gcfNN);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBN* lcmNN = LCM_NN_N(&n1, &n2);
    end = clock();

    printf("LCM_NN_N = ");
    printNt(lcmNN);
    printf(", время: %.3f ms\n\n", measure_time(start, end));

    /* ======================================== */
    /* ЦЕЛЫЕ ЧИСЛА */
    /* ======================================== */

    printf("--- Целые числа ---\n");

    NUMBZ z1 = makeZ("-12345");
    NUMBZ z2 = makeZ("6789");

    start = clock();
    NUMBZ* absZ = ABS_Z_Z(&z1);
    end = clock();

    printf("ABS_Z_Z = ");
    printZt(absZ);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBZ* addZZ = ADD_ZZ_Z(&z1, &z2);
    end = clock();

    printf("ADD_ZZ_Z = ");
    printZt(addZZ);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBZ* subZZ = SUB_ZZ_Z(&z1, &z2);
    end = clock();

    printf("SUB_ZZ_Z = ");
    printZt(subZZ);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBZ* mulZZ = MUL_ZZ_Z(&z1, &z2);
    end = clock();

    printf("MUL_ZZ_Z = ");
    printZt(mulZZ);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBZ* divZZ = DIV_ZZ_Z(&z1, &z2);
    end = clock();

    printf("DIV_ZZ_Z = ");
    printZt(divZZ);
    printf(", время: %.3f ms\n", measure_time(start, end));

    start = clock();
    NUMBZ* modZZ = MOD_ZZ_Z(&z1, &z2);
    end = clock();

    printf("MOD_ZZ_Z = ");
    printZt(modZZ);
    printf(", время: %.3f ms\n\n", measure_time(start, end));

    /* ======================================== */
    /* ОЧИСТКА ПАМЯТИ */
    /* ======================================== */

    freeNt(&n1);
    freeNt(&n2);

    freeZt(&z1);
    freeZt(&z2);

    free(addNN->A); free(addNN);
    free(subNN->A); free(subNN);
    free(mulNN->A); free(mulNN);
    free(divNN->A); free(divNN);
    free(modNN->A); free(modNN);
    free(gcfNN->A); free(gcfNN);
    free(lcmNN->A); free(lcmNN);

    free(absZ->A); free(absZ);
    free(addZZ->A); free(addZZ);
    free(subZZ->A); free(subZZ);
    free(mulZZ->A); free(mulZZ);
    free(divZZ->A); free(divZZ);
    free(modZZ->A); free(modZZ);

    printf("Тестирование завершено успешно.\n");

    return 0;
}
