/*
  Автор: Семенов Максим
  Группа: 5388
*/
/*
    MOD_PP_P – остаток от деления многочлена на многочлен.

    Выполняет деление с остатком двух многочленов, возвращает остаток.
    Коэффициенты хранятся от младшей степени к старшей (C[0] – свободный член).

    Параметры:
        1)NUMBP* A – делимое (многочлен)
        2)NUMBP* B – делитель (ненулевой многочлен)

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

NUMBP* MOD_PP_P(NUMBP* A, NUMBP* B) {
    if (!A || !B) return NULL;

    NUMBP* Q = DIV_PP_P(A, B);
    if (!Q) return NULL;

    NUMBP* S = MUL_PP_P(Q, B);
    if (!S) { freeP(Q); return NULL; }

    NUMBP* R = SUB_PP_P(A, S);
    
    freeP(Q);
    freeP(S);
    return R;
}
