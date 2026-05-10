#include "../include_DM.h"


/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  ADD_1N_N

  Добавление 1 к натуральному числу.

  Число хранится в массиве A[]:
    A[0] - младший разряд, A[n-1] - старший.

  Функция работает с копией числа, не изменяя оригинал.
  Алгоритм реализует ручное прибавление единицы с переносом:
    1) прибавляем 1 к A[0], если нет переноса - готово
    2) если есть перенос, продолжаем по разрядам
    3) если перенос вышел за пределы числа — увеличиваем разрядность

  Параметры:
    1) NUMBN* num — указатель на исходное натуральное число

  Возвращает NUMBN* — указатель на новое число, равное num + 1.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->A);
    free(result);
*/

NUMBN* ADD_1N_N(NUMBN* num) {
    if (num == NULL || num->A == NULL || num->n <= 0) {
        return NULL;
    }

    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (result == NULL) {
        return NULL;
    }

    result->n = num->n;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }

    memcpy(result->A, num->A, result->n * sizeof(int));

    int carry = 1;
    for (int i = 0; i < result->n && carry; i++) {
        int sum = result->A[i] + carry;
        result->A[i] = sum % 10;
        carry = sum / 10;
    }

    if (carry) {
        int* tmp = (int*)realloc(result->A, (result->n + 1) * sizeof(int));
        if (tmp == NULL) {
            free(result->A);
            free(result);
            return NULL;
        }
        result->A = tmp;
        result->A[result->n] = carry;
        result->n++;
    }

    return result;
}
