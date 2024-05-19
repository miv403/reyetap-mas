#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YEMEK_UZUNLUK 50
#define ASCILAR 3 // 3 aşçı varsayımı
#define YEMEK_LISTESI "./veri/yemeklistesi.txt"
#define SIPARIS "./veri/siparisler.txt"

// Yemek struct'ı
typedef struct {
    char ad[YEMEK_UZUNLUK];
    int hazirlanmaSuresi;
} Yemek;

// Aşçı struct'ı
typedef struct {
    int id;
    int hazirlanacakSure;
    char yemek[YEMEK_UZUNLUK]; // Aşçının hazırladığı yemeği tutmak için
} Asci;

// Yemek listesini okuyan fonksiyon
void yemekListesiniOku(Yemek yemekler[]) {
    FILE *dosya = fopen(YEMEK_LISTESI, "r");
    if (dosya == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
        return;
    }

    char satir[256];
    int yemekSayisi = 0;
    fgets(satir, sizeof(satir), dosya); // Başlık satırını atla

    while (fgets(satir, sizeof(satir), dosya) != NULL && yemekSayisi < YEMEK_UZUNLUK) {
        sscanf(satir, "%[^|] | %*[^|] | %d | %*[^|]", yemekler[yemekSayisi].ad, &yemekler[yemekSayisi].hazirlanmaSuresi);
        yemekSayisi++;
    }

    fclose(dosya);
}

// Siparişleri işleyen fonksiyon
void siparisleriIsle(Yemek yemekler[], Asci ascilar[]) {
    while (1) {
        FILE *dosya = fopen(SIPARIS, "r+");
        if (dosya == NULL) {
            printf("Siparisler dosyasi bulunamadi.\n");
            return;
        }

        char satir[256];
        while (fgets(satir, sizeof(satir), dosya) != NULL) {
            // Sadece onaylanmış (durum 1) siparişleri al
            if (satir[0] == '1') {
                strtok(satir, "|"); // siparisID
                strtok(NULL, "|"); // kullanıcı adı
                char *yemekAdi = strtok(NULL, "|");
                strtok(NULL, "|"); // fiyat
                strtok(NULL, "|"); // sipariş zamanı
                int hazirlanmaSuresi = atoi(strtok(NULL, "|"));
                
                // Boşta aşçı bul
                int i;
                for (i = 0; i < ASCILAR; i++) {
                    if (ascilar[i].hazirlanacakSure <= 0) {
                        ascilar[i].hazirlanacakSure = hazirlanmaSuresi;
                        strcpy(ascilar[i].yemek, yemekAdi); // Aşçının yemeğini kaydet
                        printf("Asci %d, %s siparisi hazirliyor. Tamamlanmasi %d dakika.\n", ascilar[i].id, yemekAdi, hazirlanmaSuresi);

                        // Sipariş durumunu güncelle (2: Hazırlanıyor)
                        long konum = ftell(dosya);
                        fseek(dosya, konum - strlen(satir), SEEK_SET);
                        fprintf(dosya, "2");
                        fflush(dosya); // Değişiklikleri hemen uygula
                        break;
                    }
                }

                if (i == ASCILAR) {
                    printf("Tum ascilar mesgul. Siparisler birazdan hazirlanacak...\n");
                    sleep(5); // Kısa bir süre bekle ve tekrar dene
                }
            }
        }

        // Aşçıların sürelerini azalt
        for (int i = 0; i < ASCILAR; i++) {
            if (ascilar[i].hazirlanacakSure > 0) {
                ascilar[i].hazirlanacakSure--;
                if (ascilar[i].hazirlanacakSure == 0) {
                    printf("Asci %d, %s siparisini tamamladi!\n", ascilar[i].id, ascilar[i].yemek);
                }
            }
        }

        fclose(dosya);
        sleep(60); // Her dakika kontrol et
    }
}

int main() {
    Yemek yemekler[YEMEK_UZUNLUK];
    Asci ascilar[ASCILAR];

    yemekListesiniOku(yemekler);

    for (int i = 0; i < ASCILAR; i++) {
        ascilar[i].id = i + 1;
        ascilar[i].hazirlanacakSure = 0;
        strcpy(ascilar[i].yemek, ""); // Başlangıçta boş yemek
    }

    siparisleriIsle(yemekler, ascilar);

    return 0;
}
