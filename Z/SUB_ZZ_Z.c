/*
    SUB_ZZ_Z – вычитание целых чисел.

    Выполняет A - B.
    Цифры хранятся от младшего разряда к старшему (little-endian).

    Параметры:
        1) NUMBZ* A – уменьшаемое
        2) NUMBZ* B – вычитаемое

    Возвращает: NUMBZ* – разность, или NULL при ошибке

    Автор: Семенов Максим
    Группа: 5388
*/

#include "../include_DM.h"


NUMBZ* SUB_ZZ_Z(NUMBZ* A, NUMBZ* B) {
    if (!A || !B || A->A == NULL || B->A == NULL || A->n <= 0 || B->n <= 0) {
        return NULL;
    }

    NUMBZ* minusB = MUL_ZM_Z(B);
    if (!minusB) return NULL;

    NUMBZ* result = ADD_ZZ_Z(A, minusB);
    if (!result) {
        free(minusB->A);
        free(minusB);
        return NULL;
    }

    free(minusB->A);
    free(minusB);

    return result;
}
