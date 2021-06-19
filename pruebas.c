#include <stdio.h>

int main(){
    char bosque[20][30];

    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 30; j++){
            bosque[i][j] = '-';
        }
    }
    printf(" 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\n");
    for (int i = 0; i < 20; i++){
        printf("%i", i + 1);
        for (int j = 0; j < 30; j++){
            printf(" %c", bosque[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    return 0;
}
