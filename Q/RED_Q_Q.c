#include "../include_DM.h"

/*
  Автор: Александров Ярослав
  Группа: 5388

  RED_Q_Q

  Сокращение дроби: числитель и знаменатель делятся на их НОД.
  Знак остаётся в числителе. Знаменатель всегда положителен.
  Если числитель равен нулю, возвращается 0/1.

  Параметры:
    1) NUMBQ* a – рациональное число

  Возвращает указатель на новое NUMBQ – сокращённая дробь,
  или NULL при ошибке.
*/
NUMBQ* RED_Q_Q(NUMBQ* a) {
    if (!a || !a->a.A || !a->b.A || a->b.n <= 0) return NULL;
    if (a->b.n == 1 && a->b.A[0] == 0) return NULL;

    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!result) return NULL;

    /* Числитель равен нулю → 0/1 */
    if (a->a.n == 1 && a->a.A[0] == 0) {
        result->a.b = 0;
        result->a.n = 1;
        result->a.A = (int*)calloc(1, sizeof(int));
        result->b.n = 1;
        result->b.A = (int*)malloc(sizeof(int));
        if (!result->a.A || !result->b.A) {
            free(result->a.A); free(result->b.A); free(result);
            return NULL;
        }
        result->b.A[0] = 1;
        return result;
    }

    /* 1. Получить |числитель| как натуральное число */
    NUMBZ* absNumZ = ABS_Z_Z(&a->a);
    if (!absNumZ) { free(result); return NULL; }

    NUMBN* absNum = TRANS_Z_N(absNumZ);
    free(absNumZ->A);
    free(absNumZ);

    if (!absNum) { free(result); return NULL; }

    /* 2. НОД(|числитель|, знаменатель) */
    NUMBN* gcd = GCF_NN_N(absNum, &a->b);
    free(absNum->A);
    free(absNum);

    if (!gcd) { free(result); return NULL; }

    /* 3. Создать целое представление НОД (копируем массив для безопасности) */
    NUMBZ gcdZ;
    gcdZ.b = 0;
    gcdZ.n = gcd->n;
    gcdZ.A = (int*)malloc(gcd->n * sizeof(int));
    if (!gcdZ.A) {
        free(gcd->A); free(gcd);
        free(result);
        return NULL;
    }
    memcpy(gcdZ.A, gcd->A, gcd->n * sizeof(int));

    /* 4. Разделить числитель и знаменатель на НОД */
    NUMBZ* newNum = DIV_ZZ_Z(&a->a, &gcdZ);
    NUMBN* newDen = DIV_NN_N(&a->b, gcd);

    /* gcdZ.A больше не нужен – он был копией */
    free(gcdZ.A);
    free(gcd->A);
    free(gcd);

    if (!newNum || !newDen) {
        if (newNum) { free(newNum->A); free(newNum); }
        if (newDen) { free(newDen->A); free(newDen); }
        free(result);
        return NULL;
    }

    /* 5. Сформировать результат */
    result->a = *newNum;   // копируем поля структуры
    free(newNum);          // структуру newNum удаляем, массив остаётся в result
    result->b = *newDen;
    free(newDen);

    return result;
}
