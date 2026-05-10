#include "../include_DM.h"

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  ADD_QQ_Q

  Складывает два рациональных числа.

  Алгоритм:
    1. Вычисляется НОК знаменателей.
    2. Оба числителя приводятся к общему знаменателю.
    3. Числители складываются.
    4. Полученная дробь сокращается.

  Параметры:
    1) NUMBQ* first  — первое рациональное число
    2) NUMBQ* second — второе рациональное число

  Возвращаемое значение:
    NUMBQ* — сумма двух рациональных чисел
    NULL   — при ошибке
*/
NUMBQ* ADD_QQ_Q(NUMBQ* first, NUMBQ* second) {
    /* Проверка входных данных */
    if (!first || !second || !first->a.A || !second->a.A ||
        !first->b.A || !second->b.A ||
        first->b.n <= 0 || second->b.n <= 0) {
        return NULL;
    }

    /* Находим НОК знаменателей */
    NUMBN* lcm = LCM_NN_N(&first->b, &second->b);
    if (!lcm)
        return NULL;

    /* Вычисляем множители для приведения к общему знаменателю */
    NUMBN* factor1 = DIV_NN_N(lcm, &first->b);
    NUMBN* factor2 = DIV_NN_N(lcm, &second->b);

    if (!factor1 || !factor2) {
        if (factor1) {
            free(factor1->A);
            free(factor1);
        }
        if (factor2) {
            free(factor2->A);
            free(factor2);
        }
        free(lcm->A);
        free(lcm);
        return NULL;
    }

    /* Преобразуем множители в целые числа */
    NUMBZ* factor1_z = TRANS_N_Z(factor1);
    NUMBZ* factor2_z = TRANS_N_Z(factor2);

    free(factor1->A);
    free(factor1);
    free(factor2->A);
    free(factor2);

    if (!factor1_z || !factor2_z) {
        if (factor1_z) {
            free(factor1_z->A);
            free(factor1_z);
        }
        if (factor2_z) {
            free(factor2_z->A);
            free(factor2_z);
        }
        free(lcm->A);
        free(lcm);
        return NULL;
    }

    /* Вычисляем приведённые числители */
    NUMBZ* term1 = MUL_ZZ_Z(&first->a, factor1_z);
    NUMBZ* term2 = MUL_ZZ_Z(&second->a, factor2_z);

    free(factor1_z->A);
    free(factor1_z);
    free(factor2_z->A);
    free(factor2_z);

    if (!term1 || !term2) {
        if (term1) {
            free(term1->A);
            free(term1);
        }
        if (term2) {
            free(term2->A);
            free(term2);
        }
        free(lcm->A);
        free(lcm);
        return NULL;
    }

    /* Складываем числители */
    NUMBZ* numerator = ADD_ZZ_Z(term1, term2);

    free(term1->A);
    free(term1);
    free(term2->A);
    free(term2);

    if (!numerator) {
        free(lcm->A);
        free(lcm);
        return NULL;
    }

    /* Формируем временную дробь */
    NUMBQ rawFraction;
    rawFraction.a = *numerator;
    free(numerator);   /* освобождаем только структуру */

    rawFraction.b = *lcm;
    free(lcm);         /* освобождаем только структуру */

    /* Сокращаем результат */
    NUMBQ* result = RED_Q_Q(&rawFraction);

    /* Освобождаем временные данные */
    free(rawFraction.a.A);
    free(rawFraction.b.A);

    return result;
}