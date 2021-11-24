

% Yardım Dizini

# Tanıtım

Drumstick Metronom, ALSA sıralayıcıyı kullanan MIDI tabanlı bir metronomdur.

Hedef kitle, müzisyenler ve müzik öğrencileridir. Herhangi bir fiziksel gibi
metronom müzik çalarken ritmi korumak için bir araçtır
enstrümanlar.

Dijital ses yerine MIDI kullanır, düşük CPU kullanımına izin verir ve çok hassastır 
ALSA sıralayıcı sayesinde zamanlama. Bu, bir MIDI'ye de ihtiyacınız olduğu anlamına gelir.
ses üretimi için sentezleyici, çünkü program herhangi bir ses üretmez
kendi kendine. En iyi sonuçlar bir donanım sentezleyici kullanmaktan gelir, ancak
bunun yerine yazılım sentezleyicileri de kullanın.

Destek isteyin, sorunları, hataları ve özellik isteklerini adresindeki izleme sistemine bildirin. 
[SourceForge proje sitesi](https://sourceforge.net/p/kmetronome/support-requests/)

# Drumstick Metronomunu Kullanma

## Özellikleri

Tempo kaydırıcı gibi bazı kontrolleri ayarlamanız yeterlidir.
vuruş/bar döndürme kutusu ve vuruş uzunluğu seçicisi. Oynat düğmesine basın
başlamak. Rahatınız için durdurma düğmesini kullanın.

Tempo, kaydırıcı kullanılarak 25 ila 250 QPM arasında ayarlanabilir. birimler
dakikada çeyrek (Mälzel Metronom birimleri). Ayrıca çift tıklayabilirsiniz
yeni bir giriş yapabileceğiniz bir iletişim kutusu açmak için ana pencerenin üzerine
doğrudan klavye ile tempo. Ayrıca seçebileceğiniz bir birleşik giriş kutusu var.
ve İtalyan müzik adlarını kullanarak tempoyu görüntüleyin.

Vuruş/Çubuk 1 ila 32 vuruş arasında ayarlanabilir. Bunlar vuruş sayısı
her ölçü veya çubukta ve zaman imzasındaki paydır.
not edilecekti.

Vuruş uzunluğu, zaman işareti belirtimindeki paydadır,
ve her vuruşun süresini temsil eder. Bu değeri değiştirmek,
tempo birimlerinin anlamını değiştirin.

Kalıp, bir kalıp tanımı seçmek için açılan bir listedir. Varsayılan
"Otomatik" değer, programın aşağıdakileri kullanarak kalıplar oluşturduğu anlamına gelir.
yapılandırma iletişim kutusunda (Güçlü/Zayıf) ayarlanan notlar ve ritim
"Vuruş/Çubuk" ve "Vuruş uzunluğu" tarafından sağlanan tanım. Ayrıca içerir
kullanıcı tanımlı kalıpların adları.

## Başlarken

Bu program MIDI protokolünü kullanır, bu nedenle
kavramları tam olarak anlamak için MIDI hakkında bazı temel kavramlar
arkasında. Burada iyi bir giriş bulabilirsiniz:
[MIDI nedir](https://www.midi.org/midi-articles/categories/MIDI%201.0).

Drumstick Metronom MIDI olayları üretir. Olayları duymak istiyorsanız
MIDI OUT bağlantı noktasını bundan bağlamanız gereken seslere çevrilmiş
MIDI sentezleyicinin MIDI IN bağlantı noktasına programlayın. ya bir olabilir
donanım MIDI sentezleyici veya bir yazılım. Eğer bu bir dış
donanım sentezleyici, ayrıca ALSA destekli bir MIDI arayüzüne ihtiyacınız var
bilgisayarınıza takılı ve her iki bilgisayara da bağlı bir MIDI kablosu
bilgisayarın MIDI arayüzü ve sentezleyici MIDI IN soketi.

Harici bir MIDI sentezleyiciniz yoksa, yine de
ALSA sıralayıcıyı destekleyen bir MIDI yazılım sentezleyicisine sahip program
[QSynth](https://qsynth.sourceforge.io) gibi bağlantılar. Başlangıç ​​senin
Drumstick Metronom'dan önce yazılım sentezleyici ve ardından bağlantıyı yapın
iki program arasında, ya Drumstick Metronome'un yapılandırma iletişim kutusunda 
veya gibi bir harici bağlantı yöneticisi kullanarak 
[QJackCtl](https://qjackctl.sourceforge.io).

İyi bir [Sound Font](https://en.wikipedia.org/wiki/SoundFont) yüklemeyi unutmayın 
QSynth'in "Kurulum..." iletişim kutusuna girin.

## Yapılandırma

Drumstick Metronom'un sınırlı oturum yönetimi yetenekleri vardır. Hatırlayabilir
ALSA çıkış portu için bir bağlantı ve onun için bir bağlantı
giriş portu. Program çıktığında ve hatırlandığında bağlantılar saklanır
başlangıçta. Böyle yapmayı tercih ederseniz, bu özelliğe ihtiyacınız yoktur.
aconnect veya başka bir eşdeğer yardımcı program kullanarak elle bağlantılar veya
içinde bulunan patchbay gibi harici bir oturum yöneticisi kullanıyorsanız
programı [QJackCtl](https://qjackctl.sourceforge.io).

Drumstick Metronom, .INS formatında bir enstrüman tanım dosyası kullanır, aynı
Qtractor, TSE3, Cakewalk ve Sonar olarak formatlayın. **Çıkış aracı**
açılır liste, standart General MIDI'den birini seçmenize izin verir,
Roland GS ve Yamaha XG davul haritaları. Oluşturarak daha fazla tanım ekleyebilirsiniz.
'$HOME/.local/share/kmetronome.sourceforge.net' konumunda 'drums.ins' adlı bir dosya. NS
**Banka**, **Program**, **Zayıf** ve **Güçlü not** açılır listelerinin içeriği 
da bu tanıma bağlıdır.

**Kanal** genellikle 10'dur, yani Genel MIDI'nin perküsyon kanalı
sentezleyici. 1 ile 16 arasında bir sayı olmalıdır.

**Çözünürlük**, her bir çeyrek nota için tik sayısıdır (zaman birimleri).
Değer aralığı 48 ile 960 arasındadır. Varsayılan değer 120'dir.

**Not süresi**, zaman aralığının uzunluğudur (tik sayısı olarak).
Bir NOTE ON ve buna karşılık gelen NOTE OFF olayı arasında. Bu kontrol
yalnızca **NOT KAPALI olayları gönder** de etkinleştirildiğinde etkinleştirilir. Çok düşük değerler
bazı sentezleyicilerde sessiz tıklamalara neden olabilir.

Perküsyon seslerinin genellikle daha sonra gönderilmek üzere NOTE OFF olaylarına ihtiyacı yoktur.
her NOT AÇIK. **NOT KAPALI olayları gönder** onay kutusunu seçin, yalnızca
sentezleyici veya enstrüman bu ayarı destekler veya gerektirir.

**Bank** ve **Program** enstrümanlar için davul setini değiştirmek için kullanılır
birkaç ayarı destekler. Birçok sentezleyici programı anlamıyor
perküsyon kanalı için değişiklikler.

**Otomatik** kalıp modunda, ilk vuruş olarak **Güçlü nota** sesi çalınır 
her ölçüde, aynı ölçüdeki diğer herhangi bir vuruş kullanılarak çalınır. 
**Zayıf nota** sesi. 33 ve 34 sayısal değerleri GM2 ve XG sesleridir
sırasıyla metronom tıklaması ve metronom zili için.

## Kalıp Düzenleyici

Bu iletişim kutusunu kullanarak kalıpları düzenleyebilir, test edebilir ve seçebilirsiniz. Yaratmak
yeni desenler, mevcut tanımı yeni bir adla kaydetmeniz yeterlidir.
Desenler bir tablo ile temsil edilir. Tablodaki satırlar karşılık gelir
perküsyon sesleri. Bir ses listesinden satırları kaldırabilir ve ekleyebilirsiniz.
konfigürasyon iletişim kutusundaki cihaz ayarları tarafından tanımlanır. NS
tablodaki sütun sayısı desenin uzunluğunu belirler,
herhangi bir vuruş uzunluğunda 1 ile 99 arasında eleman.

Her tablo hücresi, N=1 ile 9 arasındaki değerleri kabul eder;
Notaların MIDI hızı (N*127/9) veya sesi iptal etmek için 0. Geçerli
değerler de değişkene karşılık gelen f (=forte) ve p (=piyano) şeklindedir.
ana penceredeki döner düğmeler (Güçlü/Zayıf) tarafından tanımlanan hızlar.
Hücre değerleri, klavye kullanılarak seçilebilir ve değiştirilebilir.
veya fare. değiştirmeden önce oynatmayı durdurmaya gerek yoktur.
hücreler.

# Komut Referansı

## Ana pencere

### Dosya Menüsü

**Dosya → Kalıpları İçe Aktar**

: Kalıp tanımlarını Drumstick Metronom'a aktarır

**Dosya → Kalıpları Dışa Aktar**

: Drumstick Metronom'dan kalıp tanımlarını dışa aktarır

**Dosya → Oynat/Durdur**

: Desen oynatmayı kontrol eder

**Dosya → Çık**

: Drumstick Metronom'dan Çıkılır

### Düzenleme Menüsü

**Düzenle → Kalıpları Düzenle**

: Kalıp düzenleyiciyi açar

### Ayarlar Menüsü

**Ayarlar → Eylem Düğmelerini Göster**

: Eylem düğmelerini gösterir veya gizler. Her düğme için eşdeğer menü öğeleri vardır.

**Ayarlar → Araç Çubuğunu Göster**

: Araç çubuğunu gösterir veya gizler. Her düğme için eşdeğer menü öğeleri vardır.

**Ayarlar → Yapılandırma**

: Drumstick Metronom'u yapılandırır

### Yardım Menüsü

**Yardım → Yardım İçeriği**

: Yardım penceresini açar

**Yardım → Dil**

: Mevcut çevirileri gösteren bir alt menüyü açar

**Yardım → hakkında**

: Bu programın yaklaşık kutusunu açar

**Yardım → Qt hakkında**

: Qt çerçevelerinin yaklaşık kutusunu açar

# Harici kontrol

Drumstick Metronom, D-Bus arayüzü, System Realtime kullanılarak kontrol edilebilir
ve Sisteme Özel MIDI mesajları.

## D-Bus Arayüzü 

Drumstick Metronom, D-Bus arayüzü aracılığıyla bazı işlevler sağlar. Yapabilirsiniz
Drumstick Metronom'u "qdbusviewer" gibi kontrol etmek için D-Bus etkinleştirilmiş bir program veya 
komut satırı yardımcı programı "qdbus".

Örneğin, bu komutlar bir kabuk isteminden kullanılabilir:

    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.play
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.stop
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.cont
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTempo 150
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTimeSignature 3 8

## Evrensel Sistem Özel mesajları

Drumstick Metronom, bazı Universal System Exclusive mesajlarını anlar. Çünkü
cihaz kimliği henüz uygulanmadı, tanınan tüm mesajların
yayın olarak işaretlenir (0x7F).

Gerçek Zamanlı Mesaj: Zaman İmza Değişikliği Mesajı

    Biçim: 0xF0 0x7F 0x7F 0x03 Komut Uzunluğu Pay Payda ... 0xF7
                                0x02: Zaman İmzası Değişikliği 
                                0x42: Zaman İmzası Değişikliği Gecikmeli

Notlar: her iki komut da yalnızca sonraki zamanlanmış desenleri etkiler.
komutu alıyor. Uzunluk en az 2'dir. Aşağıdaki ekstra baytlar
Pay ve Payda dikkate alınmaz.

MMC Mesajları

    Biçim: 0xF0 0x7F 0x7F 0x06 Komut 0xF7
                                0x01: Dur
                                0x02: Oynat
                                0x03: Ertelenmiş Oynatma

## MIDI Sistemi Gerçek Zamanlı Arayüzü

için MIDI Sistem Gerçek Zamanlı mesajları üreten bir MIDI cihazı kullanabilirsiniz.
Drumstick Metronom'u kontrol edin. Kabul edilen Sistem Gerçek Zamanlı mesajları şunlardır: Başlat (0xFA),
Durdur (0xFC) ve Devam Et (0xFB).

Birçok ana MIDI klavyesi ve MIDI düğmesi aygıtı bu üçünü sağlar.
MIDI taşıma kontrolleri. Harici MIDI cihazlarınızı bağlamanız gerekiyor
MIDI (veya USB) kablolarını kullanarak bilgisayara bağlayın ve
Drumstick Metronom'un giriş portuna karşılık gelen ALSA portu. Bu bağlantı
elle veya gibi bir oturum yönetim yazılımı kullanılarak yapılmalıdır.
QJackCtl.

# Kredi ve Lisans

Program Telif Hakkı © 2005-2021 Pedro Lopez-Cabanillas

Belgeleme Telif Hakkı © 2005-2021 Pedro Lopez-Cabanillas

Bu program ücretsiz bir yazılımdır; yeniden dağıtabilir ve/veya değiştirebilirsiniz
tarafından yayınlanan GNU Genel Kamu Lisansı koşulları altında
Özgür Yazılım Vakfı; Lisansın 2. sürümü veya
(isteğe bağlı olarak) herhangi bir sonraki sürüm.

Bu program faydalı olacağı ümidiyle dağıtılmıştır,
ancak HİÇBİR GARANTİ OLMADAN; zımni garanti bile olmadan
SATILABİLİRLİK veya BELİRLİ BİR AMACA UYGUNLUK. Bkz.
Daha fazla ayrıntı için GNU Genel Kamu Lisansı.
                                                                        
GNU Genel Kamu Lisansının bir kopyasını almış olmalısınız
Bu programla birlikte. Değilse, bkz. [https://www.gnu.org/licenses](https://www.gnu.org/licenses)

# Kurulum
 
## Drumstick Metronom nasıl edinilir

Burada en son sürümü bulabilirsiniz: 
[Projenin dosyaları](https://sourceforge.net/projects/kmetronome/files/kmetronome/)

## Gereksinimler

Drumstick Metronom'u başarılı bir şekilde derlemek ve kullanmak için Qt 5 veya
daha sonra, Drumstick 2, ALSA sürücüleri ve ALSA kitaplığı.

Ayrıca ses üretmek için bir MIDI sentezleyiciye ihtiyacınız var.

Derleme sistemi [CMake](https://cmake.org) 3.14 veya daha yenisini gerektirir.

ALSA kitaplığı, sürücüler ve yardımcı programlar şu adreste bulunabilir: 
[ALSA ana sayfası](https://www.alsa-project.org).

Drumstick::ALSA, Qt kullanan ALSA sıralayıcı arabirimi etrafındaki bir C++ sarmalayıcıdır
nesneler, deyimler ve stil. adresinde bulabilirsiniz.
[Drumstick ana sayfası](https://drumstick.sourceforge.io).

Ayrıca [Drumstick Metronom ana sayfasına](https://kmetronome.sourceforge.io/kmetronome.shtml) bakın.

## Derleme ve Kurulum

Drumstick Metronom'u sisteminize derlemek ve kurmak için,
Drumstick Metronom dağıtımının temel dizininde aşağıdakiler bulunur:

    % cmake .
    % Yapmak
    % sudo kurulum yap

Drumstick Metronom 'cmake' ve 'make' kullandığından, sorun yaşamamanız gerekir
onu derlemek. Herhangi bir sorunla karşılaşırsanız, lütfen bunları
yazar veya projenin 
[hata izleme sistemi](https://sourceforge.net/p/kmetronome/bugs/).

