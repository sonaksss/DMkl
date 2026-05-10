#include "../include_DM.h"
/*
Автор: Еремченко Пётр
Группа: 5388
*/
static NUMBQ* zero_q(void) {
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
}

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

/*
Умножение многочлена на x^k.
  
Параметры:
    - NUMBP* p — многочлен
    - int k — степень x (натуральное или 0)

Возвращает NUMBP* — многочлен, умноженный на x^k.
В случае ошибки возвращает NULL.
*/
NUMBP* MUL_Pxk_P(NUMBP* p, int k) {
    if (!p || !p->C || p->m < 0 || k < 0) return NULL;
    
    NUMBP* result = (NUMBP*)malloc(sizeof(NUMBP));
    if (!result) return NULL;
    
    result->m = p->m + k;
    result->C = (NUMBQ*)malloc((result->m + 1) * sizeof(NUMBQ));
    if (!result->C) {
        free(result);
        return NULL;
    }
    
    for (int i = 0; i < k; i++) {
        NUMBQ* z = zero_q();
        if (!z) {
            for (int j = 0; j < i; j++) {
                free_q(&result->C[j]);
            }
            free(result->C);
            free(result);
            return NULL;
        }
        result->C[i] = *z;
        free(z);
    }
    
    for (int i = 0; i <= p->m; i++) {
        NUMBQ* copy = copy_q(&(p->C[i]));
        if (!copy) {
            for (int j = 0; j < i + k; j++) {
                free_q(&result->C[j]);
            }
            free(result->C);
            free(result);
            return NULL;
        }
        result->C[i + k] = *copy;
        free(copy);
    }
    
    return result;
}
