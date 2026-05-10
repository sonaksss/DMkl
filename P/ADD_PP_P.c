#include "../include_DM.h"

/*
Автор: Еремченко Пётр
Группа: 5388
*/
/*
Сложение двух многочленов.

Принимает 2 многочлена NUMBP*

Возвращает NUMBP* — сумма многочленов.
В случае ошибки возвращает NULL.
*/
static int is_zero_q(NUMBQ* q) {
    if (!q || !q->a.A || !q->b.A) return 1;
    return POZ_Z_D(&q->a) == 0;
}

/*static NUMBQ* zero_q(void) {
    NUMBQ* zero = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!zero) return NULL;
    
    zero->b.n = 1;
    zero->b.A = (int*)malloc(sizeof(int));
    if (!zero->b.A) {
        free(zero);
        return NULL;
    }
    zero->b.A[0] = 1;
    
    zero->a.b = 0;
    zero->a.n = 1;
    zero->a.A = (int*)malloc(sizeof(int));
    if (!zero->a.A) {
        free(zero->b.A);
        free(zero);
        return NULL;
    }
    zero->a.A[0] = 0;
    
    return zero;
}*/

static NUMBQ* copy_q(NUMBQ* src) {
    if (!src) return NULL;
    
    NUMBQ* dst = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!dst) return NULL;
    
    dst->a.b = src->a.b;
    dst->a.n = src->a.n;
    dst->a.A = (int*)malloc(src->a.n * sizeof(int));
    if (!dst->a.A) {
        free(dst);
        return NULL;
    }
    for (int i = 0; i < src->a.n; i++) dst->a.A[i] = src->a.A[i];
    
    dst->b.n = src->b.n;
    dst->b.A = (int*)malloc(src->b.n * sizeof(int));
    if (!dst->b.A) {
        free(dst->a.A);
        free(dst);
        return NULL;
    }
    for (int i = 0; i < src->b.n; i++) dst->b.A[i] = src->b.A[i];
    
    return dst;
}

static void free_q(NUMBQ* q) {
    if (!q) return;
    free(q->a.A);
    free(q->b.A);
}

NUMBP* ADD_PP_P(NUMBP* p1, NUMBP* p2) {
    if (!p1 || !p2 || !p1->C || !p2->C || p1->m < 0 || p2->m < 0) return NULL;

    NUMBP* longer = (p1->m >= p2->m) ? p1 : p2;
    NUMBP* shorter = (p1->m >= p2->m) ? p2 : p1;
    
    NUMBP* result = (NUMBP*)malloc(sizeof(NUMBP));
    if (!result) return NULL;
    
    result->m = longer->m;
    result->C = (NUMBQ*)malloc((result->m + 1) * sizeof(NUMBQ));
    if (!result->C) {
        free(result);
        return NULL;
    }
    
    for (int i = 0; i <= result->m; i++) {
        result->C[i].a.A = NULL;
        result->C[i].b.A = NULL;
    }
    
    for (int i = 0; i <= shorter->m; i++) {
        NUMBQ* sum = ADD_QQ_Q(&(longer->C[i]), &(shorter->C[i]));
        if (!sum) {
            for (int j = 0; j < i; j++) {
                free_q(&result->C[j]);
            }
            free(result->C);
            free(result);
            return NULL;
        }
        result->C[i] = *sum;
        free(sum);
    }
    
    for (int i = shorter->m + 1; i <= longer->m; i++) {
        NUMBQ* copy = copy_q(&(longer->C[i]));
        if (!copy) {
            for (int j = 0; j < i; j++) {
                free_q(&result->C[j]);
            }
            free(result->C);
            free(result);
            return NULL;
        }
        result->C[i] = *copy;
        free(copy);
    }
    
    while (result->m > 0 && is_zero_q(&(result->C[result->m]))) {
        free_q(&(result->C[result->m]));
        result->m--;
    }
    
    return result;
}
