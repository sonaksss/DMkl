/*
    SUB_ZZ_Z – вычитание целых чисел.

    Выполняет A - B.
    Цифры хранятся от младшего разряда к старшему (little-endian).

    Параметры:
        1) NUMBZ* A – уменьшаемое
        2) NUMBZ* B – вычитаемое

    Возвращает: NUMBZ* – разность, или NULL при ошибке

    Автор: Семенов Максим
    Группа: 5388
*/
#ifndef _SUB_ZZ_Z_H_
#define _SUB_ZZ_Z_H_


NUMBZ* SUB_ZZ_Z(NUMBZ* A, NUMBZ* B);

#endif
