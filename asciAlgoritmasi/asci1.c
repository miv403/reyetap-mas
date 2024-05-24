#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define SIPARIS_MAX 5
#define ASCILAR_MAX 3
#define AD_MAX 50
#define YEMEK_MAX 3
#define YEMEK_LISTESI "./veri/yemekListesi.csv" //TODO dosya konumunu değiştirmeyi unutma
#define SIPARIS_LISTESI "./asciAlgoritmasi/siparisler.csv"

typedef struct {
    // const char *siparisZamani; // 2024-05-23T10.00
    // const char *hazirlanmaSuresi; // X dakika
    struct tm siparisZamani;    // 2024-05-23T10.00
    struct tm hazirlanmaZamani; // 2024-05-23T10.30
    int hazirlanmaSuresi;       // 30 dakika
    int siparisNo;              // 
    char ad[AD_MAX];
    char onay[5];
} Siparis;

typedef struct {
    int siparisNo;              // siparisler.csv siparis konumu.
    struct tm * uygunZamani;    // &siparisler[i].hazirlanmaZamani
    Siparis * atananSiparis;    // Siparis pointer
}Asci;

typedef struct {
    char ad[AD_MAX];
    int hazirlanmaSuresi;
} Yemek;

struct tm sifir = { .tm_year = 100,
                    .tm_mon = 0,
                    .tm_mday = 1,
                    .tm_hour = 0,
                    .tm_min = 0,
                    .tm_sec = 0 };


void yemekListesiOku(FILE * dosya, Yemek yemekler[]);
void stringToTM (const char * stringIN, struct tm * zaman);
bool zamanKarsilastir(const struct tm *zaman1, const struct tm *zaman2);
size_t enUygunAsci(Asci ascilar[], size_t ascilarMax);

int main(void) {

    Siparis siparisler[SIPARIS_MAX];
    Asci ascilar[3];
    Yemek yemekler[YEMEK_MAX];

    
    /****** YEMEK LISTESINI OKU ******/
    
    FILE *yemekListesi;
    
    yemekListesi = fopen(YEMEK_LISTESI, "r");
    if (yemekListesi == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.");
        puts("");
        return -1;
    }

    yemekListesiOku(yemekListesi, yemekler);

    for(int i = 0; i < YEMEK_MAX; ++i){

        printf("ad: %s\nhazirlanmaSuresi: %d\n", yemekler[i].ad, yemekler[i].hazirlanmaSuresi);
        puts("");

    }

    /*********************************/

    /******** SIPARISLERI OKU *******/

    FILE *siparislerDosyasi;
    siparislerDosyasi = fopen(SIPARIS_LISTESI, "r");
    if (siparislerDosyasi == NULL) {
        puts("Siparisler dosyasi bulunamadi!");
        return -1;
    }
    
    char satir[256];
    size_t i = 0;
    fgets(satir, 256, siparislerDosyasi);
    while(fgets(satir, 256, siparislerDosyasi) != NULL && i < SIPARIS_MAX) {
        
        char siparisZamani[17];
        sscanf(satir,
        "%*[^,] , %[^,] , %*[^,] , %[^,] , %*[^,] , %*[^,] , %*[^,]", 
            siparisler[i].ad,
            siparisZamani);
        stringToTM(siparisZamani, &siparisler[i].siparisZamani);
        i++;
    }
    size_t siparisSayisi = i;

    char output[17];

    for(size_t i = 0; i < siparisSayisi; ++i) {
        printf("%s\n", siparisler[i].ad);
        strftime(output,17, "%Y-%m-%dT%H.%M", &siparisler[i].siparisZamani);
        puts(output);
    }

    /*********************************/



    /*********** ASCI ATAMA **********/

    // sadece A0 siparişleri alınacak

    /*
    uygunluk zamanı en uygun olan aşçılar numaralarına göre sırayla
    atanır.
    
    aşçıların numaralarına göre atanması konumları değiştirilmeden 
    karşılaştırma algoritması ile yapılmalı. uygunluk zamanı en son uygun 
    olduğu zamana hazirlanmaSuresi eklenerek güncellenmeli ve bu zaman aynı 
    zamanda siparişteki ilgili konuma da yazılmalı.

>   aşçının müsait olma zamanı sipariş zamanı ile karşılaştırılmalı:

    musait olma zamani siparişin verilme zamanından önceyse müsait olma zamani:
    >   sipariş verilme zamanina hazirlanma süresi eklenerek güncellenmeli.

    eğer müsait olma zamani siparişin verilme zamanindan sonra veya aynıysa 
    müsait olma zamani;
    >   müsait olma zamanina hazirlanma süresi eklenerek güncellenmeli.

    */
    // EN UYGUN AŞÇIYI BULMA

//    enUygunAsci(ascilar, ASCILAR_MAX);

    // ZAMAN KARŞILAŞTIRMA

    // zamanKarsilastir(ascilar[i].uygunZamani, &siparisler[j].siparisZamani);

    // ZAMAN EKLEME (HAZIRLANMA SURESI)

    // DOSYAYA EKLEME

    /*********************************/


    // ascilar[0].uygunZamani = &siparisler[0].siparisZamani;  
    char zamanGirdisi000[] = "2024-05-23T10.31";
    stringToTM(zamanGirdisi000,&siparisler[0].siparisZamani);

    ascilar[0].uygunZamani = &siparisler[0].siparisZamani;

    strftime(output,17, "%Y-%m-%dT%H.%M", &siparisler[0].siparisZamani);
    puts(output);
    strftime(output,17, "%Y-%m-%dT%H.%M", ascilar[0].uygunZamani);
    puts(output);
}

size_t enUygunAsci(Asci ascilar[], size_t ascilarMax) { 
    
    // en erken uygun olacak aşçının konumunu geri döndürür.

    size_t enKucuk = 0, i;

    for(i = 1; i < ascilarMax; ++i) {

        if(!(zamanKarsilastir(ascilar[i].uygunZamani, ascilar[enKucuk].uygunZamani))) {
            enKucuk = i;
        }
    }
    return enKucuk;
}

bool zamanKarsilastir(const struct tm *zaman1, const struct tm *zaman2) {
    // zaman1 >= zaman2 TRUE
    // zaman1 < zaman2 FALSE
    
    time_t hamZaman1 = mktime((struct tm*)zaman1); 
    
    // Note the cast, mktime modifies its argument

    time_t hamZaman2 = mktime((struct tm*)zaman2);

    if (hamZaman1 >= hamZaman2) return true;  // Equal or later
    if (hamZaman1 < hamZaman2) return false; // zaman1 is earlier
}

void yemekListesiOku(FILE * dosya, Yemek yemekler[]) {

    // yemek listesi dosyasından birinci(ad) ve üçüncü(hazirlanmaSuresi) sütunu okur.

    char satir[256];
    int yemekSayisi = 0;

    fgets(satir, 256, dosya);

    for(int i = 0; fgets(satir, 256, dosya) != NULL && i < YEMEK_MAX; ++i) {

        // , karakterine göre satırdan gerekli sütunları okuma

        sscanf(satir, "%[^,] , %*[^,] , %d , %*[^,]", yemekler[i].ad, &yemekler[i].hazirlanmaSuresi);
    }

}

void stringToTM (const char * stringIN, struct tm  * zaman) {

    char stringINCPY[17];
    strcpy(stringINCPY, stringIN);
    char *bolut;
    
    bolut = strtok(stringINCPY, "T");
    if (bolut != NULL) {
        bolut = strtok(bolut, "-");
        zaman->tm_year = atoi(bolut) - 1900;
        bolut = strtok(NULL, "-");
        zaman->tm_mon = atoi(bolut) - 1;
        bolut = strtok(NULL, "-");
        zaman->tm_mday = atoi(bolut);

        strcpy(stringINCPY, stringIN);

        bolut = strtok(stringINCPY, "T");
        bolut = strtok(NULL, "T");

        bolut = strtok(bolut, ".");
        zaman->tm_hour = atoi(bolut);
        bolut = strtok(NULL, ".");
        zaman->tm_min = atoi(bolut);
    }
}




