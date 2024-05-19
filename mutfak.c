#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Unix sistemlerinde sleep fonksiyonu icin

#define YEMEK_UZUNLUK 50
#define ASCILAR 3 // 3 asciya sahip bir restorani varsayalım
#define YEMEK_LISTESI "./veri/yemeklistesi.txt"
#define SIPARIS "./veri/siparisler.txt"


// Yemek struct'i tanimla 
typedef struct {
    char ad[YEMEK_UZUNLUK];
    int hazirlanmaSuresi;
} Yemek;

// Asci struct'i tanimla
typedef struct {
    int id;
    int hazirlanacakSure;
} Asci;

// Yemek listesini dosyadan okuyan ve depolayan fonksiyon
void siparisleriOkuyucu(Yemek yemekler[], int *yemekSayisi) {
    FILE *dosya = fopen(YEMEK_LISTESI, "r");
    if (dosya == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
        return;
    }

    char satir[256];
    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        char *ad = strtok(satir, "/");
        char *suresi = strtok(NULL, "/");
        suresi = strtok(suresi, "\n");

        strcpy(yemekler[*yemekSayisi].ad, ad);
        yemekler[*yemekSayisi].hazirlanmaSuresi = atoi(suresi);

        (*yemekSayisi)++;
    }

    fclose(dosya);
}

// Siparisleri isleyen ana fonksiyon
void siparisleriIsleyici() {
    while (1) {
        FILE *dosya = fopen(SIPARIS, "r+");
        if (dosya == NULL) {
            printf("Siparisler dosyasi bulunamadi.\n");
            return;
        }

        char satir[256];
        while (fgets(satir, sizeof(satir), dosya) != NULL) {
            char *durum = strtok(satir, ",");
            if (strcmp(durum, "1") == 0) {
                // Siparis hazirlaniyor
                char *siparisID = strtok(NULL, "|");
                char *kullaniciAdi = strtok(NULL, "|");
                char *yemekAdi = strtok(NULL, "|");
                char *fiyat = strtok(NULL, ",|");
                char *siparisZamani = strtok(NULL, "|");
                char *hazirlanmaSuresi = strtok(NULL, "|");
                printf("Siparis hazirlaniyor: ID: %s - Kullanici: %s - Yemek: %s - Hazirlanma Suresi: %s dakika\n", siparisID, kullaniciAdi, yemekAdi, hazirlanmaSuresi);

                // Hazirda bir asci bul
                int i;
                for (i = 0; i < ASCILAR; i++) {
                    if (ascilar[i].hazirlanacakSure == 0) {
                        // Asciya siparisi ata
                        printf("Asci %d, %s siparisi hazirliyor. Tamamlanmasi icin %d dakika bekleyecek.\n", ascilar[i].id, yemekAdi, yemekler[i].hazirlanmaSuresi);
                        ascilar[i].hazirlanacakSure = yemekler[i].hazirlanmaSuresi;
                        fseek(dosya, -1 * strlen(satir), SEEK_CUR);
                        fprintf(dosya, "2"); // Siparis durumu "hazirlandi" olarak degistir
                        break;
                    }
                }

                if (i == ASCILAR) {
                    printf("Tum ascilar mesgul. Biraz bekleniyor...\n");
                    sleep(60);
                }
            }
        }

        fclose(dosya);
    }
}

int main() {
    Yemek yemekler[YEMEK_UZUNLUK];
    int yemekSayisi = 0;

    siparisleriOkuyucu(yemekler, &yemekSayisi);

    Asci ascilar[ASCILAR];
    for (int i = 0; i < ASCILAR; i++) {
        ascilar[i].id = i + 1;
        ascilar[i].hazirlanacakSure = 0;
    }

    siparisleriIsleyici();

    return 0;
}
