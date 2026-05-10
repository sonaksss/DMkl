/*
    MUL_Nk_N – умножение натурального числа на 10^k

    Умножает длинное натуральное число на 10 в степени k.
    Цифры хранятся от младшего разряда к старшему (little-endian).

    Параметры:
        1) const NUMBN* num – исходное натуральное число
        2) int k – показатель степени (натуральное или 0)

    Возвращает: NUMBN* – результат умножения (num * 10^k), или NULL при ошибке

    Автор: Семенов Максим
    Группа: 5388
*/

#include "../include_DM.h"

NUMBN* MUL_Nk_N(const NUMBN* num, int k) {
    if (k<0 || num == NULL || num->A == NULL || num->n <= 0) return NULL;
    
    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (result == NULL) return NULL;

    result->n = num->n + k;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }

    for (int i = 0; i < k; i++) {
        result->A[i] = 0;
    }
    for (int i = 0; i < num->n; i++) {
        result->A[i + k] = num->A[i];
    }
    return result;
}
