#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./csv.c"

#define MAX_YEMEK_ADI_UZUNLUGU 100
#define MAX_SATIR_UZUNLUGU 256
#define YEMEK_LISTESI "./veri/yemeklistesi.txt"
#define SIPARIS "./veri/siparisler.txt"

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

    csvYazdir(yemekListesi, 150);
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
            "SIPARIS ZAMANI FIXME", //FIXME: buraya siparis ZAMANI 
            secilenSure,            //(ve buraya) hazirlanma ZAMANI
            kullaniciAdi,                   // gelecek suresi değil
            "A1" );
    fclose(siparisDosyasi);

    printf("Siparisiniz alinmistir. Hazirlanma suresi: %d dakika.\n", hazirlanmaSuresi);

}

// Mevcut Siparis Durumu Fonksiyonu
void mevcutSiparisDurumu() {
        // [Islev]: Aktif siparisleri gosterir.
        // Siparisler dosyasini ac
    FILE *siparisDosyasi = fopen(SIPARIS, "r");
    if (siparisDosyasi == NULL) {
        printf("Siparisler dosyasi bulunamadi.\n");
        return;
    }

        // Aktif siparisleri ekrana yazdir
    printf("Aktif Siparisler:\n");
    char satir[MAX_SATIR_UZUNLUGU];
    while (fgets(satir, MAX_SATIR_UZUNLUGU, siparisDosyasi) != NULL) {
        char *kullaniciAdi = strtok(satir, "|");
        char *yemekAdi = strtok(NULL, "|");
        char *fiyat = strtok(NULL, "|");
        char *siparisZamani = strtok(NULL, "|");
        char *hazirlanmaSuresi = strtok(NULL, "|");
        char *durum = strtok(NULL, "\n");
        if (strcmp(durum, "0") == 0) {
            printf("Kullanici: %s - Yemek: %s - Fiyat: %s TL - Siparis Zamani: %s - Hazirlanma Süresi: %s dk\n", kullaniciAdi, yemekAdi, fiyat, siparisZamani, hazirlanmaSuresi);
        }
    }
    fclose(siparisDosyasi);
}

// Önceki Siparişler Fonksiyonu
void oncekiSiparisler() {
        // [Islev]: Tamamlanmis siparisleri gosterir.
        // Siparisler dosyasini ac
    FILE *siparisDosyasi = fopen(SIPARIS, "r");
    if (siparisDosyasi == NULL) {
        printf("Siparisler dosyasi bulunamadi.\n");
        return;
    }

        // Onceki siparisleri ekrana yazdir
    printf("Onceki Siparisler:\n");
    char satir[MAX_SATIR_UZUNLUGU];
    while (fgets(satir, MAX_SATIR_UZUNLUGU, siparisDosyasi) != NULL) {
        char *kullaniciAdi = strtok(satir, ",");
        char *yemekAdi = strtok(NULL, ",");
        char *fiyat = strtok(NULL, ",");
        char *siparisZamani = strtok(NULL, ",");
        char *hazirlanmaSuresi = strtok(NULL, ",");
        char *durum = strtok(NULL, "\n");
        if (strcmp(durum, "0") != 0) {
            printf("Kullanici: %s - Yemek: %s - Fiyat: %s TL - Siparis Zamani: %s - Hazirlanma Suresi: %s dk\n", kullaniciAdi, yemekAdi, fiyat, siparisZamani, hazirlanmaSuresi);
        }
    }
    fclose(siparisDosyasi);
}
