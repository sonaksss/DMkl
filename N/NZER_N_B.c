#include "../include_DM.h"

/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
Проверка на ноль:

Принимает натуральное число
Возвращает true если не равно нулю, иначе false

*/
bool NZER_N_B(NUMBN* num) {
    if (num == NULL || num->A == NULL || num->n == 0) {
        return false;
    }
    return !(num->n == 1 && num->A[0] == 0);
}
