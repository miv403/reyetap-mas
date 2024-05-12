#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./csv.c"

#define MAX_YEMEK_ADI_UZUNLUGU 100
#define MAX_SATIR_UZUNLUGU 256
#define YEMEK_LISTESI "./veri/yemeklistesi.txt"
#define SIPARIS "./veri/siparisler.txt"

// Yeni siparisleri gosteren fonksiyon
void yeniSiparisleriGoster() {
    // Siparisler dosyasini acar, bekleyen siparisleri listeler.
    FILE *dosya = fopen(SIPARIS, "r");
    if (dosya == NULL) {
        printf("Siparisler dosyasi bulunamadi.\n");
        return;
    }

    printf("Yeni Siparisler:\n");
    char satir[MAX_SATIR_UZUNLUGU];
    while (fgets(satir, MAX_SATIR_UZUNLUGU, dosya) != NULL) {
        char *siparisID = strtok(satir, "|");
        char *kullaniciAdi = strtok(NULL, "|");
        char *yemekAdi = strtok(NULL, "|");
        char *fiyat = strtok(NULL, "|");
        char *siparisZamani = strtok(NULL, "|");
        char *hazirlanmaSuresi = strtok(NULL, "|");
        char *durum = strtok(NULL, "\n");
        if (strcmp(durum, "0") == 0) {
            printf("ID: %s - Kullanici: %s - Yemek: %s - Fiyat: %s TL - Siparis Zamani: %s - Hazirlanma Suresi: %s dk\n", siparisID, kullaniciAdi, yemekAdi, fiyat, siparisZamani, hazirlanmaSuresi);
        }
    }

}

// Yemek listesini gosteren fonksiyon
void yemekListesiniGoster() {
    // Yemek listesi dosyasını acar, mevcut yemekleri listeler.
    FILE *dosya = fopen("./veri/yemekListesi.csv", "r");
    if (dosya == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.\n");
        return;
    }
        puts(""); 
    printf("%-15s%-10s%-20s%-10s","Yemek Adi", // başlik satiri
                            "Fiyati",
                            "Hazirlanma Suresi",
                            "Durum");
    puts(""); 
    puts("-------------------------------------------------------"); // yeni satir
    csvYazdir(dosya, 150);
    fclose(dosya);
    /*
    printf("Mevcut Yemekler:\n");
    char satir[MAX_SATIR_UZUNLUGU];
    while (fgets(satir, MAX_SATIR_UZUNLUGU, dosya) != NULL) {
        printf("%s", satir);
    }
    */
}

// Gunluk rapor alan fonksiyon
void gunlukRaporAl(char* tarih) {
    // Belirtilen tarihe ait siparisleri rapor dosyasina kaydeder.
    char raporDosyaAdi[50];
    sprintf(raporDosyaAdi, "gunluk_rapor_%s.txt", tarih);

    FILE *siparisDosyasi = fopen(SIPARIS, "r");
    if (siparisDosyasi == NULL) {
        printf("Siparisler dosyasi bulunamadi.\n");
        return;
    }

    FILE *raporDosyasi = fopen(raporDosyaAdi, "w");
    if (raporDosyasi == NULL) {
        printf("Rapor dosyasi olusturulamadi.\n");
        fclose(siparisDosyasi);
        return;
    }

    fprintf(raporDosyasi, "Gunluk Rapor (%s):\n", tarih);
    fprintf(raporDosyasi, "----------------------------------------\n");

    char satir[MAX_SATIR_UZUNLUGU];
    while (fgets(satir, MAX_SATIR_UZUNLUGU, siparisDosyasi) != NULL) {
        char *siparisTarihi = strtok(satir, ",");
        if (strstr(siparisTarihi, tarih) != NULL) {
            fprintf(raporDosyasi, "%s", satir);
        }
    }

    printf("Gunluk rapor '%s' adli dosyaya kaydedildi.\n", raporDosyaAdi);

    fclose(siparisDosyasi);
    fclose(raporDosyasi);
}

// Siparis durumunu guncelleyen fonksiyon
void siparisDurumunuGuncelle(int siparisID, int yeniDurum) {
    // Belirtilen siparisin durumunu gunceller
    FILE *dosya = fopen(SIPARIS, "r+");
    if (dosya == NULL) {
        printf("Siparisler dosyasi bulunamadi.\n");
        return;
    }

    char satir[MAX_SATIR_UZUNLUGU];
    long dosyaBaslangicKonumu;
    int bulunanID;
    while (fgets(satir, MAX_SATIR_UZUNLUGU, dosya) != NULL) {
        dosyaBaslangicKonumu = ftell(dosya);
        char *siparisIDStr = strtok(satir, "|");
        bulunanID = atoi(siparisIDStr);
        if (bulunanID == siparisID) {
            fseek(dosya, dosyaBaslangicKonumu, SEEK_SET);
            fprintf(dosya, "%d", yeniDurum);
            break;
        }
    }

    fclose(dosya);

    if (bulunanID != siparisID) {
        printf("Belirtilen ID numarasina sahip bir siparis bulunamadi.\n");
    } else {
        printf("Siparis durumu basariyla guncellendi.\n");
    }
}

int main() {
    int secim;
    char tarih[11];
    int siparisID, yeniDurum;

    while (1) {
        printf("\nRestoran Yonetim Programi\n");
        printf("1. Mevcut Yemekleri Goster\n");
        printf("2. Yeni Siparisleri Goster\n");
        printf("3. Gunluk Rapor Al\n");
        printf("4. Siparis Durumunu Guncelle\n");
        printf("5. Cikis\n");
        printf("Seciminizi yapiniz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                yemekListesiniGoster();
                break;
            case 2:
                yeniSiparisleriGoster();
                break;
            case 3:
                printf("Gunluk rapor almak istediginiz tarihi (YYYY-MM-DD formatinda) giriniz: ");
                scanf("%s", tarih);
                gunlukRaporAl(tarih);
                break;
            case 4:
                printf("Guncellenecek siparisin ID numarasini giriniz: ");
                scanf("%d", &siparisID);
                printf("Yeni durumu giriniz (0: Beklemede, 1: Hazirlaniyor, 2: Teslim Edildi): ");
                scanf("%d", &yeniDurum);
                siparisDurumunuGuncelle(siparisID, yeniDurum);
                break;
            case 5:
                printf("Programdan cikis yapiliyor.\n");
                exit(0);
            default:
                printf("Gecersiz secim. Lutfen tekrar deneyiniz.\n");
        }
    }

    return 0;
}
