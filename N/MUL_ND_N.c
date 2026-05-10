#include "../include_DM.h"

/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  MUL_ND_N

  Умножение натурального числа на цифру.
  A[0] — младшая цифра (a_0), A[n-1] — старшая (a_{n-1}).

  Алгоритм: проходим по каждой цифре от младшей к старшей,
  умножаем на d, прибавляем перенос с предыдущего шага,
  записываем остаток от деления на 10, перенос = частное.
  Если после обхода всех цифр перенос != 0, добавляем его как
  новую старшую цифру.

  Параметры:
    1) NUMBN* a - натуральное число-множитель
    2) int d   - цифра-множитель (0..9)

  Возвращает NUMBN* — результат умножения a на d.
  При ошибке возвращает NULL:
    - d выходит за пределы [0..9]
    - a == NULL или a->A == NULL
    - malloc вернул NULL (недостаточно памяти)
*/
NUMBN* MUL_ND_N(NUMBN* a, int d) {
    if (a == NULL || a->A == NULL || a->n <= 0)
        return NULL;

    if (d < 0 || d > 9)
        return NULL;

    /* Случай умножения на 0: результат — ноль */
    if (d == 0) {
        NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
        if (!result) return NULL;
        result->A = (int*)malloc(sizeof(int));
        if (!result->A) { free(result); return NULL; }
        result->n = 1;
        result->A[0] = 0;
        return result;
    }

    /* Результирующее число: максимально n+1 цифр (из-за переноса) */
    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (!result) return NULL;

    result->n = a->n + 1;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (!result->A) { free(result); return NULL; }

    int carry = 0;

    /* Умножаем каждую цифру числа на d, учитываем перенос */
    for (int i = 0; i < a->n; i++) {
        int product = a->A[i] * d + carry;
        result->A[i] = product % 10;
        carry = product / 10;
    }

    /* Записываем оставшийся перенос в старшую позицию */
    result->A[a->n] = carry;

    /* Убираем ведущие нули, оставляя минимум одну цифру */
    while (result->n > 1 && result->A[result->n - 1] == 0)
        result->n--;

    return result;
}
