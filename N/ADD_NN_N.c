#include "../include_DM.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  ADD_NN_N (N-4)

  Сложение двух натуральных чисел.

  Числа хранятся в массивах A[]:
    A[0] - младший разряд, A[n-1] — старший.

  Алгоритм - поразрядное сложение с переносом:
    1) идём по разрядам от младшего к старшему
    2) складываем цифры обоих чисел и перенос из предыдущего разряда
    3) если после старшего разряда остался перенос - добавляем новый разряд

  Параметры:
    1) NUMBN* a - указатель на первое натуральное число
    2) NUMBN* b - указатель на второе натуральное число

  Возвращает NUMBN* - указатель на сумму a + b.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->A);
    free(result);
*/


NUMBN* ADD_NN_N(NUMBN* a, NUMBN* b) {
    if (a == NULL || b == NULL || a->A == NULL || b->A == NULL || 
    a->n <= 0 || b->n <= 0) {
        return NULL;
    }

    int maxLen = (a->n > b->n) ? a->n : b->n;

    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (result == NULL) {
        return NULL;
    }

    result->n = maxLen + 1;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }

    int carry = 0;
    for (int i = 0; i < maxLen; i++) {
        int digit_a = (i < a->n) ? a->A[i] : 0;
        int digit_b = (i < b->n) ? b->A[i] : 0;
        int sum = digit_a + digit_b + carry;
        result->A[i] = sum % 10;
        carry = sum / 10;
    }

    result->A[maxLen] = carry;

    if (result->A[result->n - 1] == 0) {
        result->n--;
    }

    return result;
}
