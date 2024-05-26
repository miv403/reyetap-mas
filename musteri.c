#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include "./csv.c"

#define MAX_YEMEK_ADI_UZUNLUGU 100
#define MAX_SATIR_UZUNLUGU 1024
#define YEMEK_LISTESI "./veri/yemekListesi.csv"
#define SIPARIS "./veri/siparisler.csv"
#define AD_MAX 50
#define SIPARIS_MAX 50
// FIXME BU PROGRAM DA HATALAR VAR SU AN KAFAM BASMIYO HANGISI HATALI DIYE ~~~MIV403

void yeniSiparis();
void mevcutSiparisDurumu();
void oncekiSiparisler();
int siparislerOku(FILE * dosya,  size_t siparisMax);
bool zamanKarsilastir(const struct tm *zaman1, const struct tm *zaman2);
void stringToTM (const char * stringIN, struct tm  * zaman);

int main() {
        // Kullanıcı arayuzu ve menu
    printf("1. Yeni Siparis\n");
    printf("2. Mevcut Siparis Durumu\n");
    printf("3. Onceki Siparislerim\n");

    int secim;
    printf("Seciminizi yapiniz: ");
    scanf("%d", &secim);

    switch(secim) {
        case 1:
            // Yeni siparis fonksiyonu: Kullaniciya menuyu gosterir, siparis alir ve kaydeder.
            yeniSiparis();
            break;
        case 2:
            // Mevcut siparis durumu fonksiyonu: Aktif siparisleri gosterir.
            mevcutSiparisDurumu();
            break;
        case 3:
            // Onceki siparisler fonksiyonu: Tamamlanmis siparisleri gosterir.
            oncekiSiparisler();
            break;
        default:
            printf("Gecersiz secim!\n");
    }
    return 0;
}

// Yeni Siparis Fonksiyonu
void yeniSiparis() {
        // [Islev]: Kullanıcıya menüyü gosterir, siparis alir ve kaydeder.
        // Yemek listesini yukle
    FILE *yemekListesi;
    yemekListesi = fopen("./veri/yemekListesi.csv", "r");
    if (yemekListesi == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
    }

        // Menuyu kullaniciya goster
    puts(""); 
    printf("%-15s%-10s%-12s%-10s\n%31s", // TODO: no satiri eklenmeli ayrica kayma olusuyor
                            "Yemek Adi", // başlik satiri
                            "Fiyati",
                            "Hazirlanma ",
                            "Durum",
                            "Suresi");
    puts(""); 
    puts("-------------------------------------------------------"); // yeni satir

    /* 
    Yemek Adi      Fiyati    Hazirlanma          Durum
                             Suresi
    -------------------------------------------------------
    */

    yemekListesiYazdir(yemekListesi, 150);
    fclose(yemekListesi);

    // FIXME: yemek listesi yazdırırken yemeklerin numarası yazdırılmıyor

    yemekListesi = fopen("./veri/yemekListesi.csv", "r");
    if (yemekListesi == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
        return;
    }
    char kullaniciAdi[20] = "Kullanici1";
    printf("Kullanici Adi giriniz: ");
    scanf("%19s", kullaniciAdi);
    // TODO: kullanici adi burada simdilik sabit olarak tanimlandi ama ileride kullanici adi alinacak.

    printf("Siparis vermek istediginiz yemegin numarasini giriniz: ");
    int kullaniciSecim;
    scanf("%d", &kullaniciSecim);
    
    char secilenAd[20] = "";
    char secilenFiyat[5] = "";
    char secilenSure[5] = "";
    char secilenDurum[10] = "";
    // TODO: struct kullan ve yemekListesiOku() (mutfak.c)
    csvHucreAl(yemekListesi, secilenAd, 150, kullaniciSecim, 1);
    rewind(yemekListesi);
    csvHucreAl(yemekListesi, secilenFiyat, 150, kullaniciSecim, 2);
    rewind(yemekListesi);
    csvHucreAl(yemekListesi, secilenSure, 150, kullaniciSecim, 3);
    rewind(yemekListesi);
    csvHucreAl(yemekListesi, secilenDurum, 150, kullaniciSecim, 4);
    fclose(yemekListesi);

    if(secilenDurum[0] == 'T' || secilenDurum[0] == 't') {
        puts("Su siparisi verdiniz: ");
        printf("%s%s", "Yemek Adi: ", secilenAd);
        puts("");
        printf("%s%s", "Fiyati: ", secilenFiyat);
        puts("");
        printf("%s%s", "Hazirlanma Suresi: ", secilenSure);
        puts("");
    }else {
        puts("Sectiginiz yemek su an mevcut degil: ");
        printf("%s%s", "Yemek Adi: ", secilenAd);
    }
    puts("");

        // Siparis bilgilerini dosyaya yaz

    FILE *siparisDosyasi;
    siparisDosyasi = fopen("./veri/siparisler.csv", "r");

    if(siparisDosyasi == NULL) {
        printf("Siparisler dosyasi acilamadi.\n");
        return;
    }

    int siparisSayisi = siparislerOku(siparisDosyasi, SIPARIS_MAX);

    fclose(siparisDosyasi);

    siparisDosyasi = fopen("./veri/siparisler.csv", "a");
        // Siparis bilgilerini olustur
    
    
    int hazirlanmaSuresi = atoi(secilenSure);

    // atoi(): str -> int // https://en.cppreference.com/w/c/string/byte/atoi

    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    char zamanDamgasi[17] = "";
    char siparisZamani[17] = "";
    
    strftime(siparisZamani, 17, "%Y-%m-%dT%H.%M", now);
    
    // FIXME: sıra numarası

    strftime(zamanDamgasi,17, "S-%Y%m%d", now);
    char siparisID[AD_MAX];
    sprintf(siparisID, "%s-%d",zamanDamgasi, siparisSayisi);
    // Siparis bilgilerini dosyaya yaz
    
    if(secilenDurum[0] == 'T' || secilenDurum[0] == 't') {
        csvYaz(siparisDosyasi, 7,
                siparisID,
                secilenAd,
                secilenFiyat,
                siparisZamani,
                "0", 
                kullaniciAdi,
                "XX" );
        fclose(siparisDosyasi);
        printf("Siparisiniz alinmistir. Hazirlanma suresi: %d dakika.\n", hazirlanmaSuresi);
    }
}

void mevcutSiparisDurumu() {
    FILE *siparisDosyasi = fopen(SIPARIS, "r");
    if (siparisDosyasi == NULL) {
        if (errno == ENOENT) { // dosya bulunamazsa
            printf("Siparisler dosyasi bulunamadi, yeni bir dosya olusturuluyor...\n");
            siparisDosyasi = fopen(SIPARIS, "w");
            if (siparisDosyasi == NULL) {
                perror("Yeni siparisler dosyasi olusturulamadi");
                return;
            }
            fprintf(siparisDosyasi, "SiparisID,YemekAdi,YemekFiyati,SiparisZamani,HazirlanmaZamani,KullaniciAdi,Asci\n");
            fclose(siparisDosyasi);
            printf("Yeni siparisler dosyasi olusturuldu: %s\n", SIPARIS);
            return;
        } else {
            perror("Siparisler dosyasi acilamadi");
            return;
        }
    }

    printf("Kullanici adi girin: ");
    char kullaniciAdiIN[15];
    if (scanf("%14s", kullaniciAdiIN) != 1) {
        fprintf(stderr, "Kullanici adi alinamadi.\n");
        fclose(siparisDosyasi);
        return;
    }

    printf("\n%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
            "Siparis ID",
            "Yemek Adi",
            "Fiyat (TL)",
            "Siparis Zamani",
            "Hazirlanma Zamani",
            "Kullanici Adi",
            "Asci");
    printf("-----------------------------------------------------------------------------------------\n");


    char satir[MAX_SATIR_UZUNLUGU];
    int siparisBulundu = 0;
    while (fgets(satir, MAX_SATIR_UZUNLUGU, siparisDosyasi) != NULL) {
        // satir okumasi tamamlanma kontrolu yapar
        if (strchr(satir, '\n') == NULL) {
            fprintf(stderr, "Satir tam okunamadi veya cok uzun: %s\n", satir);
            continue; // siradaki satira atlar
        }

        char *zamanDamgasi = strtok(satir, ",");
        char *yemekAdi = strtok(NULL, ",");
        char *fiyat = strtok(NULL, ",");
        char *siparisZamani = strtok(NULL, ",");
        char *hazirlanmaZamani = strtok(NULL, ",");
        char *kullaniciAdi = strtok(NULL, ",");
        char *asci = strtok(NULL, "\n");

        if(!(strcmp(hazirlanmaZamani,"0"))) {
            strcpy(hazirlanmaZamani, "Hazirlaniyor");
        }

        if (!zamanDamgasi || !yemekAdi || !fiyat || !siparisZamani || !hazirlanmaZamani || !kullaniciAdi || !asci) {
            fprintf(stderr, "Siparis dosyasi satiri tam okunamadi veya eksik veri: %s\n", satir);
            continue; // siradaki satira atlar
        }

        // struct tm hazirlanmaZamaniTM;

        // stringToTM(hazirlanmaZamani, &hazirlanmaZamaniTM);

        if (asci[0] == 'A' && strcmp(kullaniciAdi, kullaniciAdiIN) == 0) {
            siparisBulundu = 1;
            printf("%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
                    zamanDamgasi,
                    yemekAdi,
                    fiyat,
                    siparisZamani,
                    hazirlanmaZamani,
                    kullaniciAdi,
                    asci);
        }
    }

    if (!siparisBulundu) {
        printf("Bu kullanici adina ait siparis bulunamadi.\n");
    }

    fclose(siparisDosyasi);
}


// FIXME: HAZİRLANMA ZAMANİ EKRANA BASTIRILMIYOR
// HAZİRLANMA ZAMANININ KALDIRILMASINDAN ÖTÜRÜ HATALI
// FIXME: başka hatalar olabilir

void oncekiSiparisler() {
    FILE *siparisDosyasi = fopen(SIPARIS, "r");
    if (siparisDosyasi == NULL) {
        if (errno == ENOENT) { // File not found
            printf("Siparisler dosyasi bulunamadi.\n");
            return;
        } else {
            perror("Siparisler dosyasi acilamadi");
            return;
        }
    }

    char kullaniciAdiIN[20] = "";
    printf("Kullanici adi girin: ");
    scanf("%19s", kullaniciAdiIN);

    printf("\n%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
            "Siparis ID",
            "Yemek Adi",
            "Fiyat (TL)",
            "Siparis Zamani",
            "Hazirlanma Zamani",
            "Kullanici Adi",
            "Asci");
    printf("-----------------------------------------------------------------------------------------\n");

    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    char satir[MAX_SATIR_UZUNLUGU];
    int siparisBulundu = 0;
    fgets(satir, MAX_SATIR_UZUNLUGU, siparisDosyasi);
    while (fgets(satir, MAX_SATIR_UZUNLUGU, siparisDosyasi) != NULL) {
        // Check if the line read is complete
        if (strchr(satir, '\n') == NULL) {
            fprintf(stderr, "Satir tam okunamadi veya cok uzun: %s\n", satir);
            continue; // Skip to the next line
        }

        char *zamanDamgasi = strtok(satir, ",");
        char *yemekAdi = strtok(NULL, ",");
        char *fiyat = strtok(NULL, ",");
        char *siparisZamani = strtok(NULL, ",");
        char *hazirlanmaZamani = strtok(NULL, ",");
        char *kullaniciAdi = strtok(NULL, ",");
        char *asci = strtok(NULL, "\n");

        if (!zamanDamgasi || !yemekAdi || !fiyat || !siparisZamani || !hazirlanmaZamani || !kullaniciAdi || !asci) {
            fprintf(stderr, "Siparis dosyasi satiri tam okunamadi veya eksik veri: %s\n", satir);
            continue; // Skip to the next line
        }

        // TODO: hazirlanmaZamani geçmiş zamanda mı karşılaştırılmalı

        struct tm hazirlanmaZamaniTM;

        stringToTM(hazirlanmaZamani, &hazirlanmaZamaniTM);

        if (strcmp(asci, "XX") != 0 && strcmp(kullaniciAdiIN, kullaniciAdi) == 0 && !(zamanKarsilastir(&hazirlanmaZamaniTM, now))) {
            siparisBulundu = 1;
            printf("%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
                    zamanDamgasi,
                    yemekAdi,
                    fiyat,
                    siparisZamani,
                    hazirlanmaZamani,
                    kullaniciAdi,
                    asci);
        }
    }

    if (!siparisBulundu) {
        printf("Tamamlanmis siparis bulunamadi.\n");
    }

    fclose(siparisDosyasi);
}

int siparislerOku(FILE * dosya,  size_t siparisMax){

    // okunan sipariş sayisini geri döndürüyor

    char satir[MAX_SATIR_UZUNLUGU];

    int i = 1;
    fgets(satir, 256, dosya);
    while(fgets(satir, 256, dosya) != NULL && i < siparisMax) {
        i++;
    }
    int siparisSayisi = i; // okunan sipariş sayısı 

    return siparisSayisi; 
}

bool zamanKarsilastir(const struct tm *zaman1, const struct tm *zaman2) {
    // zaman1 >= zaman2 TRUE
    // zaman1 < zaman2 FALSE
    
    time_t hamZaman1 = mktime((struct tm*)zaman1); 
    
    // mktime() zaman1'i değiştirdiğinden ötürü
    // cast işleci kullanılıyor.

    time_t hamZaman2 = mktime((struct tm*)zaman2);

    if (hamZaman1 >= hamZaman2) return true;  // eşit ya da sonra
    if (hamZaman1 < hamZaman2) return false; // zaman1 daha erken
}

void stringToTM (const char * stringIN, struct tm  * zaman) {

    // bir string değişkeni tm nesnesine dönüştüren işlev.

    char stringINCPY[17]; // verilen string'in kopyası
    strcpy(stringINCPY, stringIN);
    char *bolut; // strtok() ile alınan bölütler
    
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

        zaman->tm_sec = 0; // bu tek başına girilmezse garbage value oluyor SİLME!!!
    }
}