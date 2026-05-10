#ifndef MENU_H
#define MENU_H

#include "include_DM.h"

/*
ФУНКЦИИ ОСВОБОЖДЕНИЯ ПАМЯТИ
*/
void freeNUMBN(NUMBN* num);
void freeNUMBZ(NUMBZ* num);
void freeNUMBQ(NUMBQ* num);
void freeNUMBP(NUMBP* poly);

/*
ФУНКЦИИ ПРОВЕРКИ ОШИБОК
*/
int isErrorN(NUMBN* num);
int isErrorZ(NUMBZ* num);
int isErrorQ(NUMBQ* num);
int isErrorP(NUMBP* poly);
int isZeroN(NUMBN* num);
int isZeroBZ(NUMBZ* num);  // проверка на ноль для целых

/*
ФУНКЦИИ ВВОДА
*/
NUMBN* inputN(const char* prompt);
NUMBZ* inputZ(const char* prompt);
NUMBQ* inputQ(const char* prompt);
NUMBP* inputP(const char* prompt);

/*
ФУНКЦИИ ВЫВОДА
*/
void printN(NUMBN* num);
void printZ(NUMBZ* num);
void printQ(NUMBQ* num);
void printP(NUMBP* poly);

/*
ОСНОВНЫЕ МЕНЮ
*/
void menuN(void);
void menuZ(void);
void menuQ(void);
void menuP(void);

#endif
