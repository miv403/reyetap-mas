---
date: 2024-05-04
---
# Yapılacaklar

## siparisler.txt.csv

- [ISO8601](https://en.wikipedia.org/wiki/ISO_8601) tarihlendirmesi kullanılacak
- Sipariş id *S*-`YYYYMMDD`-000
  - Örnek: S-2024-05-04-001
- `#define UZANTI ".CSV.TXT"`
- Hocaya teslim edilmeden önce uzantısı düzeltilecek (.txt olarak)

## yemekler.txt.csv

- Mevcut durumu için boolean değeri atanacak [Mevcut (True) - Mevcut Değil (False)]
- Yemek isimleri için ayrı id belirlenecek
  - Örnek: Y-`001` (Lahmacun)
- Zaman için en küçük birim dakika olacak
- Para için birim TL(₺) olacak

## uygulama

- Ödev dosyasında ödevin tek bir dosya içinde olması istenmiş ancak bizden 3 farklı .exe dosyası oluşturulması isteniyor. tek bir c dosyasında 3 defa ayrı compile nasıl edilir öğrenilecek.
    >ReYeTaP projesi 3 farklı uygulamadan oluşacaktır.
    > Kodu gönderirken tüm kodları tek bir c ya da txt dosyasına yazarak ödev yükleme
arayüzüne ödev raporu ile birlikte yüklenmelidir

- Yine ödev dosyasında müşteri.exe'nin yalnızca okuma yapması istenmiş ancak yeni sipariş kaydetmemiz için yazmamız gerekmekte. bu çelişkiler hocaya mail atarak danışılacak.
    >Bu uygulama dosya
üzerinde yalnızca okuma yapabilecektir. Müşteri tarafından yeni sipariş geçildiğinde
ilgili eylem “siparişler.txt” isimli metin dosyasında saklanacaktır.

- Girintiler `4` boşluk olacak.

- Diğer tüm fonksiyonlar `main`in altında olacak

- Uygulamaları yazma sırası:
    1. Müşteri.exe
    2. Restoran.exe

        - Günlük rapor kısmını en son yazacağız, çünkü önce girdilerin düzgün işleneceği bir sistem gerekli.
    3. Mutfak.exe
