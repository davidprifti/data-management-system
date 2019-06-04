#include <stdio.h>
#include <stdlib.h>

#include "shfaq.h"

int main() {

    int zgjedhja;

    while (1) {
        system("clear"); // system("cls"); ne Windows

        printf("================== Sistemi i menaxhimit të të dhënave ==============\n\n");

        printf("1. Sistemi i mjekëve\n\n");
        printf("2. Sistemi i pacientëve\n\n");
        printf("3. Sistemi i vizitave\n\n");
        printf("0. Dil\n\n");

        printf("=====================================================================\n\n");

        printf("\nJu lutem vendosni indeksin e sistemit ku deshironi te futeni: ");
        scanf("%d", &zgjedhja);

        switch (zgjedhja) {
        case 1:
            shfaq_sistemin_mjek();
            break;

        case 2:
            shfaq_sistemin_pacient();
            break;

        case 3:
            shfaq_sistemin_vizita();
            break;

        case 0:
            exit(0);

        default:
            printf("\nNuk ekziston asnje kategori me indeksin e kerkuar.");
            break;
        }
        getchar();
        getchar();
    }

    return 0;
}