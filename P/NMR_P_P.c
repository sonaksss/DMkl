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

static void freeP(NUMBP* p) {
    if (!p) return;
    for (int i = 0; i <= p->m; i++) {
        free(p->C[i].a.A);
        free(p->C[i].b.A);
    }
    free(p->C);
    free(p);
}

static int isZeroPolynomial(NUMBP* p) {
    if (!p || !p->C)
        return 1;

    for (int i = 0; i <= p->m; i++) {
        if (!p->C[i].a.A)
            continue;

        for (int j = 0; j < p->C[i].a.n; j++) {
            if (p->C[i].a.A[j] != 0)
                return 0;
        }
    }

    return 1;
}

/*
  deepCopyPolynomial

  Создаёт глубокую копию многочлена.

  Параметры:
    1) NUMBP* p — исходный многочлен

  Возвращаемое значение:
    NUMBP* — копия многочлена
    NULL — при ошибке выделения памяти
*/
static NUMBP* deepCopyPolynomial(NUMBP* p) {
    if (!p || !p->C)
        return NULL;

    NUMBP* copy = (NUMBP*)malloc(sizeof(NUMBP));
    if (!copy)
        return NULL;

    copy->m = p->m;
    copy->C = (NUMBQ*)malloc((p->m + 1) * sizeof(NUMBQ));
    if (!copy->C) {
        free(copy);
        return NULL;
    }

    for (int i = 0; i <= p->m; i++) {
        /* Копирование числителя */
        copy->C[i].a.b = p->C[i].a.b;
        copy->C[i].a.n = p->C[i].a.n;
        copy->C[i].a.A = (int*)malloc(p->C[i].a.n * sizeof(int));
        if (!copy->C[i].a.A) {
            for (int j = 0; j < i; j++) {
                free(copy->C[j].a.A);
                free(copy->C[j].b.A);
            }
            free(copy->C);
            free(copy);
            return NULL;
        }
        memcpy(copy->C[i].a.A, p->C[i].a.A,
               p->C[i].a.n * sizeof(int));

        /* Копирование знаменателя */
        copy->C[i].b.n = p->C[i].b.n;
        copy->C[i].b.A = (int*)malloc(p->C[i].b.n * sizeof(int));
        if (!copy->C[i].b.A) {
            free(copy->C[i].a.A);
            for (int j = 0; j < i; j++) {
                free(copy->C[j].a.A);
                free(copy->C[j].b.A);
            }
            free(copy->C);
            free(copy);
            return NULL;
        }
        memcpy(copy->C[i].b.A, p->C[i].b.A,
               p->C[i].b.n * sizeof(int));
    }

    return copy;
}

/*
  NMR_P_P

  Преобразует многочлен, заменяя кратные корни простыми.

  Алгоритм:
    1. Вычисляется производная P'.
    2. Находится НОД(P, P').
    3. Вычисляется результат P / НОД(P, P').

  Параметры:
    1) NUMBP* p — исходный многочлен

  Возвращаемое значение:
    NUMBP* — преобразованный многочлен
    NULL — при ошибке
*/
NUMBP* NMR_P_P(NUMBP* p) {
    if (!p)
        return NULL;

    /* Вычисляем производную */
    NUMBP* derivative = DER_P_P(p);
    if (!derivative)
        return NULL;

    /* Если производная нулевая, возвращаем копию исходного многочлена */
    if (isZeroPolynomial(derivative)) {
        freeP(derivative);
        return deepCopyPolynomial(p);
    }

    /* Вычисляем НОД(P, P') */
    NUMBP* gcd = GCF_PP_P(p, derivative);
    freeP(derivative);

    if (!gcd)
        return NULL;

    /* Нулевой НОД означает ошибочную ситуацию */
    if (isZeroPolynomial(gcd)) {
        freeP(gcd);
        return NULL;
    }

    /* Если НОД — константа, кратных корней нет */
    if (gcd->m == 0) {
        freeP(gcd);
        return deepCopyPolynomial(p);
    }

    /* Делим P на НОД(P, P') */
    NUMBP* result = DIV_PP_P(p, gcd);
    freeP(gcd);

    return result;
}
