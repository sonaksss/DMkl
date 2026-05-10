#include "../include_DM.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  ADD_ZZ_Z (Z-6)

  Сложение двух целых чисел.

  Знак числа хранится в поле b: 0 - плюс, 1 - минус.
  Массив цифр A[]: A[0] - младший разряд, A[n-1] - старший.

  Алгоритм:
    1) Определяем знаки через POZ_Z_D:
    2) Если одно из чисел ноль - возвращаем копию другого
    3) Если знаки одинаковы:
         результат = ADD_NN_N(|a|, |b|), знак тот же
    4) Если знаки разные - сравниваем модули через COM_NN_D:
         |a| > |b|: результат = SUB_NN_N(|a|, |b|), знак от a
         |a| < |b|: результат = SUB_NN_N(|b|, |a|), знак от b
         |a| == |b|: результат = 0

  Зависит от:
    - POZ_Z_D  (Z-2) - знак целого числа
    - ABS_Z_Z  (Z-1) - модуль целого -> целое (неотрицательное)
    - COM_NN_D (N-1) - сравнение натуральных: 2 больше, 1 меньше, 0 равно
    - ADD_NN_N (N-4) - сложение натуральных
    - SUB_NN_N (N-5) - вычитание натуральных
    - MUL_ZM_Z (Z-3) - умножение целого на (-1)

  Параметры:
    1) NUMBZ* a - указатель на первое целое число
    2) NUMBZ* b - указатель на второе целое число

  Возвращает NUMBZ* - указатель на сумму a + b.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->A);
    free(result);
*/

static NUMBZ* copyNumbZ(NUMBZ* src) {
    if (src == NULL || src->A == NULL || src->n <= 0) {
        return NULL;
    }
    NUMBZ* copy = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (copy == NULL) {
        return NULL;
    }
    copy->A = (int*)malloc(src->n * sizeof(int));
    if (copy->A == NULL) {
        free(copy);
        return NULL;
    }
    memcpy(copy->A, src->A, src->n * sizeof(int));
    copy->n = src->n;
    copy->b = src->b;
    return copy;
}

static NUMBZ* makeZero() {
    NUMBZ* zero = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (zero == NULL) {
        return NULL;
    }
    zero->A = (int*)malloc(sizeof(int));
    if (zero->A == NULL) {
        free(zero);
        return NULL;
    }
    zero->A[0] = 0;
    zero->n = 1;
    zero->b = 0;
    return zero;
}

static NUMBN* absToNatural(NUMBZ* absZ) {
    if (absZ == NULL || absZ->A == NULL || absZ->n <= 0) {
        return NULL;
    }
    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (result == NULL) {
        return NULL;
    }
    result->A = (int*)malloc(absZ->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }
    memcpy(result->A, absZ->A, absZ->n * sizeof(int));
    result->n = absZ->n;
    return result;
}

NUMBZ* ADD_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    if (a == NULL || b == NULL || a->A == NULL || b->A == NULL ||
        a->n <= 0 || b->n <= 0) {
        return NULL;
    }

    int sign_a = POZ_Z_D(a);
    int sign_b = POZ_Z_D(b);

    if (sign_a == -1 || sign_b == -1) {
        return NULL;
    }

    if (sign_a == 0) {
        NUMBZ* copy = copyNumbZ(b);
        return copy;
    }
    if (sign_b == 0) {
        NUMBZ* copy = copyNumbZ(a);
        return copy;
    }

    NUMBZ* abs_a_z = ABS_Z_Z(a);
    NUMBZ* abs_b_z = ABS_Z_Z(b);
    if (abs_a_z == NULL || abs_b_z == NULL) {
        if (abs_a_z) { free(abs_a_z->A); free(abs_a_z); }
        if (abs_b_z) { free(abs_b_z->A); free(abs_b_z); }
        return NULL;
    }

    NUMBN* abs_a = absToNatural(abs_a_z);
    NUMBN* abs_b = absToNatural(abs_b_z);
    if (abs_a == NULL || abs_b == NULL) {
        if (abs_a) { free(abs_a->A); free(abs_a); }
        if (abs_b) { free(abs_b->A); free(abs_b); }
        free(abs_a_z->A); free(abs_a_z);
        free(abs_b_z->A); free(abs_b_z);
        return NULL;
    }

    free(abs_a_z->A); free(abs_a_z);
    free(abs_b_z->A); free(abs_b_z);

    NUMBZ* result = NULL;

    if (sign_a == sign_b) {
        NUMBN* sum = ADD_NN_N(abs_a, abs_b);
        free(abs_a->A); free(abs_a);
        free(abs_b->A); free(abs_b);
        if (sum == NULL) {
            return NULL;
        }
        NUMBZ* sum_z = (NUMBZ*)malloc(sizeof(NUMBZ));
        if (sum_z == NULL) {
            free(sum->A); free(sum);
            return NULL;
        }
        sum_z->A = sum->A;
        sum_z->n = sum->n;
        sum_z->b = (sign_a == 2) ? 0 : 1;
        free(sum);
        result = sum_z;
    } else {
        int cmp = COM_NN_D(abs_a, abs_b);
        if (cmp == -1) {
            free(abs_a->A); free(abs_a);
            free(abs_b->A); free(abs_b);
            return NULL;
        }

        if (cmp == 0) {
            free(abs_a->A); free(abs_a);
            free(abs_b->A); free(abs_b);
            result = makeZero();
        } else if (cmp == 2) {
            NUMBN* diff = SUB_NN_N(abs_a, abs_b);
            free(abs_a->A); free(abs_a);
            free(abs_b->A); free(abs_b);
            if (diff == NULL) {
                return NULL;
            }
            NUMBZ* diff_z = (NUMBZ*)malloc(sizeof(NUMBZ));
            if (diff_z == NULL) {
                free(diff->A); free(diff);
                return NULL;
            }
            diff_z->A = diff->A;
            diff_z->n = diff->n;
            diff_z->b = a->b;
            free(diff);
            result = diff_z;
        } else {
            NUMBN* diff = SUB_NN_N(abs_b, abs_a);
            free(abs_a->A); free(abs_a);
            free(abs_b->A); free(abs_b);
            if (diff == NULL) {
                return NULL;
            }
            NUMBZ* diff_z = (NUMBZ*)malloc(sizeof(NUMBZ));
            if (diff_z == NULL) {
                free(diff->A); free(diff);
                return NULL;
            }
            diff_z->A = diff->A;
            diff_z->n = diff->n;
            diff_z->b = b->b;
            free(diff);
            result = diff_z;
        }
    }

    return result;
}
