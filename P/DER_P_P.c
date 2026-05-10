#include "../include_DM.h"


/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  createRationalFromInt

  Вспомогательная функция. Создаёт рациональное число NUMBQ из целого int.
  Числитель = value (знаковый), знаменатель = 1.

  Параметры:
    1) int value - целое число для преобразования

  Возвращает указатель на новый NUMBQ* или NULL при ошибке выделения памяти.
*/
static NUMBQ* createRationalFromInt(int value) {
    NUMBQ* q = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!q) return NULL;

    /* Числитель NUMBZ */
    q->a.b = (value >= 0) ? 0 : 1; /* 0 — положительное/ноль, 1 — отрицательное */
    int absVal = (value < 0) ? -value : value;

    if (absVal == 0) {
        q->a.n = 1;
        q->a.A = (int*)malloc(sizeof(int));
        if (!q->a.A) { free(q); return NULL; }
        q->a.A[0] = 0;
    } else {
        /* Считаем количество цифр */
        int tmp = absVal;
        int digits = 0;
        while (tmp > 0) { digits++; tmp /= 10; }

        q->a.n = digits;
        q->a.A = (int*)malloc(digits * sizeof(int));
        if (!q->a.A) { free(q); return NULL; }

        /* Записываем цифры: A[0] — младший разряд */
        tmp = absVal;
        for (int i = 0; i < digits; i++) {
            q->a.A[i] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Знаменатель NUMBN = 1 */
    q->b.n = 1;
    q->b.A = (int*)malloc(sizeof(int));
    if (!q->b.A) { free(q->a.A); free(q); return NULL; }
    q->b.A[0] = 1;

    return q;
}

/*
  mulRationalByInt

  Вспомогательная функция. Умножает рациональное число NUMBQ на целое int.
  Результат: новое рациональное число (числитель умножается на |int|,
  знак корректируется).

  Параметры:
    1) NUMBQ* q  - указатель на рациональное число
    2) int factor - целый множитель (> 0, т.к. степень всегда > 0)

  Возвращает указатель на новый NUMBQ* или NULL при ошибке.
*/
static NUMBQ* mulRationalByInt(NUMBQ* q, int factor) {
    if (!q || factor <= 0) return NULL;

    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!result) return NULL;

    /* Знак результата совпадает со знаком исходного числителя */
    result->a.b = q->a.b;

    /* Умножаем числитель на factor — длинная арифметика */
    int n = q->a.n;
    /* Максимально возможное число разрядов после умножения на factor */
    int maxDigits = n + 10; /* запас */
    result->a.A = (int*)calloc(maxDigits, sizeof(int));
    if (!result->a.A) { free(result); return NULL; }

    int carry = 0;
    int i;
    for (i = 0; i < n; i++) {
        int prod = q->a.A[i] * factor + carry;
        result->a.A[i] = prod % 10;
        carry = prod / 10;
    }
    /* Остаток переноса */
    while (carry > 0) {
        result->a.A[i] = carry % 10;
        carry /= 10;
        i++;
    }
    result->a.n = i;
    if (result->a.n == 0) result->a.n = 1; /* хотя бы одна цифра */

    /* Знаменатель копируем без изменений */
    result->b.n = q->b.n;
    result->b.A = (int*)malloc(q->b.n * sizeof(int));
    if (!result->b.A) { free(result->a.A); free(result); return NULL; }
    memcpy(result->b.A, q->b.A, q->b.n * sizeof(int));

    return result;
}

/*
  isZeroRational

  Проверяет, равен ли числитель рационального числа нулю.

  Параметры:
    1) NUMBQ* q - указатель на рациональное число

  Возвращает 1 если числитель == 0, иначе 0.
*/
static int isZeroRational(NUMBQ* q) {
    if (!q) return 1;
    for (int i = 0; i < q->a.n; i++) {
        if (q->a.A[i] != 0) return 0;
    }
    return 1;
}

/*
  DER_P_P

  Вычисляет производную многочлена с рациональными коэффициентами.
  Если P(x) = C[0] + C[1]*x + ... + C[m]*x^m,
  то P'(x) = C[1] + 2*C[2]*x + ... + m*C[m]*x^(m-1).
  Коэффициент i-го члена производной равен (i+1) * C[i+1].

  Особые случаи:
    - Если p == NULL, возвращается NULL.
    - Если степень многочлена равна 0 (константа), производная — нулевой многочлен.

  Параметры:
    1) NUMBP* p - указатель на исходный многочлен

  Возвращает указатель на новый многочлен NUMBP* — производную,
  или NULL при ошибке (нулевой указатель, ошибка выделения памяти).
*/
NUMBP* DER_P_P(NUMBP* p) {
    if (!p || !p->C || p->m < 0) return NULL;

    NUMBP* deriv = (NUMBP*)malloc(sizeof(NUMBP));
    if (!deriv) return NULL;

    /* Производная константы — нулевой многочлен степени 0 */
    if (p->m == 0) {
        deriv->m = 0;
        deriv->C = (NUMBQ*)malloc(sizeof(NUMBQ));
        if (!deriv->C) { free(deriv); return NULL; }

        NUMBQ* zero = createRationalFromInt(0);
        if (!zero) { free(deriv->C); free(deriv); return NULL; }

        deriv->C[0] = *zero;
        free(zero);
        return deriv;
    }

    /* Степень производной на 1 меньше */
    int newDegree = p->m - 1;

    deriv->C = (NUMBQ*)malloc((newDegree + 1) * sizeof(NUMBQ));
    if (!deriv->C) { free(deriv); return NULL; }

    /*
      Коэффициент i-го члена производной (0 <= i <= m-1):
        deriv->C[i] = (i+1) * p->C[i+1]
    */
    for (int i = 0; i <= newDegree; i++) {
        int multiplier = i + 1; /* степень исходного члена */
        NUMBQ* coeff = mulRationalByInt(&p->C[i + 1], multiplier);
        if (!coeff) {
            /* Освобождаем уже выделенные коэффициенты */
            for (int j = 0; j < i; j++) {
                free(deriv->C[j].a.A);
                free(deriv->C[j].b.A);
            }
            free(deriv->C);
            free(deriv);
            return NULL;
        }
        deriv->C[i] = *coeff;
        free(coeff);
    }

    /*
      Убираем старшие нулевые коэффициенты (нормализация),
      но степень не может быть ниже 0.
    */
    int actualDegree = newDegree;
    while (actualDegree > 0 && isZeroRational(&deriv->C[actualDegree])) {
        free(deriv->C[actualDegree].a.A);
        free(deriv->C[actualDegree].b.A);
        actualDegree--;
    }
    deriv->m = actualDegree;

    return deriv;
}
