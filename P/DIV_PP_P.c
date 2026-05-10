/*
    DIV_PP_P – неполное частное от деления многочлена на многочлен.

    Выполняет деление с остатком двух многочленов, возвращает частное.
    Коэффициенты хранятся от младшей степени к старшей (C[0] – свободный член).

    Параметры:
        1) const NUMBP* A – делимое (многочлен)
        2) const NUMBP* B – делитель (ненулевой многочлен)

    Возвращает: NUMBP* – частное Q = A / B, или NULL при ошибке

    Автор: Семенов Максим
    Группа: 5388
*/

#include "../include_DM.h"

static NUMBQ* createZeroQ() {
    NUMBQ* q = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!q) return NULL;
    q->a.b = 0;
    q->a.n = 1;
    q->a.A = (int*)malloc(sizeof(int));
    if (!q->a.A) { free(q); return NULL; }
    q->a.A[0] = 0;
    q->b.n = 1;
    q->b.A = (int*)malloc(sizeof(int));
    if (!q->b.A) { free(q->a.A); free(q); return NULL; }
    q->b.A[0] = 1;
    return q;
}

static NUMBP* createZeroP() {
    NUMBP* p = (NUMBP*)malloc(sizeof(NUMBP));
    if (!p) return NULL;
    p->m = 0;
    p->C = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!p->C) { free(p); return NULL; }
    NUMBQ* zero = createZeroQ();
    if (!zero) { free(p->C); free(p); return NULL; }
    p->C[0] = *zero;
    free(zero);
    return p;
}

static NUMBP* copyP(const NUMBP* src) {
    if (!src || !src->C) return NULL;
    NUMBP* dst = (NUMBP*)malloc(sizeof(NUMBP));
    if (!dst) return NULL;
    dst->m = src->m;
    dst->C = (NUMBQ*)malloc((dst->m + 1) * sizeof(NUMBQ));
    if (!dst->C) { free(dst); return NULL; }
    for (int i = 0; i <= dst->m; i++) {
        NUMBQ srcCoef = src->C[i];
        dst->C[i].a.b = srcCoef.a.b;
        dst->C[i].a.n = srcCoef.a.n;
        dst->C[i].a.A = (int*)malloc(dst->C[i].a.n * sizeof(int));
        if (!dst->C[i].a.A) {
            for (int j = 0; j < i; j++) {
                free(dst->C[j].a.A); free(dst->C[j].b.A);
            }
            free(dst->C); free(dst);
            return NULL;
        }
        memcpy(dst->C[i].a.A, srcCoef.a.A, dst->C[i].a.n * sizeof(int));
        dst->C[i].b.n = srcCoef.b.n;
        dst->C[i].b.A = (int*)malloc(dst->C[i].b.n * sizeof(int));
        if (!dst->C[i].b.A) {
            free(dst->C[i].a.A);
            for (int j = 0; j < i; j++) {
                free(dst->C[j].a.A); free(dst->C[j].b.A);
            }
            free(dst->C); free(dst);
            return NULL;
        }
        memcpy(dst->C[i].b.A, srcCoef.b.A, dst->C[i].b.n * sizeof(int));
    }
    return dst;
}

static void freeP(NUMBP* p) {
    if (!p) return;
    for (int i = 0; i <= p->m; i++) {
        free(p->C[i].a.A);
        free(p->C[i].b.A);
    }
    free(p->C);
    free(p);
}

static void freeQ(NUMBQ* q) {
    if (!q) return;
    free(q->a.A);
    free(q->b.A);
    free(q);
}

static int numbn_to_int(NUMBN* num) {
    if (!num || num->n > 10) return -1;
    int res = 0, mult = 1;
    for (int i = 0; i < num->n; i++) {
        res += num->A[i] * mult;
        mult *= 10;
    }
    return res;
}

NUMBP* DIV_PP_P(NUMBP* A, NUMBP* B) {
    if (!A || !B) return NULL;

    NUMBN* degA = DEG_P_N(A);
    NUMBN* degB = DEG_P_N(B);
    if (!degA || !degB) {
        if (degA) { free(degA->A); free(degA); }
        if (degB) { free(degB->A); free(degB); }
        return NULL;
    }
    int a = numbn_to_int(degA);
    int b = numbn_to_int(degB);
    free(degA->A); free(degA);
    free(degB->A); free(degB);
    if (a < 0 || b < 0) return NULL;
    if (a < b) return createZeroP();

    NUMBP* R = copyP(A);
    if (!R) return NULL;
    NUMBP* Q = createZeroP();
    if (!Q) { freeP(R); return NULL; }

    while (1) {
        NUMBN* degR = DEG_P_N(R);
        NUMBN* degB2 = DEG_P_N(B);
        if (!degR || !degB2) {
            if (degR) { free(degR->A); free(degR); }
            if (degB2) { free(degB2->A); free(degB2); }
            freeP(R); freeP(Q);
            return NULL;
        }
        int r = numbn_to_int(degR);
        int b2 = numbn_to_int(degB2);
        free(degR->A); free(degR);
        free(degB2->A); free(degB2);
        if (r < b2) break;

        int k = r - b2;

        NUMBQ* ledR = LED_P_Q(R);
        NUMBQ* ledB = LED_P_Q(B);
        if (!ledR || !ledB) {
            if (ledR) freeQ(ledR);
            if (ledB) freeQ(ledB);
            freeP(R); freeP(Q);
            return NULL;
        }
        NUMBQ* q = DIV_QQ_Q(ledR, ledB);
        freeQ(ledR); freeQ(ledB);
        if (!q) { freeP(R); freeP(Q); return NULL; }

        NUMBP* T = (NUMBP*)malloc(sizeof(NUMBP));
        if (!T) { freeQ(q); freeP(R); freeP(Q); return NULL; }
        T->m = k;
        T->C = (NUMBQ*)malloc((k+1) * sizeof(NUMBQ));
        if (!T->C) { free(T); freeQ(q); freeP(R); freeP(Q); return NULL; }

        for (int i = 0; i < k; i++) {
            NUMBQ* zero = createZeroQ();
            if (!zero) {
                for (int j = 0; j < i; j++) {
                    free(T->C[j].a.A); free(T->C[j].b.A);
                }
                free(T->C); free(T); freeQ(q); freeP(R); freeP(Q);
                return NULL;
            }
            T->C[i] = *zero;
            free(zero);
        }
        T->C[k] = *q;
        free(q);

        NUMBP* B_shifted = MUL_Pxk_P(B, k);
        if (!B_shifted) {
            for (int i = 0; i <= k; i++) {
                free(T->C[i].a.A); free(T->C[i].b.A);
            }
            free(T->C); free(T); freeP(R); freeP(Q);
            return NULL;
        }
        NUMBP* term = MUL_PQ_P(B_shifted, &T->C[k]);
        if (!term) {
            freeP(B_shifted);
            for (int i = 0; i <= k; i++) {
                free(T->C[i].a.A); free(T->C[i].b.A);
            }
            free(T->C); free(T); freeP(R); freeP(Q);
            return NULL;
        }

        NUMBP* newR = SUB_PP_P(R, term);
        freeP(R);
        freeP(B_shifted);
        freeP(term);
        if (!newR) {
            for (int i = 0; i <= k; i++) {
                free(T->C[i].a.A); free(T->C[i].b.A);
            }
            free(T->C); free(T);
            freeP(Q);
            return NULL;
        }
        R = newR;

        NUMBP* newQ = ADD_PP_P(Q, T);
        for (int i = 0; i <= k; i++) {
            free(T->C[i].a.A); free(T->C[i].b.A);
        }
        free(T->C); free(T);
        if (!newQ) {
            freeP(R); freeP(Q);
            return NULL;
        }
        freeP(Q);
        Q = newQ;
    }

    freeP(R);
    return Q;
}
