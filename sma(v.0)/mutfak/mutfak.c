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

int main() {
    FILE *dosya = fopen("../data/yemeklistesi.txt", "r");
    if (dosya == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
        return 1;
    }

    // Yemek listesini oku ve yemekleri depola
    Yemek yemekler[MAX_YEMEK_UZUNLUK];
    int yemekSayisi = 0;

    char satir[256];
    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        char *ad = strtok(satir, "/");
        char *suresi = strtok(NULL, "/");
        suresi = strtok(suresi, "\n"); // \n karakterini sil

        strcpy(yemekler[yemekSayisi].ad, ad);
        yemekler[yemekSayisi].hazirlanmaSuresi = atoi(suresi);

        yemekSayisi++;
    }

    fclose(dosya);

    // Aşçıları ve kuyruklarını oluştur
    Asci ascalar[MAX_ASCILAR];
    for (int i = 0; i < MAX_ASCILAR; i++) {
        ascalar[i].id = i + 1;
        ascalar[i].hazirlanacakSure = 0;
    }

    // Siparişleri işle
    while (1) {
        dosya = fopen("../data/siparisler.txt", "r+");
        if (dosya == NULL) {
            printf("Siparisler dosyasi bulunamadi.\n");
            return 1;
        }

        while (fgets(satir, sizeof(satir), dosya) != NULL) {
            char *durum = strtok(satir, ",");
            if (strcmp(durum, "1") == 0) {
                // Siparis hazirlaniyor
                char *siparisID = strtok(NULL, ",");
                char *kullaniciAdi = strtok(NULL, ",");
                char *yemekAdi = strtok(NULL, ",");
                char *fiyat = strtok(NULL, ",");
                char *siparisZamani = strtok(NULL, ",");
                char *hazirlanmaSuresi = strtok(NULL, ",");
                printf("Siparis hazirlaniyor: ID: %s - Kullanici: %s - Yemek: %s - Hazirlanma Suresi: %s dakika\n", siparisID, kullaniciAdi, yemekAdi, hazirlanmaSuresi);

                // Boş bir aşçıyı bul
                int i;
                for (i = 0; i < MAX_ASCILAR; i++) {
                    if (ascalar[i].hazirlanacakSure == 0) {
                        // Aşçıyı siparişe atama
                        printf("Aşçı %d, %s siparişi hazırlıyor. Tamamlanması için %d dakika bekleyecek.\n", ascalar[i].id, yemekAdi, yemekler[i].hazirlanmaSuresi);
                        ascalar[i].hazirlanacakSure = yemekler[i].hazirlanmaSuresi;
                        fseek(dosya, -1 * strlen(satir), SEEK_CUR);
                        fprintf(dosya, "2"); // Sipariş durumunu "Hazirlandi" olarak güncelle
                        break;
                    }
                }

                if (i == MAX_ASCILAR) {
                    printf("Tüm aşçılar meşgul. Biraz bekleniyor...\n");
                    sleep(60); // 1 dakika beklet ve tekrar dene
                }
            }
        }

        fclose(dosya);
    }

    return 0;
}
