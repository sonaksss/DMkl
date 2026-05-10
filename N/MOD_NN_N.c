#include "../include_DM.h"

/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  copyNumbn

  Создаёт полную копию натурального числа.

  Параметры:
    1) NUMBN* src — исходное число

  Возвращает NUMBN* — копию числа, или NULL при ошибке.
*/
static NUMBN* copyNumbn(NUMBN* src) {
    if (!src || !src->A || src->n <= 0) return NULL;
    NUMBN* copy = (NUMBN*)malloc(sizeof(NUMBN));
    if (!copy) return NULL;
    copy->n = src->n;
    copy->A = (int*)malloc(src->n * sizeof(int));
    if (!copy->A) { free(copy); return NULL; }
    for (int i = 0; i < src->n; i++) copy->A[i] = src->A[i];
    return copy;
}

/*
  freeNumbn

  Освобождает память, занятую структурой NUMBN.

  Параметры:
    1) NUMBN* num — число для освобождения
*/
static void freeNumbn(NUMBN* num) {
    if (!num) return;
    free(num->A);
    free(num);
}

/*
  MOD_NN_N

  Остаток от деления первого натурального числа на второе.
  Делитель отличен от нуля.

  Алгоритм:
    1) Проверить корректность входных данных.
    2) Вычислить неполное частное q = DIV_NN_N(a, b).
    3) Вычислить остаток r = a - q * b через цикл по цифрам частного:
         для каждой позиции i (от 0 до q->n - 1):
           если цифра q->A[i] != 0:
             создать b_shifted = b * 10^i (сдвиг вставкой нулей в A[0..i-1]),
             вычесть из текущего остатка: r = SUB_NDN_N(r, b_shifted, q->A[i]).
    4) Освободить промежуточные ресурсы и вернуть r.

  Параметры:
    1) NUMBN* a — делимое
    2) NUMBN* b — делитель (не равен нулю)

  Возвращает NUMBN* — остаток от деления a на b.
  В случае ошибки возвращает NULL.
*/
NUMBN* MOD_NN_N(NUMBN* a, NUMBN* b) {
    /* Шаг 1: проверка входных данных */
    if (!a || !a->A || a->n <= 0) return NULL;
    if (!b || !b->A || b->n <= 0) return NULL;

    /* Шаг 2: вычислить неполное частное q = a / b */
    NUMBN* q = DIV_NN_N(a, b);
    if (!q) return NULL;

    /* Начальное значение остатка r = копия a */
    NUMBN* r = copyNumbn(a);
    if (!r) {
        freeNumbn(q);
        return NULL;
    }

    /*
      Шаг 3: вычесть из r каждое слагаемое q[i] * b * 10^i.
      A[0] — самая младшая цифра (позиция 0),
      A[n-1] — самая старшая.
      Для сдвига b на i позиций создаём b_shifted с i нулями в начале массива.
    */
    for (int i = 0; i < q->n; i++) {
        int digit = q->A[i];
        if (digit == 0) continue; /* нет вклада — пропустить */

        /* Создать b_shifted = b * 10^i: скопировать b и добавить i нулей снизу */
        NUMBN* b_shifted = (NUMBN*)malloc(sizeof(NUMBN));
        if (!b_shifted) {
            freeNumbn(q);
            freeNumbn(r);
            return NULL;
        }
        b_shifted->n = b->n + i;
        b_shifted->A = (int*)malloc(b_shifted->n * sizeof(int));
        if (!b_shifted->A) {
            free(b_shifted);
            freeNumbn(q);
            freeNumbn(r);
            return NULL;
        }

        /* Заполнить нулями младшие i позиций (сдвиг) */
        for (int j = 0; j < i; j++) b_shifted->A[j] = 0;
        /* Скопировать цифры b в старшие позиции */
        for (int j = 0; j < b->n; j++) b_shifted->A[i + j] = b->A[j];

        /* r = r - b_shifted * digit */
        NUMBN* r_new = SUB_NDN_N(r, b_shifted, digit);
        freeNumbn(b_shifted);

        if (!r_new) {
            freeNumbn(q);
            freeNumbn(r);
            return NULL;
        }

        freeNumbn(r);
        r = r_new;
    }

    /* Шаг 4: освободить частное и вернуть остаток */
    freeNumbn(q);
    return r;
}
