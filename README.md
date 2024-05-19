---
date: 2024-05-05
lastChange: 2024-05-05#mvarr
---

<!--BELGEYİ DÜZENLEYECEK KİŞİ: 
    - girintiler iki boşluk olacak biçimide yazılmıştır.
    - üstteki `lastChange` girdisini uygun biçimide değiştirebilirsin
    - eklediğin yapılacaklar girdisinin sonuna `~~~~isim` yazman güzel olabilir.
    - KOLAY GELSİN -->

# Restoran Yemek Takip Uygulaması - MAS

```text
┬─┐┌─┐┬ ┬┌─┐┌┬┐┌─┐┌─┐       ┌┬┐┌─┐┌─┐
├┬┘├┤ └┬┘├┤  │ ├─┤├─┘  ───  │││├─┤└─┐
┴└─└─┘ ┴ └─┘ ┴ ┴ ┴┴         ┴ ┴┴ ┴└─┘
```

> ödev hakkındaki kısa bilgi için [ODEV.md](https://github.com/miv403/reyetap-mas/blob/master/ODEV.md) belgesi incelenebilir.
> **bu dosya** projenin takibi için oluşturulmuştur.

## katkıda bulunanlar

<a href="https://github.com/mvarr">
  <img src="https://avatars.githubusercontent.com/u/147834815?v=4" alt="Avatar" height=50>
</a>
<a href="https://github.com/miv403">
  <img src="https://avatars.githubusercontent.com/u/62180556?v=4" alt="Avatar" height=50>
</a>
<a href="https://github.com/Dartsemih">
  <img src="https://avatars.githubusercontent.com/u/165713244?v=4" alt="Avatar" height=50>
</a>

## yapılacaklar

- [ ] kaynak kodu içinde [TODO](https://marketplace.visualstudio.com/items?itemName=Gruntfuggly.todo-tree) kullanımı. başka eklenti de olabilir. yorum sayırı içinde TODO FIXME gibi sözcükleri algılayıp ayrı bir yerde gösteriyor
- [ ] [markdownlint](https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint): markdown belgeleri için eklenti. kurala uyulmadığında hata veriyo
- [x] `./other/odev-aciklamasi.pdf` içindeki [sorunlar](#sorunlar) ders yetkilisine sorulacak.
  - [ ] bu bölümde ödev raporu diye bir ibare de var. ~~~~miv403
- [ ] [yemekler veri dosyası](#yemekler-veri-dosyası) düzgün belirlenmedi! ~~~~miv403
- [ ] değişken isimleri tartışılmadı. örneğin işaretçi (*pointer*) isimlendirmeleri `pDegisken` ya da `degiskenPtr` biçiminde olabilir. ~~~~miv403
  - [ ] degisken isimleri icin benim yazmis oldugum prototipin degiskenlerini kullanabiliriz :) ~~~~mvarr
- [ ] `Musteri.exe` icinde 97. satirda henuz degisken kullanici adlari tanimlanmadigi icin sabit bir kullanıcı belirlenmis bir sekilde ayarlandi. Uygulama ilk acildiginda kullanici girisi yapilacak sekilde hem ana menu hem de belirttigim yerdeki kodda degisiklik yapilacak. ~~~~mvarr

  ```C
  /*>>>97*/ char *kullaniciAdi = "Kullanici1";
  ```

## 2024-05-04 tarihli kararlar

### sorunlar

- [x] Ödev dosyasında ödevin tek bir dosya içinde olması istenmiş ancak bizden 3 farklı `.exe` dosyası oluşturulması isteniyor. tek bir c dosyasında 3 defa ayrı ikili dosyaya derleme nasıl yapılır araştırılmalı.

`./other/odev-aciklamasi.pdf` dosyasından alıntı;
> ReYeTaP projesi 3 farklı uygulamadan oluşacaktır.
> Kodu gönderirken tüm kodları tek bir `.c` ya da `.txt` dosyasına yazarak ödev yükleme
> arayüzüne <u>ödev raporu</u> ile birlikte yüklenmelidir.

- [x] Yine ödev dosyasında `müşteri.exe`nin yalnızca okuma yapması istenmiş ancak yeni sipariş kaydetmemiz için yazmamız gerekmekte. bu çelişkiler danışılacak.

`./other/odev-aciklamasi.pdf` dosyasından alıntı;
> Bu uygulama dosya
> üzerinde yalnızca okuma yapabilecektir. Müşteri tarafından yeni sipariş geçildiğinde
> ilgili eylem “siparişler.txt” isimli metin dosyasında saklanacaktır.

#### program geliştirme sırası

1. muşteri
2. restoran
    - Günlük rapor bölümünün en son yazılmasına karar verildi. önce girdilerin düzgün işleneceği bir program gerekli.
3. mutfak

biçiminde ilerleyecek.

### biçimlendirme

**girintiler:** `4` boşluk olacak.

**fonksiyonların konumu:** tüm fonksiyonlar ana fonksiyonun (`main`) altında olacak tüm fonksiyon prototipleri sırasıyla ana fonksiyonun üzerine yazılacak.

```C
int fonkTaslagi(int * pInt);

int main(void) {
  puts("merhaba!");
}
```

**veri dosyası uzantıları:** `.txt.csv`

```C
#define UZANTI ".CSV.TXT"`
// teslim edilmeden önce `.txt` olarak DÜZELTİLECEK () TODO
```

örnek csv için bkz.: [yemekler.txt.csv](#yemekler-veri-dosyası)

**tarih**: [ISO 8601](https://en.wikipedia.org/wiki/ISO_8601) `2024-05-04T12:00:00`

**hazırlanma süresi:** zaman için en küçük birim [dakika](https://en.wikipedia.org/wiki/Minute) olacaktır.

**yemek ücretleri:** ücretler için en küçük birim Türk Lirası (₺) kullanılacak. kuruş alt birimi kullanılmayacak.

#### siparişler veri dosyası

`siparisler.txt.csv`

*sipariş id:* `S-YYYYMMDD-000` (örneğin: `S-20240504-003`)

#### yemekler veri dosyası

`yemekler.txt.csv` için;

```csv
yAdi,yFiyat,yHSure,yDurum
Lahmacun,7500,1200,True
```

bir örnek biçimlendirme olabilir.

**yemeklerin *mevcut durumu:*** `yDurum` için `<stdbool.h>` başlık dosyası ile *boolean* değerler kullanılacak. [^1]

**yemek isimleri:** her yemek için bir *ID* belirlenecek. (örneğin: `Y-003` değeri *Lahmacun*u temsil edebilir.)

<!--MARKDOWN LINT BOS YAPMASIN DİYE VAR-->
<!-- markdownlint-configure-file { 
  "no-inline-html": {
    "allowed_elements": [
  "a",
  "img",
  "u",
  "br",
    ]
  }
} -->