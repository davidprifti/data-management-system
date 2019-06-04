#include <ctype.h>
#include <stdio.h>
#include <string.h>

void lower_string(char *p)
{
    for (; *p; ++p)
    {
        *p = tolower(*p);
    }
}

int merr_zgjedhje_perdoruesi(void)
{
    char choice[3];
    printf("\nDëshironi të vazhdoni? P (Po) ose J (Jo): ");
    scanf("%s", choice);

    if (!strcmp(choice, "p") || !strcmp(choice, "P"))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void jep_error(void)
{
    printf("\nJu kërkojmë ndjesë! Ky veprim nuk mund të kryhet në këtë moment.\n");
}