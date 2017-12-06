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

klo 13-17, 18:15-20:15

## Miten ohjelma on edistynyt?
Mukavasti.
Vaikka en täysin ymmärrä kaikkia segfaulteja niin intuitiiviset ratkaisut toimivat.

## Mitä opin tällä viikolla / tänään?
Opin, että muistia ei kannata varata miten sattuu.
Jatkuva muistin varaaminen ja vapauttaminen ilmeisesti koituu segfaultiksi melko helposti..

## Mikä jäi epäselväksi tai tuottanut vaikeuksia?

## Mitä teen seuraavaksi?
