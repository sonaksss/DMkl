/*
  Автор: Семенов Максим
  Группа: 5388
*/
/*
    DIV_NN_Dk – первая цифра частного от деления большего натурального на меньшее,
    домноженное на 10^k, где k – номер позиции этой цифры (номер считается с нуля).

    Функция находит такую десятичную цифру q (0..9), что
    q * (B * 10^k) <= A < (q+1) * (B * 10^k),
    где k подбирается автоматически как разность длин чисел.

    Цифры хранятся от младшего разряда к старшему (little-endian).

    Параметры:
        1) const NUMBN* A – делимое (натуральное число)
        2) const NUMBN* B – делитель (натуральное число, > 0)

    Возвращает: int – цифра q (0, если A < B, иначе 1..9)
*/

#include "../include_DM.h"

int DIV_NN_Dk(NUMBN* A, NUMBN* B) {
    if (A == NULL || B == NULL || A->A == NULL || B->A == NULL || 
        A->n <= 0 || B->n <= 0) {
        return 0;
    }
    
    if (B->n == 1 && B->A[0] == 0) {
        return 0;
    }
    
    int cmp = COM_NN_D(A, B);
    if (cmp == -1) {
        return 0;  // Ошибка в COM_NN_D
    }
    if (cmp == 1) {
        return 0;  // A < B
    }
    
    int k = A->n - B->n;
    NUMBN* Bk = MUL_Nk_N(B, k);
    if (Bk == NULL) {
        return 0;
    }
    
    while (k >= 0) {
        cmp = COM_NN_D(A, Bk);
        if (cmp == -1) {
            free(Bk->A); free(Bk);
            return 0;
        }
        if (cmp != 1) break;  // A >= Bk
        
        // A < Bk
        free(Bk->A); free(Bk);
        if (k == 0) return 0;
        k--;
        Bk = MUL_Nk_N(B, k);
        if (Bk == NULL) return 0;
    }
    
    for (int q = 9; q >= 1; q--) {
        NUMBN* product = MUL_ND_N(Bk, q);
        if (product == NULL) continue;
        
        cmp = COM_NN_D(product, A);
        free(product->A); free(product);
        
        if (cmp == -1) {
            free(Bk->A); free(Bk);
            return 0;
        }
        if (cmp != 2) {  // product <= A
            int result = q;
            free(Bk->A); free(Bk);
            return result;
        }
    }
    
    free(Bk->A); free(Bk);
    return 1;
}
