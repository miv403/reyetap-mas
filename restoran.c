#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "./csv.c"
#include <stdbool.h>
#define MAX_SATIR_UZUNLUGU 4096
#define YEMEK_LISTESI "./veri/yemekListesi.csv"
#define SIPARIS "./veri/siparisler.csv"
#define ASCI_DOSYASI "./veri/ascilar.txt"
#define AD_MAX 50
#define YEMEK_MAX 100
#define MAX_SATIR_C 150
#define MAX_SIPARIS 1000
#define MAX_SATIR_UZUNLUGUX 256
#define SIPARISLER_CSV "./veri/siparisler.csv"
// Asci sutunundaki karakter sayisini sinirlayan bir tanimlama
#define ASCI_MAX_UZUNLUK 3
typedef struct {
    char siparisID[20];
    char yemekAdi[50];
    char yemekFiyati[10];
    char siparisZamani[20];
    char hazirlanmaZamani[20];
    char kullaniciAdi[50];
    char asci[ASCI_MAX_UZUNLUK];
    bool silinecek;
} Siparisa;


typedef struct {
    char siparisID[20];
    char yemekAdi[50];
    double yemekFiyati;
    struct tm siparisZamani;
    char hazirlanmaZamani[20];
    char kullaniciAdi[50];
    char asci[10];
} Siparis;
typedef struct {
    char ad[AD_MAX];
    char fiyat[AD_MAX];
    char sure[AD_MAX];
    char durum[AD_MAX];
}Yemek;
void yeniSiparisleriGoster();
void yemekListesiniGoster();
void gunlukRaporOlustur(const char* kaynakDosya);
size_t yemekListesiOku(FILE * dosya, Yemek yemekler[]);
void yemekListesiSecenekler();
void raporArayuzu();
void raporGoruntule(const char* date);
void csvYaz0(FILE *dosya,  int hucreSayisi, ... );
void asciSayisiBelirle();
void stringToTM(const char *stringIN, struct tm *zaman);
void loadSiparisler(Siparis siparisler[], int *siparisSayisi);
double gunlukKazanc(const Siparis siparisler[], int siparisSayisi, const char *tarih);
double aylikKazanc(const Siparis siparisler[], int siparisSayisi, const char *ay);
double donemKazanci(const Siparis siparisler[], int siparisSayisi, const char *baslangic, const char *bitis);
void runAnalyses();
void onaylanmamisSiparisleriGosterVeIslemYap();





int main() {
    int secim;
    char tarih[11];
    int siparisID, yeniDurum;

    while (1) {
        printf("\nRestoran Yonetim Programi\n");
        printf("1. Yemek Listesini Duzenle\n");
        printf("2. Yeni Siparisleri Denetle\n");
        printf("3. Gunluk Rapor Al\n");
        printf("4. Analiz\n");
        printf("5. Asci Sayisi Belirle\n");
        printf("6. Cikis\n");
        printf("Seciminizi yapiniz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                yemekListesiniGoster();
                // TODO: ilgili yemek numarası ile ilgili satir numarasi istemi
                yemekListesiSecenekler();
                break;
            case 2:
                yeniSiparisleriGoster();
                onaylanmamisSiparisleriGosterVeIslemYap();
                break;
            case 3: // Gunluk Rapor
                raporArayuzu();
                break;
            case 4:
                runAnalyses();
                break;
            case 5:
                // AŞÇI SAYISI
                asciSayisiBelirle();
                break;
            case 6:
                printf("Programdan cikis yapiliyor.\n");
                exit(0);
            default:
                printf("Gecersiz secim. Lutfen tekrar deneyiniz.\n");
        }
    }

    return 0;
}

void raporArayuzu() {
    int secim;  // Kullan�c� se�imi i�in de�i�ken
    char date[11];  // Tarihi tutmak i�in dizi

    while (1) {  // S�rekli d�ng�, kullan�c� program� sonland�rana kadar devam eder
        // Kullan�c�ya men� se�enekleri sunar
        printf("1. Yeni rapor olustur\n");
        printf("2. Gunluk rapor goruntulemek\n");
        printf("3. Cikis\n");
        printf("Secimini gir: ");
        scanf("%d", &secim);  // Kullan�c� se�imini al�r

        switch (secim) {  // Kullan�c� se�imine g�re i�lem yapar
        case 1:
            gunlukRaporOlustur(SIPARIS);  // G�nl�k raporu olu�turur
            break;
        case 2:
            // Kullan�c�dan tarih al�r
            printf("Tarihi gir (format: YYYY-MM-DD): ");
            scanf("%s", date);  // Tarihi al�r
            raporGoruntule(date);  // �lgili tarihe ait raporu g�r�nt�ler
            break;
        case 3:
            exit(0);  // Program� sonland�r�r
        default:
            printf("Gecersiz secim.\n");  // Ge�ersiz se�im mesaj� verir
        }
    }
    return;  // Program�n ba�ar�l� bir �ekilde bitti�ini belirtir

}

// Yeni siparisleri gosteren fonksiyon
void yeniSiparisleriGoster() {
    // Siparisler dosyasini acar, bekleyen siparisleri listeler.
    FILE *siparisDosyasi;
    siparisDosyasi = fopen("./veri/siparisler.csv", "r");
    if (siparisDosyasi == NULL) {
        printf("Siparisler dosyasi bulunamadi.\n");
        return;
    }

    printf("Yeni Siparisler:\n");
    puts("");

    printf("%-15s%-15s%-15s%-20s%-20s%-15s%-10s",
                "Siparis ID",
                "Yemek Adi",
                "Yemek Fiyati",
                "Siparis Zamani",
                "Hazirlanma Zamani",
                "Kullanici Adi",
                "Asci");
    puts("---------------------------------------------------------------------------------------------------------");
    siparislerYazdir(siparisDosyasi, 150);
}

void yemekListesiSecenekler() {
    void yemekEkle();
    void yemekSil();
    void yemekGuncelle();

    puts("1. Yemek Ekle");
    puts("2. Yemek Sil");
    puts("3. Yemeği Düzenle");
    int kullaniciSecim;
    do{
        printf("? ");
        scanf("%d", &kullaniciSecim);
    }while(kullaniciSecim < 1 || kullaniciSecim > 3);


    switch (kullaniciSecim) {
    case 1:
        yemekEkle();
        break;
    case 2:
        yemekSil();
        break;
    case 3:
        yemekGuncelle();
        break;
    default:
        puts("Hatali secim.");
        yemekListesiSecenekler(); //FIXME recursive iyi bir seçim olmayabilir
        break;
    }
}

void yemekEkle() {

    Yemek yemek;

    FILE * yemekListesi;
    yemekListesi = fopen("./veri/yemekListesi.csv", "a");

    if(yemekListesi == NULL) {
        printf("Yemek Listesi dosyasi acilamadi.\n");
        return;
    }

    printf("Yemek adi girin: ");
    scanf("%50s", yemek.ad); // FIXME: boşluk karakter işi bozuyor. gets() derleme uyarısı veriyor.
    printf("Fiyat girin (TL) girin: ");
    scanf("%50s", yemek.fiyat);
    printf("Hazirlanma suresi girin: ");
    scanf("%50s", yemek.sure);
    printf("Yemegin durumunu girin[Mevcut/Mevcut degil]: ");
    scanf("%50s", yemek.durum);

    if(!(strcmp(yemek.durum, "Mevcut"))) strcpy(yemek.durum, "True");
    else                              strcpy(yemek.durum, "False");
    csvYaz(yemekListesi, 4,
            yemek.ad,
            yemek.fiyat,
            yemek.sure,
            yemek.durum);
    fclose(yemekListesi);
}

// TODO: yemek silme

void yemekSil() {

    Yemek yemekler[YEMEK_MAX];

    FILE * yemekListesi;
    yemekListesi = fopen(YEMEK_LISTESI, "r");

    size_t yemekSayisi = yemekListesiOku(yemekListesi,yemekler);

    fclose(yemekListesi);

    printf("Silmek istediginiz yemek adi: ");
    char kullaniciYemekIN[AD_MAX];

    scanf("%49s", kullaniciYemekIN);

    printf("%s a\n", kullaniciYemekIN);

    int j = 0;

    for(; j < yemekSayisi; ++j) {
        if(strcmp(kullaniciYemekIN, yemekler[j].ad) == 0){
            break;
        }
    }
    printf("%d\n", j);

    FILE *yemekListeYENI;

    yemekListeYENI = fopen("./veri/yemekListesi0.csv", "w");

    fprintf(yemekListeYENI, "yemekAdi,yemekFiyati,hazirlanmaSuresi,durum\n");
    // yemek listesini tekrar yazdir
    for(size_t i = 0; i < yemekSayisi; ++i) {

        if(i == j) continue;

        csvYaz0(yemekListeYENI, 4,
                yemekler[i].ad,
                yemekler[i].fiyat,
                yemekler[i].sure,
                yemekler[i].durum);
    }
    fprintf(yemekListeYENI, "\n");
    fclose(yemekListeYENI);

    remove(YEMEK_LISTESI);
    rename("./veri/yemekListesi0.csv",YEMEK_LISTESI);
}

void yemekGuncelle() {

    Yemek yemekler[YEMEK_MAX];

    FILE * yemekListesi;
    yemekListesi = fopen(YEMEK_LISTESI, "r");

    size_t yemekSayisi = yemekListesiOku(yemekListesi,yemekler);

    fclose(yemekListesi);

    printf("Guncellemek istediginiz yemek adi: ");
    char kullaniciYemekIN[AD_MAX];

    scanf("%49s", kullaniciYemekIN);

    printf("%s a\n", kullaniciYemekIN);

    puts("1. Yemek adi guncelle");
    puts("2. Fiyat guncelle");
    puts("3. Hazirlanma suresi guncelle");
    puts("4. Durum guncelle");

    int kullaniciSecim;
    do{
        printf("? ");
        scanf("%d", &kullaniciSecim);
    } while(kullaniciSecim < 1 || kullaniciSecim > 4);

    char kullaniciGuncellemeIN[AD_MAX];


    switch (kullaniciSecim)
    {
    case 1:
        printf("Yeni ad girin: ");
        scanf("%49s", kullaniciGuncellemeIN);
        break;
    case 2:
        printf("Yeni fiyat girin: ");
        scanf("%49s", kullaniciGuncellemeIN);
        break;
    case 3:
        printf("Yeni sure girin: ");
        scanf("%49s", kullaniciGuncellemeIN);
        break;
    case 4:
        printf("Yeni durum girin[Mevcut/Mevcut Degil]: ");
        scanf("%49s", kullaniciGuncellemeIN);
        break;
    default:
        break;
    }

    for(size_t i = 0; i < yemekSayisi; ++i) {

        if(!(strcmp(kullaniciYemekIN, yemekler[i].ad))) {
            switch (kullaniciSecim) {
                case 1:
                    strcpy(yemekler[i].ad, kullaniciGuncellemeIN);
                    break;
                case 2:
                    strcpy(yemekler[i].fiyat, kullaniciGuncellemeIN);
                    break;
                case 3:
                    strcpy(yemekler[i].sure, kullaniciGuncellemeIN);
                    break;
                case 4:
                    if(strcmp(kullaniciGuncellemeIN, "MevcutDegil") == 0) {

                        strcpy(yemekler[i].durum, "False");

                    } else if (strcmp(kullaniciGuncellemeIN, "Mevcut") == 0){

                        strcpy(yemekler[i].durum, "True");
                    }
                    break;
                default:
                    printf("varsayilan durum yemek guncelle");
                    break;
            }
        }
    }

    FILE *yemekListeYENI;

    yemekListeYENI = fopen("./veri/yemekListesi0.csv", "w");

    fprintf(yemekListeYENI, "yemekAdi,yemekFiyati,hazirlanmaSuresi,durum\n");
    // yemek listesini tekrar yazdir
    for(size_t i = 0; i < yemekSayisi; ++i) {

        csvYaz0(yemekListeYENI, 4,
                yemekler[i].ad,
                yemekler[i].fiyat,
                yemekler[i].sure,
                yemekler[i].durum);
    }
    fprintf(yemekListeYENI, "\n");
    fclose(yemekListeYENI);

    rename("./veri/yemekListesi0.csv",YEMEK_LISTESI);
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
    yemekListesiYazdir(dosya, 150);
    fclose(dosya);

}

void gunlukRaporOlustur(const char* sourceFileName) {

    time_t now = time(NULL);  // Mevcut zaman bilgisini al�r (time_t zaman t�r�)
    struct tm* t = localtime(&now);  // Zaman bilgisini yerel zamana �evirir (struct tm yap� t�r�)

    char newFileName[64];  // Yeni dosya ad� i�in bir dizi tan�mlar
    strftime(newFileName, sizeof(newFileName) - 1, "report_%Y-%m-%d.csv", t);  // Dosya ad�n� tarih ile olu�turur

    FILE* sourceFile = fopen(sourceFileName, "r");  // Kaynak dosyay� okuma modunda a�ar ("r" modu, sadece okuma)
    FILE* destFile = fopen(newFileName, "w");  // Hedef dosyay� yazma modunda a�ar ("w" modu, dosya yoksa olu�turur)

    if (sourceFile == NULL || destFile == NULL) {
        perror("Dosya acilamadi");  // Dosyalar a��lamazsa hata mesaj� verir
        if (sourceFile != NULL) fclose(sourceFile);  // Kaynak dosya a��ksa kapat�r
        if (destFile != NULL) fclose(destFile);  // Hedef dosya a��ksa kapat�r
        return;
    }

    char buffer[256];  // Okuma ve yazma i�in bir buffer tan�mlar
    while (fgets(buffer, sizeof(buffer), sourceFile) != NULL) {  // Dosyadan sat�r sat�r okur
        fprintf(destFile, "%s", buffer);  // Veriyi hedef dosyaya yazar
    }

    fclose(sourceFile);  // Kaynak dosyay� kapat�r
    fclose(destFile);  // Hedef dosyay� kapat�r
    printf("Gunluk rapor olusturuldu: %s\n", newFileName);  // Ba�ar� mesaj� verir
}

// Belirli bir tarihe ait raporu g�r�nt�leyen fonksiyon
void raporGoruntule(const char* date) {
    char fileName[64];  // Dosya ad� i�in bir dizi tan�mlar
    snprintf(fileName, sizeof(fileName), "report_%s.csv", date);  // Dosya ad�n� tarih ile olu�turur

    FILE* file = fopen(fileName, "r");  // Dosyay� okuma modunda a�ar
    if (file == NULL) {
        perror("Dosya acilamadi");  // Dosya a��lamazsa hata mesaj� verir
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
    printf("-----------------------------------------------------------\n");

    char satir[MAX_SATIR_UZUNLUGU];
    fgets(satir, sizeof(satir), file);
    while (fgets(satir, sizeof(satir), file) != NULL) {  // Dosyan�n sonuna kadar okur
        char* siparisID = strtok(satir, ",");
        char* yemekAdi = strtok(NULL, ",");
        char* fiyat = strtok(NULL, ",");
        char* siparisZamani = strtok(NULL, ",");
        char *hazirlanmaZamani = strtok(NULL, ",");
        char* kullaniciAdi = strtok(NULL, ",");
        char* asci = strtok(NULL, "\n");

        if (!siparisID || !yemekAdi || !fiyat || !siparisZamani || !hazirlanmaZamani || !kullaniciAdi || !asci) {
            fprintf(stderr, "%s\n", satir);
            continue;  // Hatal� sat�rlar� atlar
        }

        printf("%-15s%-15s%-10s%-20s%-20s%-15s%-10s\n",
                siparisID,
                yemekAdi,
                fiyat,
                siparisZamani,
                hazirlanmaZamani,
                kullaniciAdi,
                asci);
    }
    fclose(file);  // Dosyay� kapat�r
}
size_t yemekListesiOku(FILE * dosya, Yemek yemekler[]) {

    // yemek listesi dosyasından birinci(ad) ve üçüncü(hazirlanmaSuresi) sütunu okur.

    // yemek listesi dosyasından tüm verileri okur ve yemekler[] kaydeder
    char satir[256];
    int yemekSayisi = 0;

    fgets(satir, 256, dosya);

    for(int i = 0; fgets(satir, 256, dosya) != NULL && i < YEMEK_MAX; ++i) {

        // , karakterine göre satırdan gerekli sütunları okuma

        sscanf(satir, "%[^,] , %[^,] , %[^,] , %[^,]", yemekler[i].ad, yemekler[i].fiyat, yemekler[i].sure, yemekler[i].durum);

        yemekSayisi++;
    }
    return yemekSayisi;
}
void csvYaz0(FILE *dosya,  int hucreSayisi, ... ) {

    va_list args;
    va_start(args, hucreSayisi);

    for (int i = 0; i < hucreSayisi - 1; ++i) {
        char *str = va_arg(args, char *);
        fprintf(dosya, "%s,", str);
    }
    char *str = va_arg(args, char *);
    fprintf(dosya, "%s", str);
    //fprintf(dosya, "\n");

    va_end(args);
}

void asciSayisiBelirle() {

    puts("");
    FILE * asciDosyasi;
    asciDosyasi = fopen(ASCI_DOSYASI, "r");
    char asciSayisi[10];
    fgets(asciSayisi, 9, asciDosyasi);
    printf("Su an %s asci var.", asciSayisi);
    fclose(asciDosyasi);
    asciDosyasi = fopen(ASCI_DOSYASI, "w");
    printf("Asci sayisi belirleyin: ");
    int asciWrite = 0;
    scanf("%d", &asciWrite);

    fprintf(asciDosyasi, "%d", asciWrite);
    puts("Ascilar atandı.");
    fclose(asciDosyasi);
}
// stringToTM fonksiyonunu mutfak.c den al
void stringToTM(const char *stringIN, struct tm *zaman) {
    char stringINCPY[17];
    strcpy(stringINCPY, stringIN);
    char *bolut;

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

        zaman->tm_sec = 0;
    }
}

// csv dosyasini okuyarak siparisleri struct array ine yukleyen fonksiyon
void loadSiparisler(Siparis siparisler[], int *siparisSayisi) {
    FILE *dosya = fopen("./veri/siparisler.csv", "r");
    if (dosya == NULL) {
        perror("Dosya acilirken hata olustu");
        return;
    }
    char satir[MAX_SATIR_C];
    *siparisSayisi = 0;

    // csv basligini atla
    fgets(satir, MAX_SATIR_C, dosya);

    while (fgets(satir, MAX_SATIR_C, dosya) != NULL && *siparisSayisi < MAX_SIPARIS) {
        Siparis *siparis = &siparisler[*siparisSayisi];
        char *token = strtok(satir, ",");
        // SiparisID
        if (token != NULL) strcpy(siparis->siparisID, token);
        // YemekAdi
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(siparis->yemekAdi, token);
        // YemekFiyati
        token = strtok(NULL, ",");
        if (token != NULL) siparis->yemekFiyati = atof(token);
        // SiparisZamani
        token = strtok(NULL, ",");
        if (token != NULL) stringToTM(token, &siparis->siparisZamani);
        // HazirlanmaZamani
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(siparis->hazirlanmaZamani, token);
        // KullaniciAdi
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(siparis->kullaniciAdi, token);
        // Asci
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(siparis->asci, token);
        // Asci kontrolu
        if (strcmp(siparis->asci, "A0") == 0 || strcmp(siparis->asci, "XX") == 0) {
            continue;  // Gecersiz ascilar
        }

        (*siparisSayisi)++;
    }

    fclose(dosya);
    printf("Analiz acildi. Toplam %d siparis yuklendi.\n", *siparisSayisi);
}


// gunluk kazanci hesaplar
double gunlukKazanc(const Siparis siparisler[], int siparisSayisi, const char *tarih) {
    struct tm targetDate = {0};
    stringToTM(tarih, &targetDate);

    double toplamKazanc = 0.0;
    for (int i = 0; i < siparisSayisi; i++) {
        if (siparisler[i].siparisZamani.tm_year == targetDate.tm_year &&
            siparisler[i].siparisZamani.tm_mon == targetDate.tm_mon &&
            siparisler[i].siparisZamani.tm_mday == targetDate.tm_mday) {
            toplamKazanc += siparisler[i].yemekFiyati;
        }
    }

    return toplamKazanc;
}

// aylik kazanci hesaplar
double aylikKazanc(const Siparis siparisler[], int siparisSayisi, const char *ay) {
    struct tm targetMonth = {0};
    stringToTM(ay, &targetMonth);

    double toplamKazanc = 0.0;
    for (int i = 0; i < siparisSayisi; i++) {
        if (siparisler[i].siparisZamani.tm_year == targetMonth.tm_year &&
            siparisler[i].siparisZamani.tm_mon == targetMonth.tm_mon) {
            toplamKazanc += siparisler[i].yemekFiyati;
        }
    }

    return toplamKazanc;
}

// belirtilen donemdeki kazanci hesaplar
double donemKazanci(const Siparis siparisler[], int siparisSayisi, const char *baslangic, const char *bitis) {
    struct tm baslangicTarih = {0}, bitisTarih = {0};
    stringToTM(baslangic, &baslangicTarih);
    stringToTM(bitis, &bitisTarih);

    double toplamKazanc = 0.0;
    for (int i = 0; i < siparisSayisi; i++) {
        struct tm siparisTarih = siparisler[i].siparisZamani;
        if (mktime(&siparisTarih) >= mktime(&baslangicTarih) &&
            mktime(&siparisTarih) <= mktime(&bitisTarih)) {
            toplamKazanc += siparisler[i].yemekFiyati;
        }
    }

    return toplamKazanc;
}

// analiz fonksiyonu
void runAnalyses() {
    Siparis siparisler[MAX_SIPARIS];
    int siparisSayisi;
    int choice;
    char tarih[11], ay[8], baslangic[11], bitis[11];

    loadSiparisler(siparisler, &siparisSayisi);

    printf("Hangi analizi yapmak istiyorsunuz?\n");
    printf("1. Gunluk Kazanc\n");
    printf("2. Aylik Kazanc\n");
    printf("3. Donem Kazanci\n");
    printf("Seciminiz: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Tarihi girin (YYYY-MM-DD): ");
            scanf("%s", tarih);
            printf("Gunluk Kazanc (%s): %.2f\n", tarih, gunlukKazanc(siparisler, siparisSayisi, tarih));
            break;
        case 2:
            printf("Ayi girin (YYYY-MM): ");
            scanf("%s", ay);
            printf("Aylik Kazanc (%s): %.2f\n", ay, aylikKazanc(siparisler, siparisSayisi, ay));
            break;
        case 3:
            printf("Baslangic tarihini girin (YYYY-MM-DD): ");
            scanf("%s", baslangic);
            printf("Bitis tarihini girin (YYYY-MM-DD): ");
            scanf("%s", bitis);
            printf("Donem Kazanci (%s - %s): %.2f\n", baslangic, bitis, donemKazanci(siparisler, siparisSayisi, baslangic, bitis));
            break;
        default:
            printf("Gecersiz secim.\n");
            break;
    }
}
// Onaylanmamis siparisleri gosteren ve islem yapan fonksiyon
void onaylanmamisSiparisleriGosterVeIslemYap() {
    FILE *siparisDosyasi = fopen(SIPARISLER_CSV, "r");
    if (siparisDosyasi == NULL) {
        printf("Siparisler dosyasi bulunamadi.\n");
        return;
    }

    Siparisa siparisler[100];
    int siparisSayisi = 0;

    printf("Onaylanmamis Siparisler:\n\n");
    printf("%-15s%-15s%-15s%-20s%-20s%-15s%-10s\n",
           "Siparis ID", "Yemek Adi", "Yemek Fiyati",
           "Siparis Zamani", "Hazirlanma Zamani", "Kullanici Adi", "Asci");
    puts("---------------------------------------------------------------------------------------------------------");

    char satir[MAX_SATIR_UZUNLUGUX];
    // Baslik satirini oku ve yok say
    fgets(satir, MAX_SATIR_UZUNLUGUX, siparisDosyasi);

    // Gecici olarak bellekte tum siparisleri sakla
    while (fgets(satir, MAX_SATIR_UZUNLUGUX, siparisDosyasi) != NULL) {
        char *siparisID = strtok(satir, ",");
        char *yemekAdi = strtok(NULL, ",");
        char *yemekFiyati = strtok(NULL, ",");
        char *siparisZamani = strtok(NULL, ",");
        char *hazirlanmaZamani = strtok(NULL, ",");
        char *kullaniciAdi = strtok(NULL, ",");
        char *asciAlani = strtok(NULL, ",");

        if (asciAlani == NULL) {
            asciAlani = "";
        }

        char asci[ASCI_MAX_UZUNLUK];
        strncpy(asci, asciAlani, ASCI_MAX_UZUNLUK - 1);
        asci[ASCI_MAX_UZUNLUK - 1] = '\0';
        size_t len = strlen(asci);
        if (asci[len - 1] == '\n') {
            asci[len - 1] = '\0';
        }

        Siparisa siparis;
        strncpy(siparis.siparisID, siparisID, sizeof(siparis.siparisID) - 1);
        strncpy(siparis.yemekAdi, yemekAdi, sizeof(siparis.yemekAdi) - 1);
        strncpy(siparis.yemekFiyati, yemekFiyati, sizeof(siparis.yemekFiyati) - 1);
        strncpy(siparis.siparisZamani, siparisZamani, sizeof(siparis.siparisZamani) - 1);
        strncpy(siparis.hazirlanmaZamani, hazirlanmaZamani, sizeof(siparis.hazirlanmaZamani) - 1);
        strncpy(siparis.kullaniciAdi, kullaniciAdi, sizeof(siparis.kullaniciAdi) - 1);
        strncpy(siparis.asci, asci, sizeof(siparis.asci) - 1);
        siparis.silinecek = false;

        if (asci[1] == 'X') {
            printf("%-15s%-15s%-15s%-20s%-20s%-15s%-10s\n",
                   siparisID, yemekAdi, yemekFiyati, siparisZamani,
                   hazirlanmaZamani, kullaniciAdi, asci);
            char cevap;
            printf("\nSiparis ID %s icin onaylamak icin 'o', reddetmek icin 'r' giriniz: ", siparis.siparisID);
            scanf(" %c", &cevap);

            if (cevap == 'o') {
                strncpy(siparis.asci, "A0", sizeof(siparis.asci) - 1);
            } else if (cevap == 'r') {
                siparis.silinecek = true;  // Silinmesi icin isaretle
            }
        }

        siparisler[siparisSayisi++] = siparis;
    }
    fclose(siparisDosyasi);

    // Dosyayi yazma modu ile ac
    siparisDosyasi = fopen(SIPARISLER_CSV, "w");
    if (siparisDosyasi == NULL) {
        printf("Siparisler dosyasi yazma modunda acilamadi.\n");
        return;
    }

    // Baslik satirini yeniden yaz
    fputs("Siparis ID,Yemek Adi,Yemek Fiyati,Siparis Zamani,Hazirlanma Zamani,Kullanici Adi,Asci\n", siparisDosyasi);

    // Guncellenmis siparisleri dosyaya yaz
    for (int i = 0; i < siparisSayisi; i++) {
        if (!siparisler[i].silinecek) {
            fprintf(siparisDosyasi, "%s,%s,%s,%s,%s,%s,%s\n",
                    siparisler[i].siparisID, siparisler[i].yemekAdi, siparisler[i].yemekFiyati,
                    siparisler[i].siparisZamani, siparisler[i].hazirlanmaZamani,
                    siparisler[i].kullaniciAdi, siparisler[i].asci);
        }
    }

    fclose(siparisDosyasi);
    printf("Islemler tamamlandi.\n");
}
