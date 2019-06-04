#include <stdio.h>
#include <stdlib.h>

#include "strukturat.h"
#include "funksione_mjek.h"
#include "funksione_pacient.h"
#include "funksione_vizita.h"

int main();

void shfaq_sistemin_mjek() {
    int zgjedhja;

    while (1) {
        system("clear");

        printf("================== Sistemi i menaxhimit të të dhënave ==============\n\n");

        printf("1. Shto mjek\n\n");
        printf("2. Ndrysho të dhënat\n\n");
        printf("3. Shfaq të gjithë\n\n");
        printf("4. Kërko mjek\n\n");
        printf("0. Kthehu prapa\n\n");

        printf("====================================================================\n\n");

        printf("\nJu lutem vendosni indeksin e sistemit ku deshironi te futeni: ");
        scanf("%d", &zgjedhja);

        switch (zgjedhja)
        {
        case 1:
            shto_mjek();
            break;

        case 2:
            modifiko_mjek();
            break;

        case 3:
            shfaq_mjeket();
            break;

        case 4:
            kerko_mjek();
            break;

        case 0:
            main();
            break;

        default:
            printf("\nNuk ekziston asnje kategori me indeksin e kerkuar.");
            break;
        }

        getchar();
        getchar();
    }
}

void shfaq_sistemin_pacient() {
    int zgjedhja;

    while (1) {
        system("clear");

        printf("================== Sistemi i menaxhimit të të dhënave ==============\n\n");

        printf("1. Shto pacient\n\n");
        printf("2. Ndrysho të dhënat\n\n");
        printf("3. Shfaq të gjithë\n\n");
        printf("4. Kërko pacient\n\n");
        printf("0. Kthehu prapa\n\n");

        printf("====================================================================\n\n");

        printf("\nJu lutem vendosni indeksin e sistemit ku deshironi te futeni: ");
        scanf("%d", &zgjedhja);

        switch (zgjedhja)
        {
        case 1:
            shto_pacient();
            break;

        case 2:
            modifiko_pacient();
            break;

        case 3:
            shfaq_pacientet();
            break;

        case 4:
            kerko_pacient();
            break;

        case 0:
            main();
            break;

        default:
            printf("\nNuk ekziston asnje kategori me indeksin e kerkuar.");
            break;
        }

        getchar();
        getchar();
    }
}

void shfaq_sistemin_vizita() {
    int zgjedhja;

    while (1) {
        system("clear");

        printf("================== Sistemi i menaxhimit të të dhënave ==============\n\n");

        printf("1. Shto një vizitë\n\n");
        printf("2. Shfaq vizitat e një pacienti\n\n");
        printf("3. Vizitat e mjekut si numër sipas datave\n\n");
        printf("4. Numri i vizitave sipas specialiteteve\n\n");
        printf("0. Kthehu prapa\n\n");

        printf("====================================================================\n\n");

        printf("\nJu lutem vendosni indeksin e sistemit ku deshironi te futeni: ");
        scanf("%d", &zgjedhja);

        switch (zgjedhja)
        {
        case 1:
            shto_vizite();
            break;

        case 2:
            shfaq_vizta_pacient();
            break;

        case 3:
            vizita_mjek_data();
            break;

        case 4:
            vizita_specialitete();
            break;

        case 0:
            main();
            break;

        default:
            printf("\nNuk ekziston asnje kategori me indeksin e kerkuar.");
            break;
        }

        getchar();
        getchar();
    }
}