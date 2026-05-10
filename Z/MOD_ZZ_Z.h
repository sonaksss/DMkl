#ifndef MOD_ZZ_Z_H
#define MOD_ZZ_Z_H


/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
Остаток от деления первого целого числа на второе.

Параметры:
    - NUMBZ* a — делимое
    - NUMBZ* b — делитель

Возвращает NUMBZ* — остаток от деления a на b.
*/
NUMBZ* MOD_ZZ_Z(NUMBZ* a, NUMBZ* b);

#endif
