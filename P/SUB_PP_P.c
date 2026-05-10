#include "../include_DM.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  isZeroQ

  Проверяет, равен ли рациональный коэффициент нулю.

  Параметры:
    1) NUMBQ* q — указатель на рациональное число

  Возвращаемое значение:
    1 — коэффициент равен нулю
    0 — коэффициент не равен нулю
*/
static int isZeroQ(NUMBQ* q) {
    if (q == NULL || q->a.A == NULL || q->a.n <= 0)
        return 1;

    for (int i = 0; i < q->a.n; i++) {
        if (q->a.A[i] != 0)
            return 0;
    }

    return 1;
}

/*
  trimDegree

  Удаляет ведущие нулевые коэффициенты и корректирует степень многочлена.

  Параметры:
    1) NUMBP* P — указатель на многочлен

  Функция ничего не возвращает.
*/
static void trimDegree(NUMBP* P) {
    while (P->m > 0 && isZeroQ(&P->C[P->m])) {
        free(P->C[P->m].a.A);
        free(P->C[P->m].b.A);
        P->m--;
    }
}

/*
  makeZeroQ

  Создаёт рациональное число 0/1.

  Параметры:
    1) NUMBQ* q — указатель на рациональное число

  Возвращаемое значение:
    1 — успех
    0 — ошибка выделения памяти
*/
static int makeZeroQ(NUMBQ* q) {
    q->a.b = 0;
    q->a.n = 1;
    q->a.A = (int*)calloc(1, sizeof(int));
    if (!q->a.A)
        return 0;

    q->b.n = 1;
    q->b.A = (int*)malloc(sizeof(int));
    if (!q->b.A) {
        free(q->a.A);
        return 0;
    }

    q->b.A[0] = 1;
    return 1;
}

/*
  SUB_PP_P

  Вычисляет разность двух многочленов:
      A - B

  Алгоритм:
    1. Определяется максимальная степень.
    2. Выделяется память под результат.
    3. Для каждого коэффициента вычисляется разность.
    4. Удаляются ведущие нулевые коэффициенты.

  Параметры:
    1) NUMBP* A — уменьшаемое
    2) NUMBP* B — вычитаемое

  Возвращаемое значение:
    Указатель на новый многочлен или NULL при ошибке.
*/
NUMBP* SUB_PP_P(NUMBP* A, NUMBP* B) {
    if (A == NULL || B == NULL)
        return NULL;

    /* Максимальная степень результата */
    int maxDeg = (A->m > B->m) ? A->m : B->m;

    /* Выделение памяти под результирующий многочлен */
    NUMBP* result = (NUMBP*)malloc(sizeof(NUMBP));
    if (result == NULL)
        return NULL;

    result->m = maxDeg;
    result->C = (NUMBQ*)malloc((maxDeg + 1) * sizeof(NUMBQ));
    if (result->C == NULL) {
        free(result);
        return NULL;
    }

    /* Инициализация указателей для безопасного освобождения при ошибке */
    for (int i = 0; i <= maxDeg; i++) {
        result->C[i].a.A = NULL;
        result->C[i].b.A = NULL;
    }

    /* Временный нулевой коэффициент 0/1 */
    NUMBQ zeroQ;
    if (!makeZeroQ(&zeroQ)) {
        free(result->C);
        free(result);
        return NULL;
    }

    /* Вычисление разности коэффициентов */
    for (int i = 0; i <= maxDeg; i++) {
        NUMBQ* coeffA = (i <= A->m) ? &A->C[i] : &zeroQ;
        NUMBQ* coeffB = (i <= B->m) ? &B->C[i] : &zeroQ;

        NUMBQ* diff = SUB_QQ_Q(coeffA, coeffB);
        if (diff == NULL) {
            /* Освобождение уже вычисленных коэффициентов */
            for (int j = 0; j < i; j++) {
                free(result->C[j].a.A);
                free(result->C[j].b.A);
            }

            free(zeroQ.a.A);
            free(zeroQ.b.A);
            free(result->C);
            free(result);
            return NULL;
        }

        /* Копирование коэффициента в результат */
        result->C[i] = *diff;
        free(diff);
    }

    /* Освобождение временного коэффициента */
    free(zeroQ.a.A);
    free(zeroQ.b.A);

    /* Удаление ведущих нулей */
    trimDegree(result);

    return result;
}