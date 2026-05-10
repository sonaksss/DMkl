/*
   MUL_NN_N

   Умножение двух натуральных чисел.

   Алгоритм:
     Результат = a * b  =  sum_{i=0}^{a->n-1} ( (b * a->A[i]) * 10^i )
     Для каждой цифры a (от младшей к старшей) умножаем b на эту цифру (MUL_ND_N),
     затем сдвигаем результат на i позиций (MUL_Nk_N) и прибавляем к накопленной сумме (ADD_NN_N).

   Числа хранятся от младшего разряда к старшему: A[0] — единицы, A[n-1] — старший разряд.

   Параметры:
     1) NUMBN* a – первый множитель
     2) NUMBN* b – второй множитель

   Возвращает NUMBN* – произведение a * b, или NULL при ошибке (некорректные данные или нехватка памяти).
*/

#include "../include_DM.h"

NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b) {
    // Проверка входных данных
    if (!a || !b || !a->A || !b->A || a->n <= 0 || b->n <= 0) {
        return NULL;
    }

    // Создаём результат, начальное значение 0
    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (!result) return NULL;

    result->n = 1;
    result->A = (int*)calloc(1, sizeof(int));
    if (!result->A) {
        free(result);
        return NULL;
    }
    result->A[0] = 0;

    // Перебираем все цифры числа a (от младшей к старшей)
    for (int i = 0; i < a->n; i++) {
        int digit = a->A[i];
        if (digit == 0) continue;  // ничего не добавляем

        // Умножаем b на цифру a->A[i]
        NUMBN* temp = MUL_ND_N(b, digit);
        if (!temp) {
            free(result->A);
            free(result);
            return NULL;
        }

        // Сдвигаем на i десятичных разрядов (умножаем на 10^i)
        NUMBN* shifted = MUL_Nk_N(temp, i);
        free(temp->A);
        free(temp);
        if (!shifted) {
            free(result->A);
            free(result);
            return NULL;
        }

        // Прибавляем к результату
        NUMBN* new_result = ADD_NN_N(result, shifted);
        free(result->A);
        free(result);
        free(shifted->A);
        free(shifted);
        if (!new_result) {
            return NULL;
        }
        result = new_result;
    }

    // Удаление ведущих нулей (если они есть) – результат уже канонический, но на всякий случай
    int new_n = result->n;
    while (new_n > 1 && result->A[new_n - 1] == 0) {
        new_n--;
    }
    if (new_n != result->n) {
        int* tmp = (int*)realloc(result->A, new_n * sizeof(int));
        if (!tmp) {
            free(result->A);
            free(result);
            return NULL;
        }
        result->A = tmp;
        result->n = new_n;
    }

    return result;
}
