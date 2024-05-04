#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Unix sistemlerinde sleep fonksiyonu için

#define MAX_YEMEK_UZUNLUK 50
#define MAX_ASCILAR 3 // 3 aşçıya sahip bir restoranı varsayalım

// Yemek struct'ı tanımla
typedef struct {
    char ad[MAX_YEMEK_UZUNLUK];
    int hazirlanmaSuresi;
} Yemek;

// Aşçı struct'ı tanımla
typedef struct {
    int id;
    int hazirlanacakSure;
} Asci;

// Yemek listesini dosyadan okuyan ve depolayan fonksiyon
void yemekListesiniOku(Yemek yemekler[], int *yemekSayisi) {
    // Dosyayı aç
    // Satırları oku
    // Yemekleri struct'a kaydet
}

// Siparişleri işleyen ana fonksiyon
void siparisleriIsle() {
    // Sipariş dosyasını aç
    // Bekleyen siparişleri işle
    // Aşçıları kontrol et ve siparişleri atama
}

int main() {
    // Ana işlevi başlat
    // Döngü içinde siparişleri işle
    return 0;
}
