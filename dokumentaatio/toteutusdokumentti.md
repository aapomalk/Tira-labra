# Toteutusdokumentti

## Ohjelman yleisrakenne

Ohjelma koostuu verkosta (sis�lt�� my�s simulaatio laatikon ja naapurien etsint��n soveltuvan box -rakenteen) ja A* -algoritmista (keko tietorakenteineen).

Mainittujen tietorakenteiden ja algoritmien lis�ksi ohjelma sis�lt�� kolmen tiedoston lukemiseen omat funktionsa.
Tiedostoja ovat pdb-, parametri- ja xtc -tiedostot.

Lopuksi kaikki tiedostojen lukeminen sek� tietorakenteet ja algoritmit on yhdistetty yhdeksi ohjelmaksi.

### Verkko

Verkon yksi t�rkeimmist� ominaisuuksista ohjelmassa on se, ett� yhteydet lasketaan vasta algoritmin suorituksen aikana, ja varsinkin se ett� solmut on jaettu pienempiin alueisiin (box -tietorakenteessa), jolloin naapurien etsint� on k�yt�nn�ss� kertaluokkaa O(n) [todellisuudessa pahin tapaus on kuitenkin O(n^2), sill� kaikki solmut voivat edelleen olla vain yhdess� box -rakenteen sis�lt�m�ss� alueessa].

Verkko tarjoaa algoritmille solmujen v�liset yhteydet.

### A* -algoritmi

Algoritmi tarvitsee tiedon alkusolmusta ja kohdesolmusta sek� itse verkon. T�m�n j�lkeen yhteyksi�/kaaria aletaan k�ym��n l�pi ja kekoon lis�t��n uudet solmut et�isyysarvioineen sek� vanhat solmut mik�li l�ydet��n pienempi et�isyysarvio. N�iden lis�ksi pidet��n kirjaa mist� mihink�kin solmuun lyhin yhteys on tullut ja mik� on t�m�n yhteyden pituus. N�ist� saadaan laskettua lyhimm�n yhteyden pituus ja mitten solmujen kautta polku kulkee.

### Tiedostojen lukeminen

#### pdb
Pdb tarkoittaa "protein data bank" ja kyseess� on formaatti johon tallennetaan esimerkiksi juuri proteiinien atomistisia rakenteita, mutta my�s simulaatioissa k�ytett�vi� rakenteita, jotka sis�lt�v�t usein proteiinin lis�ksi my�s verimolekyylej� ja solukalvon.

Pdb -tiedosto luetaan kahdesti. Ensimm�isell� kerralla selvitet��n kuinka monta solmua verkkoon lis�t��n. Solmut tunnistetaan parametrien avulla.

#### parametrit
Parametreihin kuuluu solmujen tunnistautumistiedot sek� yleisi� muuttujia.
Formaattiin kuuluu, ett� kukin komento alkaa nelikirjaimisella koodisanalla.
T�m� tarkoittaa sit�, ett� kommentit saa kirjoittaa miten tahtoo, kunhan kyseinen rivi ei ala mill��n koodisanoista.

#### xtc
Xtc tarkoittaa "kompressoitu trajektori".
Kyseess� on GROMACS -ohjelman formaatti.
Kurssin puitteissa ei ollut aikaa kirjoittaa omaa xtc -lukijaa,
joten k�ytin GROMACS:n tarjoamia C-kielell� kirjoitettuja funktioita.

### Ohjelman k�ytt�liittym�

## Saavutetut aika- ja tilavaativuudet (m.m. O-analyysit pseudokoodista)

Kuten m��rittelydokumentissa todettiin:

Aikavaativuus on O((|E|+|V|)log(|V|)) ja tilavaativuus O(|V|) (|E| on yhteyksien lukum��r� ja |V| solmujen lukum��r�).

Itse verkon rakentaminen on luokkaa O(|V|) sill� solmut eiv�t voi olla p��llekk�in (atomit eiv�t voi olla p��llekk�in) ja yhteydell� on my�s maksimiet�isyys (vetysidos on lyhyen matkan vuorovaikuttamista, kunhan k�ytet��n verkon kokoon suhteutettua alueisiin jakoa.

Mik�li k�ytet��n vain yht� aluetta (�l� k�yt� vain yht� aluetta paitsi ��rimm�isen pieniss� tapauksissa) niin verkon rakentaminen on luokkaa O(|V|^2),
sill� jokainen solmu testaa kuinka kaukana kukin solmu on.

## Suorityskyky- ja O-analyysivertailu (mik�li ty� vertailupainotteinen)

Kuten testausdokumentista n�hd��n on A*:n ja Dijkstran pahimmat tapaukset
suunnilleen yht� pahoja.
A* ei kuitenkaan ole koskaan merkitt�v�sti huonompi kuin Dijkstra.

## Ty�n mahdolliset puutteet ja parannusehdotukset

Todenn�k�isesti viimeist��n kev��ll� tulen toteuttamaan ohjelmasta rinnakkaisajoon soveltuvan version.

## L�hteet