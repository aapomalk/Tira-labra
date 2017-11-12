
# Polunetsijä

Tavoitteena tehdä mahdollisimman geneerinen polunetsijä.

## Verkko

Verkko koostuu solmuista ja solmujen välisistä (suunnatuista) yhteyksistä, kaarista.
Kaarilla on paino, joka talletetaan osaksi verkkoa.
A* algoritmia varten tarvitaan myös heuristiikkafunktio, 
mutta tätä ei sisällytetä osaksi verkkoa vaan funktio laskee sen kahdesta solmusta.

## Heuristiikkafunktio

Heuristiikkafunktio kertoo arvion kahden solmun etäisyydestä. 
Arvion täytyy olla enintään todellinen etäisyys, jos halutaan löytää lyhin polku.
Algoritmia nopeuttaakseen esimerkiksi kaukana olevia solmuja voidaan arvioida vielä kauemmaksi,
jotta polku löytyisi mahdollisesti nopeammin (nykyisessä tapauksessa tämä ei ole tarpeen tai edes toivottavaa).

## Algoritmi

Itse algoritmi saa parametreinaan alku- ja loppusolmun. 
Sillä on myös käytettävissään verkko ja heuristiikkafunktio.
Lisäksi tarvitaan tieto missä solmussa on käyty/mikä on käsittelyssä.
Käsittelyssä oleva solmu (tai tunniste kyseisestä solmusta) on talletettuna tietorakenteeseen 
(esimerkiksi minimi-keko; kutsutaan tästä lähtien keoksi),
joka kertoo seuraavan solmun tunnisteen.
Seuraavan solmun tunnistaa siitä, että kuljettu matka (josta pitää myös pitää kirjaa), 
kulkematon kaari ja solmusta loppuun heuristiikkafunktion arvo yhteenlaskettuna on pienin.
Kuljettu matka on aina matka siihen solmuun, jonka naapurina uusi solmu oli ennen käsittelyyn ottamista.
Solmun käsittely päättyy, kun kaikki sen naapurit on saatu lisättyä kekoon.

## Solmu

Tässä kyseisessä tapauksessa solmu sisältää yhden ei-vetyatomin koordinaatit 
ja nollan tai enemmän vetyatomin koordinaatit.
Kaari lähtee aina vetyatomin suuntaan ja saapuu ei-vetyatomiin.
Kaaren olemassaolon määrittelevät kaksi arvoa: etäisyys ja kulma.
Verkkoa määriteltäessä päätetään etäisyyden ja kulman lisäksi, että mitkä ei-vetyatomit voivat toimia solmuina.
Heuristiikkafunktio laskee aina ei-vetyatomien välisen karteesisen etäisyyden.
Naapurisolmujen välinen etäisyys lasketaan ensin solmun sisällä vetyatommiin ja tästä seuraavan solmun ei-vetyatomiin.

## Kirjanpito

Kuljetun matkan ja käsittelemättömyyden lisäksi algoritmi pitää kirjaa lyhimmistä reiteistä, 
eli mistä solmusta lyhin reitti saapui kyseiseen solmuun.
Kuljetun matkan ja käsittelemättömyyden voi yhdistää yhdeksi arvoksi asettamalla käsittelemättömyyden negatiiviseksi matkaksi.
Yleisemmässä tapauksessa, missä kaarien paino voi olla negatiivinen tämä ei toimi.

## Toteutus

Verkko toteutetaan siten, että verkko rakennetaan algoritmin edetessä.
Tämä on mahdollista siten, että atomit/solmut tallennetaan muistiin alueittain pieniin "laatikoihin".
Naapureita etsittäessä riittää käydä läpi oma ja mahdollisesti viereiset laatikot.
Laatikoista täytyy siis tietää koko ja muoto (sama kaikilla laatikoilla).
Laatikon täytyy olla tarpeeksi iso, ettei tyhjiä laatikoita syntyisi liikaa 
(kukin tyhjä laatikko vastaa vähintään yhtä ylimääräistä atomia).
Toisaalta laatikon tulisi olla mahdollisimman pieni, jotta läpikäytävien atomien lukumäärä olisi mahdollisimman pieni.
Naapureita etsittäessä ensin voidaan käytettää heuristiikkafunktiota, 
ja jos etäisyys on tarpeeksi pieni lasketaan kulma ja todellinen etäisyys.
Algoritmin nopeuttamiseksi on myös mahdollista käyttää pelkästään ei-vetyatomien välistä suoraa etäisyyttä.