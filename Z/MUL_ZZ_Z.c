#include "../include_DM.h"


/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBZ:
    - b      — знак числа: 0 — плюс (положительное или ноль), 1 — минус
    - n      — количество цифр в числе
    - A[0]   — младшая цифра числа
    - A[n-1] — старшая цифра числа

  Примечание по структуре NUMBN:
    - n      — количество цифр в числе
    - A[0]   — младшая цифра числа
    - A[n-1] — старшая цифра числа
*/

/*
  MUL_ZZ_Z

  Умножение двух целых чисел.
  Алгоритм:
    1) Проверка входных данных.
    2) Определяем знаки через POZ_Z_D.
    3) Если один из множителей ноль — возвращаем ноль.
    4) Берём абсолютные величины через ABS_Z_Z.
    5) Преобразуем в натуральные через TRANS_Z_N.
    6) Перемножаем их через MUL_NN_N.
    7) Знак: одинаковые знаки → плюс, разные → минус.

  Параметры:
    1) NUMBZ* a - первый множитель
    2) NUMBZ* b - второй множитель

  Возвращает указатель на новое NUMBZ = a * b, или NULL при ошибке
*/
NUMBZ* MUL_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    if (!a || !b || !a->A || !b->A || a->n <= 0 || b->n <= 0) {
        return NULL;
    }
    
    int signA = POZ_Z_D(a);
    int signB = POZ_Z_D(b);
    
    if (signA == -1 || signB == -1) {
        return NULL;
    }
    
    if (signA == 0 || signB == 0) {
        NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
        if (!result) return NULL;
        result->b = 0;
        result->n = 1;
        result->A = (int*)calloc(1, sizeof(int));
        if (!result->A) {
            free(result);
            return NULL;
        }
        return result;
    }
    
    NUMBZ* absA_z = ABS_Z_Z(a);
    NUMBZ* absB_z = ABS_Z_Z(b);
    if (!absA_z || !absB_z) {
        if (absA_z) { free(absA_z->A); free(absA_z); }
        if (absB_z) { free(absB_z->A); free(absB_z); }
        return NULL;
    }
    
    NUMBN* absA = TRANS_Z_N(absA_z);
    NUMBN* absB = TRANS_Z_N(absB_z);
    
    free(absA_z->A); free(absA_z);
    free(absB_z->A); free(absB_z);
    
    if (!absA || !absB) {
        if (absA) { free(absA->A); free(absA); }
        if (absB) { free(absB->A); free(absB); }
        return NULL;
    }
    
    NUMBN* prod = MUL_NN_N(absA, absB);
    free(absA->A); free(absA);
    free(absB->A); free(absB);
    
    if (!prod) {
        return NULL;
    }
    
    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (!result) {
        free(prod->A); free(prod);
        return NULL;
    }
    
    result->n = prod->n;
    result->A = prod->A;
    free(prod);
    result->b = (signA == signB) ? 0 : 1;
    
    return result;
}
