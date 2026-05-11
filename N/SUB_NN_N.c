/*
Автор: Александров Ярослав 5388
*/

/*
   SUB_NN_N
 
   Вычитание из первого натурального числа второго (первое >= второе)
 
   Параметры:
   1) NUMBN* a - Первое число (уменьшаемое)
   2) NUMBN* b - Второе число (вычитаемое)
 
   Функция возвращает NUMBN* - результат вычитания, или NULL при ошибке
*/

#include "../include_DM.h"

NUMBN* SUB_NN_N(NUMBN* a, NUMBN* b) {
    if (!a || !b || !a->A || !b->A || a->n <= 0 || b->n <= 0) {
        return NULL;
    }
    
    int cmp = COM_NN_D(a, b);
    if (cmp == -1) {
        return NULL;
    }
    if (cmp == 1) {
        return NULL;
    }
    
    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (!result) return NULL;
    
    result->n = a->n;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (!result->A) {
        free(result);
        return NULL;
    }
    
    for (int i = 0; i < a->n; i++) {
        result->A[i] = a->A[i];
    }
    
    int borrow = 0;
    for (int i = 0; i < b->n; i++) {
        result->A[i] = result->A[i] - b->A[i] - borrow;
        borrow = 0;
        if (result->A[i] < 0) {
            result->A[i] += 10;
            borrow = 1;
        }
    }
    
    for (int i = b->n; i < a->n && borrow; i++) {
        result->A[i] = result->A[i] - borrow;
        borrow = 0;
        if (result->A[i] < 0) {
            result->A[i] += 10;
            borrow = 1;
        }
    }
    
    int new_n = result->n;
    while (new_n > 1 && result->A[new_n - 1] == 0) {
        new_n--;
    }
    
    if (new_n != result->n) {
        result->n = new_n;
        int* tmp = (int*)realloc(result->A, new_n * sizeof(int));
        if (!tmp) {
            free(result->A);
            free(result);
            return NULL;
        }
        result->A = tmp;
    }
    
    return result;
}
