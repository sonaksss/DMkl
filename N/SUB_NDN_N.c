#include "../include_DM.h"

/*
  Автор: Ефанов Денис
  Группа: 5388

  SUB_NDN_N

  Вычитание из натурального другого натурального, умноженного на цифру,
  для случая с неотрицательным результатом: result = a - b * d.

  Предусловие: a >= b * d.
  Цифра d должна быть в диапазоне 0..9.

  Параметры:
    1) NUMBN* a — уменьшаемое
    2) NUMBN* b — натуральное число, которое умножается на d
    3) int d    — цифра-множитель (0..9)

  Возвращает NUMBN* — результат a - b*d.
  В случае ошибки возвращает NULL.
*/
NUMBN* SUB_NDN_N(NUMBN* a, NUMBN* b, int d) {
    if (a == NULL || a->A == NULL || a->n <= 0) return NULL;
    if (b == NULL || b->A == NULL|| b->n <= 0) return NULL;
    if (d < 0 || d > 9) return NULL;

    if (d == 0) {
        NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
        if (!result) return NULL;
        result->n = a->n;
        result->A = (int*)malloc(a->n * sizeof(int));
        if (!result->A) { free(result); return NULL; }
        for (int i = 0; i < a->n; i++) result->A[i] = a->A[i];
        return result;
    }

    NUMBN* bd = MUL_ND_N(b, d);
    if (bd == NULL) return NULL;
    
    int cmp = COM_NN_D(a, bd);

    if (cmp == -1 || cmp == 1) {
        free(bd->A);
        free(bd);
        return NULL;
    }

    NUMBN* result = SUB_NN_N(a, bd);
    free(bd->A);
    free(bd);
    
    if (result == NULL) return NULL;
    return result;
}
