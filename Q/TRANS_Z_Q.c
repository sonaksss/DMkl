#include "../include_DM.h"


/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
Преобразование целого числа в дробное.
  
Принимает NUMBZ* z — исходное целое число

Возвращает NUMBQ* — дробь вида z/1.
В случае ошибки возвращает NULL.
*/
NUMBQ* TRANS_Z_Q(NUMBZ* z) {
    if (!z || !z->A || z->n <= 0) return NULL;
    
    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!result) return NULL;
    
    result->a.b = z->b;
    result->a.n = z->n;
    result->a.A = (int*)malloc(z->n * sizeof(int));
    if (!result->a.A) {
        free(result);
        return NULL;
    }
    for (int i = 0; i < z->n; i++) {
        result->a.A[i] = z->A[i];
    }
    
    result->b.n = 1;
    result->b.A = (int*)malloc(sizeof(int));
    if (!result->b.A) {
        free(result->a.A);
        free(result);
        return NULL;
    }
    result->b.A[0] = 1;
    
    return result;
}
