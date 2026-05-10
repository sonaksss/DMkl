#include "../include_DM.h"


/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  TRANS_Z_N

  Преобразование целого неотрицательного числа в натуральное.
  Копирует массив цифр из NUMBZ в новую структуру NUMBN.

  Параметры:
    1) NUMBZ* z - указатель на целое неотрицательное число

  Возвращает NUMBN* — указатель на натуральное число,
  или NULL при ошибке:
    - z == NULL
    - число отрицательное (b == 1)
    - z->A == NULL или z->n <= 0
    - ошибка выделения памяти
*/
NUMBN* TRANS_Z_N(NUMBZ* z) {
    if (z == NULL) {
        return NULL;
    }

    if (z->b == 1) {
        return NULL;
    }

    if (z->A == NULL || z->n <= 0) {
        return NULL;
    }

    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (result == NULL) {
        return NULL;
    }

    result->n = z->n;
    result->A = (int*)malloc(z->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }

    for (int i = 0; i < z->n; i++) {
        result->A[i] = z->A[i];
    }

    return result;
}
