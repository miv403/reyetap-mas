# özet

ödev üç farklı izlenceden oluşacak.

1. müşteri.exe
2. restoran.exe
3. mutfak.exe

## müşteri.exe

**yazılımdan beklenenler:** 

1. müşteri sipariş verebilecek
2. siparişin durumunu takip edebilecek
3. önceden vermiş olduğu siparişleri görebilecek

**yazılımda yer alacak menüler:**

1. yeni sipariş
2. mevcut sipariş durumu
3. daha önceki siparişlerim

## restoran.exe

restoran yönetiminin süreçleri takip ettiği uygulama

**yazılımdan beklenenler:**

1. gelen siparişlerin denetimi ve onaylanması
2. günlük rapor ve analiz
     - günlük kazanç
     - en çok tüketilen yemek

**yazılımda yer alacak menüler:**

1. yemek ekleme - güncelleme - silme
2. yeni sipariş onayı ve ret
3. günlük rapor çıkarma görüntüleme
     - analiz:
       - günlük kazanç
       - aylık kazanç
       - dönem kazancı
       - en çok tüketim
4. aşçı sayısı belirleme

## mutfak.exe

gelen siparişlerin yapım süresinin yönetimi.
onaylanan siparişlerin için süre belirlenmesi.
`restoran.exe` tarafından belirlenen aşçı sayısında
göre siparişlerin hazırlanma süresinin hesaplanması.
bir aşçı aynı anda bir tek yemek yapabilmekte bundan
ötürü hazırlanma süresine göre aşçılar siparişi hazırlama
kuyruğuna alınmalıdır.
