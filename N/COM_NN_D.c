/*
   COM_NN_D 
 
   Функция сравнивает два целых числа, и возвращает: 2, если первое больше второго,
   0, если они равны, 1, если первое меньше второго,
   -1, если ошибка
 
   Параметры:
   1) const NUMBN* fNumb - Первое число
   2) const NUMBN* sNumb - Второе число
 
   Функция возвращает результат типа - int
*/
#include "../include_DM.h"


int COM_NN_D(NUMBN* fNumb, NUMBN* sNumb) {
    if (!fNumb || !sNumb || !fNumb->A || !sNumb->A || 
        fNumb->n <= 0 || sNumb->n <= 0) {
        return -1;
    }
    
    if (fNumb->n > sNumb->n) {
        return 2;
    }
    if (fNumb->n < sNumb->n) {
        return 1;
    }
    
    for (int i = fNumb->n - 1; i >= 0; i--) {
        if (fNumb->A[i] > sNumb->A[i]) {
            return 2;
        }
        if (fNumb->A[i] < sNumb->A[i]) {
            return 1;
        }
    }
    
    return 0;
}
