#include "../include_DM.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  MUL_PQ_P (P-3)

  Умножение многочлена на рациональное число.

  Многочлен NUMBP хранится как:
    m    - степень многочлена
    C[]  - массив коэффициентов типа NUMBQ, размером m+1
    C[0] - коэффициент при x^0 (свободный член)
    C[m] - коэффициент при x^m (старший)

  Алгоритм:
    Умножаем каждый коэффициент C[i] на q через MUL_QQ_Q.
    Степень многочлена не меняется.

  Зависит от:
    - MUL_QQ_Q (Q-7) - умножение рациональных чисел

  Параметры:
    1) NUMBP* p - указатель на многочлен
    2) NUMBQ* q - указатель на рациональное число

  Возвращает NUMBP* - указатель на новый многочлен, равный p * q.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    for (int i = 0; i <= result->m; i++) {
        free(result->C[i].a.A);
        free(result->C[i].b.A);
    }
    free(result->C);
    free(result);
*/

NUMBP* MUL_PQ_P(NUMBP* p, NUMBQ* q) {
    if (p == NULL || q == NULL) {
        return NULL;
    }
    if (p->C == NULL || q->a.A == NULL || q->b.A == NULL) {
        return NULL;
    }

    NUMBP* result = (NUMBP*)malloc(sizeof(NUMBP));
    if (result == NULL) {
        return NULL;
    }

    result->m = p->m;
    result->C = (NUMBQ*)malloc((p->m + 1) * sizeof(NUMBQ));
    if (result->C == NULL) {
        free(result);
        return NULL;
    }

    for (int i = 0; i <= p->m; i++) {
        NUMBQ* coef = MUL_QQ_Q(&p->C[i], q);
        if (coef == NULL) {
            for (int j = 0; j < i; j++) {
                free(result->C[j].a.A);
                free(result->C[j].b.A);
            }
            free(result->C);
            free(result);
            return NULL;
        }
        result->C[i] = *coef;
        free(coef);
    }

    return result;
}
