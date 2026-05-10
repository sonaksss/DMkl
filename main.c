#include "menu.h"
#include "include_DM.h"

/*
ТОЧКА ВХОДА В ПРОГРАММУ
*/

int main(void) {
    
    printf("============================================\n");
    printf("     КОМПЬЮТЕРНАЯ АЛГЕБРА\n");
    printf("============================================\n");
    printf("  N - Натуральные числа\n");
    printf("  Z - Целые числа\n");
    printf("  Q - Рациональные числа\n");
    printf("  P - Многочлены\n");
    printf("  0 - Выход\n");
    
    
    char choice;
    while (1){
        printf("Выберите тип чисел:\n");
        scanf(" %c", &choice);
        switch (choice) {
            case 'N':
            case 'n':
                menuN();
                break;
            
            case 'Z':
            case 'z':
                menuZ();
                break;
            
            case 'Q':
            case 'q':
                menuQ();
                break;
            
            case 'P':
            case 'p':
                menuP();
                break;
            
            case '0':
                printf("Работа завершена!\n");
                return 0;
            
            default:
                printf("ОШИБКА: неверный выбор типа\n");
                printf("Допустимые варианты: N, Z, Q, P, 0\n");
                return 1;
        }
        printf("\n============================================\n");
    }
    return 0;
}
