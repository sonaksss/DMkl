#include "../include_DM.h"

/*
  Автор: Еремченко Пётр
  Группа: 5388
*/

/*
  is_zero_p

  Проверяет, является ли многочлен нулевым.

  Параметры:
    1) NUMBP* p — указатель на многочлен

  Возвращаемое значение:
    1 — многочлен равен нулю
    0 — многочлен не равен нулю
*/


static int isZeroPolynomial(NUMBP* p) {
    if (!p || !p->C) return 1;
    for (int i = 0; i <= p->m; i++) {
        for (int j = 0; j < p->C[i].a.n; j++) {
            if (p->C[i].a.A[j] != 0) return 0;
        }
    }
    return 1;
}

static void freeQ(NUMBQ* q) {
    if (!q) return;
    free(q->a.A);
    free(q->b.A);
    free(q);
}

static NUMBQ* createOneQ() {
    NUMBQ* q = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!q) return NULL;
    q->a.b = 0; q->a.n = 1;
    q->a.A = (int*)malloc(sizeof(int));
    if (!q->a.A) { free(q); return NULL; }
    q->a.A[0] = 1;
    q->b.n = 1;
    q->b.A = (int*)malloc(sizeof(int));
    if (!q->b.A) { free(q->a.A); free(q); return NULL; }
    q->b.A[0] = 1;
    return q;
}

static NUMBP* copyPolynomial(NUMBP* src) {
    if (!src || !src->C) return NULL;
    NUMBP* dst = (NUMBP*)malloc(sizeof(NUMBP));
    if (!dst) return NULL;
    dst->m = src->m;
    dst->C = (NUMBQ*)malloc((dst->m + 1) * sizeof(NUMBQ));
    if (!dst->C) { free(dst); return NULL; }
    for (int i = 0; i <= src->m; i++) {
        NUMBQ* sc = &src->C[i];
        dst->C[i].a.b = sc->a.b;
        dst->C[i].a.n = sc->a.n;
        dst->C[i].a.A = (int*)malloc(sc->a.n * sizeof(int));
        dst->C[i].b.n = sc->b.n;
        dst->C[i].b.A = (int*)malloc(sc->b.n * sizeof(int));
        if (!dst->C[i].a.A || !dst->C[i].b.A) {
            for (int j = 0; j < i; j++) { free(dst->C[j].a.A); free(dst->C[j].b.A); }
            free(dst->C[i].a.A); free(dst->C[i].b.A);
            free(dst->C); free(dst);
            return NULL;
        }
        memcpy(dst->C[i].a.A, sc->a.A, sc->a.n * sizeof(int));
        memcpy(dst->C[i].b.A, sc->b.A, sc->b.n * sizeof(int));
    }
    return dst;
}

static void freePolynomial(NUMBP* p) {
    if (!p) return;
    for (int i = 0; i <= p->m; i++) {
        free(p->C[i].a.A);
        free(p->C[i].b.A);
    }
    free(p->C);
    free(p);
}

NUMBP* GCF_PP_P(NUMBP* a, NUMBP* b) {
    if (!a || !b || !a->C || !b->C) return NULL;

    NUMBP* A = copyPolynomial(a);
    NUMBP* B = copyPolynomial(b);
    if (!A || !B) {
        if (A) freePolynomial(A);
        if (B) freePolynomial(B);
        return NULL;
    }

    while (!isZeroPolynomial(B)) {
        NUMBP* R = MOD_PP_P(A, B);
        if (!R) {
            freePolynomial(A);
            freePolynomial(B);
            return NULL;
        }
        freePolynomial(A);
        A = B;
        B = R;
    }

    if (isZeroPolynomial(A)) {
        freePolynomial(A);
        freePolynomial(B);
        NUMBP* one = (NUMBP*)malloc(sizeof(NUMBP));
        one->m = 0;
        one->C = (NUMBQ*)malloc(sizeof(NUMBQ));
        NUMBQ* oneQ = createOneQ();
        if (!oneQ) { free(one->C); free(one); return NULL; }
        one->C[0] = *oneQ;
        free(oneQ);
        return one;
    }

    NUMBQ* lead = LED_P_Q(A);
    if (!lead) {
        freePolynomial(A);
        freePolynomial(B);
        return NULL;
    }

    int lead_is_one = (lead->a.n == 1 && lead->a.A[0] == 1 &&
                       lead->b.n == 1 && lead->b.A[0] == 1 && lead->a.b == 0);

    if (!lead_is_one) {
        NUMBQ* oneQ = createOneQ();
        if (!oneQ) { freeQ(lead); freePolynomial(A); freePolynomial(B); return NULL; }
        NUMBQ* factor = DIV_QQ_Q(oneQ, lead);
        freeQ(oneQ);
        freeQ(lead);
        if (!factor) { freePolynomial(A); freePolynomial(B); return NULL; }

        NUMBP* scaled = MUL_PQ_P(A, factor);
        freeQ(factor);
        if (!scaled) { freePolynomial(A); freePolynomial(B); return NULL; }

        freePolynomial(A);
        A = scaled;

        lead = LED_P_Q(A);
        if (lead && lead->a.b == 1) {
            NUMBQ* minusOne = createOneQ();
            minusOne->a.b = 1;
            NUMBP* final = MUL_PQ_P(A, minusOne);
            freeQ(minusOne);
            freeQ(lead);
            if (final) {
                freePolynomial(A);
                A = final;
            }
        } else if (lead) {
            freeQ(lead);
        }
    } else {
        freeQ(lead);
    }

    freePolynomial(B);
    return A;
}
