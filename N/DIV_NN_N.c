/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "../include_DM.h"

static NUMBN* copyN(const NUMBN* src) {
    if (!src) return NULL;
    NUMBN* dst = (NUMBN*)malloc(sizeof(NUMBN));
    if (!dst) return NULL;
    dst->n = src->n;
    dst->A = (int*)malloc(dst->n * sizeof(int));
    if (!dst->A) { free(dst); return NULL; }
    for (int i = 0; i < src->n; i++) dst->A[i] = src->A[i];
    return dst;
}

NUMBN* DIV_NN_N(NUMBN* A, NUMBN* B) {
    if (!A || !B) return NULL;

    NUMBN* zero = (NUMBN*)malloc(sizeof(NUMBN));
    if (!zero) return NULL;
    zero->n = 1;
    zero->A = (int*)calloc(1, sizeof(int));
    if (!zero->A) { free(zero); return NULL; }
    zero->A[0] = 0;

    int cmp = COM_NN_D(A, B);
    if (cmp == -1) {
        free(zero->A); free(zero);
        return NULL;
    }
    if (cmp == 1) return zero;

    NUMBN* R = copyN(A);
    if (!R) {
        free(zero->A); free(zero);
        return NULL;
    }
    NUMBN* Q = copyN(zero);
    if (!Q) {
        free(R->A); free(R);
        free(zero->A); free(zero);
        return NULL;
    }

    while (COM_NN_D(R, B) != 1) {
        int k = R->n - B->n;
        NUMBN* Bk = MUL_Nk_N(B, k);
        if (!Bk) {
            free(R->A); free(R);
            free(Q->A); free(Q);
            free(zero->A); free(zero);
            return NULL;
        }
        while (k >= 0 && COM_NN_D(R, Bk) == 1) {
            int cmpBk = COM_NN_D(R, Bk);
            if (cmpBk == -1) {
                free(Bk->A); free(Bk);
                free(R->A); free(R);
                free(Q->A); free(Q);
                free(zero->A); free(zero);
                return NULL;
            }
            free(Bk->A); free(Bk);
            k--;
            if (k >= 0) {
                Bk = MUL_Nk_N(B, k);
                if (!Bk) {
                    free(R->A); free(R);
                    free(Q->A); free(Q);
                    free(zero->A); free(zero);
                    return NULL;
                }
            } else {
                Bk = NULL;
                break;
            }
        }
        if (k < 0 || !Bk) {
            if (Bk) { free(Bk->A); free(Bk); }
            break;
        }

        int q = DIV_NN_Dk(R, B);
        if (q == 0) {
            free(Bk->A); free(Bk);
            free(R->A); free(R);
            free(Q->A); free(Q);
            free(zero->A); free(zero);
            return NULL;
        }
        free(Bk->A); free(Bk);

        NUMBN* shiftedB = MUL_Nk_N(B, k);
        if (!shiftedB) {
            free(R->A); free(R);
            free(Q->A); free(Q);
            free(zero->A); free(zero);
            return NULL;
        }
        NUMBN* newR = SUB_NDN_N(R, shiftedB, q);
        if (!newR) {
            free(shiftedB->A); free(shiftedB);
            free(R->A); free(R);
            free(Q->A); free(Q);
            free(zero->A); free(zero);
            return NULL;
        }
        free(R->A); free(R);
        free(shiftedB->A); free(shiftedB);
        R = newR;

        NUMBN* qNum = (NUMBN*)malloc(sizeof(NUMBN));
        if (!qNum) {
            free(R->A); free(R);
            free(Q->A); free(Q);
            free(zero->A); free(zero);
            return NULL;
        }
        qNum->n = 1;
        qNum->A = (int*)malloc(sizeof(int));
        if (!qNum->A) {
            free(qNum);
            free(R->A); free(R);
            free(Q->A); free(Q);
            free(zero->A); free(zero);
            return NULL;
        }
        qNum->A[0] = q;

        NUMBN* addTerm = MUL_Nk_N(qNum, k);
        free(qNum->A); free(qNum);
        if (!addTerm) {
            free(R->A); free(R);
            free(Q->A); free(Q);
            free(zero->A); free(zero);
            return NULL;
        }

        NUMBN* newQ = ADD_NN_N(Q, addTerm);
        free(Q->A); free(Q);
        free(addTerm->A); free(addTerm);
        if (!newQ) {
            free(R->A); free(R);
            free(zero->A); free(zero);
            return NULL;
        }
        Q = newQ;
    }

    free(R->A); free(R);
    free(zero->A); free(zero);
    return Q;
}
