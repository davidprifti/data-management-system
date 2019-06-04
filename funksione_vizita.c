#include <stdio.h>
#include <string.h>

#include "strukturat.h"
#include "globals.h"
#include "funksione_mjek.h"
#include "funksione_pacient.h"

char f_vizita[] = {"./databaza/vizitadb.dat"};
char f_mjek2[] = {"./databaza/mjekdb.dat"};
char f_pacient2[] = {"./databaza/pacientdb.dat"};

/* Funksioni shfaq_vizite() ka si parameter te dhenat e vizites dhe printon te dhenat. */

void shfaq_vizite(int id_m, int id_p, int data, int muaji, int viti, char *diagnoza) {
    printf("%-14s%-16s%-22s%s\n", "ID e mjekut", "ID e pacientit", "Data e vizitës", "Diagnoza");
    printf("\n====================================================================\n");
    printf("%-14d%-16d%-2d/%-2d/%-15d%s", id_m, id_p, data, muaji, viti, diagnoza);
    printf("\n====================================================================\n");
}

/* Funksioni shto_vizite() i kerkon perdoruesit nje ID mjeku dhe nje ID pacienti. Nese
ID e mjekut dhe e pacientit nuk ekzistojne nuk e lejon vazhdimin e regjistrimit te vizites. Nese
ekzistojne kerkohen te dhenat e tjera te vizites dhe ruhet ne file-in "vizitadb.dat".*/

void shto_vizite() {
    FILE *fp, *fp1, *fp2;
    fp1 = fopen(f_pacient2, "rb");
    fp2 = fopen(f_mjek2, "rb");
    fp = fopen(f_vizita, "ab");

    if (fp1 != NULL && fp2 != NULL) {

        int id_m;
        int id_p;
        int found_m;
        int found_p;
        int choice;
        char temp;

        struct vizita v;
        struct mjeku m;
        struct pacienti p;

        printf("\nJepni ID e mjekut: ");
        scanf("%d", &id_m);

        found_m = ekziston_ID_mjek(fp2, id_m, m);
        if (found_m) {
            choice = merr_zgjedhje_perdoruesi();
        } else {
            printf("\nKjo ID mjeku nuk ekziston.\n");
        }
        
        if (found_m == 1 && choice) {
            printf("\nJepni ID e pacientit: ");
            scanf("%d", &id_p);

            found_p = ekziston_ID_pacient(fp1, id_p, p);
            if (found_p) {
                choice = merr_zgjedhje_perdoruesi();
            } else {
                printf("\nKjo ID pacienti nuk ekziston.\n");
            }
        }

        if (found_p == 1 && choice) {
            v.mjeku_id = id_m;
            v.pacienti_id = id_p;
            printf("\nJepni datën në formatin (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &v.date.data, &v.date.muaji, &v.date.viti);
            printf("\nJepni diagnozën: ");
            scanf("%c", &temp);
            scanf("%[^\n]s", v.diagnoza);

            printf("\n\n====================================================================\n\n");
            printf("\t\t Detajet e vizitës që kërkoni të shtoni\n\n");
            printf("====================================================================\n\n");

            shfaq_vizite(v.mjeku_id, v.pacienti_id, v.date.data, v.date.muaji, v.date.viti, v.diagnoza);

            if (merr_zgjedhje_perdoruesi()) {
                fwrite(&v, sizeof(v), 1, fp);
                printf("\nNdryshimet u kryen me sukses!\n");
            }  else {
                printf("\nNuk u kryen ndryshime në sistem!\n");
            }
        }
    } else {
        printf("\nKujdes! Nuk mund të shtoni vizita në sistem nëse nuk ka mjekë dhe pacientë të regjistruar.\n");
    }
    fclose(fp);

    printf("\nRikthehuni duke shtypur ENTER...");
}

/* Funksioni shfaq_vizta_pacient() i kerkon perdoruesit nje ID pacienti. Kerkohet ne file-in
e pacienteve per emrin dhe mbiemrin e pacientit. Me pas lexohet ne file-in e vizitave per
te gjitha ato vizita per te cilat pacienti_id = ID e futur nga perdoruesi dhe shfaqen.*/

void shfaq_vizta_pacient() {

    FILE *fp, *fp1;
    fp = fopen(f_vizita, "rb");
    fp1 = fopen(f_pacient2, "rb");

    if (fp != NULL && fp1 != NULL) {

        char *emri_p;
        char *mbiemri_p;
        int found = 0;
        struct vizita v;
        struct pacienti p;
        int id;

        printf("\nJepni ID e pacientit të cilit dëshironi ti shikoni vizitat: ");
        scanf("%d", &id);

        while (fread(&p, sizeof(p), 1, fp1)) {
            if (id == p.pacienti_id)
            {
                found = 1;
                emri_p = p.emri;
                mbiemri_p = p.mbiemri;
                break;
            }
        }

        if (found == 1) {
            int vizita = 0;
            printf("\n====================================================================\n\n");
            printf("\t Vizitat e pacientit me ID %d: %s %s\n\n", id, emri_p, mbiemri_p);
            printf("====================================================================\n\n");

            printf("%-14s%-24s%s\n", "ID e mjekut", "Data e vizitës", "Diagnoza");
            while (fread(&v, sizeof(v), 1, fp)) {
                if (id == v.pacienti_id)
                {
                    vizita++;
                    printf("\n%-14d%-2d/%-2d/%-17d%s", v.mjeku_id, v.date.data, v.date.muaji, v.date.viti, v.diagnoza);
                }
            }

            if (!vizita) {
                printf("\n\tKy pacient nuk ka kryer asnjë vizitë...");
            }

            printf("\n====================================================================\n");

            if (vizita) {
                printf("\nKy pacient ka kryer në total %d vizita.\n", vizita);
            }
        } else {
            printf("\nNuk ekziston asnjë pacient me këtë ID!\n");
        }
    } else {
        jep_error();
    }

    fclose(fp);
    fclose(fp1);

    printf("\nKthehu prapa duke shtypur ENTER...");
}

/* Funksioni vizita_mjek_data() shfaq vizitat e mjekut sipas datave. 
1. Kerkohet nje ID mjeku nga perdoruesi dhe krijohet nje vektor strukturash info
qe permbajne daten dhe numrin e vizitave. 
2. Kerkohet ne file-in e vizitave per ato vizita ku mjek_id == ID e futur nga perdoruesi. 
Ne rastin kur jane te barabarta kontrollon ne vektorin e strukturave info per daten e vizites. 
Nese e gjen ate date ne vektor i shton nje vizite. Ne te kundert krijon nje date te re me 1 vizite. */

void vizita_mjek_data() {

    FILE *fp;
    fp = fopen(f_vizita, "rb");

    if (fp != NULL) {
        int id;
        int i = 0;
        int nr_vizitave_total = 0;

        struct info
        {
            struct date date;
            int nr_vizitave;
        };

        struct info temp_info[100];
        struct vizita v;

        printf("\nJepni ID e mjekut që kërkoni ti shikoni vizitat: ");
        scanf("%d", &id);

        while (fread(&v, sizeof(v), 1, fp))
        {
            int found = 0;

            if (id == v.mjeku_id)
            {
                for (int k = 0; k < i; k++)
                {
                    if (temp_info[k].date.data == v.date.data &&
                        temp_info[k].date.muaji == v.date.muaji &&
                        temp_info[k].date.viti == v.date.viti)
                    {
                        found = 1;
                        temp_info[k].nr_vizitave += 1;
                        break;
                    }
                }

                if (!found)
                {
                    temp_info[i].date.data = v.date.data;
                    temp_info[i].date.muaji = v.date.muaji;
                    temp_info[i].date.viti = v.date.viti;
                    temp_info[i].nr_vizitave = 1;
                    i++;
            }
        }
    }

    if (i) {
        for (int j = 0; j < i; j++) {
            nr_vizitave_total += temp_info[j].nr_vizitave;
            printf("\nData %d/%d/%d: %d\n", temp_info[j].date.data, temp_info[j].date.muaji, temp_info[j].date.viti, temp_info[j].nr_vizitave);
        }

        printf("\nKy mjek ka kryer në total %d vizita.\n", nr_vizitave_total);
    } else {
        printf("\nAsnjë vizitë e kryer.\n");
    }
    } else {
        jep_error();
    }
    fclose(fp);

    printf("\nRikthehuni duke shtypur ENTER..."); 
}

/* Funksioni vizita_specialitete() shfaq vizitat  sipas specialiteteve. 
1. Krijohet nje vektor strukturash info dhe info_f.
2. Ne vektorin e strukturave info ruhen te gjitha ID e mjekeve qe
kane kryer vizita se bashku me specialitetin e tyre.
3. Ne vektorin e strukturave info_f ruhen specialitet e mjekeve (vetem nga nje here)
se bashku me numrin e vizitave per ate specialitet.
4. Lexohen vizitat nga file-i i viztave.
5. Per ato vizita ku mjeku_id eshte e barabarte me ID e nje mjeku te vektorit te
strukturave info, behet nje kerkim ne vektorin e strukturave info_f dhe gjendet ai
specialitet mjeku dhe i shtohet nje vizite.
6. Vektori i strukturave info_f renditet sipas numrit te viztave ne rend zbrites.
7. Shfaqet vektori i strukturave info_f.*/

void vizita_specialitete() {
    FILE *fp, *fp1;
    fp = fopen(f_vizita, "rb");
    fp1 = fopen(f_mjek2, "rb");

    if (fp != NULL && fp1 != NULL) {
        int id;
        int i = 0;
        int l = 0;

        struct info_f {
            char specialiteti[50];
            int nr_vizitave;
        };

        struct info
        {
            int mjeku_id;
            char specialiteti[50];
        };

        struct info temp_info[100];
        struct info_f temp_info_f[100];
        struct vizita v;
        struct mjeku m;

        while (fread(&m, sizeof(m), 1, fp1)) {
            int found = 0;
            temp_info[i].mjeku_id = m.mjeku_id;
            strcpy(temp_info[i].specialiteti, m.specializimi);
            i++;

            for (int k = 0; k < l; k++) {
                lower_string(temp_info_f[k].specialiteti);
                lower_string(m.specializimi);
                if (!strcmp(temp_info_f[k].specialiteti, m.specializimi)) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                strcpy(temp_info_f[l].specialiteti, m.specializimi);
                temp_info_f[l].nr_vizitave = 0;
                l++;
            }
        }

        while (fread(&v, sizeof(v), 1, fp)) {
            for (int k = 0; k < i; k++) {
                if (temp_info[k].mjeku_id == v.mjeku_id) {
                    for (int m = 0; m < l; m++) {
                        lower_string(temp_info_f[m].specialiteti);
                        lower_string(temp_info[k].specialiteti);
                        if (!strcmp(temp_info_f[m].specialiteti, temp_info[k].specialiteti)) {
                            temp_info_f[m].nr_vizitave += 1;
                            break; 
                        }
                    }
                    break;
                }
            }
        }

    if (l) {
        int sorted = 0;
        while (!sorted) {
            sorted = 1;

            for (int j = 1; j < l; j++) {
                if (temp_info_f[j].nr_vizitave > temp_info_f[j-1].nr_vizitave) {
                    struct temp {
                        char specialiteti[70];
                        int nr_vizitave;
                    };
                    struct temp temp;

                    strcpy(temp.specialiteti, temp_info_f[j].specialiteti);
                    temp.nr_vizitave = temp_info_f[j].nr_vizitave;

                    strcpy(temp_info_f[j].specialiteti, temp_info_f[j-1].specialiteti);
                    temp_info_f[j].nr_vizitave = temp_info_f[j-1].nr_vizitave;

                    strcpy(temp_info_f[j-1].specialiteti, temp.specialiteti);
                    temp_info_f[j-1].nr_vizitave = temp.nr_vizitave;

                    sorted = 0;
                }
            }
        }

        for (int k = 0; k < l; k++) {
            printf("\n%s: %d\n", temp_info_f[k].specialiteti, temp_info_f[k].nr_vizitave);
        }
    } else {
        printf("\nNuk është kryer asnjë vizitë\n");
    }
    } else {
        jep_error();
    }
    fclose(fp1);
    fclose(fp);
    printf("\nRikthehuni duke shtypur ENTER...");
}