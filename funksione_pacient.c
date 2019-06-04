#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strukturat.h"
#include "globals.h"

char f_pacient[] = {"./databaza/pacientdb.dat"};

/* Funksioni shfaq_pacient() ka si parameter te dhenat e pacientit dhe i printon ato. */

void shfaq_pacient(int id, char *emri, char *mbiemri, char *adresa, char *nr_tel) {
    printf("%-5s%-14s%-14s%-20s%s\n", "ID", "Emri", "Mbiemri", "Adresa", "Nr. Telefoni");
    printf("====================================================================");
    printf("\n%-5d%-14s%-14s%-20s%s\n", id, emri, mbiemri, adresa, nr_tel);
    printf("====================================================================\n");
}

/* Funksioni ekziston_ID_pacient() kontrollon ne file nje ID pacienti dhe kthen 1 n.q.s
 ekziston ose 0 n.q.s nuk ekziston. */

int ekziston_ID_pacient(FILE *fp, int id, struct pacienti p) {
    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.pacienti_id == id) {
            printf("\n");
            shfaq_pacient(p.pacienti_id, p.emri, p.mbiemri, p.adresa, p.nr_tel);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* Funksioni shto_pacient() kontrollon ne file nese ID e dhene nga perdoruesi ekziston permes
funksionit ekziston_ID_pacient(). Nqs ekziston nje ID ne file nuk e lejon vazhdimin
e regjistrimit. Ne te kundert e vazhdon duke kerkuar te dhenat e pacientit dhe duke 
e ruajtur ne file. */

void shto_pacient() {
    FILE *fp;
    fp = fopen(f_pacient, "rb");
    struct pacienti p;
    char temp;
    int id;
    int found = 0;

    printf("\nJepni ID: ");
    scanf("%d", &id);

    if (fp != NULL) {
        found = ekziston_ID_pacient(fp, id, p);
        if (found) {
            printf("\nKujdes! ID duhet të jetë unike. Aktualisht ekziston një person tjetër me të njëjtën ID.\n\n");
        }
    }

    if (!found) {
        fp = fopen(f_pacient, "ab");
        p.pacienti_id = id;
        printf("\nJepni emrin: ");
        scanf("%s", p.emri);
        printf("\nJepni mbiemrin: ");
        scanf("%s", p.mbiemri);
        printf("\nJepni adresën: ");
        scanf("%c", &temp);
        scanf("%[^\n]s", p.adresa);
        printf("\nJepni numrin e telefonit: ");
        scanf("%s", p.nr_tel);

        printf("\n\n====================================================================\n\n");
        printf("\t\t Detajet e pacientit që kërkoni të shtoni\n\n");
        printf("====================================================================\n\n");

        shfaq_pacient(p.pacienti_id, p.emri, p.mbiemri, p.adresa, p.nr_tel);

        if (merr_zgjedhje_perdoruesi()) {
            fwrite(&p, sizeof(p), 1, fp);
            printf("\nNdryshimet u kryen me sukses!\n");
        } else {
            printf("\nNuk u kryen ndryshime në sistem!\n");
        }
    }
    fclose(fp);

    printf("\nRikthehuni duke shtypur ENTER...");
}

/* Funksioni shfaq_pacientet() lexon file-n e pacienteve dhe i shfaq te gjithe pacientet. */

void shfaq_pacientet() {
    FILE *fp;
    fp = fopen(f_pacient, "rb");

    if (fp != NULL) {
        struct pacienti p;
        system("clear");

        printf("====================================================================\n\n");
        printf("\t\t Detajet për të gjithë pacientët\n\n");
        printf("====================================================================\n\n");

        printf("%-5s%-14s%-14s%-20s%s\n", "ID", "Emri", "Mbiemri", "Adresa", "Nr. Telefoni");

        while (fread(&p, sizeof(p), 1, fp)) {
            printf("\n%-5d%-14s%-14s%-20s%s", p.pacienti_id, p.emri, p.mbiemri, p.adresa, p.nr_tel);
        }
        printf("\n====================================================================\n");
    } else {
        jep_error();
    }
    fclose(fp);

    printf("\nKthehu prapa duke shtypur ENTER...");
}

/* Funksioni modifiko_pacient() i kerkon perdoruesit nje ID pacienti. Kontrollon
ne file-in e pacienteve. Nese ID e futur nga perdoruesi nuk eshte e barabarte me ID e
nje pacienti atehere e shkruan ate pacient ne nje file te perkohshem te quajtur "temp.dat". 
Nese ID eshte e barabarte me nje ID pacienti, kerkohen te dhenat e reja dhe ruhet ne "temp.dat".
Te dhenat rishkruhen nga "temp.dat" ne "pacientdb.dat". */

void modifiko_pacient() {
    FILE *fp, *fp1;
    fp = fopen(f_pacient, "rb");
    fp1 = fopen("./databaza/temp.dat", "wb");

    if (fp != NULL && fp1 != NULL) {

        struct pacienti p;
        int id;
        int found = 0;
        int choice = 0;
        char temp;

        printf("\nJepni ID e pacientit që dëshironi të modifikoni: ");
        scanf("%d", &id);

        while (fread(&p, sizeof(p), 1, fp)) {
            if (p.pacienti_id == id) {
                found = 1;
                printf("\n");
                shfaq_pacient(id, p.emri, p.mbiemri, p.adresa, p.nr_tel);
                int z = merr_zgjedhje_perdoruesi();

                if (z) {
                    printf("\nEmri i ri: ");
                    scanf("%s", p.emri);
                    printf("\nMbiemri i ri: ");
                    scanf("%s", p.mbiemri);
                    printf("\nAdresa e re: ");
                    scanf("%c", &temp);
                    scanf("%[^\n]s", p.adresa);
                    printf("\nNrumri i telefonit: ");
                    scanf("%s", p.nr_tel);

                    printf("====================================================================\n\n");
                    printf("\t\t Detajet e reja për këtë pacient\n\n");
                    printf("====================================================================\n\n");

                    shfaq_pacient(id, p.emri, p.mbiemri, p.adresa, p.nr_tel);
                    choice = merr_zgjedhje_perdoruesi();
                } else {
                    printf("\nModifikimi i të dhënave nuk vazhdoi...");
                    fclose(fp);
                    fclose(fp1);
                    return;
                }
                
                if (choice) {
                    fwrite(&p, sizeof(p), 1, fp1); 
                }
            } else {
                fwrite(&p, sizeof(p), 1, fp1);
            }
        }
        fclose(fp);
        fclose(fp1);

        if (found == 0) {
            printf("Nuk ekziston asnje pacient me ID e vendosur\n\n");
        } else {
            if (choice) {
                fp = fopen(f_pacient, "wb");
                fp1 = fopen("./databaza/temp.dat", "rb");

                if (fp != NULL && fp1 != NULL) {
                    while (fread(&p, sizeof(p), 1, fp1)) {
                    fwrite(&p, sizeof(p), 1, fp);
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

/* Funksioni kerko_pacient() merr nga perdoruesi nje emer dhe mbiemer dhe kerkon ne file.
Nqs gjendet nje pacient me ate emer dhe mbiemer ne file shfaqet ai pacient. Nese nuk gjendet
asnje shfaqet mesazhi qe "Nuk u gjend asnje pacient". Funksioni eshte case insensitive. */

void kerko_pacient() {
    FILE *fp;
    fp = fopen(f_pacient, "rb");

    if (fp != NULL) {  

        struct pacienti p;
        int found = 0;
        char emri[100];
        char mbiemri[100];

        printf("\nJepni emrin dhe mbiemrin e pacientit: ");
        scanf("%s %s", emri, mbiemri);

        while (fread(&p, sizeof(p), 1, fp)) {
            lower_string(emri);
            lower_string(p.emri);
            lower_string(mbiemri);
            lower_string(p.mbiemri);

            if (strcmp(emri, p.emri) == 0 && strcmp(mbiemri, p.mbiemri) == 0) {
                found = 1;
                printf("\n====================================================================\n\n");
                printf("\t\t Detajet e pacientit %s %s\n\n", p.emri, p.mbiemri);
                printf("====================================================================\n\n");

                shfaq_pacient(p.pacienti_id, p.emri, p.mbiemri, p.adresa, p.nr_tel);
            }
        }

        if (!found) {
            printf("\nNuk u gjet asnjë pacient me të dhënat e vendosura.");
        }
    }
    else {
        jep_error();
    }
    fclose(fp);

    printf("\nKthehu prapa duke shtypur ENTER...");
}