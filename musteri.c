#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Dosya yolları
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

    // Fonksiyon içeriği
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
