# Toteutusdokumentti

## Ohjelman yleisrakenne

Ohjelma koostuu verkosta (sis�lt�� my�s simulaatio laatikon ja naapurien etsint��n soveltuvan box -rakenteen) ja A* -algoritmista (keko tietorakenteineen).

### Verkko

Verkon yksi t�rkeimmist� ominaisuuksista ohjelmassa on se, ett� yhteydet lasketaan vasta algoritmin suorituksen aikana, ja varsinkin se ett� solmut on jaettu pienempiin alueisiin (box -tietorakenteessa), jolloin naapurien etsint� on k�yt�nn�ss� kertaluokkaa O(n) [todellisuudessa pahin tapaus on kuitenkin O(n^2), sill� kaikki solmut voivat edelleen olla vain yhdess� box -rakenteen sis�lt�m�ss� alueessa].

Verkko tarjoaa algoritmille solmujen v�liset yhteydet.

### A* -algoritmi

Algoritmi tarvitsee tiedon alkusolmusta ja kohdesolmusta sek� itse verkon. T�m�n j�lkeen yhteyksi�/kaaria aletaan k�ym��n l�pi ja kekoon lis�t��n uudet solmut et�isyysarvioineen sek� vanhat solmut mik�li l�ydet��n pienempi et�isyysarvio. N�iden lis�ksi pidet��n kirjaa mist� mihink�kin solmuun lyhin yhteys on tullut ja mik� on t�m�n yhteyden pituus. N�ist� saadaan laskettua lyhimm�n yhteyden pituus ja mitten solmujen kautta polku kulkee.

## Saavutetut aika- ja tilavaativuudet (m.m. O-analyysit pseudokoodista)

## Suorityskyky- ja O-analyysivertailu (mik�li ty� vertailupainotteinen)

## Ty�n mahdolliset puutteet ja parannusehdotukset

Todenn�k�isesti viimeist��n kev��ll� tulen toteuttamaan ohjelmasta rinnakkaisajoon soveltuvan version.

## L�hteet