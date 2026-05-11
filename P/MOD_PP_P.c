/*
    MOD_PP_P – остаток от деления многочлена на многочлен.

    Выполняет деление с остатком двух многочленов, возвращает остаток.
    Коэффициенты хранятся от младшей степени к старшей (C[0] – свободный член).

    Параметры:
        1) NUMBP* A – делимое (многочлен)
        2) NUMBP* B – делитель (ненулевой многочлен)

    Возвращает: NUMBP* – остаток R = A mod B, или NULL при ошибке

    Автор: Семенов Максим
    Группа: 5388
*/
#include "../include_DM.h"

static void freeP(NUMBP* p) {
    if (!p) return;
    for (int i = 0; i <= p->m; i++) {
        free(p->C[i].a.A);
        free(p->C[i].b.A);
    }
    free(p->C);
    free(p);
}

/*
  isZeroP

  Проверяет, является ли многочлен нулевым.

  Параметры:
    1) NUMBP* p — многочлен

  Возвращает 1 если нулевой, 0 иначе.
*/
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

/*
  createZeroP

  Создаёт нулевой многочлен степени 0 с коэффициентом 0/1.
*/
static NUMBP* createZeroP() {
    NUMBP* p = (NUMBP*)malloc(sizeof(NUMBP));
    if (!p) return NULL;
    p->m = 0;
    p->C = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!p->C) { free(p); return NULL; }
    p->C[0].a.b = 0;
    p->C[0].a.n = 1;
    p->C[0].a.A = (int*)calloc(1, sizeof(int));
    p->C[0].b.n = 1;
    p->C[0].b.A = (int*)malloc(sizeof(int));
    if (!p->C[0].a.A || !p->C[0].b.A) {
        free(p->C[0].a.A); free(p->C[0].b.A);
        free(p->C); free(p);
        return NULL;
    }
    p->C[0].b.A[0] = 1;
    return p;
}

NUMBP* MOD_PP_P(NUMBP* A, NUMBP* B) {
    if (!A || !B) return NULL;

    if (isZeroP(B)) return NULL;

    NUMBP* Q = DIV_PP_P(A, B);
    if (!Q) return NULL;
    if (isZeroP(Q)) {
        freeP(Q);
        NUMBP* copy = (NUMBP*)malloc(sizeof(NUMBP));
        if (!copy) return NULL;
        copy->m = A->m;
        copy->C = (NUMBQ*)malloc((A->m + 1) * sizeof(NUMBQ));
        if (!copy->C) { free(copy); return NULL; }
        for (int i = 0; i <= A->m; i++) {
            copy->C[i].a.b = A->C[i].a.b;
            copy->C[i].a.n = A->C[i].a.n;
            copy->C[i].a.A = (int*)malloc(A->C[i].a.n * sizeof(int));
            copy->C[i].b.n = A->C[i].b.n;
            copy->C[i].b.A = (int*)malloc(A->C[i].b.n * sizeof(int));
            if (!copy->C[i].a.A || !copy->C[i].b.A) {
                for (int j = 0; j < i; j++) {
                    free(copy->C[j].a.A); free(copy->C[j].b.A);
                }
                free(copy->C[i].a.A); free(copy->C[i].b.A);
                free(copy->C); free(copy);
                return NULL;
            }
            memcpy(copy->C[i].a.A, A->C[i].a.A, A->C[i].a.n * sizeof(int));
            memcpy(copy->C[i].b.A, A->C[i].b.A, A->C[i].b.n * sizeof(int));
        }
        return copy;
    }

    NUMBP* S = MUL_PP_P(Q, B);
    if (!S) { freeP(Q); return NULL; }

    NUMBP* R = SUB_PP_P(A, S);

    freeP(Q);
    freeP(S);

    if (R && isZeroP(R)) {
        freeP(R);
        return createZeroP();
    }

    return R;
}
