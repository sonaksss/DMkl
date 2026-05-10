#include "../include_DM.h"

/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
  Освобождает память, занятую структурой NUMBZ.

  Параметры:
    - NUMBZ* num — число для освобождения.

  Ничего не возвращает.
*/
static void freeNumbZ(NUMBZ* num) {
    if (!num) return;
    free(num->A);
    free(num);
}

/*
Остаток от деления первого целого числа на второе.

Параметры:
    - NUMBZ* a — делимое
    - NUMBZ* b — делитель

Возвращает NUMBZ* — остаток от деления a на b.
*/
NUMBZ* MOD_ZZ_Z(NUMBZ* a, NUMBZ* b){
    if (a == NULL || b == NULL || a->A == NULL || b->A == NULL ||
        a->n <= 0 || b->n <= 0) {
        return NULL;
    }
    
    if (POZ_Z_D(b) == 0) {
        return NULL;
    }
    
    NUMBZ* quotient = DIV_ZZ_Z(a, b);
    if (quotient == NULL) {
        return NULL;
    }
    
    NUMBZ* product = MUL_ZZ_Z(quotient, b);
    if (product == NULL) {
        freeNumbZ(quotient);
        return NULL;
    }
    
    NUMBZ* result = SUB_ZZ_Z(a, product);
    if (result == NULL) {
        freeNumbZ(quotient);
        freeNumbZ(product);
        return NULL;
    }
    
    freeNumbZ(quotient);
    freeNumbZ(product);
    
    return result;
}
