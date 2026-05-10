#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct numbN{
   int n;		// Кол-во цифр в числе
   int* A;		// Динамический массив для числа
} NUMBN;

typedef struct numbZ{
   int b;      // Знак числа
   int n;		// Кол-во цифр в числе
   int* A;		// Динамический массив для числа    
} NUMBZ;

typedef struct numbQ{
   NUMBZ a;        // Числитель
   NUMBN b;        // Знаменатель     
} NUMBQ;

typedef struct numbP{
   int m;		// Степень многочлена	
   NUMBQ* C;	// Массив коэффициентов
} NUMBP;

// НАТУРАЛЬНЫЕ ЧИСЛА (N)
#include "N/COM_NN_D.h"
#include "N/NZER_N_B.h"
#include "N/ADD_1N_N.h"
#include "N/ADD_NN_N.h"
#include "N/SUB_NN_N.h"
#include "N/MUL_ND_N.h"
#include "N/MUL_Nk_N.h"
#include "N/MUL_NN_N.h"
#include "N/SUB_NDN_N.h"
#include "N/DIV_NN_Dk.h"
#include "N/DIV_NN_N.h"
#include "N/MOD_NN_N.h"
#include "N/GCF_NN_N.h"
#include "N/LCM_NN_N.h"

// ЦЕЛЫЕ ЧИСЛА (Z)
#include "Z/ABS_Z_Z.h"
#include "Z/POZ_Z_D.h"
#include "Z/MUL_ZM_Z.h"
#include "Z/TRANS_N_Z.h"
#include "Z/TRANS_Z_N.h"
#include "Z/ADD_ZZ_Z.h"
#include "Z/SUB_ZZ_Z.h"
#include "Z/MUL_ZZ_Z.h"
#include "Z/DIV_ZZ_Z.h"
#include "Z/MOD_ZZ_Z.h"

// РАЦИОНАЛЬНЫЕ ЧИСЛА (Q)
#include "Q/RED_Q_Q.h"
#include "Q/INT_Q_B.h"
#include "Q/TRANS_Z_Q.h"
#include "Q/TRANS_Q_Z.h"
#include "Q/ADD_QQ_Q.h"
#include "Q/SUB_QQ_Q.h"
#include "Q/MUL_QQ_Q.h"
#include "Q/DIV_QQ_Q.h"

// МНОГОЧЛЕНЫ (P)
#include "P/ADD_PP_P.h"
#include "P/SUB_PP_P.h"
#include "P/MUL_PQ_P.h"
#include "P/MUL_Pxk_P.h"
#include "P/LED_P_Q.h"
#include "P/DEG_P_N.h"
#include "P/FAC_P_Q.h"
#include "P/MUL_PP_P.h"
#include "P/DIV_PP_P.h"
#include "P/MOD_PP_P.h"
#include "P/GCF_PP_P.h"
#include "P/DER_P_P.h"
#include "P/NMR_P_P.h"
