#include "menu.h"
#include "include_DM.h"
#include <ctype.h>
#include <limits.h>
#include <errno.h>

/* ========== Функции освобождения памяти ========== */

void freeNUMBN(NUMBN* num) {
    if (num) {
        free(num->A);
        num->A = NULL;
        num->n = 0;
        free(num);
    }
}

void freeNUMBZ(NUMBZ* num) {
    if (num) {
        free(num->A);
        num->A = NULL;
        num->n = 0;
        num->b = 0;
        free(num);
    }
}

void freeNUMBQ(NUMBQ* num) {
    if (num) {
        free(num->a.A);
        num->a.A = NULL;
        free(num->b.A);
        num->b.A = NULL;
        free(num);
    }
}

void freeNUMBP(NUMBP* poly) {
    if (poly) {
        if (poly->C) {
            for (int i = 0; i <= poly->m; i++) {
                free(poly->C[i].a.A);
                free(poly->C[i].b.A);
            }
            free(poly->C);
            poly->C = NULL;
        }
        poly->m = -1;
        free(poly);
    }
}

/* ========== Функции проверки ошибок ========== */

int isErrorN(NUMBN* num) { return num == NULL; }
int isErrorZ(NUMBZ* num) { return num == NULL; }
int isErrorQ(NUMBQ* num) {
    if (!num) return 1;
    return (!num->a.A || !num->b.A);
}
int isErrorP(NUMBP* poly) { return poly == NULL; }

int isZeroN(NUMBN* num) {
    if (!num || !num->A || num->n <= 0) return 1;
    return (num->n == 1 && num->A[0] == 0);
}

int isZeroBZ(NUMBZ* num) {
    if (!num || !num->A || num->n <= 0) return 1;
    return (num->n == 1 && num->A[0] == 0);
}

/* ========== Вспомогательные функции ввода ========== */

static int* inputString(int* len) {
    char ch;
    char* arr = NULL;
    char* temp_arr = NULL;
    int* numb = NULL;
    *len = 0;

    printf(": ");

    do {
        if (*len == 0) {
            arr = (char*)malloc(sizeof(char));
            if (!arr) { printf("ОШИБКА: нет памяти\n"); return NULL; }
        } else {
            temp_arr = (char*)realloc(arr, (*len + 1) * sizeof(char));
            if (!temp_arr) { printf("ОШИБКА: нет памяти\n"); free(arr); return NULL; }
            arr = temp_arr;
        }

        scanf("%c", &ch);

        if (ch == '\n' && *len == 0) {
            printf("ОШИБКА: пустой ввод\n");
            free(arr);
            return NULL;
        }

        if (ch >= '0' && ch <= '9') {
            arr[*len] = ch;
            (*len)++;
        } else if (ch != '\n') {
            printf("ОШИБКА: введите только цифры\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            free(arr);
            return NULL;
        }
    } while (ch != '\n');

    if (*len == 0) { free(arr); return NULL; }

    /* Удаляем ведущие нули */
    int start = 0;
    while (start < *len - 1 && arr[start] == '0') start++;

    *len = *len - start;
    numb = (int*)malloc((*len) * sizeof(int));
    if (!numb) { printf("ОШИБКА: нет памяти\n"); free(arr); return NULL; }

    for (int i = 0; i < *len; i++)
        numb[i] = arr[*len - 1 - i + start] - '0';

    free(arr);
    return numb;
}

/*
  inputZString — считывает целое число со знаком.
  ИСПРАВЛЕНО: убрано дублированное объявление.
*/
static NUMBZ* inputZString(const char* prompt) {
    printf("%s", prompt);

    char ch;
    int sign = 0;
    int has_digit = 0;
    char* arr = NULL;
    char* temp_arr = NULL;
    int len = 0;

    do { scanf("%c", &ch); } while (ch == ' ' || ch == '\t');

    if (ch == '+')      { sign = 0; scanf("%c", &ch); }
    else if (ch == '-') { sign = 1; scanf("%c", &ch); }

    while (ch >= '0' && ch <= '9') {
        has_digit = 1;
        temp_arr = (char*)realloc(arr, (len + 1) * sizeof(char));
        if (!temp_arr) { printf("ОШИБКА: нет памяти\n"); free(arr); exit(1); }
        arr = temp_arr;
        arr[len++] = ch;
        scanf("%c", &ch);
    }

    if (!has_digit) {
        printf("ОШИБКА: ожидались цифры после знака\n");
        free(arr);
        exit(1);
    }
    if (ch != '\n') {
        printf("ОШИБКА: неверный формат числа\n");
        free(arr);
        while ((ch = getchar()) != '\n' && ch != EOF);
        exit(1);
    }

    int start = 0;
    while (start < len - 1 && arr[start] == '0') start++;
    len = len - start;
    if (len == 1 && arr[start] == '0') sign = 0;

    int* digits = (int*)malloc(len * sizeof(int));
    if (!digits) { printf("ОШИБКА: нет памяти\n"); free(arr); exit(1); }

    for (int i = 0; i < len; i++)
        digits[i] = arr[len - 1 - i + start] - '0';
    free(arr);

    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (!result) { printf("ОШИБКА: нет памяти\n"); free(digits); exit(1); }
    result->b = sign;
    result->n = len;
    result->A = digits;
    return result;
}

/* ========== Функции ввода ========== */

NUMBN* inputN(const char* prompt) {
    printf("%s", prompt);
    int len = 0;
    int* digits = inputString(&len);
    if (!digits || len == 0) {
        printf("ОШИБКА: некорректный ввод натурального числа\n");
        exit(1);
    }
    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (!result) { printf("ОШИБКА: нет памяти\n"); free(digits); exit(1); }
    result->n = len;
    result->A = digits;
    return result;
}

NUMBZ* inputZ(const char* prompt) {
    return inputZString(prompt);
}

NUMBQ* inputQ(const char* prompt) {
    printf("%s", prompt);

    char ch;
    char* numerator_str = NULL;
    char* denominator_str = NULL;
    int numerator_len = 0, denominator_len = 0;
    int has_slash = 0, sign = 0;

    do { scanf("%c", &ch); } while (ch == ' ' || ch == '\t');

    if (ch == '+')      { sign = 0; scanf("%c", &ch); }
    else if (ch == '-') { sign = 1; scanf("%c", &ch); }

    while (ch >= '0' && ch <= '9') {
        char* tmp = (char*)realloc(numerator_str, (numerator_len + 1) * sizeof(char));
        if (!tmp) { free(numerator_str); exit(1); }
        numerator_str = tmp;
        numerator_str[numerator_len++] = ch;
        scanf("%c", &ch);
    }

    if (ch == '/') {
        has_slash = 1;
        scanf("%c", &ch);
        while (ch == ' ' || ch == '\t') scanf("%c", &ch);
        while (ch >= '0' && ch <= '9') {
            char* tmp = (char*)realloc(denominator_str, (denominator_len + 1) * sizeof(char));
            if (!tmp) { free(numerator_str); free(denominator_str); exit(1); }
            denominator_str = tmp;
            denominator_str[denominator_len++] = ch;
            scanf("%c", &ch);
        }
    }

    if (ch != '\n') {
        printf("ОШИБКА: неверный формат дроби\n");
        free(numerator_str); free(denominator_str);
        while ((ch = getchar()) != '\n' && ch != EOF);
        exit(1);
    }

    if (numerator_len == 0) {
        printf("ОШИБКА: дробь без числителя\n");
        free(numerator_str); free(denominator_str); exit(1);
    }

    if (!has_slash) {
        denominator_len = 1;
        denominator_str = (char*)malloc(sizeof(char));
        if (!denominator_str) { free(numerator_str); exit(1); }
        denominator_str[0] = '1';
    }

    if (denominator_len == 1 && denominator_str[0] == '0') {
        printf("ОШИБКА: знаменатель не может быть 0\n");
        free(numerator_str); free(denominator_str); exit(1);
    }

    int num_start = 0;
    while (num_start < numerator_len - 1 && numerator_str[num_start] == '0') num_start++;
    numerator_len -= num_start;

    int den_start = 0;
    while (den_start < denominator_len - 1 && denominator_str[den_start] == '0') den_start++;
    denominator_len -= den_start;

    int* num_digits = (int*)malloc(numerator_len * sizeof(int));
    if (!num_digits) { free(numerator_str); free(denominator_str); exit(1); }
    for (int i = 0; i < numerator_len; i++)
        num_digits[i] = numerator_str[numerator_len - 1 - i + num_start] - '0';

    int is_zero_num = (numerator_len == 1 && num_digits[0] == 0);

    int* den_digits = (int*)malloc(denominator_len * sizeof(int));
    if (!den_digits) { free(num_digits); free(numerator_str); free(denominator_str); exit(1); }
    for (int i = 0; i < denominator_len; i++)
        den_digits[i] = denominator_str[denominator_len - 1 - i + den_start] - '0';

    free(numerator_str); free(denominator_str);

    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!result) { free(num_digits); free(den_digits); exit(1); }

    result->a.b = is_zero_num ? 0 : sign;
    result->a.n = numerator_len;
    result->a.A = num_digits;
    result->b.n = denominator_len;
    result->b.A = den_digits;
    return result;
}

NUMBP* inputP(const char* prompt) {
    printf("%s\n", prompt);
    printf("Введите степень многочлена: "); fflush(stdout);

    int degree;
    if (scanf("%d", &degree) != 1 || degree < 0) {
        printf("ОШИБКА: степень должна быть целым неотрицательным числом\n");
        while (getchar() != '\n');
        exit(1);
    }
    while (getchar() != '\n');

    /* Выделяем массив коэффициентов, заполняем нулями */
    NUMBP* result = (NUMBP*)malloc(sizeof(NUMBP));
    if (!result) { printf("ОШИБКА: нет памяти\n"); exit(1); }
    result->m = degree;
    result->C = (NUMBQ*)malloc((degree + 1) * sizeof(NUMBQ));
    if (!result->C) { free(result); printf("ОШИБКА: нет памяти\n"); exit(1); }

    /* Инициализируем все коэффициенты нулём (0/1) */
    for (int i = 0; i <= degree; i++) {
        result->C[i].a.b = 0;
        result->C[i].a.n = 1;
        result->C[i].a.A = (int*)calloc(1, sizeof(int));
        result->C[i].b.n = 1;
        result->C[i].b.A = (int*)malloc(sizeof(int));
        if (!result->C[i].a.A || !result->C[i].b.A) {
            printf("ОШИБКА: нет памяти\n"); exit(1);
        }
        result->C[i].b.A[0] = 1;
    }

    printf("Введите ненулевые коэффициенты (формат: степень коэффициент, например: 1000 1)\n");
    printf("Введите пустую строку для завершения.\n");

    char line[4096];
    while (1) {
        printf("  > "); fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;

        int blank = 1;
        for (int i = 0; line[i] && line[i] != '\n'; i++) {
            if (line[i] != ' ' && line[i] != '\t') { blank = 0; break; }
        }
        if (blank) break;

        int power;
        int consumed = 0;
        if (sscanf(line, "%d %n", &power, &consumed) != 1) {
            printf("  ОШИБКА: ожидается формат «степень коэффициент»\n");
            continue;
        }
        if (power < 0 || power > degree) {
            printf("  ОШИБКА: степень должна быть от 0 до %d\n", degree);
            continue;
        }


        char coef_str[4000];
        strncpy(coef_str, line + consumed, sizeof(coef_str) - 1);
        coef_str[sizeof(coef_str) - 1] = '\0';

        size_t L = strlen(coef_str);
        while (L > 0 && (coef_str[L-1] == '\n' || coef_str[L-1] == '\r'))
            coef_str[--L] = '\0';

        if (L == 0) {
            printf("  ОШИБКА: не указан коэффициент\n");
            continue;
        }


        int idx = 0, sign = 0;
        if (coef_str[0] == '+') idx++;
        else if (coef_str[0] == '-') { sign = 1; idx++; }


        int slash = -1;
        for (int i = idx; coef_str[i]; i++) {
            if (coef_str[i] == '/') { slash = i; break; }
        }

        char num_buf[2000] = {0}, den_buf[2000] = "1";
        if (slash == -1) {
            strncpy(num_buf, coef_str + idx, sizeof(num_buf) - 1);
        } else {
            strncpy(num_buf, coef_str + idx, slash - idx);
            num_buf[slash - idx] = '\0';
            strncpy(den_buf, coef_str + slash + 1, sizeof(den_buf) - 1);
        }

        int bad = 0;
        for (int i = 0; num_buf[i]; i++) if (!isdigit((unsigned char)num_buf[i])) { bad=1; break; }
        for (int i = 0; den_buf[i]; i++) if (!isdigit((unsigned char)den_buf[i])) { bad=1; break; }
        if (bad || strlen(num_buf) == 0) {
            printf("  ОШИБКА: некорректный коэффициент\n"); continue;
        }
        if (den_buf[0] == '0' && strlen(den_buf) == 1) {
            printf("  ОШИБКА: знаменатель не может быть 0\n"); continue;
        }

        int ns = 0, ds = 0;
        while (ns < (int)strlen(num_buf)-1 && num_buf[ns]=='0') ns++;
        while (ds < (int)strlen(den_buf)-1 && den_buf[ds]=='0') ds++;

        int nlen = strlen(num_buf) - ns;
        int dlen = strlen(den_buf) - ds;

        free(result->C[power].a.A);
        free(result->C[power].b.A);

        result->C[power].a.b = (nlen==1 && num_buf[ns]=='0') ? 0 : sign;
        result->C[power].a.n = nlen;
        result->C[power].a.A = (int*)malloc(nlen * sizeof(int));
        result->C[power].b.n = dlen;
        result->C[power].b.A = (int*)malloc(dlen * sizeof(int));
        if (!result->C[power].a.A || !result->C[power].b.A) {
            printf("ОШИБКА: нет памяти\n"); exit(1);
        }
        for (int i = 0; i < nlen; i++)
            result->C[power].a.A[i] = num_buf[nlen - 1 - i + ns] - '0';
        for (int i = 0; i < dlen; i++)
            result->C[power].b.A[i] = den_buf[dlen - 1 - i + ds] - '0';
    }

    while (result->m > 0) {
        if (result->C[result->m].a.n == 1 && result->C[result->m].a.A[0] == 0) {
            free(result->C[result->m].a.A);
            free(result->C[result->m].b.A);
            result->m--;
        } else break;
    }

    return result;
}

/* ========== Функции вывода ========== */

void printN(NUMBN* num) {
    if (!num || !num->A || num->n <= 0) { printf("(ошибка)"); return; }
    for (int i = num->n - 1; i >= 0; i--) printf("%d", num->A[i]);
}

void printZ(NUMBZ* num) {
    if (!num || !num->A || num->n <= 0) { printf("(ошибка)"); return; }
    if (num->n == 1 && num->A[0] == 0) { printf("0"); return; }
    if (num->b == 1) printf("-");
    for (int i = num->n - 1; i >= 0; i--) printf("%d", num->A[i]);
}

void printQ(NUMBQ* num) {
    if (!num || !num->a.A || !num->b.A) { 
        printf("(ошибка)"); 
        return; 
    }
    if (num->a.n == 1 && num->a.A[0] == 0) { 
        printf("0"); 
        return; 
    }
    printZ(&num->a);
    if (!(num->b.n == 1 && num->b.A[0] == 1)) {
        printf("/");
        printN(&num->b);
    }
}

void printP(NUMBP* poly) {
    if (poly == NULL || poly->C == NULL || poly->m < 0) {
        printf("ошибка: некорректные данные многочлена");
        return;
    }

    int first = 1;

    for (int i = poly->m; i >= 0; i--) {
        NUMBQ* coeff = &poly->C[i];

        if (coeff->a.A == NULL || coeff->b.A == NULL) {
            printf("ошибка: некорректные данные коэффициента многочлена");
            return;
        }

        if (coeff->a.n == 1 && coeff->a.A[0] == 0)
            continue;

        NUMBQ* reduced = RED_Q_Q(coeff);
        if (!reduced) {
            printf("ошибка: не удалось сократить коэффициент");
            return;
        }

        int is_negative = (reduced->a.b == 1);
        int is_one = (reduced->a.n == 1 && reduced->a.A[0] == 1 &&
                      reduced->b.n == 1 && reduced->b.A[0] == 1);

        if (first) {
            if (is_negative) printf("-");
            first = 0;
        } else {
            printf(is_negative ? " - " : " + ");
        }

        if (!is_one || i == 0) {
            for (int k = reduced->a.n - 1; k >= 0; k--)
                printf("%d", reduced->a.A[k]);
            if (!(reduced->b.n == 1 && reduced->b.A[0] == 1)) {
                printf("/");
                for (int k = reduced->b.n - 1; k >= 0; k--)
                    printf("%d", reduced->b.A[k]);
            }
        }

        if (i == 1)
            printf("x");
        else if (i > 1)
            printf("x^%d", i);

        free(reduced->a.A);
        free(reduced->b.A);
        free(reduced);
    }

    if (first) printf("0");
}


/* ========================================================================
   ИСПРАВЛЕНО: во всех меню двузначные операции теперь читаются как строка
   (не как одиночный char), поэтому scanf читает int, а не char.
   case-ветки заменены на if/else if по числовому значению.
   ======================================================================== */

/* ========== Меню для натуральных чисел ========== */
void menuN(void) {
    printf("\n========== НАТУРАЛЬНЫЕ ЧИСЛА ==========\n");
    printf(" 1 - COM_NN_D   (Сравнение)\n");
    printf(" 2 - NZER_N_B   (Проверка на ноль)\n");
    printf(" 3 - ADD_1N_N   (Добавление 1)\n");
    printf(" 4 - ADD_NN_N   (Сложение)\n");
    printf(" 5 - SUB_NN_N   (Вычитание, a >= b)\n");
    printf(" 6 - MUL_ND_N   (Умножение на цифру)\n");
    printf(" 7 - MUL_Nk_N   (Умножение на 10^k)\n");
    printf(" 8 - MUL_NN_N   (Умножение)\n");
    printf(" 9 - SUB_NDN_N  (Вычитание a - b*d)\n");
    printf("10 - DIV_NN_Dk  (Первая цифра деления)\n");
    printf("11 - DIV_NN_N   (Деление, частное)\n");
    printf("12 - MOD_NN_N   (Деление, остаток)\n");
    printf("13 - GCF_NN_N   (НОД)\n");
    printf("14 - LCM_NN_N   (НОК)\n");
    printf(" 0 - Назад\n");
    printf("> ");

    /* ИСПРАВЛЕНО: читаем int, а не char — иначе двузначные номера недостижимы */
    int op;
    if (scanf("%d", &op) != 1) { printf("ОШИБКА: введите номер операции\n"); exit(1); }
    while (getchar() != '\n');

    if (op == 0) return;

    if (op == 1) {
        NUMBN* a = inputN("Введите первое число:");
        NUMBN* b = inputN("Введите второе число:");
        int cmp = COM_NN_D(a, b);
        if (cmp == -1) { printf("ОШИБКА при сравнении\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        if (cmp == 2)       printf("Результат: первое больше второго\n");
        else if (cmp == 0)  printf("Результат: числа равны\n");
        else                printf("Результат: первое меньше второго\n");
        freeNUMBN(a); freeNUMBN(b);

    } else if (op == 2) {
        NUMBN* a = inputN("Введите число");
        printf("Результат: %s\n", NZER_N_B(a) ? "не ноль" : "ноль");
        freeNUMBN(a);

    } else if (op == 3) {
        NUMBN* a = inputN("Введите число");
        NUMBN* res = ADD_1N_N(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(res);

    } else if (op == 4) {
        NUMBN* a = inputN("Введите первое число");
        NUMBN* b = inputN("Введите второе число");
        NUMBN* res = ADD_NN_N(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else if (op == 5) {
        NUMBN* a = inputN("Введите уменьшаемое");
        NUMBN* b = inputN("Введите вычитаемое");
        int cmp = COM_NN_D(a, b);
        if (cmp == -1) { printf("ОШИБКА при сравнении\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        if (cmp == 1) { printf("ОШИБКА: уменьшаемое меньше вычитаемого\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        NUMBN* res = SUB_NN_N(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else if (op == 6) {
        NUMBN* a = inputN("Введите число");
        int d;
        printf("Введите цифру (0-9): ");
        if (scanf("%d", &d) != 1 || d < 0 || d > 9) {
            printf("ОШИБКА: цифра 0..9\n"); freeNUMBN(a); exit(1);
        }
        while (getchar() != '\n');
        NUMBN* res = MUL_ND_N(a, d);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(res);

    } else if (op == 7) {
        NUMBN* a = inputN("Введите число: ");
        int k;
        printf("Введите k: ");
        if (scanf("%d", &k) != 1 || k < 0) {
            printf("ОШИБКА: k >= 0\n"); freeNUMBN(a); exit(1);
        }
        while (getchar() != '\n');
        NUMBN* res = MUL_Nk_N(a, k);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(res);

    } else if (op == 8) {
        NUMBN* a = inputN("Введите первый множитель: ");
        NUMBN* b = inputN("Введите второй множитель: ");
        NUMBN* res = MUL_NN_N(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else if (op == 9) {
        NUMBN* a = inputN("Введите уменьшаемое (a): ");
        NUMBN* b = inputN("Введите число (b): ");
        int d;
        printf("Введите цифру (0-9): ");
        if (scanf("%d", &d) != 1 || d < 0 || d > 9) {
            printf("ОШИБКА: цифра 0..9\n"); freeNUMBN(a); freeNUMBN(b); exit(1);
        }
        while (getchar() != '\n');
        NUMBN* bd = MUL_ND_N(b, d);
        if (!bd) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        int cmp = COM_NN_D(a, bd);
        freeNUMBN(bd);
        if (cmp == 1) { printf("ОШИБКА: a < b*d\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        NUMBN* res = SUB_NDN_N(a, b, d);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else if (op == 10) {
        NUMBN* a = inputN("Введите делимое: ");
        NUMBN* b = inputN("Введите делитель: ");
        if (isZeroN(b)) { printf("ОШИБКА: деление на ноль\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        int cmp = COM_NN_D(a, b);
        if (cmp == 1) { printf("ОШИБКА: a < b\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        int digit = DIV_NN_Dk(a, b);
        printf("Первая цифра частного: %d\n", digit);
        freeNUMBN(a); freeNUMBN(b);

    } else if (op == 11) {
        NUMBN* a = inputN("Введите делимое: ");
        NUMBN* b = inputN("Введите делитель: ");
        if (isZeroN(b)) { printf("ОШИБКА: деление на ноль\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        NUMBN* res = DIV_NN_N(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("Частное: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else if (op == 12) {
        NUMBN* a = inputN("Введите делимое: ");
        NUMBN* b = inputN("Введите делитель: ");
        if (isZeroN(b)) { printf("ОШИБКА: деление на ноль\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        NUMBN* res = MOD_NN_N(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("Остаток: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else if (op == 13) {
        NUMBN* a = inputN("Введите первое число: ");
        NUMBN* b = inputN("Введите второе число: ");
        if (isZeroN(a) && isZeroN(b)) {
            printf("ОШИБКА: НОД(0,0) не определён\n"); freeNUMBN(a); freeNUMBN(b); exit(1);
        }
        NUMBN* res = GCF_NN_N(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("НОД: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else if (op == 14) {
        NUMBN* a = inputN("Введите первое число: ");
        NUMBN* b = inputN("Введите второе число: ");
        if (isZeroN(a) || isZeroN(b)) {
            printf("Результат (НОК с нулём): 0\n"); freeNUMBN(a); freeNUMBN(b); return;
        }
        NUMBN* res = LCM_NN_N(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); freeNUMBN(b); exit(1); }
        printf("НОК: "); printN(res); printf("\n");
        freeNUMBN(a); freeNUMBN(b); freeNUMBN(res);

    } else {
        printf("ОШИБКА: неверный выбор операции\n");
        exit(1);
    }
}

/* ========== Меню для целых чисел ========== */
void menuZ(void) {
    printf("\n========== ЦЕЛЫЕ ЧИСЛА ==========\n");
    printf(" 1 - ABS_Z_Z   (Абсолютная величина)\n");
    printf(" 2 - POZ_Z_D   (Определение положительности)\n");
    printf(" 3 - MUL_ZM_Z  (Умножение на -1)\n");
    printf(" 4 - TRANS_N_Z (Нат. → целое)\n");
    printf(" 5 - TRANS_Z_N (Целое неотриц. → нат.)\n");
    printf(" 6 - ADD_ZZ_Z  (Сложение)\n");
    printf(" 7 - SUB_ZZ_Z  (Вычитание)\n");
    printf(" 8 - MUL_ZZ_Z  (Умножение)\n");
    printf(" 9 - DIV_ZZ_Z  (Частное)\n");
    printf("10 - MOD_ZZ_Z  (Остаток)\n");
    printf(" 0 - Назад\n");
    printf("> ");

    int op;
    if (scanf("%d", &op) != 1) { printf("ОШИБКА: введите номер\n"); exit(1); }
    while (getchar() != '\n');

    if (op == 0) return;

    if (op == 1) {
        NUMBZ* a = inputZ("Введите целое число: ");
        NUMBZ* res = ABS_Z_Z(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); exit(1); }
        printf("Абсолютная величина: "); printZ(res); printf("\n");
        freeNUMBZ(a); freeNUMBZ(res);

    } else if (op == 2) {
        NUMBZ* a = inputZ("Введите целое число: ");
        int sign = POZ_Z_D(a);
        if (sign == -1) { printf("ОШИБКА\n"); freeNUMBZ(a); exit(1); }
        if (sign == 2)      printf("Результат: положительное\n");
        else if (sign == 0) printf("Результат: ноль\n");
        else                printf("Результат: отрицательное\n");
        freeNUMBZ(a);

    } else if (op == 3) {
        NUMBZ* a = inputZ("Введите целое число: ");
        NUMBZ* res = MUL_ZM_Z(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); exit(1); }
        printf("Результат: "); printZ(res); printf("\n");
        freeNUMBZ(a); freeNUMBZ(res);

    } else if (op == 4) {
        NUMBN* a = inputN("Введите натуральное число: ");
        NUMBZ* res = TRANS_N_Z(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBN(a); exit(1); }
        printf("Результат: "); printZ(res); printf("\n");
        freeNUMBN(a); freeNUMBZ(res);

    } else if (op == 5) {
        NUMBZ* a = inputZ("Введите целое неотрицательное число: ");
        int sign = POZ_Z_D(a);
        if (sign == 1) { printf("ОШИБКА: число отрицательное\n"); freeNUMBZ(a); exit(1); }
        NUMBN* res = TRANS_Z_N(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); exit(1); }
        printf("Результат: "); printN(res); printf("\n");
        freeNUMBZ(a); freeNUMBN(res);

    } else if (op == 6) {
        NUMBZ* a = inputZ("Введите первое число: ");
        NUMBZ* b = inputZ("Введите второе число: ");
        NUMBZ* res = ADD_ZZ_Z(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); freeNUMBZ(b); exit(1); }
        printf("Результат: "); printZ(res); printf("\n");
        freeNUMBZ(a); freeNUMBZ(b); freeNUMBZ(res);

    } else if (op == 7) {
        NUMBZ* a = inputZ("Введите уменьшаемое: ");
        NUMBZ* b = inputZ("Введите вычитаемое: ");
        NUMBZ* res = SUB_ZZ_Z(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); freeNUMBZ(b); exit(1); }
        printf("Результат: "); printZ(res); printf("\n");
        freeNUMBZ(a); freeNUMBZ(b); freeNUMBZ(res);

    } else if (op == 8) {
        NUMBZ* a = inputZ("Введите первый множитель: ");
        NUMBZ* b = inputZ("Введите второй множитель: ");
        NUMBZ* res = MUL_ZZ_Z(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); freeNUMBZ(b); exit(1); }
        printf("Результат: "); printZ(res); printf("\n");
        freeNUMBZ(a); freeNUMBZ(b); freeNUMBZ(res);

    } else if (op == 9) {
        NUMBZ* a = inputZ("Введите делимое: ");
        NUMBZ* b = inputZ("Введите делитель: ");
        if (isZeroBZ(b)) { printf("ОШИБКА: деление на ноль\n"); freeNUMBZ(a); freeNUMBZ(b); exit(1); }
        NUMBZ* res = DIV_ZZ_Z(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); freeNUMBZ(b); exit(1); }
        printf("Частное: "); printZ(res); printf("\n");
        freeNUMBZ(a); freeNUMBZ(b); freeNUMBZ(res);

    } else if (op == 10) {
        NUMBZ* a = inputZ("Введите делимое: ");
        NUMBZ* b = inputZ("Введите делитель: ");
        if (isZeroBZ(b)) { printf("ОШИБКА: деление на ноль\n"); freeNUMBZ(a); freeNUMBZ(b); exit(1); }
        NUMBZ* res = MOD_ZZ_Z(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); freeNUMBZ(b); exit(1); }
        printf("Остаток: "); printZ(res); printf("\n");
        freeNUMBZ(a); freeNUMBZ(b); freeNUMBZ(res);

    } else {
        printf("ОШИБКА: неверный выбор операции\n");
        exit(1);
    }
}

/* ========== Меню для рациональных чисел ========== */
void menuQ(void) {
    printf("\n========== РАЦИОНАЛЬНЫЕ ЧИСЛА ==========\n");
    printf("1 - RED_Q_Q   (Сокращение дроби)\n");
    printf("2 - INT_Q_B   (Проверка на целое)\n");
    printf("3 - TRANS_Z_Q (Целое → дробное)\n");
    printf("4 - TRANS_Q_Z (Дробное → целое)\n");
    printf("5 - ADD_QQ_Q  (Сложение)\n");
    printf("6 - SUB_QQ_Q  (Вычитание)\n");
    printf("7 - MUL_QQ_Q  (Умножение)\n");
    printf("8 - DIV_QQ_Q  (Деление)\n");
    printf("0 - Назад\n");
    printf("> ");

    int op;
    if (scanf("%d", &op) != 1) { printf("ОШИБКА: введите номер\n"); exit(1); }
    while (getchar() != '\n');

    if (op == 0) return;

    if (op == 1) {
        NUMBQ* a = inputQ("Введите дробь: ");
        NUMBQ* res = RED_Q_Q(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBQ(a); exit(1); }
        printf("Сокращённая дробь: "); printQ(res); printf("\n");
        freeNUMBQ(a); freeNUMBQ(res);

    } else if (op == 2) {
        NUMBQ* a = inputQ("Введите дробь: ");
        int r = INT_Q_B(a);
        printf("Результат: %s\n", r == 1 ? "целое" : r == 0 ? "не целое" : "ошибка");
        freeNUMBQ(a);

    } else if (op == 3) {
        NUMBZ* a = inputZ("Введите целое число: ");
        NUMBQ* res = TRANS_Z_Q(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBZ(a); exit(1); }
        printf("Результат: "); printQ(res); printf("\n");
        freeNUMBZ(a); freeNUMBQ(res);

    } else if (op == 4) {
        NUMBQ* a = inputQ("Введите дробь: ");
        NUMBQ* reduced = RED_Q_Q(a);
        if (!reduced) { printf("ОШИБКА\n"); freeNUMBQ(a); exit(1); }
        if (!(reduced->b.n == 1 && reduced->b.A[0] == 1)) {
            printf("ОШИБКА: знаменатель не равен 1\n");
            freeNUMBQ(a); freeNUMBQ(reduced); exit(1);
        }
        NUMBZ* res = TRANS_Q_Z(reduced);
        freeNUMBQ(reduced);
        if (!res) { printf("ОШИБКА\n"); freeNUMBQ(a); exit(1); }
        printf("Результат: "); printZ(res); printf("\n");
        freeNUMBQ(a); freeNUMBZ(res);

    } else if (op == 5) {
        NUMBQ* a = inputQ("Введите первую дробь: ");
        NUMBQ* b = inputQ("Введите вторую дробь: ");
        NUMBQ* res = ADD_QQ_Q(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBQ(a); freeNUMBQ(b); exit(1); }
        printf("Результат: "); printQ(res); printf("\n");
        freeNUMBQ(a); freeNUMBQ(b); freeNUMBQ(res);

    } else if (op == 6) {
        NUMBQ* a = inputQ("Введите уменьшаемое: ");
        NUMBQ* b = inputQ("Введите вычитаемое: ");
        NUMBQ* res = SUB_QQ_Q(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBQ(a); freeNUMBQ(b); exit(1); }
        printf("Результат: "); printQ(res); printf("\n");
        freeNUMBQ(a); freeNUMBQ(b); freeNUMBQ(res);

    } else if (op == 7) {
        NUMBQ* a = inputQ("Введите первую дробь: ");
        NUMBQ* b = inputQ("Введите вторую дробь: ");
        NUMBQ* res = MUL_QQ_Q(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBQ(a); freeNUMBQ(b); exit(1); }
        printf("Результат: "); printQ(res); printf("\n");
        freeNUMBQ(a); freeNUMBQ(b); freeNUMBQ(res);

    } else if (op == 8) {
        NUMBQ* a = inputQ("Введите делимое: ");
        NUMBQ* b = inputQ("Введите делитель: ");
        if (isZeroBZ(&b->a)) {
            printf("ОШИБКА: делитель равен нулю\n"); freeNUMBQ(a); freeNUMBQ(b); exit(1);
        }
        NUMBQ* res = DIV_QQ_Q(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBQ(a); freeNUMBQ(b); exit(1); }
        printf("Результат: "); printQ(res); printf("\n");
        freeNUMBQ(a); freeNUMBQ(b); freeNUMBQ(res);

    } else {
        printf("ОШИБКА: неверный выбор операции\n");
        exit(1);
    }
}

/* ========== Меню для многочленов ========== */
void menuP(void) {
    printf("\n========== МНОГОЧЛЕНЫ ==========\n");
    printf(" 1 - ADD_PP_P   (Сложение)\n");
    printf(" 2 - SUB_PP_P   (Вычитание)\n");
    printf(" 3 - MUL_PQ_P   (Умножение на рац. число)\n");
    printf(" 4 - MUL_Pxk_P  (Умножение на x^k)\n");
    printf(" 5 - LED_P_Q    (Старший коэффициент)\n");
    printf(" 6 - DEG_P_N    (Степень)\n");
    printf(" 7 - FAC_P_Q    (НОД числит. / НОК знам.)\n");
    printf(" 8 - MUL_PP_P   (Умножение многочленов)\n");
    printf(" 9 - DIV_PP_P   (Частное)\n");
    printf("10 - MOD_PP_P   (Остаток)\n");
    printf("11 - GCF_PP_P   (НОД многочленов)\n");
    printf("12 - DER_P_P    (Производная)\n");
    printf("13 - NMR_P_P    (Кратные корни → простые)\n");
    printf(" 0 - Назад\n");
    printf("> ");

    int op;
    if (scanf("%d", &op) != 1) { printf("ОШИБКА: введите номер\n"); exit(1); }
    while (getchar() != '\n');

    if (op == 0) return;

    if (op == 1) {
        NUMBP* a = inputP("Введите первый многочлен: ");
        NUMBP* b = inputP("Введите второй многочлен: ");
        NUMBP* res = ADD_PP_P(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); freeNUMBP(b); exit(1); }
        printf("Результат: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(b); freeNUMBP(res);

    } else if (op == 2) {
        NUMBP* a = inputP("Введите уменьшаемое: ");
        NUMBP* b = inputP("Введите вычитаемое: ");
        NUMBP* res = SUB_PP_P(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); freeNUMBP(b); exit(1); }
        printf("Результат: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(b); freeNUMBP(res);

    } else if (op == 3) {
        NUMBP* a = inputP("Введите многочлен: ");
        NUMBQ* b = inputQ("Введите рациональное число: ");
        NUMBP* res = MUL_PQ_P(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); freeNUMBQ(b); exit(1); }
        printf("Результат: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBQ(b); freeNUMBP(res);

    } else if (op == 4) {
        NUMBP* a = inputP("Введите многочлен: ");
        int k;
        printf("Введите k: ");
        if (scanf("%d", &k) != 1) {
            printf("ОШИБКA\n"); freeNUMBP(a); exit(1);
        }
        while (getchar() != '\n');
        NUMBP* res = MUL_Pxk_P(a, k);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); exit(1); }
        printf("Результат: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(res);

    } else if (op == 5) {
        NUMBP* a = inputP("Введите многочлен: ");
        NUMBQ* res = LED_P_Q(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); exit(1); }
        printf("Старший коэффициент: "); printQ(res); printf("\n");
        freeNUMBP(a); freeNUMBQ(res);

    } else if (op == 6) {
        NUMBP* a = inputP("Введите многочлен: ");
        NUMBN* res = DEG_P_N(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); exit(1); }
        printf("Степень: "); printN(res); printf("\n");
        freeNUMBP(a); freeNUMBN(res);

    } else if (op == 7) {
        NUMBP* a = inputP("Введите многочлен: ");
        NUMBQ* res = FAC_P_Q(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); exit(1); }
        printf("Вынесенный множитель: "); printQ(res); printf("\n");
        freeNUMBP(a); freeNUMBQ(res);

    } else if (op == 8) {
        NUMBP* a = inputP("Введите первый многочлен: ");
        NUMBP* b = inputP("Введите второй многочлен: ");
        NUMBP* res = MUL_PP_P(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); freeNUMBP(b); exit(1); }
        printf("Результат: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(b); freeNUMBP(res);

    } else if (op == 9) {
        NUMBP* a = inputP("Введите делимое: ");
        NUMBP* b = inputP("Введите делитель: ");
        NUMBP* res = DIV_PP_P(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); freeNUMBP(b); exit(1); }
        printf("Частное: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(b); freeNUMBP(res);

    } else if (op == 10) {
        NUMBP* a = inputP("Введите делимое: ");
        NUMBP* b = inputP("Введите делитель: ");
        NUMBP* res = MOD_PP_P(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); freeNUMBP(b); exit(1); }
        printf("Остаток: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(b); freeNUMBP(res);

    } else if (op == 11) {
        NUMBP* a = inputP("Введите первый многочлен: ");
        NUMBP* b = inputP("Введите второй многочлен: ");
        NUMBP* res = GCF_PP_P(a, b);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); freeNUMBP(b); exit(1); }
        printf("НОД: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(b); freeNUMBP(res);

    } else if (op == 12) {
        NUMBP* a = inputP("Введите многочлен: ");
        NUMBP* res = DER_P_P(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); exit(1); }
        printf("Производная: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(res);

    } else if (op == 13) {
        NUMBP* a = inputP("Введите многочлен: ");
        NUMBP* res = NMR_P_P(a);
        if (!res) { printf("ОШИБКА\n"); freeNUMBP(a); exit(1); }
        printf("Кратные корни → простые: "); printP(res); printf("\n");
        freeNUMBP(a); freeNUMBP(res);

    } else {
        printf("ОШИБКА: неверный выбор операции\n");
        exit(1);
    }
}

