#include "../include_DM.h"


/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  freeNUMBN

  Вспомогательная функция: освобождает память, занятую NUMBN.

  Параметры:
    1) NUMBN* num — указатель на число для освобождения
*/
static void freeNUMBN(NUMBN* num) {
    if (!num) return;
    free(num->A);
    free(num);
}

/*
  freeNUMBZ

  Вспомогательная функция: освобождает память, занятую NUMBZ.

  Параметры:
    1) NUMBZ* num — указатель на число для освобождения
*/
static void freeNUMBZ(NUMBZ* num) {
    if (!num) return;
    free(num->A);
    free(num);
}

/*
  SUB_QQ_Q

  Вычитание двух рациональных дробей: result = a - b

  Алгоритм:
    Пусть a = p/q, b = r/s.
    1. lcm        = LCM_NN_N(q, s)      — НОК знаменателей
    2. fa         = DIV_NN_N(lcm, q)    — множитель для числителя a
    3. fb         = DIV_NN_N(lcm, s)    — множитель для числителя b
    4. fa_z       = TRANS_N_Z(fa)       — fa как целое для умножения
    5. fb_z       = TRANS_N_Z(fb)       — fb как целое для умножения
    6. pa         = MUL_ZZ_Z(p, fa_z)   — скорректированный числитель a
    7. pb         = MUL_ZZ_Z(r, fb_z)   — скорректированный числитель b
    8. num        = SUB_ZZ_Z(pa, pb)    — разность числителей
    9. result     = num / lcm           — итоговая дробь

  Параметры:
    1) NUMBQ* a — указатель на первую дробь (уменьшаемое)
    2) NUMBQ* b — указатель на вторую дробь (вычитаемое)

  Возвращает NUMBQ* — новую дробь или NULL при ошибке.
  Сокращение дроби не выполняется — это задача RED_Q_Q (Q-1).
*/
NUMBQ* SUB_QQ_Q(NUMBQ* a, NUMBQ* b) {
    if (!a || !b) return NULL;
    if (!a->b.A || !b->b.A) return NULL;
    if (a->b.n <= 0 || b->b.n <= 0) return NULL;

    NUMBN* lcm = LCM_NN_N(&a->b, &b->b);
    if (!lcm) return NULL;

    NUMBN* fa = DIV_NN_N(lcm, &a->b);
    if (!fa) { freeNUMBN(lcm); return NULL; }

    NUMBN* fb = DIV_NN_N(lcm, &b->b);
    if (!fb) { freeNUMBN(lcm); freeNUMBN(fa); return NULL; }

    NUMBZ* fa_z = TRANS_N_Z(fa);
    NUMBZ* fb_z = TRANS_N_Z(fb);
    freeNUMBN(fa);
    freeNUMBN(fb);
    if (!fa_z || !fb_z) {
        freeNUMBN(lcm);
        if (fa_z) freeNUMBZ(fa_z);
        if (fb_z) freeNUMBZ(fb_z);
        return NULL;
    }

    NUMBZ* pa = MUL_ZZ_Z(&a->a, fa_z);
    freeNUMBZ(fa_z);
    if (!pa) { freeNUMBN(lcm); freeNUMBZ(fb_z); return NULL; }

    NUMBZ* pb = MUL_ZZ_Z(&b->a, fb_z);
    freeNUMBZ(fb_z);
    if (!pb) { freeNUMBN(lcm); freeNUMBZ(pa); return NULL; }

    NUMBZ* num = SUB_ZZ_Z(pa, pb);
    freeNUMBZ(pa);
    freeNUMBZ(pb);
    if (!num) { freeNUMBN(lcm); return NULL; }

    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (!result) { freeNUMBN(lcm); freeNUMBZ(num); return NULL; }

    result->a.b = num->b;
    result->a.n = num->n;
    result->a.A = num->A;
    free(num);

    result->b.n = lcm->n;
    result->b.A = lcm->A;
    free(lcm);

    return result;
}
