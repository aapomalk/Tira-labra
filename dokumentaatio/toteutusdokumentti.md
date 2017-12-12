# Toteutusdokumentti

## Ohjelman yleisrakenne

Ohjelma koostuu verkosta (sisältää myös simulaatio laatikon ja naapurien etsintään soveltuvan box -rakenteen) ja A* -algoritmista (keko tietorakenteineen).

Mainittujen tietorakenteiden ja algoritmien lisäksi ohjelma sisältää kolmen tiedoston lukemiseen omat funktionsa.
Tiedostoja ovat pdb-, parametri- ja xtc -tiedostot.

Lopuksi kaikki tiedostojen lukeminen sekä tietorakenteet ja algoritmit on yhdistetty yhdeksi ohjelmaksi.

### Verkko

Verkon yksi tärkeimmistä ominaisuuksista ohjelmassa on se, että yhteydet lasketaan vasta algoritmin suorituksen aikana, ja varsinkin se että solmut on jaettu pienempiin alueisiin (box -tietorakenteessa), jolloin naapurien etsintä on käytännössä kertaluokkaa O(n) [todellisuudessa pahin tapaus on kuitenkin O(n^2), sillä kaikki solmut voivat edelleen olla vain yhdessä box -rakenteen sisältämässä alueessa].

Verkko tarjoaa algoritmille solmujen väliset yhteydet.

### A* -algoritmi

Algoritmi tarvitsee tiedon alkusolmusta ja kohdesolmusta sekä itse verkon. Tämän jälkeen yhteyksiä/kaaria aletaan käymään läpi ja kekoon lisätään uudet solmut etäisyysarvioineen sekä vanhat solmut mikäli löydetään pienempi etäisyysarvio. Näiden lisäksi pidetään kirjaa mistä mihinkäkin solmuun lyhin yhteys on tullut ja mikä on tämän yhteyden pituus. Näistä saadaan laskettua lyhimmän yhteyden pituus ja mitten solmujen kautta polku kulkee.

### Tiedostojen lukeminen

#### pdb
Pdb tarkoittaa "protein data bank" ja kyseessä on formaatti johon tallennetaan esimerkiksi juuri proteiinien atomistisia rakenteita, mutta myös simulaatioissa käytettäviä rakenteita, jotka sisältävät usein proteiinin lisäksi myös verimolekyylejä ja solukalvon.

Pdb -tiedosto luetaan kahdesti. Ensimmäisellä kerralla selvitetään kuinka monta solmua verkkoon lisätään. Solmut tunnistetaan parametrien avulla.

#### parametrit
Parametreihin kuuluu solmujen tunnistautumistiedot sekä yleisiä muuttujia.
Formaattiin kuuluu, että kukin komento alkaa nelikirjaimisella koodisanalla.
Tämä tarkoittaa sitä, että kommentit saa kirjoittaa miten tahtoo, kunhan kyseinen rivi ei ala millään koodisanoista.

#### xtc
Xtc tarkoittaa "kompressoitu trajektori".
Kyseessä on GROMACS -ohjelman formaatti.
Kurssin puitteissa ei ollut aikaa kirjoittaa omaa xtc -lukijaa,
joten käytin GROMACS:n tarjoamia C-kielellä kirjoitettuja funktioita.

### Ohjelman käyttöliittymä

## Saavutetut aika- ja tilavaativuudet (m.m. O-analyysit pseudokoodista)

Kuten määrittelydokumentissa todettiin:

Aikavaativuus on O((|E|+|V|)log(|V|)) ja tilavaativuus O(|V|) (|E| on yhteyksien lukumäärä ja |V| solmujen lukumäärä).

Itse verkon rakentaminen on luokkaa O(|V|) sillä solmut eivät voi olla päällekkäin (atomit eivät voi olla päällekkäin) ja yhteydellä on myös maksimietäisyys (vetysidos on lyhyen matkan vuorovaikuttamista, kunhan käytetään verkon kokoon suhteutettua alueisiin jakoa.

Mikäli käytetään vain yhtä aluetta (älä käytä vain yhtä aluetta paitsi äärimmäisen pienissä tapauksissa) niin verkon rakentaminen on luokkaa O(|V|^2),
sillä jokainen solmu testaa kuinka kaukana kukin solmu on.

## Suorityskyky- ja O-analyysivertailu (mikäli työ vertailupainotteinen)

Kuten testausdokumentista nähdään on A*:n ja Dijkstran pahimmat tapaukset
suunnilleen yhtä pahoja.
A* ei kuitenkaan ole koskaan merkittävästi huonompi kuin Dijkstra.

## Työn mahdolliset puutteet ja parannusehdotukset

Todennäköisesti viimeistään keväällä tulen toteuttamaan ohjelmasta rinnakkaisajoon soveltuvan version.

## Lähteet