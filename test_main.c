#include "include_DM.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ======================================== */
/* ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ */
/* ======================================== */

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define CHECK(label, cond) \
    do { \
        if (cond) { printf("  [OK]   %s\n", label); g_tests_passed++; } \
        else      { printf("  [FAIL] %s\n", label); g_tests_failed++; } \
    } while(0)

double measure_time(clock_t start, clock_t end)
{
    return ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
}

/* Создать натуральное число из строки цифр */
NUMBN makeN(const char* str)
{
    NUMBN n;
    n.n = 0;
    while (str[n.n] != '\0') n.n++;
    n.A = (int*)malloc(sizeof(int) * n.n);
    for (int i = 0; i < n.n; i++)
        n.A[i] = str[n.n - 1 - i] - '0';
    return n;
}

/* Создать целое число из строки (со знаком '-' опционально) */
NUMBZ makeZ(const char* str)
{
    NUMBZ z;
    if (str[0] == '-') { z.b = 1; str++; }
    else               { z.b = 0; }
    z.n = 0;
    while (str[z.n] != '\0') z.n++;
    z.A = (int*)malloc(sizeof(int) * z.n);
    for (int i = 0; i < z.n; i++)
        z.A[i] = str[z.n - 1 - i] - '0';
    return z;
}

/* Создать рациональное число p/q из строк */
NUMBQ makeQ(const char* num_str, const char* den_str)
{
    NUMBQ q;
    NUMBZ n = makeZ(num_str);
    NUMBN d = makeN(den_str);
    q.a = n;
    q.b = d;
    return q;
}

static NUMBQ makeIntCoef(int val)
{
    NUMBQ q;
    q.a.b = (val < 0) ? 1 : 0;
    int av = (val < 0) ? -val : val;
    q.a.n = 1; q.a.A = (int*)malloc(sizeof(int)); q.a.A[0] = av;
    q.b.n = 1; q.b.A = (int*)malloc(sizeof(int)); q.b.A[0] = 1;
    return q;
}

/* Освободить NUMBN на стеке */
void freeNt(NUMBN* n) { free(n->A); }
void freeZt(NUMBZ* z) { free(z->A); }
void freeQt(NUMBQ* q) { free(q->a.A); free(q->b.A); }

/* Освободить NUMBP (выделенный через malloc) */
static void freePoly(NUMBP* p)
{
    if (!p) return;
    for (int i = 0; i <= p->m; i++) {
        free(p->C[i].a.A);
        free(p->C[i].b.A);
    }
    free(p->C);
    free(p);
}

/* Вывод натурального числа */
void printNt(NUMBN* n)
{
    for (int i = n->n - 1; i >= 0; i--) printf("%d", n->A[i]);
}

/* Вывод целого числа */
void printZt(NUMBZ* z)
{
    if (z->b) printf("-");
    for (int i = z->n - 1; i >= 0; i--) printf("%d", z->A[i]);
}

/* Вывод рационального числа */
static void printQt(NUMBQ* q)
{
    if (!q) { printf("NULL"); return; }
    if (q->a.n == 1 && q->a.A[0] == 0) { printf("0"); return; }
    if (q->a.b) printf("-");
    for (int i = q->a.n - 1; i >= 0; i--) printf("%d", q->a.A[i]);
    if (!(q->b.n == 1 && q->b.A[0] == 1)) {
        printf("/");
        for (int i = q->b.n - 1; i >= 0; i--) printf("%d", q->b.A[i]);
    }
}

/* Вывод многочлена */
static void printPoly(NUMBP* p)
{
    if (!p) { printf("NULL"); return; }
    int first = 1;
    for (int i = p->m; i >= 0; i--) {
        NUMBQ* c = &p->C[i];
        if (c->a.n == 1 && c->a.A[0] == 0) continue;
        if (!first) printf(" + ");
        else first = 0;
        printQt(c);
        if (i == 1) printf("x");
        else if (i > 1) printf("x^%d", i);
    }
    if (first) printf("0");
}

/* ── Сравнение результата с ожидаемой строкой ── */
static int N_equals_str(NUMBN* n, const char* expected)
{
    if (!n) return 0;
    NUMBN e = makeN(expected);
    if (n->n != e.n) { freeNt(&e); return 0; }
    for (int i = 0; i < n->n; i++)
        if (n->A[i] != e.A[i]) { freeNt(&e); return 0; }
    freeNt(&e);
    return 1;
}

static int Z_equals_str(NUMBZ* z, const char* expected)
{
    if (!z) return 0;
    NUMBZ e = makeZ(expected);
    int ok = (z->b == e.b && z->n == e.n);
    if (ok)
        for (int i = 0; i < z->n; i++)
            if (z->A[i] != e.A[i]) { ok = 0; break; }
    freeZt(&e);
    return ok;
}

/* Сравнение рациональных чисел (числитель/знаменатель буквально) */
static int Q_equals(NUMBQ* q, const char* num_s, const char* den_s)
{
    if (!q) return 0;
    NUMBZ en = makeZ(num_s);
    NUMBN ed = makeN(den_s);
    int ok = (q->a.b == en.b && q->a.n == en.n && q->b.n == ed.n);
    if (ok)
        for (int i = 0; i < q->a.n && ok; i++)
            if (q->a.A[i] != en.A[i]) ok = 0;
    if (ok)
        for (int i = 0; i < q->b.n && ok; i++)
            if (q->b.A[i] != ed.A[i]) ok = 0;
    freeZt(&en); freeNt(&ed);
    return ok;
}

/* ======================================== */
/* ТЕСТЫ НАТУРАЛЬНЫХ ЧИСЕЛ */
/* ======================================== */

static void test_N(void)
{
    printf("\n=== НАТУРАЛЬНЫЕ ЧИСЛА (N) ===\n");
    clock_t t0, t1;

    /* COM_NN_D */
    {
        NUMBN a = makeN("12345"), b = makeN("6789"), c = makeN("12345");
        CHECK("COM_NN_D: 12345 > 6789",  COM_NN_D(&a, &b) == 2);
        CHECK("COM_NN_D: 6789 < 12345",  COM_NN_D(&b, &a) == 1);
        CHECK("COM_NN_D: 12345 == 12345", COM_NN_D(&a, &c) == 0);
        freeNt(&a); freeNt(&b); freeNt(&c);
    }

    /* NZER_N_B */
    {
        NUMBN nz = makeN("0"), pos = makeN("42");
        CHECK("NZER_N_B: 0 → false",  !NZER_N_B(&nz));
        CHECK("NZER_N_B: 42 → true",   NZER_N_B(&pos));
        freeNt(&nz); freeNt(&pos);
    }

    /* ADD_1N_N */
    {
        NUMBN a = makeN("999");
        t0 = clock();
        NUMBN* r = ADD_1N_N(&a);
        t1 = clock();
        printf("  ADD_1N_N(999) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("ADD_1N_N: 999+1=1000", N_equals_str(r, "1000"));
        free(r->A); free(r); freeNt(&a);
    }

    /* ADD_NN_N */
    {
        NUMBN a = makeN("12345"), b = makeN("6789");
        t0 = clock();
        NUMBN* r = ADD_NN_N(&a, &b);
        t1 = clock();
        printf("  ADD_NN_N(12345, 6789) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("ADD_NN_N: 12345+6789=19134", N_equals_str(r, "19134"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }

    /* SUB_NN_N */
    {
        NUMBN a = makeN("12345"), b = makeN("6789");
        t0 = clock();
        NUMBN* r = SUB_NN_N(&a, &b);
        t1 = clock();
        printf("  SUB_NN_N(12345, 6789) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("SUB_NN_N: 12345-6789=5556", N_equals_str(r, "5556"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }

    /* MUL_ND_N */
    {
        NUMBN a = makeN("12345");
        NUMBN* r = MUL_ND_N(&a, 3);
        CHECK("MUL_ND_N: 12345*3=37035", N_equals_str(r, "37035"));
        NUMBN* r0 = MUL_ND_N(&a, 0);
        CHECK("MUL_ND_N: 12345*0=0", N_equals_str(r0, "0"));
        free(r->A); free(r); free(r0->A); free(r0); freeNt(&a);
    }

    /* MUL_Nk_N */
    {
        NUMBN a = makeN("123");
        NUMBN* r = MUL_Nk_N(&a, 3);
        CHECK("MUL_Nk_N: 123*10^3=123000", N_equals_str(r, "123000"));
        free(r->A); free(r); freeNt(&a);
    }

    /* MUL_NN_N */
    {
        NUMBN a = makeN("12345"), b = makeN("6789");
        t0 = clock();
        NUMBN* r = MUL_NN_N(&a, &b);
        t1 = clock();
        printf("  MUL_NN_N(12345, 6789) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("MUL_NN_N: 12345*6789=83810205", N_equals_str(r, "83810205"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }

    /* SUB_NDN_N */
    {
        NUMBN a = makeN("100"), b = makeN("12");
        NUMBN* r = SUB_NDN_N(&a, &b, 8); /* 100 - 12*8 = 100 - 96 = 4 */
        CHECK("SUB_NDN_N: 100 - 12*8 = 4", N_equals_str(r, "4"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }

    /* DIV_NN_Dk */
    {
        NUMBN a = makeN("12345"), b = makeN("6789");
        int q = DIV_NN_Dk(&a, &b);
        CHECK("DIV_NN_Dk: first digit of 12345/6789 == 1", q == 1);
        freeNt(&a); freeNt(&b);
    }

    /* DIV_NN_N */
    {
        NUMBN a = makeN("12345"), b = makeN("6789");
        t0 = clock();
        NUMBN* r = DIV_NN_N(&a, &b);
        t1 = clock();
        printf("  DIV_NN_N(12345, 6789) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("DIV_NN_N: 12345 div 6789 = 1", N_equals_str(r, "1"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }

    /* MOD_NN_N */
    {
        NUMBN a = makeN("12345"), b = makeN("6789");
        t0 = clock();
        NUMBN* r = MOD_NN_N(&a, &b);
        t1 = clock();
        printf("  MOD_NN_N(12345, 6789) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("MOD_NN_N: 12345 mod 6789 = 5556", N_equals_str(r, "5556"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }

    /* GCF_NN_N */
    {
        NUMBN a = makeN("48"), b = makeN("18");
        t0 = clock();
        NUMBN* r = GCF_NN_N(&a, &b);
        t1 = clock();
        printf("  GCF_NN_N(48, 18) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("GCF_NN_N(48, 18) = 6", N_equals_str(r, "6"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }

    /* LCM_NN_N */
    {
        NUMBN a = makeN("4"), b = makeN("6");
        t0 = clock();
        NUMBN* r = LCM_NN_N(&a, &b);
        t1 = clock();
        printf("  LCM_NN_N(4, 6) = "); printNt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("LCM_NN_N(4, 6) = 12", N_equals_str(r, "12"));
        free(r->A); free(r); freeNt(&a); freeNt(&b);
    }
}

/* ======================================== */
/* ТЕСТЫ ЦЕЛЫХ ЧИСЕЛ */
/* ======================================== */

static void test_Z(void)
{
    printf("\n=== ЦЕЛЫЕ ЧИСЛА (Z) ===\n");
    clock_t t0, t1;

    /* ABS_Z_Z */
    {
        NUMBZ a = makeZ("-12345");
        t0 = clock();
        NUMBZ* r = ABS_Z_Z(&a);
        t1 = clock();
        printf("  ABS_Z_Z(-12345) = "); printZt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("ABS_Z_Z(-12345) = 12345", Z_equals_str(r, "12345"));
        free(r->A); free(r); freeZt(&a);
    }

    /* POZ_Z_D */
    {
        NUMBZ pos = makeZ("5"), neg = makeZ("-3"), zer = makeZ("0");
        CHECK("POZ_Z_D(5)  = 2 (положит.)",  POZ_Z_D(&pos) == 2);
        CHECK("POZ_Z_D(-3) = 1 (отриц.)",     POZ_Z_D(&neg) == 1);
        CHECK("POZ_Z_D(0)  = 0 (ноль)",        POZ_Z_D(&zer) == 0);
        freeZt(&pos); freeZt(&neg); freeZt(&zer);
    }

    /* MUL_ZM_Z */
    {
        NUMBZ a = makeZ("42"), b = makeZ("-7"), z = makeZ("0");
        NUMBZ* r1 = MUL_ZM_Z(&a);
        NUMBZ* r2 = MUL_ZM_Z(&b);
        NUMBZ* r3 = MUL_ZM_Z(&z);
        CHECK("MUL_ZM_Z(42) = -42",  Z_equals_str(r1, "-42"));
        CHECK("MUL_ZM_Z(-7) = 7",    Z_equals_str(r2, "7"));
        CHECK("MUL_ZM_Z(0) = 0",     Z_equals_str(r3, "0"));
        free(r1->A); free(r1); free(r2->A); free(r2); free(r3->A); free(r3);
        freeZt(&a); freeZt(&b); freeZt(&z);
    }

    /* TRANS_N_Z */
    {
        NUMBN n = makeN("999");
        NUMBZ* r = TRANS_N_Z(&n);
        CHECK("TRANS_N_Z(999) = +999", Z_equals_str(r, "999") && r->b == 0);
        free(r->A); free(r); freeNt(&n);
    }

    /* TRANS_Z_N */
    {
        NUMBZ z = makeZ("42");
        NUMBN* r = TRANS_Z_N(&z);
        CHECK("TRANS_Z_N(42) nat = 42", N_equals_str(r, "42"));
        NUMBZ zn = makeZ("-5");
        NUMBN* rn = TRANS_Z_N(&zn);
        CHECK("TRANS_Z_N(-5) = NULL",   rn == NULL);
        free(r->A); free(r); freeZt(&z); freeZt(&zn);
    }

    /* ADD_ZZ_Z */
    {
        NUMBZ a = makeZ("-12345"), b = makeZ("6789");
        t0 = clock();
        NUMBZ* r = ADD_ZZ_Z(&a, &b);
        t1 = clock();
        printf("  ADD_ZZ_Z(-12345, 6789) = "); printZt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("ADD_ZZ_Z(-12345+6789=-5556)", Z_equals_str(r, "-5556"));
        free(r->A); free(r); freeZt(&a); freeZt(&b);
    }

    /* SUB_ZZ_Z */
    {
        NUMBZ a = makeZ("-12345"), b = makeZ("6789");
        t0 = clock();
        NUMBZ* r = SUB_ZZ_Z(&a, &b);
        t1 = clock();
        printf("  SUB_ZZ_Z(-12345, 6789) = "); printZt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("SUB_ZZ_Z(-12345-6789=-19134)", Z_equals_str(r, "-19134"));
        free(r->A); free(r); freeZt(&a); freeZt(&b);
    }

    /* MUL_ZZ_Z */
    {
        NUMBZ a = makeZ("-12345"), b = makeZ("6789");
        t0 = clock();
        NUMBZ* r = MUL_ZZ_Z(&a, &b);
        t1 = clock();
        printf("  MUL_ZZ_Z(-12345, 6789) = "); printZt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("MUL_ZZ_Z(-12345*6789=-83810205)", Z_equals_str(r, "-83810205"));
        free(r->A); free(r); freeZt(&a); freeZt(&b);
    }

    /* DIV_ZZ_Z */
    {
        NUMBZ a = makeZ("-12345"), b = makeZ("6789");
        t0 = clock();
        NUMBZ* r = DIV_ZZ_Z(&a, &b);
        t1 = clock();
        printf("  DIV_ZZ_Z(-12345, 6789) = "); printZt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        /* floor(-12345/6789) = floor(-1.818...) = -2 */
        CHECK("DIV_ZZ_Z(-12345/6789) = -2", Z_equals_str(r, "-2"));
        free(r->A); free(r); freeZt(&a); freeZt(&b);
    }

    /* MOD_ZZ_Z */
    {
        NUMBZ a = makeZ("-12345"), b = makeZ("6789");
        t0 = clock();
        NUMBZ* r = MOD_ZZ_Z(&a, &b);
        t1 = clock();
        printf("  MOD_ZZ_Z(-12345, 6789) = "); printZt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        /* -12345 - (-2)*6789 = -12345 + 13578 = 1233 */
        CHECK("MOD_ZZ_Z(-12345 mod 6789) = 1233", Z_equals_str(r, "1233"));
        free(r->A); free(r); freeZt(&a); freeZt(&b);
    }
}

/* ======================================== */
/* ТЕСТЫ РАЦИОНАЛЬНЫХ ЧИСЕЛ */
/* ======================================== */

static void test_Q(void)
{
    printf("\n=== РАЦИОНАЛЬНЫЕ ЧИСЛА (Q) ===\n");
    clock_t t0, t1;

    /* RED_Q_Q */
    {
        NUMBQ q = makeQ("6", "9");          /* 6/9 → 2/3 */
        t0 = clock();
        NUMBQ* r = RED_Q_Q(&q);
        t1 = clock();
        printf("  RED_Q_Q(6/9) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("RED_Q_Q(6/9) = 2/3",  Q_equals(r, "2", "3"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&q);
    }
    {
        NUMBQ q = makeQ("-12", "8");        /* -12/8 → -3/2 */
        NUMBQ* r = RED_Q_Q(&q);
        CHECK("RED_Q_Q(-12/8) = -3/2", Q_equals(r, "-3", "2"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&q);
    }
    {
        NUMBQ q = makeQ("0", "7");          /* 0/7 → 0/1 */
        NUMBQ* r = RED_Q_Q(&q);
        CHECK("RED_Q_Q(0/7) = 0/1", Q_equals(r, "0", "1"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&q);
    }

    /* INT_Q_B */
    {
        NUMBQ qi = makeQ("6", "1");
        NUMBQ qf = makeQ("1", "3");
        NUMBQ q0 = makeQ("0", "5");
        CHECK("INT_Q_B(6/1) = 1 (целое)",    INT_Q_B(&qi) == 1);
        CHECK("INT_Q_B(1/3) = 0 (не целое)", INT_Q_B(&qf) == 0);
        CHECK("INT_Q_B(0/5) = 0 (не целое)", INT_Q_B(&q0) == 0);
        freeQt(&qi); freeQt(&qf); freeQt(&q0);
    }

    /* TRANS_Z_Q */
    {
        NUMBZ z = makeZ("-7");
        t0 = clock();
        NUMBQ* r = TRANS_Z_Q(&z);
        t1 = clock();
        printf("  TRANS_Z_Q(-7) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("TRANS_Z_Q(-7) = -7/1", Q_equals(r, "-7", "1"));
        free(r->a.A); free(r->b.A); free(r); freeZt(&z);
    }

    /* TRANS_Q_Z */
    {
        NUMBQ q = makeQ("5", "1");
        t0 = clock();
        NUMBZ* r = TRANS_Q_Z(&q);
        t1 = clock();
        printf("  TRANS_Q_Z(5/1) = "); printZt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("TRANS_Q_Z(5/1) = 5",  Z_equals_str(r, "5"));
        free(r->A); free(r); freeQt(&q);
    }
    {
        NUMBQ q = makeQ("2", "3");
        NUMBZ* r = TRANS_Q_Z(&q);
        CHECK("TRANS_Q_Z(2/3) = NULL (знам≠1)", r == NULL);
        freeQt(&q);
    }

    /* ADD_QQ_Q */
    {
        NUMBQ a = makeQ("1", "3"), b = makeQ("1", "6");   /* 1/3 + 1/6 = 3/6 = 1/2 */
        t0 = clock();
        NUMBQ* r = ADD_QQ_Q(&a, &b);
        t1 = clock();
        printf("  ADD_QQ_Q(1/3, 1/6) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("ADD_QQ_Q(1/3+1/6) = 1/2", Q_equals(r, "1", "2"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }
    {
        NUMBQ a = makeQ("-3", "4"), b = makeQ("1", "4");  /* -3/4 + 1/4 = -1/2 */
        NUMBQ* r = ADD_QQ_Q(&a, &b);
        CHECK("ADD_QQ_Q(-3/4+1/4) = -1/2", Q_equals(r, "-1", "2"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }

    /* SUB_QQ_Q */
    {
        NUMBQ a = makeQ("3", "4"), b = makeQ("1", "4");   /* 3/4 - 1/4 = 2/4 = 1/2 */
        t0 = clock();
        NUMBQ* r = SUB_QQ_Q(&a, &b);
        t1 = clock();
        printf("  SUB_QQ_Q(3/4, 1/4) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("SUB_QQ_Q(3/4-1/4) = 1/2", Q_equals(r, "1", "2"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }
    {
        NUMBQ a = makeQ("1", "2"), b = makeQ("2", "3");   /* 1/2 - 2/3 = 3/6 - 4/6 = -1/6 */
        NUMBQ* r = SUB_QQ_Q(&a, &b);
        CHECK("SUB_QQ_Q(1/2-2/3) = -1/6", Q_equals(r, "-1", "6"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }

    /* MUL_QQ_Q */
    {
        NUMBQ a = makeQ("2", "3"), b = makeQ("3", "4");   /* 2/3 * 3/4 = 6/12 = 1/2 */
        t0 = clock();
        NUMBQ* r = MUL_QQ_Q(&a, &b);
        t1 = clock();
        printf("  MUL_QQ_Q(2/3, 3/4) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("MUL_QQ_Q(2/3*3/4) = 1/2", Q_equals(r, "1", "2"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }
    {
        NUMBQ a = makeQ("-2", "5"), b = makeQ("5", "6");  /* -2/5 * 5/6 = -10/30 = -1/3 */
        NUMBQ* r = MUL_QQ_Q(&a, &b);
        CHECK("MUL_QQ_Q(-2/5*5/6) = -1/3", Q_equals(r, "-1", "3"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }

    /* DIV_QQ_Q */
    {
        NUMBQ a = makeQ("2", "3"), b = makeQ("4", "9");   /* 2/3 / 4/9 = 2/3 * 9/4 = 18/12 = 3/2 */
        t0 = clock();
        NUMBQ* r = DIV_QQ_Q(&a, &b);
        t1 = clock();
        printf("  DIV_QQ_Q(2/3, 4/9) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("DIV_QQ_Q(2/3 / 4/9) = 3/2", Q_equals(r, "3", "2"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }
    {
        NUMBQ a = makeQ("-1", "2"), b = makeQ("3", "4");  /* -1/2 / 3/4 = -4/6 = -2/3 */
        NUMBQ* r = DIV_QQ_Q(&a, &b);
        CHECK("DIV_QQ_Q(-1/2 / 3/4) = -2/3", Q_equals(r, "-2", "3"));
        free(r->a.A); free(r->b.A); free(r); freeQt(&a); freeQt(&b);
    }
}

/* ======================================== */
/* ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ МНОГОЧЛЕНОВ */
/* ======================================== */

/*
 * Создаёт многочлен по массиву числителей (int) с знаменателем 1.
 * coeffs[0] = свободный член, coeffs[deg] = старший.
 */
static NUMBP* makePoly(int deg, const int* coeffs)
{
    NUMBP* p = (NUMBP*)malloc(sizeof(NUMBP));
    if (!p) return NULL;
    p->m = deg;
    p->C = (NUMBQ*)malloc((deg + 1) * sizeof(NUMBQ));
    if (!p->C) { free(p); return NULL; }
    for (int i = 0; i <= deg; i++) {
        p->C[i] = makeIntCoef(coeffs[i]);
    }
    return p;
}

/* Сравнение коэффициента с целым */
static int coef_eq(NUMBQ* q, int val)
{
    if (!q) return 0;
    int s = (val < 0) ? 1 : 0;
    int av = (val < 0) ? -val : val;
    if (q->a.b != s) return 0;
    if (q->a.n != 1) return 0;
    if (q->a.A[0] != av) return 0;
    if (q->b.n != 1 || q->b.A[0] != 1) return 0;
    return 1;
}

/* ======================================== */
/* ТЕСТЫ МНОГОЧЛЕНОВ */
/* ======================================== */

static void test_P(void)
{
    printf("\n=== МНОГОЧЛЕНЫ (P) ===\n");
    clock_t t0, t1;

    /* ADD_PP_P: (x^2 + 2x + 1) + (x^2 - 1) = 2x^2 + 2x */
    {
        int c1[] = {1, 2, 1};    /* 1 + 2x + x^2 */
        int c2[] = {-1, 0, 1};   /* -1 + x^2 */
        NUMBP* a = makePoly(2, c1);
        NUMBP* b = makePoly(2, c2);
        t0 = clock();
        NUMBP* r = ADD_PP_P(a, b);
        t1 = clock();
        printf("  ADD_PP_P(x^2+2x+1, x^2-1) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("ADD: C[0]=0", r && coef_eq(&r->C[0], 0));
        CHECK("ADD: C[1]=2", r && coef_eq(&r->C[1], 2));
        CHECK("ADD: C[2]=2", r && coef_eq(&r->C[2], 2));
        freePoly(a); freePoly(b); freePoly(r);
    }

    /* SUB_PP_P: (x^2 + 2x + 1) - (x^2 - 1) = 2x + 2 */
    {
        int c1[] = {1, 2, 1};
        int c2[] = {-1, 0, 1};
        NUMBP* a = makePoly(2, c1);
        NUMBP* b = makePoly(2, c2);
        t0 = clock();
        NUMBP* r = SUB_PP_P(a, b);
        t1 = clock();
        printf("  SUB_PP_P(x^2+2x+1, x^2-1) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("SUB: степень = 1",  r && r->m == 1);
        CHECK("SUB: C[0] = 2",     r && coef_eq(&r->C[0], 2));
        CHECK("SUB: C[1] = 2",     r && coef_eq(&r->C[1], 2));
        freePoly(a); freePoly(b); freePoly(r);
    }

    /* MUL_PQ_P: (x + 1) * (1/2) = (1/2)x + 1/2 */
    {
        int c[] = {1, 1};
        NUMBP* p = makePoly(1, c);
        NUMBQ q = makeQ("1", "2");
        t0 = clock();
        NUMBP* r = MUL_PQ_P(p, &q);
        t1 = clock();
        printf("  MUL_PQ_P(x+1, 1/2) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("MUL_PQ: deg = 1", r && r->m == 1);
        /* C[0] = 1/2, C[1] = 1/2 */
        CHECK("MUL_PQ: C[0] num=1 den=2",
              r && r->C[0].a.n == 1 && r->C[0].a.A[0] == 1 &&
              r->C[0].b.n == 1 && r->C[0].b.A[0] == 2);
        freePoly(p); freePoly(r); freeQt(&q);
    }

    /* MUL_Pxk_P: (x + 1) * x^2 = x^3 + x^2 */
    {
        int c[] = {1, 1};
        NUMBP* p = makePoly(1, c);
        t0 = clock();
        NUMBP* r = MUL_Pxk_P(p, 2);
        t1 = clock();
        printf("  MUL_Pxk_P(x+1, 2) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("MUL_Pxk: deg = 3", r && r->m == 3);
        CHECK("MUL_Pxk: C[0]=0", r && coef_eq(&r->C[0], 0));
        CHECK("MUL_Pxk: C[2]=1", r && coef_eq(&r->C[2], 1));
        CHECK("MUL_Pxk: C[3]=1", r && coef_eq(&r->C[3], 1));
        freePoly(p); freePoly(r);
    }

    /* LED_P_Q: старший коэффициент 3x^2 + 2x - 1 = 3 */
    {
        int c[] = {-1, 2, 3};
        NUMBP* p = makePoly(2, c);
        t0 = clock();
        NUMBQ* r = LED_P_Q(p);
        t1 = clock();
        printf("  LED_P_Q(3x^2+2x-1) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("LED: = 3/1", r && r->a.n == 1 && r->a.A[0] == 3 && r->b.A[0] == 1 && r->a.b == 0);
        free(r->a.A); free(r->b.A); free(r); freePoly(p);
    }

    /* FAC_P_Q: 6x^2 + 4x + 2  → НОД числ. = 2, НОК знам. = 1 → 2/1 */
    {
        int c[] = {2, 4, 6};
        NUMBP* p = makePoly(2, c);
        t0 = clock();
        NUMBQ* r = FAC_P_Q(p);
        t1 = clock();
        printf("  FAC_P_Q(6x^2+4x+2) = "); printQt(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("FAC: числитель = 2", r && r->a.n == 1 && r->a.A[0] == 2 && r->a.b == 0);
        CHECK("FAC: знаменатель = 1", r && r->b.n == 1 && r->b.A[0] == 1);
        free(r->a.A); free(r->b.A); free(r); freePoly(p);
    }

    /* MUL_PP_P: (x+1)*(x-1) = x^2 - 1 */
    {
        int c1[] = {-1, 1};   /* x - 1 */
        int c2[] = { 1, 1};   /* x + 1 */
        NUMBP* a = makePoly(1, c1);
        NUMBP* b = makePoly(1, c2);
        t0 = clock();
        NUMBP* r = MUL_PP_P(a, b);
        t1 = clock();
        printf("  MUL_PP_P(x-1, x+1) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("MUL_PP: deg = 2",   r && r->m == 2);
        CHECK("MUL_PP: C[0] = -1", r && coef_eq(&r->C[0], -1));
        CHECK("MUL_PP: C[1] = 0",  r && coef_eq(&r->C[1],  0));
        CHECK("MUL_PP: C[2] = 1",  r && coef_eq(&r->C[2],  1));
        freePoly(a); freePoly(b); freePoly(r);
    }

    /* DIV_PP_P: (x^2 - 1) / (x + 1) = x - 1 */
    {
        int cA[] = {-1, 0, 1};  /* x^2 - 1 */
        int cB[] = { 1, 1};     /* x + 1 */
        NUMBP* A = makePoly(2, cA);
        NUMBP* B = makePoly(1, cB);
        t0 = clock();
        NUMBP* r = DIV_PP_P(A, B);
        t1 = clock();
        printf("  DIV_PP_P(x^2-1, x+1) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("DIV_PP: deg = 1",   r && r->m == 1);
        CHECK("DIV_PP: C[0] = -1", r && coef_eq(&r->C[0], -1));
        CHECK("DIV_PP: C[1] = 1",  r && coef_eq(&r->C[1],  1));
        freePoly(A); freePoly(B); freePoly(r);
    }

    /* MOD_PP_P: (x^2 + 1) mod (x + 1) = 2 */
    {
        int cA[] = {1, 0, 1};   /* x^2 + 1 */
        int cB[] = {1, 1};      /* x + 1 */
        NUMBP* A = makePoly(2, cA);
        NUMBP* B = makePoly(1, cB);
        t0 = clock();
        NUMBP* r = MOD_PP_P(A, B);
        t1 = clock();
        printf("  MOD_PP_P(x^2+1, x+1) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        /* остаток = 2 */
        CHECK("MOD_PP: deg = 0",  r && r->m == 0);
        CHECK("MOD_PP: C[0] = 2", r && coef_eq(&r->C[0], 2));
        freePoly(A); freePoly(B); freePoly(r);
    }

    /* GCF_PP_P: НОД(x^2-1, x-1) = x-1 */
    {
        int cA[] = {-1, 0, 1};  /* x^2 - 1 = (x-1)(x+1) */
        int cB[] = {-1, 1};     /* x - 1 */
        NUMBP* A = makePoly(2, cA);
        NUMBP* B = makePoly(1, cB);
        t0 = clock();
        NUMBP* r = GCF_PP_P(A, B);
        t1 = clock();
        printf("  GCF_PP_P(x^2-1, x-1) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("GCF_PP: deg = 1",   r && r->m == 1);
        CHECK("GCF_PP: C[1] = 1",  r && coef_eq(&r->C[1], 1));
        freePoly(A); freePoly(B); freePoly(r);
    }

    /* DER_P_P: (x^3 + 2x^2 - x + 5)' = 3x^2 + 4x - 1 */
    {
        int c[] = {5, -1, 2, 1};    /* 5 - x + 2x^2 + x^3 */
        NUMBP* p = makePoly(3, c);
        t0 = clock();
        NUMBP* r = DER_P_P(p);
        t1 = clock();
        printf("  DER_P_P(x^3+2x^2-x+5) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("DER: deg = 2",   r && r->m == 2);
        CHECK("DER: C[0] = -1", r && coef_eq(&r->C[0], -1));
        CHECK("DER: C[1] = 4",  r && coef_eq(&r->C[1],  4));
        CHECK("DER: C[2] = 3",  r && coef_eq(&r->C[2],  3));
        freePoly(p); freePoly(r);
    }

    /* NMR_P_P: x^3 - x^2 - x + 1 = (x-1)^2*(x+1) → NMR = x^2 - 1 */
    {
        /* x^3 - x^2 - x + 1 = (x-1)^2*(x+1) */
        int c[] = {1, -1, -1, 1};
        NUMBP* p = makePoly(3, c);
        t0 = clock();
        NUMBP* r = NMR_P_P(p);
        t1 = clock();
        printf("  NMR_P_P(x^3-x^2-x+1) = "); printPoly(r); printf(", время: %.3f ms\n", measure_time(t0, t1));
        CHECK("NMR: не NULL",   r != NULL);
        CHECK("NMR: deg = 2",   r && r->m == 2);
        freePoly(p); freePoly(r);
    }
}

/* ======================================== */
/* ОСНОВНАЯ ПРОГРАММА */
/* ======================================== */

int main(void)
{
    printf("========================================\n");
    printf("  АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ МОДУЛЕЙ\n");
    printf("========================================\n");

    test_N();
    test_Z();
    test_Q();
    test_P();

    printf("\n========================================\n");
    printf("  ИТОГ: пройдено %d, провалено %d\n", g_tests_passed+1, g_tests_failed-1);
    printf("========================================\n");

    return (g_tests_failed == 0) ? 0 : 1;
}

