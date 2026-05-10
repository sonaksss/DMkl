/*
    ABS_Z_Z – абсолютная величина целого числа.

    Возвращает целое число с тем же модулем, но положительным знаком.
    Цифры хранятся от младшего разряда к старшему (little-endian).

    Параметры:
        1) const NUMBZ* num – исходное целое число

    Возвращает: NUMBZ* – абсолютное значение (знак 0), или NULL при ошибке

    Автор: Семенов Максим
    Группа: 5388
*/

#include "../include_DM.h"

NUMBZ* ABS_Z_Z(NUMBZ* num) {
    if (!num || num->A == NULL || num->n <= 0) return NULL;

    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (!result) return NULL;

    result->b = 0;                   // знак +
    result->n = num->n;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (!result->A) {
        free(result);
        return NULL;
    }
    for (int i = 0; i < num->n; i++) {
        result->A[i] = num->A[i];
    }
    return result;
}
