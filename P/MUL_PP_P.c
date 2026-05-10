#include "../include_DM.h"

/*
    Автор: Семенов Максим
    Группа: 5388
*/

/*
    MUL_PP_P – умножение многочленов с рациональными коэффициентами.

    Параметры:
        1) const NUMBP* A – первый многочлен
        2) const NUMBP* B – второй многочлен

    Возвращает:
        NUMBP* – произведение A * B
        NULL – при ошибке
*/

/*
    createZeroQ_mul

    Создаёт рациональное число 0/1.
*/
static NUMBQ* createZeroQ_mul(void) {
    NUMBQ* q = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!q)
        return NULL;

    q->a.b = 0;
    q->a.n = 1;
    q->a.A = (int*)calloc(1, sizeof(int));
    if (!q->a.A) {
        free(q);
        return NULL;
    }

    q->b.n = 1;
    q->b.A = (int*)malloc(sizeof(int));
    if (!q->b.A) {
        free(q->a.A);
        free(q);
        return NULL;
    }

    q->b.A[0] = 1;
    return q;
}

/*
    freeQ_mul

    Освобождает память, выделенную под рациональное число.
*/
static void freeQ_mul(NUMBQ* q) {
    if (!q)
        return;

    free(q->a.A);
    free(q->b.A);
    free(q);
}

/*
    MUL_PP_P

    Вычисляет произведение двух многочленов.

    Алгоритм:
        1. Создаётся массив коэффициентов результата и заполняется нулями.
        2. Для каждой пары коэффициентов A[i] и B[j]
           вычисляется произведение и прибавляется к result[i + j].

    Параметры:
        1) const NUMBP* A – первый многочлен
        2) const NUMBP* B – второй многочлен

    Возвращает:
        NUMBP* – произведение A * B
        NULL – при ошибке
*/
NUMBP* MUL_PP_P(const NUMBP* A, const NUMBP* B) {
    if (!A || !B || !A->C || !B->C || A->m < 0 || B->m < 0)
        return NULL;

    NUMBP* result = (NUMBP*)malloc(sizeof(NUMBP));
    if (!result)
        return NULL;

    result->m = A->m + B->m;
    result->C = (NUMBQ*)malloc((result->m + 1) * sizeof(NUMBQ));
    if (!result->C) {
        free(result);
        return NULL;
    }

    for (int i = 0; i <= result->m; i++) {
        NUMBQ* zero = createZeroQ_mul();
        if (!zero) {
            for (int j = 0; j < i; j++) {
                free(result->C[j].a.A);
                free(result->C[j].b.A);
            }
            free(result->C);
            free(result);
            return NULL;
        }

        result->C[i] = *zero;
        free(zero);
    }

    for (int i = 0; i <= A->m; i++) {
        for (int j = 0; j <= B->m; j++) {
            NUMBQ* term = MUL_QQ_Q(&A->C[i], &B->C[j]);
            if (!term) {
                for (int k = 0; k <= result->m; k++) {
                    free(result->C[k].a.A);
                    free(result->C[k].b.A);
                }
                free(result->C);
                free(result);
                return NULL;
            }

            NUMBQ* sum = ADD_QQ_Q(&result->C[i + j], term);
            freeQ_mul(term);

            if (!sum) {
                for (int k = 0; k <= result->m; k++) {
                    free(result->C[k].a.A);
                    free(result->C[k].b.A);
                }
                free(result->C);
                free(result);
                return NULL;
            }

            free(result->C[i + j].a.A);
            free(result->C[i + j].b.A);

            result->C[i + j] = *sum;
            free(sum);
        }
    }

    return result;
}