# Toteutusdokumentti

## Ohjelman yleisrakenne

Ohjelma koostuu verkosta (sisältää myös simulaatio laatikon ja naapurien etsintään soveltuvan box -rakenteen) ja A* -algoritmista (keko tietorakenteineen).

### Verkko

Verkon yksi tärkeimmistä ominaisuuksista ohjelmassa on se, että yhteydet lasketaan vasta algoritmin suorituksen aikana, ja varsinkin se että solmut on jaettu pienempiin alueisiin (box -tietorakenteessa), jolloin naapurien etsintä on käytännössä kertaluokkaa O(n) [todellisuudessa pahin tapaus on kuitenkin O(n^2), sillä kaikki solmut voivat edelleen olla vain yhdessä box -rakenteen sisältämässä alueessa].

Verkko tarjoaa algoritmille solmujen väliset yhteydet.

### A* -algoritmi

Algoritmi tarvitsee tiedon alkusolmusta ja kohdesolmusta sekä itse verkon. Tämän jälkeen yhteyksiä/kaaria aletaan käymään läpi ja kekoon lisätään uudet solmut etäisyysarvioineen sekä vanhat solmut mikäli löydetään pienempi etäisyysarvio. Näiden lisäksi pidetään kirjaa mistä mihinkäkin solmuun lyhin yhteys on tullut ja mikä on tämän yhteyden pituus. Näistä saadaan laskettua lyhimmän yhteyden pituus ja mitten solmujen kautta polku kulkee.

## Saavutetut aika- ja tilavaativuudet (m.m. O-analyysit pseudokoodista)

## Suorityskyky- ja O-analyysivertailu (mikäli työ vertailupainotteinen)

## Työn mahdolliset puutteet ja parannusehdotukset

Todennäköisesti viimeistään keväällä tulen toteuttamaan ohjelmasta rinnakkaisajoon soveltuvan version.

## Lähteet