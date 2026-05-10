#include "../include_DM.h"


/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Алгоритм:
    Для многочлена с коэффициентами a_i/b_i:
      множитель = НОД(|a_0|, |a_1|, ..., |a_m|) / НОК(b_0, b_1, ..., b_m)
    Знак множителя = знак старшего ненулевого коэффициента.
*/

/* ─── Вспомогательные функции ─── */

static int isZeroN(NUMBN* a) {
    return a->n == 1 && a->A[0] == 0;
}

static NUMBN* copyNatural(NUMBN* src) {
    if (!src || !src->A || src->n <= 0) return NULL;
    NUMBN* copy = (NUMBN*)malloc(sizeof(NUMBN));
    if (!copy) return NULL;
    copy->n = src->n;
    copy->A = (int*)malloc(src->n * sizeof(int));
    if (!copy->A) {
        free(copy);
        return NULL;
    }
    for (int i = 0; i < src->n; i++) copy->A[i] = src->A[i];
    return copy;
}

/* ─── Основная функция ─── */

/*
  FAC_P_Q

  Алгоритм:
    1) НОД числителей всех коэффициентов (по абсолютной величине) через GCF_NN_N и ABS_Z_N.
    2) НОК знаменателей всех коэффициентов через LCM_NN_N.
    3) Собираем результат: числитель = НОД, знаменатель = НОК.
    4) Знак = знак старшего ненулевого коэффициента.

  Параметры:
    1) NUMBP* p - многочлен с рациональными коэффициентами

  Возвращает указатель на новое NUMBQ = рациональный множитель
*/
NUMBQ* FAC_P_Q(NUMBP* p) {
    if (!p || p->m < 0 || !p->C) return NULL;

    for (int i = 0; i <= p->m; i++) {
        if (!p->C[i].a.A || !p->C[i].b.A || p->C[i].a.n <= 0 || p->C[i].b.n <= 0) {
            return NULL;
        }
    }
    
    int firstNonZero = -1;
    for (int i = 0; i <= p->m; i++) {
        if (!isZeroN(TRANS_Z_N(&p->C[i].a))) {
            firstNonZero = i;
            break;
        }
    }
    
    if (firstNonZero == -1) {
        return NULL;
    }
    
    NUMBN* gcdNum = TRANS_Z_N(ABS_Z_Z(&p->C[firstNonZero].a));
    if (!gcdNum) return NULL;

    for (int i = firstNonZero + 1; i <= p->m; i++) {
        if (isZeroN(TRANS_Z_N(&p->C[i].a))) continue;
        
        NUMBN* absCoef = TRANS_Z_N(ABS_Z_Z(&p->C[i].a));
        if (!absCoef) {
            free(gcdNum->A); free(gcdNum);
            return NULL;
        }
        if (isZeroN(absCoef)) {
            free(absCoef->A); free(absCoef);
            continue;
        }
        NUMBN* newGcd = GCF_NN_N(gcdNum, absCoef);
        free(gcdNum->A); free(gcdNum);
        free(absCoef->A); free(absCoef);
        if (!newGcd) return NULL;
        gcdNum = newGcd;
    }

    NUMBN* lcmDen = copyNatural(&p->C[firstNonZero].b);
    if (!lcmDen) {
        free(gcdNum->A); free(gcdNum);
        return NULL;
    }

    for (int i = firstNonZero + 1; i <= p->m; i++) {
        if (isZeroN(TRANS_Z_N(&p->C[i].a))) continue;
        
        NUMBN* newLcm = LCM_NN_N(lcmDen, &p->C[i].b);
        free(lcmDen->A); free(lcmDen);
        if (!newLcm) {
            free(gcdNum->A); free(gcdNum);
            return NULL;
        }
        lcmDen = newLcm;
    }

    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!result) {
        free(gcdNum->A); free(gcdNum);
        free(lcmDen->A); free(lcmDen);
        return NULL;
    }
    
    result->b = *lcmDen;
    free(lcmDen);

    result->a.n = gcdNum->n;
    result->a.A = gcdNum->A;
    free(gcdNum);

    result->a.b = 0;
    for (int i = p->m; i >= 0; i--) {
        if (!(p->C[i].a.n == 1 && p->C[i].a.A[0] == 0)) {
            result->a.b = p->C[i].a.b;
            break;
        }
    }

    return result;
}
