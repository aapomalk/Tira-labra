# Viikkoraportti 6

## Viikon työlista

+Dokumentaatio: Kirjoitettu koodi kommentoitua. Toteutus- ja testausdokumentaatiota kirjoitettu.

+Viikkoraportti numero 6

+Ohjelma: Tietorakenteet ja algoritmit toteutettu itse.

+Testaus: Koodin kattava yksikkötestaus. Suorituskykytestausta tehty.

## Mitä olen tehnyt tällä viikolla?

### 6.12.2017
Vertaisarvio nro 2.

Mysteerinen segfault lähti pois, kun muutin muistinvaraamis -järjestystä.
Nyt pdb_reader laskee ensin solmujen lukumäärän,
varaa kaikille kerralla muistia ja sitten vasta alkaa lisäilemään atomeja verkkoon.

test_pdb_reader toimii!

Parametrien lukeminen aloitettu.

klo 13-17, 18:15-20:15, 22:30-23:30

### 9.12.2017
Parametrien lukeminen onnistuu ja pelaa yhteen pdb -lukijan kanssa.
Seuraavaksi pitäisikin toteuttaa xtc -lukeminen ja pistää ensimmäinen toimiva
versio ohjelmasta pakettiin.

klo 16:30-18:15, 18:50-20:30

### 10.12.2017
Xtc -lukeminen aloitettu.
Testaus täytyy suorittaa koneella, jossa on GROMACS (työkone).
Kaikki näyttää toimivan (kunhan muistaa, että decompositio on alustettu).

klo 15:40-16:50, 18:30-19:35, 20:40-0:30

Yhteensä (4h + 2h + 1h) + (1h45min + 1h40min) + (1h10min + 1h5min + 3h50min)
= 7h + 3h25min + 6h5min = 16h30min

## Miten ohjelma on edistynyt?
Mukavasti.
Vaikka en täysin ymmärrä kaikkia segfaulteja niin intuitiiviset ratkaisut toimivat.

Ohjelma on kokoamista (ja debuggaamista) vaille valmis.

## Mitä opin tällä viikolla / tänään?
Opin, että muistia ei kannata varata miten sattuu.
Jatkuva muistin varaaminen ja vapauttaminen ilmeisesti koituu segfaultiksi melko helposti isoilla syötteillä..

Miten olen päässyt näin pitkälle ilman Valgrindiä?
Ehkä mysteeriset segfaultit katoaa valgrind -testauksen myötä!

## Mikä jäi epäselväksi tai tuottanut vaikeuksia?

## Mitä teen seuraavaksi?
Paljon Valgrind -testausta