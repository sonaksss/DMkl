#include "../include_DM.h"

/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  isZeroPolynomial

  Проверяет, является ли многочлен нулевым.

  Параметры:
    1) NUMBP* p — указатель на многочлен

  Возвращаемое значение:
    1 — многочлен равен нулю
    0 — многочлен не равен нулю
*/


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

static int isZeroP(NUMBP* p) {
    if (!p || !p->C) return 1;
    for (int i = 0; i <= p->m; i++) {
        if (!p->C[i].a.A) continue;
        for (int j = 0; j < p->C[i].a.n; j++) {
            if (p->C[i].a.A[j] != 0) return 0;
        }
    }
    return 1;
}

NUMBP* NMR_P_P(NUMBP* p) {
    if (!p) return NULL;

    NUMBP* derivative = DER_P_P(p);
    if (!derivative) return NULL;

    if (isZeroP(derivative)) {
        freeP(derivative);
        return copyP(p);
    }

    NUMBP* gcd = GCF_PP_P(p, derivative);
    freeP(derivative);
    if (!gcd) return NULL;

    if (isZeroP(gcd)) {
        freeP(gcd);
        return NULL;
    }

    if (gcd->m == 0) {
        freeP(gcd);
        return copyP(p);
    }

    NUMBP* result = DIV_PP_P(p, gcd);
    freeP(gcd);

    return result;
}
