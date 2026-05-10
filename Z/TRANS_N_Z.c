#include "../include_DM.h"


/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  TRANS_N_Z (Z-4)

  Преобразование натурального числа в целое.

  Натуральное число всегда положительно, поэтому знак b = 0 (плюс).
  Массив цифр A[] копируется глубоко, оригинал не изменяется.

  Параметры:
    1) NUMBN* num - указатель на натуральное число

  Возвращает NUMBZ* - указатель на целое число, равное num со знаком "+".

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->A);
    free(result);
*/

NUMBZ* TRANS_N_Z(NUMBN* num) {
    if (num == NULL || num->A == NULL || num->n <= 0) {
        return NULL;
    }

    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (result == NULL) {
        return NULL;
    }

    result->A = (int*)malloc(num->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }

    memcpy(result->A, num->A, num->n * sizeof(int));
    result->n = num->n;
    result->b = 0;

    return result;
}
