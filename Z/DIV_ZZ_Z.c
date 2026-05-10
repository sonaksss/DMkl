#include "../include_DM.h"

/*
  DIV_ZZ_Z

  Частное от деления целого на целое (делитель отличен от нуля).
  Реализует математическое целочисленное деление:
    q = floor(a / b),  остаток всегда >= 0 и < |b|.

  Параметры:
    1) NUMBZ* a – делимое
    2) NUMBZ* b – делитель (не ноль)

  Возвращает NUMBZ* – частное q, или NULL при ошибке.
*/

static int isZeroZ(const NUMBZ* z) {
    return z->n == 1 && z->A[0] == 0;
}

NUMBZ* DIV_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    if (!a || !b || !a->A || !b->A || a->n <= 0 || b->n <= 0) return NULL;
    if (isZeroZ(b)) return NULL;

    if (isZeroZ(a)) {
        NUMBZ* zero = (NUMBZ*)malloc(sizeof(NUMBZ));
        if (!zero) return NULL;
        zero->b = 0;
        zero->n = 1;
        zero->A = (int*)calloc(1, sizeof(int));
        if (!zero->A) { free(zero); return NULL; }
        return zero;
    }

    NUMBZ* absA_z = ABS_Z_Z(a);
    if (!absA_z) return NULL;
    NUMBN* natA = TRANS_Z_N(absA_z);
    free(absA_z->A); free(absA_z);
    if (!natA) return NULL;

    NUMBZ* absB_z = ABS_Z_Z(b);
    if (!absB_z) { free(natA->A); free(natA); return NULL; }
    NUMBN* natB = TRANS_Z_N(absB_z);
    free(absB_z->A); free(absB_z);
    if (!natB) { free(natA->A); free(natA); return NULL; }

    NUMBN* natQ = DIV_NN_N(natA, natB);
    NUMBN* natR = MOD_NN_N(natA, natB);
    free(natA->A); free(natA);

    if (!natQ || !natR) {
        if (natQ) { free(natQ->A); free(natQ); }
        if (natR) { free(natR->A); free(natR); }
        free(natB->A); free(natB);
        return NULL;
    }

    int r_not_zero = !(natR->n == 1 && natR->A[0] == 0);
    free(natR->A); free(natR);

    NUMBZ* Q = NULL;
    if (a->b == 0) {
        Q = (NUMBZ*)malloc(sizeof(NUMBZ));
        if (!Q) { free(natQ->A); free(natQ); free(natB->A); free(natB); return NULL; }
        Q->b = 0;
        Q->n = natQ->n;
        Q->A = natQ->A;
        free(natQ);
    } else {
        NUMBN* tempQ = natQ;
        if (r_not_zero) {
            NUMBN* incremented = ADD_1N_N(natQ);
            free(natQ->A); free(natQ);
            tempQ = incremented;
            if (!tempQ) { free(natB->A); free(natB); return NULL; }
        }
        Q = (NUMBZ*)malloc(sizeof(NUMBZ));
        if (!Q) { free(tempQ->A); free(tempQ); free(natB->A); free(natB); return NULL; }
        Q->b = 1;
        Q->n = tempQ->n;
        Q->A = tempQ->A;
        free(tempQ);
    }

    free(natB->A); free(natB);

    NUMBZ* result = NULL;
    if (b->b == 0) {
        result = Q;
    } else {
        result = MUL_ZM_Z(Q);
        free(Q->A); free(Q);
    }

    return result;
}
