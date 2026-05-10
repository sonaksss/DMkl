#include "../include_DM.h"
/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  DIV_QQ_Q (Q-8)

  Деление двух рациональных чисел (делитель отличен от нуля).

  Рациональное число NUMBQ хранится как пара:
    a - числитель (NUMBZ, целое)
    b - знаменатель (NUMBN, натуральное, всегда > 0)

  Формула:
    (p/q) / (r/s) = (p * s) / (q * r)

  Алгоритм:
    1) Проверяем что делитель b не равен нулю
    2) Числитель результата: p * s через MUL_ZZ_Z
       (s - знаменатель b, преобразуем в NUMBZ через TRANS_N_Z)
    3) Знаменатель результата: q * r через MUL_ZZ_Z, берём модуль
       (r - числитель b, знаменатель всегда натуральный)
    4) Сокращаем результат через RED_Q_Q

  Зависит от:
    - MUL_ZZ_Z (Z-8)  - умножение целых чисел
    - MUL_NN_N (N-8)  - умножение натуральных (для знаменателей)
    - TRANS_N_Z (Z-4) - преобразование натурального в целое
    - ABS_Z_N  (Z-1)  - модуль целого -> натуральное (для знаменателя)
    - RED_Q_Q  (Q-1)  - сокращение дроби

  Параметры:
    1) NUMBQ* a - указатель на делимое
    2) NUMBQ* b - указатель на делитель (b != 0)

  Возвращает NUMBQ* - указатель на частное a / b в сокращённом виде.

  ОШИБКА: возвращает NULL при невалидных входных данных,
  нулевом делителе или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->a.A);
    free(result->b.A);
    free(result);
*/

/*
  Заглушки:
NUMBZ* MUL_ZZ_Z(NUMBZ* a, NUMBZ* b)  { return a; }
NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b)  { return a; }
NUMBZ* TRANS_N_Z(NUMBN* a)           { return NULL; }
NUMBN* ABS_Z_N(NUMBZ* a)             { return NULL; }
NUMBQ* RED_Q_Q(NUMBQ* q)             { return q; }
*/

/* Вспомогательная функция: проверка числителя на ноль */
static int isZeroZ(NUMBZ* z) {
    return z->n == 1 && z->A[0] == 0;
}

NUMBQ* DIV_QQ_Q(NUMBQ* a, NUMBQ* b) {
    if (a == NULL || b == NULL || a->b.n <= 0 || b->b.n <= 0) {
        return NULL;
    }
    if (a->a.A == NULL || a->b.A == NULL ||
        b->a.A == NULL || b->b.A == NULL) {
        return NULL;
    }

    if (isZeroZ(&b->a)) {
        return NULL;
    }

    NUMBZ* s_as_z = TRANS_N_Z(&b->b);
    if (s_as_z == NULL) {
        return NULL;
    }

    NUMBZ* num = MUL_ZZ_Z(&a->a, s_as_z);
    free(s_as_z->A);
    free(s_as_z);
    if (num == NULL) {
        return NULL;
    }

    NUMBZ* r_abs = ABS_Z_Z(&b->a);
    if (r_abs == NULL) {
        free(num->A); free(num);
        return NULL;
    }

    NUMBN* den = MUL_NN_N(&a->b, TRANS_Z_N(r_abs));
    free(r_abs->A); free(r_abs);
    if (den == NULL) {
        free(num->A); free(num);
        return NULL;
    }

    if (b->a.b == 1) {
        NUMBZ* num_neg = MUL_ZM_Z(num);
        free(num->A); free(num);
        if (num_neg == NULL) {
            free(den->A); free(den);
            return NULL;
        }
        num = num_neg;
    }

    NUMBQ* raw = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (raw == NULL) {
        free(num->A); free(num);
        free(den->A); free(den);
        return NULL;
    }

    raw->a = *num;
    raw->b = *den;
    free(num);
    free(den);

    NUMBQ* result = RED_Q_Q(raw);
    free(raw->a.A);
    free(raw->b.A);
    free(raw);

    return result;
}
