/*  CSV DOSYA ISLEMLERI
    date: 2024-05-12
    author: miv403
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: fonksiyonlar satir ve sütün sınır degerlerine toleransli olmayabilir. 

/***********************************************************************/

// verilen satır ve sütun numarasını dosyadan geri döndüren fonksiyon

// TODO bu tanımlanamalar ile ilgili sınır değerleri eklenebilir.
// TODO csvHucreAl() adı csvHucreAl ya da csvAl csvGet gibi daha 
//      açıklayıcı bir ad ile değiştirilebilir.


// HATA TOLERANSLI FONKSIYON ICIN GEREKLILER BAZI PARAMETRELER

//#define MAX_SATIR 150 // satır maks. karakter sayisi
//#define MAX_SUTUN 4
//#define CSV_YOL "./yemekListesi.csv"
//#define SUTUN_SINIRI 4

int csvHucreAl(FILE *dosya,        // dosya işaretçisi
                char *hucreOut,     // çıktı adresi
                size_t maxSatirC,   // maks. okunacak satir karakteri sayisi
                                        // char[] için gerekli.
                int satirNo,        // istenen satir numarasi 1 < satirNo
                int sutunNo) {      // istenen sütün numarasi 1 < sutunNo

// satirNo ve sutunNo 1 degerinden baslamali. ancak fonksyion satir ve
// hucre numaralarina 0 degerinden basliyormus gibi davranacak.

    char satir[maxSatirC];
    char *hucre;
    
    fgets(satir, maxSatirC, dosya); // ilk satır alınmayacak bundan ötürü daima 
                                    // bir kez çalıştırılıyor.
    for(size_t i = 0;i < satirNo; ++i){
        fgets(satir, maxSatirC, dosya);
        // dosyanın satirNo satirina kadar satir alınıyor
        // son alınan satır istenen satır oluyor.
        if (satir == NULL){
            return 0;
        }
    }

    hucre = strtok(satir, ",");
    for(size_t i = 1; (i < sutunNo) && (hucre != NULL); ++i) {
        // en son alınan satır istenen hücreye kadar bölünüyor.
        // son alınan hücre istenen hücre oluyor
        hucre = strtok(NULL, ",");
    }

    // hücrenin değeri verilen char * 'a yazdırılıyor.
    while(*hucre != '\0'){ //FIXME: strcpy()
        *hucreOut = *hucre;
        ++hucre;
        ++hucreOut;
    }

    return 1;
}

/***********************************************************************/

//.csv dosyasının içeriğini ekrana çizelge biçiminde yazdıracak fonksiyon

// TODO bu tanımlanamalar ile ilgili sınır değerleri eklenebilir.

// HATA TOLERANSLI FONKSIYON ICIN GEREKLILER
//#define CSV_YOL "./yemekListesi.csv"
//#define MAX_SATIR_C 150 // satirdaki maks karakter sayisi
//#define MAX_HUCRE 4     // TODO satirdaki maks hucre sayisi
//#define SUTUN_SINIRI 4  // TODO EKLENMEDI: sutun sınırı koyulabilir

int yemekListesiYazdir(FILE *dosya,      // okunacak dosya işaretçisi
            size_t maxSatirC) { // okunmak istenen satir sayisi

// UYARI: .csv dosyalarının içerisinde Türkçe karakter bulunmamalı      <<<<<
// Türkçe karakterler fazladan karakter yeri kaplayarak çıktıda kaymalara
// neden oluyorlar.

    char satir[maxSatirC];
    char *hucre;
    int sutunNo = 0;
    int satirNo = 0;

    while( fgets(satir, maxSatirC, dosya) != NULL) {
    // dosyadan tek bir satır alınıyor

    // satır hücrelere bölünüyor
        hucre = strtok(satir, ",");
        sutunNo = 0;
        while(hucre != NULL) {
            switch (satirNo) {
                case  0:
                    /* başlık satırı kullanılmaz */
                    break;
                default:
                    switch (sutunNo) {  // sütun numarasına göre farklı
                                        // büyüklükte biçimlendirme
                                        // gerektiğinden switch-case kullandım.

                    // TODO: her .csv üzerinde
                    // çalışabilmesi için switch-case
                    // kaldırılıp tekil bir tablo çıktısı
                    // almak gerekiyor.
                        case 0:
                            printf("%-15s", hucre);
                            break;
                        case 1:
                            printf("%-10s", hucre);
                            /* code */
                            break;
                        case 2:
                            printf("%-20s", hucre);
                            /* code */
                            break;
                        case 3: // TODO: bu durum şu anlık sadece yemekListesi
                                // csv'deki durum sütunu için yazılmıştır. Her
                                // .csv dosyası için tekrardan düzenlenmelidir

                            /* bu bölümün amacın sondaki \n karakterini
                                yoksaymak içindir.
                            
                                while(*hucre != '\n'){
                                    printf("%c", *hucre);
                                    ++hucre;
                                }

                                            YA DA

                                printf("%-10s", *hucre);

                            bu kullanılırsa puts("") kaldırılmalı. */
                            if(*hucre == 't' || *hucre == 'T') {
                            // yemekListesi için bu hucre bool degerdedir
                            // FIXME bool icin geçici çözüm baş karakter
                                printf("%-10s", "✓");
                            } else {
                                printf("%-10s", "⨯");
                            }
                            puts("");
                            break;
                        default:
                            break;
                    }
                    break;
            }
            hucre = strtok(NULL, ",");
            sutunNo++;
        }
        ++satirNo;
    }
    
}

/***********************************************************************/

// siparisler.csv içeriğini yazacak fonksiyon

int siparislerYazdir(FILE *dosya,   // okunacak dosya işaretçisi
            size_t maxSatirC) {     // okunmak istenen satir sayisi

// UYARI: .csv dosyalarının içerisinde Türkçe karakter bulunmamalı      <<<<<
// Türkçe karakterler fazladan karakter yeri kaplayarak çıktıda kaymalara
// neden oluyorlar.

    char satir[maxSatirC];
    char *hucre;
    int sutunNo = 0;
    int satirNo = 0;

    while( fgets(satir, maxSatirC, dosya) != NULL) {
    // dosyadan tek bir satır alınıyor

    // satır hücrelere bölünüyor
        hucre = strtok(satir, ",");
        sutunNo = 0;
        while(hucre != NULL) {
            switch (satirNo) {
                case  0:
                    break;
                default:
                    switch (sutunNo) {  

                        case 0:
                            printf("%-15s", hucre);
                            break;
                        case 1:
                            printf("%-15s", hucre);
                            /* code */
                            break;
                        case 2:
                            printf("%-15s",hucre);
                            /* code */
                            break;
                        case 3:
                            printf("%-20s", hucre);
                            break;
                        case 4:
                            printf("%-20s", hucre);
                            break;
                        case 5:
                            printf("%-10s", hucre);
                            break;
                        case 6:
                            printf("%-8s", " ");
                            while(*hucre != '\n') {
                                printf("%c", *hucre);
                                hucre++;
                            }
                            puts("");
                            break;
                        default:
                            break;
                    }
                    break;
            }
            hucre = strtok(NULL, ",");
            sutunNo++;
        }
        ++satirNo;
    }
    
}

/***********************************************************************/

// .csv dosyasına verileri yazacak program

// değişken sayida argüman almak için:
//      https://www.youtube.com/watch?v=3iX9a_l9W9Yo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//#define CSV_YOL "./siparisler.csv"

// hucreSayisi: fonksiyona kaç tane hücre argüman olarak verilmişse sayısı
// fonksiyon tüm argümanları char[] olarak bekliyor

void csvYaz(FILE *dosya,  int hucreSayisi, ... ) {

    va_list args;
    va_start(args, hucreSayisi);

    for (int i = 0; i < hucreSayisi - 1; ++i) {
        char *str = va_arg(args, char *);
        fprintf(dosya, "%s,", str);
    }
    char *str = va_arg(args, char *);
    fprintf(dosya, "%s", str);
    fprintf(dosya, "\n");

    va_end(args);
}

/**********************  FONKSIYON TESTLERI  ***************************/
/***********************************************************************/

//                  TESTLERİ KULLANMADAN ÖNCE OKU!

// YORUM SATIRLARINI GERI KOYMAYI UNUTMA
// BURADAKİ TESTLER AYRI AYRI ÇALIŞMAK ÜZERE YAZILMIŞTIR.
// BÜTÜN YORUM SATIRLARINI AYNI ANDA KALDIRMAYIN.

// DOSYA YOLLARI ÖRNEK BİÇİMDEDİR: İSMİ UYUŞAN DOSYANIN TAM YOLUNU
// DEĞİŞTİRMEDEN ÇALIŞTIRMAYIN.

// int main(void) { // testlerin hepsi bu dosya derlenerek çalışmaktadır.
                    // bundan ötürü main() yorum satırı olmamalı.

/************************  csvHucreAl() TEST  **************************/
/*
    FILE *dosya;

    dosya = fopen("./yemekListesi.csv", "r");

    if(dosya == NULL) {
        puts("dosya acilirken hata olustu");
        return 1;
    }

    char istenenDeger[30];
    csvHucreAl(dosya, istenenDeger, 150 , 2, 1);

    printf("%s\n", istenenDeger);


*/
/************************  yemekListesiYazdir() TEST  ***************************/
/*
    FILE *dosya;

    dosya = fopen("./yemekListesi.csv", "r");
    if (dosya == NULL) {
        puts("dosya açılırken hata oluştu.");
        return 1;
    }

    printf("%-15s%-10s%-20s%-10s","Yemek Adi", // başlik satiri
                            "Fiyati",
                            "Hazirlanma Suresi",
                            "Durum");
    puts(""); // yeni satir

    yemekListesiYazdir(dosya, 150);

    fclose(dosya);

    return 0;
*/
/***************************  csvYaz() TEST  *****************************/
/*
    FILE *dosya;
    dosya = fopen("./siparisler.csv", "a");
    if (dosya == NULL){
        puts("dosya acilirken hata olustu.");
        return 1;
    }

    char yemekID[] = "SIP2024-02-31_006";
    char yemekAdi[] = "Cig Kofte :)";
    char yemekFiyati[] = "61";

    csvYaz(dosya, 3, yemekID, yemekAdi, yemekFiyati);

    fclose(dosya);

    return 0;
*/
/***********************************************************************/

// }
