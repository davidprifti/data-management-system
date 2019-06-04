#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strukturat.h"
#include "globals.h"

char f_mjek[] = {"./databaza/mjekdb.dat"};

/* Funksioni shfaq_mjek() ka si parameter te dhenat e vizites dhe i printon ato. */

void shfaq_mjek(int id, char *emri, char *mbiemri, char *specializimi) {
    printf("%-5s%-14s%-16s%s\n", "ID", "Emri", "Mbiemri", "Specializimi");
    printf("====================================================================\n");
    printf("%-5d%-14s%-16s%s", id, emri, mbiemri, specializimi);
    printf("\n====================================================================\n");
}

/* Funksioni ekziston_ID_mjek() kontrollon ne file nje ID mjeku dhe kthen 1 n.q.s
 ekziston ose 0 n.q.s nuk ekziston. */

int ekziston_ID_mjek(FILE *fp, int id, struct mjeku m) {
    while (fread(&m, sizeof(m), 1, fp)) {
        if (m.mjeku_id == id) {
            printf("\n");
            shfaq_mjek(m.mjeku_id, m.emri, m.mbiemri, m.specializimi);
            fclose(fp);
            return 1;
        }
    }
    return 0;
}

/* Funksioni shto_mjek() kontrollon ne file nese ID e dhene nga perdoruesi ekziston permes
funksionit ekziston_ID_mjek(). Nqs ekziston nje ID ne file nuk e lejon vazhdimin
e regjistrimit. Ne te kundert e vazhdon duke kerkuar te dhenat e tjera dhe duke 
i ruajtur ne file. */

void shto_mjek() {
    FILE *fp;
    fp = fopen(f_mjek, "rb");
    struct mjeku m;
    int id;
    char temp;
    int found = 0;

    printf("\nJepni ID: ");
    scanf("%d", &id);

    if (fp != NULL) {
        found = ekziston_ID_mjek(fp, id, m); 
        if (found) {
            printf("\nKujdes! ID duhet të jetë unike. Aktualisht ekziston një person tjetër me të njëjtën ID.\n\n");
        }
    }

    if (!found) {
        fp = fopen(f_mjek, "ab");
        m.mjeku_id = id;
        printf("\nJepni emrin: ");
        scanf("%s", m.emri);
        printf("\nJepni mbiemrin: ");
        scanf("%s", m.mbiemri);
        printf("\nJepni specializimin: ");
        scanf("%c", &temp);
        scanf("%[^\n]s", m.specializimi);

        printf("\n\n====================================================================\n\n");
        printf("\t\t Detajet e mjekut që kërkoni të shtoni\n\n");
        printf("====================================================================\n\n");

        shfaq_mjek(m.mjeku_id, m.emri, m.mbiemri, m.specializimi);

        if (merr_zgjedhje_perdoruesi()) {
            fwrite(&m, sizeof(m), 1, fp);
            printf("\nNdryshimet u kryen me sukses!\n");
        } else {
            printf("\nNuk u kryen ndryshime në sistem!\n");
        }
    }
    fclose(fp);

    printf("\nRikthehuni duke shtypur ENTER...");
}

/* Funksioni shfaq_mjeket() lexon file-n e mjekeve dhe i shfaq te gjithe mjeket. */

void shfaq_mjeket() {
    FILE *fp;
    fp = fopen(f_mjek, "rb");

    if (fp != NULL) {
        struct mjeku m;
        system("clear");

        printf("====================================================================\n\n");
        printf("\t\t Detajet për të gjithë mjekët\n\n");
        printf("====================================================================\n\n");
        printf("%-5s%-14s%-16s%s\n", "ID", "Emri", "Mbiemri", "Specializimi");

        while (fread(&m, sizeof(m), 1, fp)) {
            printf("\n%-5d%-14s%-16s%s", m.mjeku_id, m.emri, m.mbiemri, m.specializimi);
        }
        printf("\n====================================================================\n");
    } else {
        jep_error();
    }
    fclose(fp);
    
    printf("\nKthehu prapa duke shtypur ENTER...");
}

/* Funksioni modifiko_mjek() i kerkon perdoruesit nje ID mjeku dhe kontrollon
ne file-in e mjekeve. Nese ID e futur nga perdoruesi nuk eshte e barabarte me ID e
nje mjeku atehere e shkruan ate mjek ne nje file te perkohshem te quajtur "temp.dat". Nese
ID eshte e barabarte me nje ID mjeku, kerkohen te dhenat e reja dhe ruhet ne "temp.dat"
me te dhenat e reja. Te dhenat rishkruhen nga "temp.dat" ne "mjekdb.dat". */

void modifiko_mjek() {
    FILE *fp, *fp1;

    fp = fopen(f_mjek, "rb");
    fp1 = fopen("./databaza/temp.dat", "wb");

    if (fp != NULL && fp1 != NULL) {
        struct mjeku m;
        int id;
        int found = 0;
        char temp;
        int choice = 0;
        printf("\nJepni ID e mjekut që dëshironi të modifikoni: ");
        scanf("%d", &id);

        while (fread(&m, sizeof(m), 1, fp)) {
            if (m.mjeku_id == id)
            {
                found = 1;
                printf("\n");
                shfaq_mjek(id, m.emri, m.mbiemri, m.specializimi);
                printf("\nA është ky mjeku të cilit dëshironi ti ndryshoni të dhënat?\n");
                int z = merr_zgjedhje_perdoruesi();
                if (z) {
                    printf("\nEmri i ri: ");
                    scanf("%s", m.emri);
                    printf("\nMbiemri i ri: ");
                    scanf("%s", m.mbiemri);
                    printf("\nSpecializimi i ri: ");
                    scanf("%c", &temp);
                    scanf("%[^\n]s", m.specializimi);

                    printf("====================================================================\n\n");
                    printf("\t\t Detajet e reja për këtë mjek\n\n");
                    printf("====================================================================\n\n");

                    shfaq_mjek(id, m.emri, m.mbiemri, m.specializimi);
                    choice = merr_zgjedhje_perdoruesi();
                } else {
                    printf("\nModifikimi i të dhënave nuk vazhdoi...");
                    fclose(fp);
                    fclose(fp1);
                    return;
                }
                
                if (choice) {
                    fwrite(&m, sizeof(m), 1, fp1);    
                }
            } else {
                fwrite(&m, sizeof(m), 1, fp1);
            }
        }
        fclose(fp);
        fclose(fp1);

        if (found == 0) {
            printf("Nuk ekziston asnje mjek me ID e vendosur\n\n");
        } else {
            if (choice)
            {
                fp = fopen(f_mjek, "wb");
                fp1 = fopen("./databaza/temp.dat", "rb");
                if (fp != NULL && fp1 != NULL) {
                    while (fread(&m, sizeof(m), 1, fp1)) {
                    fwrite(&m, sizeof(m), 1, fp);
                }
                    printf("\nNdryshimet u kryen me sukses!\n");
                } else {
                    jep_error();
                }
            } else {
                printf("\nNuk u kryen ndryshime në sistem!\n");
            }
        }
    } else {
        jep_error();
    }
    fclose(fp);
    fclose(fp1);
    printf("\nKthehu prapa duke shtypur ENTER...");
}

/* Funksioni kerko_mjek() merr nga perdoruesi nje emer dhe mbiemer dhe kerkon ne file.
Nqs gjendet nje mjek me ate emer dhe mbiemer ne file shfaqet ai mjek. Nese nuk gjendet
asnje shfaqet mesazhi qe "Nuk u gjend asnje mjek". Funksioni eshte case insensitive. */

void kerko_mjek() {
    FILE *fp;
    fp = fopen(f_mjek, "rb");

    if (fp != NULL) {   
        struct mjeku m;
        int found = 0;
        char emri[100];
        char mbiemri[100];
        printf("\nJepni emrin dhe mbiemrin e mjekut: ");
        scanf("%s %s", emri, mbiemri);

        while (fread(&m, sizeof(m), 1, fp)) {
            lower_string(emri);
            lower_string(m.emri);
            lower_string(mbiemri);
            lower_string(m.mbiemri);

            if (strcmp(emri, m.emri) == 0 && strcmp(mbiemri, m.mbiemri) == 0) {
                found = 1;
                printf("\n====================================================================\n\n");
                printf("\t\t Detajet e mjekut %s %s\n\n", m.emri, m.mbiemri);
                printf("====================================================================\n\n");

                shfaq_mjek(m.mjeku_id, m.emri, m.mbiemri, m.specializimi);
            }
        }

        if (!found) {
            printf("\nNuk u gjet asnjë mjek me të dhënat e vendosura.");
        }
    } else {
        jep_error();
    }
    fclose(fp);
    printf("\nKthehu prapa duke shtypur ENTER...");
}