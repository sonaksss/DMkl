#include "../include_DM.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  DEG_P_N (P-6)

  Степень многочлена.

  Многочлен NUMBP хранит степень в поле m (int).
  Функция упаковывает это значение в структуру NUMBN
  (длинное натуральное число) для совместимости с остальными модулями.

  Алгоритм:
    1) Берём значение p->m
    2) Разбиваем его на цифры и записываем в массив A[]
       A[0] - младший разряд, A[n-1] - старший

  Зависимостей нет.

  Параметры:
    1) NUMBP* p - указатель на многочлен

  Возвращает NUMBN* - указатель на натуральное число, равное степени многочлена.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->A);
    free(result);
*/

/* Вспомогательная функция: подсчёт количества цифр в числе */
static int countDigits(int value) {
    if (value == 0) return 1;
    int count = 0;
    while (value > 0) {
        count++;
        value /= 10;
    }
    return count;
}

NUMBN* DEG_P_N(NUMBP* p) {
    if (p == NULL || p->C == NULL || p->m < 0) {
        return NULL;
    }

    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (result == NULL) {
        return NULL;
    }

    int deg = p->m;
    result->n = countDigits(deg);
    result->A = (int*)malloc(result->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }

    if (deg == 0) {
        result->A[0] = 0;
    } else {
        for (int i = 0; i < result->n; i++) {
            result->A[i] = deg % 10;
            deg /= 10;
        }
    }

    return result;
}
