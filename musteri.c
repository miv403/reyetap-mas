#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "./csv.c"

// FIXME BU PROGRAM DA HATALAR VAR SU AN KAFAM BASMIYO HANGISI HATALI DIYE ~~~MIV403

#define MAX_YEMEK_ADI_UZUNLUGU 100
#define MAX_SATIR_UZUNLUGU 256
#define YEMEK_LISTESI "./veri/yemekListesi.csv"
#define SIPARIS "./veri/siparisler.csv"

// Fonksiyon prototipleri 
void yeniSiparis();
void mevcutSiparisDurumu();
void oncekiSiparisler();

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
    printf("%-15s%-10s%-12s%-10s\n%31s","Yemek Adi", // başlik satiri
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

    printf("Siparis vermek istediginiz yemegin numarasini giriniz: ");
    int kullaniciSecim;
    scanf("%d", &kullaniciSecim);
    
    char secilenAd[20] = "";
    char secilenFiyat[5] = "";
    char secilenSure[5] = "";
    char secilenDurum[6] = "";

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

    FILE *siparisDosyasi = fopen("./veri/siparisler.csv", "a");
    
    siparisDosyasi = fopen("./veri/siparisler.csv", "a");

    if(siparisDosyasi == NULL) {
        printf("Siparisler dosyasi acilamadi.\n");
        return;
    }

        // Siparis bilgilerini olustur
    
    char *kullaniciAdi = "Kullanici1"; // TODO: kullanici adi burada simdilik sabit olarak tanimlandi ama ileride kullanici adi alinacak.
    
    int hazirlanmaSuresi = atoi(secilenSure);

    // atoi(): str -> int // https://en.cppreference.com/w/c/string/byte/atoi

    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    char zamanDamgasi[15] = ""; // zaman damgasi icin 15 karakterlik bir dizi olusturuldu. (S-YYYYMMDD-XYZ)
    static int sira = 1; // FIXME: bu değişken gereksiz kullanıcı sipariş verdikten sonra program kapanacak ve değişken silinecek.
    char siraStr[4];
    sprintf(siraStr, "%03d", sira);
    strftime(zamanDamgasi, sizeof(zamanDamgasi), "S-%Y%m%d-%siraStr", now);
    sira++;


    // Siparis bilgilerini dosyaya yaz
    
    csvYaz(siparisDosyasi, 7,
            zamanDamgasi,
            secilenAd,
            secilenFiyat,
            "FIXME", //FIXME: buraya siparis ZAMANI 
            secilenSure,            //(ve buraya) hazirlanma ZAMANI
            kullaniciAdi,                   // gelecek suresi değil
            "A1" );
    fclose(siparisDosyasi);

    printf("Siparisiniz alinmistir. Hazirlanma suresi: %d dakika.\n", hazirlanmaSuresi);

}

// FIXME: HAZİRLANMA ZAMANİ EKRANA BASTIRILMIYOR
// HAZİRLANMA ZAMANININ KALDIRILMASINDAN ÖTÜRÜ HATALI

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

    printf("\n%-15s%-15s%-10s%-20s%-15s%-10s\n",
    // printf("\n%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
           "Siparis ID",
           "Yemek Adi",
           "Fiyat (TL)",
           "Siparis Zamani",
           // "Hazirlanma Zamani",
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
        // char *hazirlanmaZamani = strtok(NULL, ",");
        char *kullaniciAdi = strtok(NULL, ",");
        char *asci = strtok(NULL, "\n");

        if (!zamanDamgasi || !yemekAdi || !fiyat || !siparisZamani || !kullaniciAdi || !asci) {
        // if (!zamanDamgasi || !yemekAdi || !fiyat || !siparisZamani || !hazirlanmaZamani || !kullaniciAdi || !asci) {
            fprintf(stderr, "Siparis dosyasi satiri tam okunamadi veya eksik veri: %s\n", satir);
            continue; // siradaki satira atlar
        }

        if (strcmp(kullaniciAdi, kullaniciAdiIN) == 0) {
            siparisBulundu = 1;
            printf("%-15s%-15s%-10s%-20s%-20s%-15s\n",
            //printf("%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
                   zamanDamgasi,
                   yemekAdi,
                   fiyat,
                   siparisZamani,
                  // hazirlanmaZamani,
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

    printf("\n%-15s%-15s%-10s%-20s%-15s%-10s\n",
    // printf("\n%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
           "Siparis ID",
           "Yemek Adi",
           "Fiyat (TL)",
           "Siparis Zamani",
           // "Hazirlanma Zamani",
           "Kullanici Adi",
           "Asci");
    printf("-----------------------------------------------------------------------------------------\n");

    char satir[MAX_SATIR_UZUNLUGU];
    int siparisBulundu = 0;
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
        // char *hazirlanmaZamani = strtok(NULL, ",");
        char *kullaniciAdi = strtok(NULL, ",");
        char *asci = strtok(NULL, "\n");

        if (!zamanDamgasi || !yemekAdi || !fiyat || !siparisZamani || !kullaniciAdi || !asci) {
        // if (!zamanDamgasi || !yemekAdi || !fiyat || !siparisZamani || !hazirlanmaZamani || !kullaniciAdi || !asci) {
            fprintf(stderr, "Siparis dosyasi satiri tam okunamadi veya eksik veri: %s\n", satir);
            continue; // Skip to the next line
        }

        if (strcmp(asci, "0") != 0) {
            siparisBulundu = 1;
            printf("%-15s%-15s%-10s TL %-20s%-15s%-10s\n",
            //printf("%-15s%-15s%-10s TL %-20s%-20s%-15s%-10s\n",
                   zamanDamgasi,
                   yemekAdi,
                   fiyat,
                   siparisZamani,
                   // hazirlanmaZamani,
                   kullaniciAdi,
                   asci);
        }
    }

    if (!siparisBulundu) {
        printf("Tamamlanmis siparis bulunamadi.\n");
    }

    fclose(siparisDosyasi);
}
