#include "../include_DM.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  LED_P_Q (P-5)

  Старший коэффициент многочлена.

  Многочлен NUMBP хранится как:
    m    — степень многочлена
    C[]  — массив коэффициентов типа NUMBQ, размером m+1
    C[0] — коэффициент при x^0 (свободный член)
    C[m] — коэффициент при x^m (старший)

  Функция возвращает глубокую копию C[m], не изменяя оригинал.

  Зависимостей нет.

  Параметры:
    1) NUMBP* p — указатель на многочлен

  Возвращает NUMBQ* — указатель на копию старшего коэффициента.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->a.A);
    free(result->b.A);
    free(result);
*/

NUMBQ* LED_P_Q(NUMBP* p) {
    if (p == NULL || p->C == NULL || p->m < 0) {
        return NULL;
    }

    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (result == NULL) {
        return NULL;
    }

    result->a.b = p->C[p->m].a.b;
    result->a.n = p->C[p->m].a.n;
    result->a.A = (int*)malloc(result->a.n * sizeof(int));
    if (result->a.A == NULL) {
        free(result);
        return NULL;
    }
    memcpy(result->a.A, p->C[p->m].a.A, result->a.n * sizeof(int));

    result->b.n = p->C[p->m].b.n;
    result->b.A = (int*)malloc(result->b.n * sizeof(int));
    if (result->b.A == NULL) {
        free(result->a.A);
        free(result);
        return NULL;
    }
    memcpy(result->b.A, p->C[p->m].b.A, result->b.n * sizeof(int));

    return result;
}
