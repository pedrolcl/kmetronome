% Index nápovědy

# Úvod

Drumstick Metronome je metronom založený na MIDI využívající sekvencer ALSA.

Zamýšleným publikem jsou hudebníci a studenti hudby. Jako každá fyzická
metronom je nástroj k udržení rytmu při hraní muzikálu
nástroje.

Místo digitálního zvuku používá MIDI, což umožňuje nízké využití procesoru a velmi přesné 
časování díky sekvenceru ALSA. To znamená, že potřebujete také MIDI
syntezátor pro generování zvuku, protože program neprodukuje žádný zvuk
sama o sobě. Nejlepších výsledků dosáhnete při použití hardwarového syntezátoru, ale můžete
místo toho také použijte softwarové syntezátory.

Požádejte o podporu, nahlaste problémy, chyby a požadavky na funkce sledovacímu systému na adrese 
[Stránka projektu SourceForge](https://sourceforge.net/p/kmetronome/support-requests/)

# Použití Drumstick Metronome

## Funkce

Potřebujete pouze upravit některé ovládací prvky, jako je posuvník tempa
otáčecí pole beatů/taktů a volič délky taktu. Stiskněte tlačítko přehrávání
začít. Podle potřeby použijte tlačítko stop.

Tempo lze nastavit od 25 do 250 QPM pomocí posuvníku. Jednotky jsou
čtvrt za minutu (jednotky Mälzel Metronome). Můžete také dvakrát kliknout
přes hlavní okno otevřete dialogové okno, kde můžete zadat nový
tempo přímo pomocí klávesnice. Na výběr je také combo box
a zobrazit tempo pomocí italských hudebních jmen.

Beats/Bar lze nastavit od 1 do 32 taktů. Toto jsou počty úderů
na každém taktu nebo taktu a je to čitatel na taktu jako
bylo by to notováno.

Délka taktu je jmenovatelem specifikace taktu,
a představuje dobu trvání každé doby. Změna této hodnoty ne
změnit význam jednotek tempa.

Vzor je rozevírací seznam pro výběr definice vzoru. Výchozí
Hodnota "Automatic" znamená, že program generuje vzory pomocí
noty nastavené v konfiguračním dialogu (Strong/Weak) a rytmus
definice poskytnutá "údery/takty" a "délka taktu". Obsahuje také
názvy uživatelsky definovaných vzorů.

## Začínáme

Tento program používá protokol MIDI, takže je dobré mít
některé základní pojmy o MIDI, abyste plně porozuměli konceptům
za tím. Zde najdete dobrý úvod:
[Co je MIDI](https://www.midi.org/midi-articles/categories/MIDI%201.0).

Drumstick Metronome produkuje MIDI události. Pokud chcete slyšet události
přeloženo do zvuků, z tohoto musíte připojit port MIDI OUT
program do MIDI IN portu MIDI syntezátoru. Může to být buď a
hardwarový MIDI syntezátor nebo softwarový. Pokud se jedná o externí
hardwarový syntezátor, potřebujete také MIDI rozhraní s podporou ALSA
nainstalovaný ve vašem počítači a MIDI kabel připojený k oběma
MIDI rozhraní počítače a konektor MIDI IN syntezátoru.

Pokud nemáte externí MIDI syntezátor, můžete jej stále používat
program se softwarovým MIDI syntezátorem podporujícím sekvencer ALSA
připojení, jako je [QSynth](https://qsynth.sourceforge.io). Začněte svůj
softwarový syntezátor před Drumstick Metronome a poté proveďte připojení
mezi těmito dvěma programy, buď v konfiguračním dialogu Drumstick Metronome 
nebo pomocí externího správce připojení jako 
[QJackCtl](https://qjackctl.sourceforge.io).

Nezapomeňte si nainstalovat dobrý [Sound Font](https://en.wikipedia.org/wiki/SoundFont) 
do dialogu "Nastavení..." QSynth.

## Konfigurace

Drumstick Metronome má omezené možnosti správy relací. Může si to pamatovat
jedno připojení pro výstupní port ALSA a jedno připojení pro jeho
vstupní port. Připojení jsou uložena při ukončení programu a zapamatována
při spuštění. Tuto funkci nepotřebujete, pokud ji chcete vytvořit
připojení ručně, pomocí aconnect nebo jiného ekvivalentního nástroje, popř
pokud používáte externího správce relací, jako je patchbay obsažený v
program [QJackCtl](https://qjackctl.sourceforge.io).

Drumstick Metronome používá soubor definice nástroje ve formátu .INS, stejný
formát jako Qtractor, TSE3, Cakewalk a Sonar. **Výstupní nástroj**
rozevírací seznam umožňuje vybrat jeden ze standardních General MIDI,
Bubnové mapy Roland GS a Yamaha XG. Můžete přidat další vytváření definic
soubor s názvem `drums.ins` na `$HOME/.local/share/kmetronome.sourceforge.net`. The
obsah rozbalovacích seznamů **Banka**, **Program**, **Slabá** a **Silná poznámka** 
závisí také na této definici.

**Kanál** je obvykle 10, což znamená kanál perkusí obecného MIDI
syntetizér. Musí to být číslo mezi 1 a 16.

**Rozlišení** je počet tiků (časových jednotek) pro každou čtvrtletní notu.
Rozsah hodnot od 48 do 960. Výchozí hodnota je 120.

**Trvání poznámky** je délka (v počtu zaškrtnutí) časového rozpětí
mezi událostí NOTE ON a odpovídající událostí NOTE OFF. Tato kontrola je
povoleno pouze v případě, že je také povoleno **Odeslat události VYPNUTO POZNÁMKA**. Velmi nízké hodnoty
může u některých syntezátorů způsobit ztlumená kliknutí.

Zvuky perkusí obvykle nevyžadují odeslání událostí NOTE OFF
každá POZNÁMKA ZAPNUTA. Zaškrtněte políčko **Posílat události NOTE OFF** pouze v případě, že jste
syntetizér nebo nástroj toto nastavení podporuje nebo vyžaduje.

**Banka** a **Program** slouží k výměně bicí sady pro nástroje
podpora několika nastavení. Mnoho syntezátorů nerozumí programu
změny pro kanál perkusí.

V režimu **Automatic** pattern se jako první doba přehraje zvuk **Silná nota** 
v každém taktu, zatímco jakýkoli jiný takt ve stejném taktu se hraje pomocí 
zvuk **Slabá nota**. Číselné hodnoty 33 a 34 jsou zvuky GM2 a XG
pro kliknutí metronomu a zvonek metronomu.

## Editor vzorů

Pomocí tohoto dialogového okna můžete upravovat, testovat a vybírat vzory. Vytvořit
nové vzory, jednoduše uložíte aktuální definici pod novým názvem.
Vzory jsou reprezentovány tabulkou. Řádky v tabulce odpovídají
zazní perkuse. Ze seznamu zvuků můžete odstranit a přidat řádky
definované nastavením přístroje v konfiguračním dialogu. The
počet sloupců v tabulce určuje délku vzoru,
mezi 1 a 99 prvky libovolné délky taktu.

Každá buňka tabulky přijímá hodnoty mezi N=1 a 9, což odpovídá
MIDI dynamika (N*127/9) not, nebo 0 pro zrušení zvuku. Platný
hodnoty jsou také f (=forte) a p (=piano) odpovídající proměnné
rychlosti definované otočnými knoflíky (Strong/Weak) v hlavním okně.
Hodnoty buněk lze vybrat a upravit pomocí klávesnice
nebo myš. Před úpravou není třeba zastavovat přehrávání
buňky.

# Reference příkazů

## Hlavní okno

### Nabídka Soubor

**Soubor → Importovat vzory**

: Importuje definice vzoru do Drumstick Metronome

**Soubor → Exportovat vzory**

: Exportuje definice vzoru z Drumstick Metronome

**Soubor → Přehrát/Zastavit**

: Ovládá přehrávání vzoru

**Soubor → Konec**

: Ukončí Drumstick Metronome

### Nabídka Úpravy

**Upravit → Upravit vzory**

: Otevře editor vzorů

### Nabídka Nastavení

**Nastavení → Zobrazit tlačítka akcí**

: Zobrazí nebo skryje tlačítka akcí. Pro každé tlačítko existují ekvivalentní položky nabídky.

**Nastavení → Zobrazit lištu**

: Zobrazí nebo skryje panel nástrojů. Pro každé tlačítko existují ekvivalentní položky nabídky.

**Nastavení → Konfigurace**

: Konfiguruje metronom bicí paličky

### Nabídka Nápověda

**Nápověda → Obsah nápovědy**

: Otevře okno nápovědy

**Nápověda → Jazyk**

: Otevře podnabídku s dostupnými překlady

**Nápověda → o**

: Otevře okno o tomto programu

**Nápověda → o Qt**

: Otevře okno o rámcích Qt

# Externí kontrola

Drumstick Metronome lze ovládat pomocí rozhraní D-Bus, System Realtime
a MIDI zprávy System Exclusive.

## Rozhraní D-Bus 

Drumstick Metronome poskytuje některé funkce prostřednictvím rozhraní D-Bus. Můžeš
použijte program podporující D-Bus k ovládání Drumstick Metronome, jako je `qdbusviewer` nebo další 
nástroj příkazového řádku `qdbus`.

Tyto příkazy lze například použít z příkazového řádku:

    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.play
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.stop
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.cont
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTempo 150
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTimeSignature 3 8

## Zprávy Universal System Exclusive

Drumstick Metronome rozumí některým zprávám Universal System Exclusive. Protože
ID zařízení ještě není implementováno, musí být všechny rozpoznané zprávy
být označen jako vysílání (0x7F).

Zpráva v reálném čase: Zpráva o změně časového podpisu

    Formát: 0xF0 0x7F 0x7F 0x03 Délka příkazu Čitatel Jmenovatel ... 0xF7
                                0x02: Změna časového podpisu 
                                0x42: Změna časového podpisu zpožděna

Poznámky: oba příkazy ovlivňují pouze následující naplánované vzory po
přijímání příkazu. Délka je alespoň 2. Následují další bajty
Čitatel a jmenovatel jsou ignorovány.

Zprávy MMC

    Formát: 0xF0 0x7F 0x7F 0x06 Příkaz 0xF7
                                0x01: Stop
                                0x02: Hrajte
                                0x03: Odložená hra

## Rozhraní systému MIDI v reálném čase

Můžete použít MIDI zařízení generující zprávy MIDI System Realtime
ovládání Drumstick Metronome. Systémové zprávy v reálném čase jsou přijímány: Start (0xFA),
Zastavit (0xFC) a pokračovat (0xFB).

Mnoho hlavních MIDI klávesnic a zařízení s MIDI tlačítky poskytuje tyto tři
Ovládací prvky přenosu MIDI. Musíte připojit vaše externí MIDI zařízení
k počítači pomocí MIDI (nebo USB) kabelů a připojte
odpovídající port ALSA ke vstupnímu portu Drumstick Metronome. Toto spojení
musí být provedeno ručně nebo pomocí softwaru pro správu relací, jako je
QJackCtl.

# Kredity a licence

Copyright © 2005-2024 Pedro Lopez-Cabanillas

Dokumentace Copyright © 2005-2024 Pedro Lopez-Cabanillas

Tento program je svobodný software; můžete jej dále distribuovat a/nebo upravovat
to za podmínek GNU General Public License, jak byla zveřejněna
nadace pro svobodný software; buď verze 2 licence, nebo
(podle vašeho uvážení) jakékoli pozdější verze.

Tento program je distribuován v naději, že bude užitečný,
ale BEZ JAKÉKOLI ZÁRUKY; dokonce bez předpokládané záruky
OBCHODOVATELNOST nebo VHODNOST PRO KONKRÉTNÍ ÚČEL. Viz
Další podrobnosti naleznete v GNU General Public License.
                                                                        
Měli byste obdržet kopii GNU General Public License
spolu s tímto programem. Pokud ne, přejděte na [https://www.gnu.org/licenses](https://www.gnu.org/licenses)

# Instalace
 
## Jak získat Drumstick Metronome

Zde najdete nejnovější verzi: 
[Soubory projektu](https://sourceforge.net/projects/kmetronome/files/kmetronome/)

## Požadavky

Abyste mohli úspěšně zkompilovat a používat Drumstick Metronome, potřebujete Qt 5 resp
později Drumstick 2, ovladače ALSA a knihovna ALSA.

K produkci zvuku potřebujete také MIDI syntezátor.

Systém sestavení vyžaduje [CMake](https://cmake.org) 3.14 nebo novější.

Knihovnu ALSA, ovladače a nástroje naleznete na 
[Domovská stránka ALSA](https://www.alsa-project.org).

Drumstick::ALSA je obal C++ kolem rozhraní sekvenceru ALSA využívající Qt
předměty, idiomy a styl. Najdete ho na
[Domovská stránka paličky](https://drumstick.sourceforge.io).

Podívejte se také na [domovskou stránku Drumstick Metronome] (https://kmetronome.sourceforge.io/kmetronome.shtml).

## Kompilace a instalace

Chcete-li zkompilovat a nainstalovat Drumstick Metronome na váš systém, zadejte
následující v základním adresáři distribuce Drumstick Metronome:

    % cmake .
    % udělat
    % sudo make install

Protože Drumstick Metronome používá `cmake` a `make`, neměli byste mít žádné potíže
jeho sestavování. Pokud narazíte na problémy, nahlaste je na
autora nebo projektu 
[systém sledování chyb](https://sourceforge.net/p/kmetronome/bugs/).

