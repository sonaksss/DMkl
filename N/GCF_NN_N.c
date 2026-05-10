#include "../include_DM.h"

/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
  copyNumbn
  Создаёт полную копию натурального числа.
  Параметры:
    1) NUMBN* src — исходное число
  Возвращает NUMBN* — копия числа, либо NULL при ошибке.
*/
static NUMBN* copyNumbn(NUMBN* src) {
    if (!src || !src->A || src->n < 0) return NULL;
    NUMBN* copy = (NUMBN*)malloc(sizeof(NUMBN));
    if (!copy) return NULL;
    copy->n = src->n;
    copy->A = (int*)malloc(src->n * sizeof(int));
    if (!copy->A) { free(copy); return NULL; }
    for (int i = 0; i < src->n; i++) copy->A[i] = src->A[i];
    return copy;
}

/*
  freeNumbn
  Освобождает память, занятую структурой NUMBN.
  Параметры:
    1) NUMBN* num — число для освобождения
  Ничего не возвращает.
*/
static void freeNumbn(NUMBN* num) {
    if (!num) return;
    free(num->A);
    free(num);
}

/*
НОД натуральных чисел

Принимает 2 натуральных числа
Вычисляет НОД и возвращет натуральное число
*/
NUMBN* GCF_NN_N(NUMBN* a, NUMBN* b) {
    if (!a || !b || !a->A || !b->A || a->n <= 0 || b->n <= 0) {
        return NULL;
    }
    
    NUMBN* temp_a = copyNumbn(a);
    if (!temp_a) return NULL;
    
    NUMBN* temp_b = copyNumbn(b);
    if (!temp_b) {
        freeNumbn(temp_a);
        return NULL;
    }

    while (NZER_N_B(temp_a) && NZER_N_B(temp_b)) {
        int cmp = COM_NN_D(temp_a, temp_b);
        if (cmp == -1) {
            freeNumbn(temp_a);
            freeNumbn(temp_b);
            return NULL;
        }
        
        if (cmp == 2) {  // temp_a > temp_b
            NUMBN* new_a = MOD_NN_N(temp_a, temp_b);
            if (!new_a) {
                freeNumbn(temp_a);
                freeNumbn(temp_b);
                return NULL;
            }
            freeNumbn(temp_a);
            temp_a = new_a;
        } else if (cmp == 1) {  // temp_a < temp_b
            NUMBN* new_b = MOD_NN_N(temp_b, temp_a);
            if (!new_b) {
                freeNumbn(temp_a);
                freeNumbn(temp_b);
                return NULL;
            }
            freeNumbn(temp_b);
            temp_b = new_b;
        } else if (cmp == 0) {  // temp_a = temp_b
            break;
        }
    }

    NUMBN* result = NZER_N_B(temp_a) ? temp_a : temp_b;
    NUMBN* to_free = NZER_N_B(temp_a) ? temp_b : temp_a;
    freeNumbn(to_free);
    
    return result;
}
