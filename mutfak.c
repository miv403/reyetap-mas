#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "./csv.c"

#define SIPARIS_MAX 10
#define ASCILAR_MAX 3
    // FIXME: ascilar restoran.c ile alınacak
#define AD_MAX 50
#define YEMEK_MAX 50
#define YEMEK_LISTESI "./veri/yemekListesi.csv" 
#define SIPARIS_LISTESI "./veri/siparisler.csv"
#define SATIR_MAX 256

char output[17]; // FIXME: hata ayıklama için göndermeden önce sil

typedef struct {
    int asciID;
    int siparisNo;              // siparisler.csv siparis konumu.
    struct tm * uygunZamani;    // &siparisler[i].hazirlanmaZamani
//    Siparis * atananSiparis;    // Siparis pointer

}Asci;

typedef struct {
    char siparisID[AD_MAX];
    char ad[AD_MAX];
    char yemekFiyati[5];
    struct tm siparisZamani;    // 2024-05-23T10.00
    char kullaniciAdi[AD_MAX];
    struct tm hazirlanmaZamani; // 2024-05-23T10.30
    int hazirlanmaSuresi;       // 30 dakika
    int siparisNo;              // 
    char onay[5];
    Asci * atananAsci;

} Siparis;


typedef struct {
    char ad[AD_MAX];
    int hazirlanmaSuresi;
} Yemek;

struct tm sifir = { .tm_year = 10,
                    .tm_mon = 0,
                    .tm_mday = 1,
                    .tm_hour = 0,
                    .tm_min = 0,
                    .tm_sec = 0 };


size_t yemekListesiOku(FILE * dosya, Yemek yemekler[]);
void stringToTM (const char * stringIN, struct tm * zaman);
bool zamanKarsilastir(const struct tm *zaman1, const struct tm *zaman2);
size_t siparislerOku(FILE * dosya, Siparis siparisler[], size_t siparisMax);
void zamanEkle(const struct tm * zaman1, int eklenecekDakika, struct tm * zamanOut);
size_t enUygunAsci(Asci ascilar[], size_t ascilarMax);

int main(void) {

    Siparis siparisler[SIPARIS_MAX];
    Asci ascilar[3];
    Yemek yemekler[YEMEK_MAX];

    
    /****** YEMEK LISTESINI OKU ******/
    
    FILE *yemekListesi;
    
    yemekListesi = fopen(YEMEK_LISTESI, "r");
    if (yemekListesi == NULL) {
        printf("Yemek listesi dosyasi bulunamadi.");
        puts("");
        return -1;
    }

    size_t yemekSayisi = yemekListesiOku(yemekListesi, yemekler);

    fclose(yemekListesi);

    for(int i = 0; i < YEMEK_MAX; ++i){

        printf("ad: %s\nhazirlanmaSuresi: %d\n", yemekler[i].ad, yemekler[i].hazirlanmaSuresi);
        puts("");

    }

    /*********************************/

    /******** SIPARISLERI OKU *******/

    // TODO: siparisleriOku() işlevine asci hücresini okuma eklenmeli
    // bu sayede siparisin onaylanma durumu ve ayrıca atanmış aşçı önceden belirlenebilir.

    FILE *siparislerDosyasi;
    siparislerDosyasi = fopen(SIPARIS_LISTESI, "r");
    if (siparislerDosyasi == NULL) {
        puts("Siparisler dosyasi bulunamadi!");
        return -1;
    }

    size_t siparisSayisi = siparislerOku(siparislerDosyasi, siparisler, SIPARIS_MAX);

    printf("siparisSayisi: %zu\n", siparisSayisi);

    fclose(siparislerDosyasi);

    /*  hata ayiklama  */

    for(size_t i = 0; i < siparisSayisi; ++i) {
        printf("%s\n", siparisler[i].ad);
        strftime(output,17, "%Y-%m-%dT%H.%M", &siparisler[i].siparisZamani);
        puts(output);
    }
    /*********************************/

    /* YEMEK LİSTESİ <--> SİPARİŞLER  */

    // yemek listesindeki adlar ile siparişlerdeki adlar eşleştirilip
    // hazirlanma süreleri siparişlere atanacak.

    for(size_t i = 0; i < siparisSayisi; ++i) {

        for(size_t j = 0; j < yemekSayisi; ++j) {

            if(!(strcmp(siparisler[i].ad, yemekler[j].ad))){
                // yemek adi ile siparişteki yemek adi eşleştirilip
                // hazirlanmaSuresi verisi sipariş nesnesine kopyalanıyor
                siparisler[i].hazirlanmaSuresi =  yemekler[j].hazirlanmaSuresi;
            }
        }
    }

    /*********** ASCI ATAMA **********/
    
    // başlangıçta tüm aşçılar sırayla ilk ASCILAR_MAX tane siparişe atanması   
    // için tüm uygunZamani işaretçileri sıfır nesnesine işaret ettiriliyor.
    // ve aşçıID leri atanıyor

    for(size_t i = 0; i < ASCILAR_MAX; ++i) {
        ascilar[i].uygunZamani = &sifir;
        ascilar[i].asciID = i + 1;
    }

    // TODO sadece A0 siparişleri alınacak

    /*
    uygunluk zamanı en uygun olan aşçılar numaralarına göre sırayla
    atanır.
    
    aşçıların numaralarına göre atanması konumları değiştirilmeden 
    karşılaştırma algoritması ile yapılmalı. uygunluk zamanı en son uygun 
    olduğu zamana hazirlanmaSuresi eklenerek güncellenmeli ve bu zaman aynı 
    zamanda siparişteki ilgili konuma da yazılmalı.

>   aşçının müsait olma zamanı sipariş zamanı ile karşılaştırılmalı:

    musait olma zamani siparişin verilme zamanından önceyse müsait olma zamani:
    >   sipariş verilme zamanina hazirlanma süresi eklenerek güncellenmeli.

    eğer müsait olma zamani siparişin verilme zamanindan sonra veya aynıysa 
    müsait olma zamani;
    >   müsait olma zamanina hazirlanma süresi eklenerek güncellenmeli.

    */

    // EN UYGUN AŞÇIYI BULMA
    for(size_t i = 0; i < siparisSayisi; ++i) {
        int asciID = enUygunAsci(ascilar, ASCILAR_MAX);

        //siparisler[i].atananAsci atanmış mı denetle
        // en uygun aşçı alınır ve ID numarası alınır

        // ZAMAN KARŞILAŞTIRMA
    // zamanKarsilastir(ascilar[i].uygunZamani, &siparisler[j].siparisZamani);
        if(zamanKarsilastir(ascilar[asciID].uygunZamani,
                            &siparisler[i].siparisZamani)) {
            // zaman1 >= zaman2 TRUE
            // ZAMAN EKLEME (HAZIRLANMA SURESI)
            // zamanEkle()

            //siparis[i] hazirlanma zamani = asci[j] uygun zamani + hazirlanma suresi


            // ascinin uygun olma zamanına hazırlanmaSuresi ekleniyor
            // ve siparişin hazirlanmaZamani üyesine atanıyor
            zamanEkle(ascilar[asciID].uygunZamani, // &siparisler[X].hazirlanmaZamani
                        siparisler[i].hazirlanmaSuresi,
                        &siparisler[i].hazirlanmaZamani);

            ascilar[asciID].uygunZamani = &siparisler[i].hazirlanmaZamani;
            siparisler[i].atananAsci = &ascilar[asciID];

        }else {
            // zaman1 < zaman2 FALSE

            // sipariş[i] hazirlanma zamani = sipariş[i] zamani + hazirlanma suresi
            // aşçı uygun zamani = & sipariş[i] hazirlanma zamanio
            // asci[j] atananSiparis = &siparis[i]
            // siparis[i] atananAsci = &asci[j]

            zamanEkle(&siparisler[i].siparisZamani,
                        siparisler[i].hazirlanmaSuresi,
                        &siparisler[i].hazirlanmaZamani);
            ascilar[asciID].uygunZamani = &siparisler[i].hazirlanmaZamani;
            siparisler[i].atananAsci = &ascilar[asciID];

        }
        
        puts("asci algo:");

            strftime(output, 17, "%Y-%m-%dT%H.%M", &siparisler[i].siparisZamani);
            printf("siparisler[%zu].siparisZamani: %s", i, output);
            puts("");

            strftime(output, 17, "%Y-%m-%dT%H.%M", &siparisler[i].hazirlanmaZamani);
            printf("siparisler[%zu].hazirlanmaZamani: %s", i, output);
            puts("");

            printf("siparisler[%zu].hazirlanmaSuresi: %d", i,
                    siparisler[i].hazirlanmaSuresi);
            puts("");
            
            printf("siparisler[%zu].atananAsci->asciID: %d", i,
                    siparisler[i].atananAsci->asciID);
            puts("");

            printf("ascilar[%d].asciID:  %d", asciID, ascilar[asciID].asciID);
            puts("");
            
            strftime(output, 17, "%Y-%m-%dT%H.%M", ascilar[asciID].uygunZamani);
            printf("ascilar[%d]->uygunZamani: %s", asciID , output);
            puts("");
        

    }
        // DOSYAYA İŞLEME

        // siparis[i]'ye atanan asci ile ilgili bilgiler çekilebilir
        // T0DO: siparis struct'ı genişlet ve siparisler.csv'deki tüm verileri 
        // kaydet. onaylanmayan siparişler haricindeki tüm siparişleri "w"
        // ile dosyaya tekrar yazdır.

        FILE * siparislerYeni;
        siparislerYeni = fopen("./veri/siparisler001.csv", "w");

        if(siparislerYeni == NULL) {
            puts("siparislerYeni dosyasi olusuturlamadi.");
            return -1;
        }

        fputs("siparisID, yemekAdi, yemekFiyati, siparisZamani, hazirlanmaZamani, kullaniciAdi, asci\n", siparislerYeni);

        for(size_t i = 0; i < siparisSayisi; ++i) {

            
            // TODO: test et

            char siparisZamani[17];
            char hazirlanmaZamani[17];
            char asci0[5];
            sprintf(asci0, "A%d", siparisler[i].atananAsci->asciID);
            strftime(siparisZamani,17, "%Y-%m-%dT%H.%M",
                        &siparisler[i].siparisZamani);
            
            strftime(hazirlanmaZamani,17, "%Y-%m-%dT%H.%M",
                        &siparisler[i].hazirlanmaZamani);

            csvYaz(siparislerYeni, 7,
                    siparisler[i].siparisID,
                    siparisler[i].ad,
                    siparisler[i].yemekFiyati,
                    siparisZamani,
                    hazirlanmaZamani,
                    siparisler[i].kullaniciAdi,
                    asci0);
        }


        fclose(siparislerYeni);
        rename("./veri/siparisler001.csv", "./veri/siparisler.csv");
    /*********************************/


    // ascilar[0].uygunZamani = &siparisler[0].siparisZamani;  
    
    // asci yapısındaki ilgili pointer'ı sınayan ifadeler
    char zamanGirdisi000[] = "2024-05-23T10.61";
    stringToTM(zamanGirdisi000,&siparisler[0].siparisZamani);

    ascilar[0].uygunZamani = &siparisler[0].siparisZamani;

    strftime(output,17, "%Y-%m-%dT%H.%M", &siparisler[0].siparisZamani);
    puts(output);
    strftime(output, 17, "%Y-%m-%dT%H.%M", ascilar[0].uygunZamani);
    puts(output);
}

void zamanEkle(const struct tm * zaman1,    // aşçının uygun olma zamani
                int eklenecekDakika,        // eklenecek dakika
                struct tm * zamanOut) {     // atanacak sipariş zamani pointer

    // TODO: hata ifadelerini düzenle!

    if (zaman1 == NULL) {
        puts("Hata: Geçersiz tm işaretçisi.");
        return;
    }

    time_t unixSaniye = mktime((struct tm*) zaman1);

    if (unixSaniye == -1) {
        puts("Hata: mktime() dönüşümü başarısız oldu.");
        return;
    }

    // verilen dakika süresini saniye olarak ekleme
    unixSaniye += eklenecekDakika * 60;

    // Convert back to struct tm
    if (localtime_r(&unixSaniye, zamanOut) == NULL) {
        puts("Hata: localtime_r() dönüşümü başarısız oldu.");
        return;
    }
}

size_t siparislerOku(FILE * dosya, Siparis siparisler[], size_t siparisMax){

    // siparişler dosyasının ikinci dorduncu sütununu okur ve Siparis nesnesine işler. Sipariş sayisini geri döndürür.

    // siparişler ile ilgili tüm verileri oku ve hepsini Siparis nesnesine ekle.

    char satir[SATIR_MAX];

    size_t i = 0;
    fgets(satir, 256, dosya);
    while(fgets(satir, 256, dosya) != NULL && i < siparisMax) {
        
        char siparisZamani[17]; // siparisZamani tm ögesine dönüştürülmeden
                                // önce char[] olarak alınıyor

        //TODO: daha önce bir asci alınmış mı denetlenebilir.

        sscanf(satir,
        "%[^,] , %[^,] , %[^,] , %[^,] , %*[^,] , %[^,] , %*[^,]", 
            siparisler[i].siparisID,
            siparisler[i].ad,
            siparisler[i].yemekFiyati,
            siparisZamani,
            // burada hazirlanmaZamani bilerek atlanmıştır.
            // (bu program zaten onu atayacak!!!)
            siparisler[i].kullaniciAdi); // asci sütunu şimdilik boş bırakıldı (bkz ust satir todo)
            
        stringToTM(siparisZamani, &siparisler[i].siparisZamani);
        
        i++;
    }
    size_t siparisSayisi = i; // okunan sipariş sayısı 

    return siparisSayisi; 
}

size_t enUygunAsci(Asci ascilar[], size_t ascilarMax) { 
    
    // en erken uygun olacak aşçının konumunu geri döndürür.

    size_t enKucuk = 0, i;

    for(i = 1; i < ascilarMax; ++i) {

        if(!(zamanKarsilastir(ascilar[i].uygunZamani, ascilar[enKucuk].uygunZamani))) {
            enKucuk = i;
        }
    }
    return enKucuk;
}

bool zamanKarsilastir(const struct tm *zaman1, const struct tm *zaman2) {
    // zaman1 >= zaman2 TRUE
    // zaman1 < zaman2 FALSE
    
    time_t hamZaman1 = mktime((struct tm*)zaman1); 
    
    // mktime() zaman1'i değiştirdiğinden ötürü
    // cast işleci kullanılıyor.

    time_t hamZaman2 = mktime((struct tm*)zaman2);

    if (hamZaman1 >= hamZaman2) return true;  // eşit ya da sonra
    if (hamZaman1 < hamZaman2) return false; // zaman1 daha erken
}

size_t yemekListesiOku(FILE * dosya, Yemek yemekler[]) {

    // yemek listesi dosyasından birinci(ad) ve üçüncü(hazirlanmaSuresi) sütunu okur.
    char satir[256];
    int yemekSayisi = 0;

    fgets(satir, 256, dosya);

    for(int i = 0; fgets(satir, 256, dosya) != NULL && i < YEMEK_MAX; ++i) {

        // , karakterine göre satırdan gerekli sütunları okuma

        sscanf(satir, "%[^,] , %*[^,] , %d , %*[^,]", yemekler[i].ad, &yemekler[i].hazirlanmaSuresi);

        yemekSayisi++;
    }

    return yemekSayisi;
}

void stringToTM (const char * stringIN, struct tm  * zaman) {

    // bir string değişkeni tm nesnesine dönüştüren işlev.

    char stringINCPY[17]; // verilen string'in kopyası
    strcpy(stringINCPY, stringIN);
    char *bolut; // strtok() ile alınan bölütler
    
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

        zaman->tm_sec = 0; // bu tek başına girilmezse garbage value oluyor SİLME!!!
    }
}
