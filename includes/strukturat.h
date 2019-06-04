struct mjeku
{
    int mjeku_id;
    char emri[30];
    char mbiemri[30];
    char specializimi[70];
};

struct pacienti
{
    int pacienti_id;
    char emri[30];
    char mbiemri[30];
    char adresa[70];
    char nr_tel[20];
};

struct date
{
    int data;
    int muaji;
    int viti;
};

struct vizita
{
    struct date date;
    int pacienti_id;
    int mjeku_id;
    char diagnoza[100];
};