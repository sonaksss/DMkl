#include "../include_DM.h"


/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
Преобразование сокращённого дробного числа в целое.
Возможно только если знаменатель равен 1.

Принимает NUMBQ* q — сокращённая дробь

Возвращает NUMBZ* — целое число, если знаменатель = 1.
Если знаменатель != 1, возвращает NULL.
*/
NUMBZ* TRANS_Q_Z(NUMBQ* q) {
    if (!q || !q->a.A || !q->b.A || q->a.n <= 0 || q->b.n <= 0) return NULL;
    
    if (q->b.n != 1 || q->b.A[0] != 1) {
        return NULL;
    }
    
    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (!result) return NULL;
    
    result->b = q->a.b;
    result->n = q->a.n;
    result->A = (int*)malloc(q->a.n * sizeof(int));
    if (!result->A) {
        free(result);
        return NULL;
    }
    for (int i = 0; i < q->a.n; i++) {
        result->A[i] = q->a.A[i];
    }
    
    return result;
}
