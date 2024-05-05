#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_YEMEK_ADI_UZUNLUGU 100
#define MAX_SATIR_UZUNLUGU 256
#define YEMEK_LISTESI_DOSYASI "../data/yemeklistesi.txt"
#define SIPARIS_DOSYASI "../data/siparisler.txt"

// Fonksiyon prototipleri
void yeniSiparis();
void mevcutSiparisDurumu();
void oncekiSiparisler();

int main() {
    // Kullanıcı arayüzü ve menü
    printf("1. Yeni Siparis\n");
    printf("2. Mevcut Siparis Durumu\n");
    printf("3. Onceki Siparislerim\n");

    int secim;
    printf("Seciminizi yapiniz: ");
    scanf("%d", &secim);

    switch(secim) {
        case 1:
            // Yeni sipariş fonksiyonu: Kullanıcıya menüyü gösterir, sipariş alır ve kaydeder.
            yeniSiparis();
            break;
        case 2:
            // Mevcut sipariş durumu fonksiyonu: Aktif siparişleri gösterir.
            mevcutSiparisDurumu();
            break;
        case 3:
            // Önceki siparişler fonksiyonu: Tamamlanmış siparişleri gösterir.
            oncekiSiparisler();
            break;
        default:
            printf("Gecersiz secim!\n");
    }

    return 0;
}

// Yeni Sipariş Fonksiyonu
void yeniSiparis() {
    // [İşlev]: Kullanıcıya menüyü gösterir, sipariş alır ve kaydeder.
    // Yemek listesini yukle
    FILE *dosya = fopen("../data/yemeklistesi.txt", "r");
    if (dosya == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
        return;
    }

        // Menuyu kullaniciya goster
    printf("Menu:\n");
    char satir[MAX_SATIR_UZUNLUGU];
    while (fgets(satir, MAX_SATIR_UZUNLUGU, dosya) != NULL) {
        char *yemekAdi = strtok(satir, "|"); // strtok fonksiyonu ile satırı parçalara ayırır
        char *fiyat = strtok(NULL, "|"); // "|" karakterine göre ayırır.
        char *sure = strtok(NULL, "|"); 
        char *durum = strtok(NULL, "\n");
        if (strcmp(durum, "mevcut") == 0) {
            printf("%s - Fiyat: %s TL - Hazirlanma Suresi: %s dk\n", yemekAdi, fiyat, sure); 
        }
    }
    fclose(dosya);

        // Kullanicidan siparis al
    printf("Siparis vermek istediginiz yemegin adini giriniz: ");
    char secilenYemek[MAX_YEMEK_ADI_UZUNLUGU]; 
    scanf("%s", secilenYemek); 

        // Secilen yemegin bilgilerini dosyadan bul
    dosya = fopen("../data/yemeklistesi.txt", "r");
    if (dosya == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
        return;
    }

        // Secilen yemek bilgilerini tut
    char secilenYemekBilgisi[MAX_SATIR_UZUNLUGU]; 
    while (fgets(satir, MAX_SATIR_UZUNLUGU, dosya) != NULL) { 
        if (strstr(satir, secilenYemek) != NULL) {
            strcpy(secilenYemekBilgisi, satir);
            break;
        }
    }
    fclose(dosya);

    // Siparis bilgilerini dosyaya yaz
    FILE *siparisDosyasi = fopen("../data/siparisler.txt", "a");
    if (siparisDosyasi == NULL) {
        printf("Siparisler dosyasi acilamadi.\n");
        return;
    }
    // Siparis bilgilerini olustur
    char *kullaniciAdi = "Kullanici1"; // kullanici adi burada simdilik sabit olarak tanimlandi ama ileride kullanici adi alinacak.
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    char zamanDamgasi[15]; // zaman damgasi icin 15 karakterlik bir dizi olusturuldu. (S-YYYYMMDD-XYZ)
    static int sira = 1;
    char siraStr[4];
    sprintf(siraStr, "%03d", sira);
    strftime(zamanDamgasi, sizeof(zamanDamgasi), "S-%Y%m%d-%siraStr", now);
    sira++;

    // Hazırlanma süresini dosyadan al
    int hazirlanmaSuresi;
    sscanf(secilenYemekBilgisi, "%*[^|]|%*[^|]|%d", &hazirlanmaSuresi); // bu kodu buradan alip daha uygun bir kisima tasimamiz daha iyi olabilir.

    // Siparis bilgilerini dosyaya yaz
    fprintf(siparisDosyasi, "%s,%s,%s,%d,%d\n", kullaniciAdi, secilenYemekBilgisi, zamanDamgasi, hazirlanmaSuresi, 0); 
    fclose(siparisDosyasi);

    printf("Siparisiniz alinmistir. Hazirlanma suresi: %d dakika.\n", hazirlanmaSuresi);
    
}

// Mevcut Sipariş Durumu Fonksiyonu
void mevcutSiparisDurumu() {
    // [İşlev]: Aktif siparişleri gösterir.

    // Fonksiyon içeriği
}

// Önceki Siparişler Fonksiyonu
void oncekiSiparisler() {
    // [İşlev]: Tamamlanmış siparişleri gösterir.

    // Fonksiyon içeriği
}
