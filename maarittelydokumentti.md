# Määrittelydokumentti

## Mitä algoritmeja ja tietorakenteita toteutat työssäsi
Polunetsintään toteutan A* algoritmin ja mahdollisesti jonkin vertailualgoritmin.
Tietorakenteista toteutan tarvittavat tietorakenteet. (selvitän pian)
Tarvittaessa toteutan myös algoritmin (ja tietorakenteet) lähimpien naapurien löytämiseen 
(riippuen missä muodossa koordinaatit saadaan xtc -tiedostosta).
Toinen vaihtoehto on muodostaa koko verkko ennen polun etsimistä.

## Mitä ongelmaa ratkaiset ja miksi valitsit kyseiset algoritmit/tietorakenteet
Ongelma on löytää molekyylidynaamisesta (MD) simulaatiosta kaikki aika-askeleet, 
jolloin kahden valitun atomin välille muodostuu vetysidoksista polku 
(vesimolekyylien ja mahdollisesti varattujen proteiini residuitten kautta).
Algoritmit/tietorakenteet valitsin, koska ne ovat yleisesti käytettyjä ja nopeita.

## Mitä syötteitä ohjelma saa ja miten näitä käytetään
Ohjelma saa syötteenä xtc (portable trajectory) tiedoston nimen 
sekä alku- ja loppuatomin tunnisteet.
Muita mahdollisia parametrejä ovat tulostetiedoston nimi 
sekä vetysidoksen määritelmään liittyvät vakiot (etäisyys, kulma ja kelpaavat atomit).
Xtc tiedostosta luetaan atomien koordinaatit, joita käytetään verkon muodostamiseen 
ja siten polun etsintään.
Alku- ja loppuatomin tunnisteilla löydetään alku- ja loppuatomin koordinaatit xtc tiedostosta.
Tulostetiedostoon kirjataan kunkin aika-askeleen tulos, esimerkiksi 1 jos polku löytyi 
ja 0 jos ei löytynyt.
Myös esimerkiksi polun pituus voidaan laskea (jos halutaan tietää lyhin polku).
Vetysidoksen määritelmä määrittelee hyvin vahvasti muodostuvan verkon.
Yleensä kelpaavat atomit ovat typpi (N), happi (O) ja fluori (F).
Yläraja vetysidoksen etäisyydelle on yleensä noin 2-3Å (ångström; 0.1nm) 
ja vetysidoksen maksimikulmalle noin 20 astetta.
Riippuen valituista arvoista (erityisesti kulma) kaikki läheisetkään atomit eivät välttämättä 
muodosta vetysidosta.

## Tavoitteena olevat aika- ja tilavaativuudet (m.m. O-analyysit)
Aikavaativuus on O(|E|) (eli yhteyksien lukumäärä) ja tilavaativuus O(|V|) (eli atomien lukumäärä).
Käytännössä (kun käytetään suunnilleen oletusmääritelmää vetysidokselle) algoritmin ei tarvitse 
pitää kirjaa lähellekään kaikista atomeista.
3Å etäisyydellä olevat atomit ovat enintään kymmenien luokkaa 
ja näistäkin mahdollisesti suurin osa voidaan jättää koko verkosta pois.
Kun taas yhdessä simulaatiossa saattaa olla helposti yhteensä satoja tuhansia atomeja.
Niissä aika-askeleissa, joissa polkua ei löydy päädytään lähelle O(|E|).
Mainituissa tapauksissa huomattavan nopeutuksen saattaa tuoda, jos polkua lähdetään etsimään 
sekä alusta (eteenpäin) että lopusta (taaksepäin).

## Lähteet
http://sbcb.bioch.ox.ac.uk/jgromacs/ xtc tiedostojen lukemiseen