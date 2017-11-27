# Vertaisarviointi

Paketti ladattu 10:38 23.11.2017

Mielenkiintoinen aihe.

(koodia luettu 27.11. klo 21:30-

Mietin, että jos luolastosta muodostuu verkko, jossa on negatiivisia painoja niin sopiiko A* tällaiseen? 
Voisiko Bellmanin-Fordin algoritmia muokata muuttuville verkonpainoille?

Ilmeisesti etsitään kuitenkin ajallisesti nopeinta reittiä, johon A* kyllä soveltuu:
Jos maali löytyy kulkematta yhdenkään erikoissolmun kautta niin se on silloin varmasti nopein eikä erikoissolmuista tarvitse välittää.

Toinen pohdinnan aihe on, että etsitkö erikoissolmuista lähteviä polkuja samassa sopassa kuin muita polkuja?
Tämä ehkä selviää lukemalla koodia tarkemmin (jos ehdin).
Jos haluaa että nopein reitti löytyy ensimmäisenä, eikä tarvitse alkaa vertailemaan vaihtoehtoisia reittejä 
niin silloin erikseen aloitetut etsinnät tulisi hakea samassa sopassa muiden polkujen kanssa.
Samalla sopalla tarkoitan, että uuden polun etsintä tapahtuu samaa kekoa käyttäen.
Tällöin ensimmäisenä maaliin löytyvä reitti omaa lyhimmän ajan.

## AStarStepComparator.java

**heuristic:** onko hyötyä ottaa karteesista etäisyyttä "sqrt(x*x+y*y)" jos luolastossa voi liikkua vain vierekkäisiin ruutuihin ruutu kerrallaan? Todellinen minimi kuljettu etäisyys siis olisi "x+y" jos olen oikein ymmärtänyt. Jos vinottainen liike on myös mahdollista niin silloin todellinen etäisyys on "max(x, y)".

**comparator:** toisaalta karteesisen etäisyyden tarkkuus menetetään kun muutetaan double -> int. Tässä etäisyyden voisi kertoa jollain vakiolla, jolloin saataisiin tarkempi vertailu. Tai esimerkiksi palauttaa 1 jos "val1 > val2", 0 jos "val1 == val2" ja -1 jos "val1 < val2". Jos järjestys on epätäsmällinen niin tuottaako ohjelma lyhintä/parasta polkua luolaston läpi? Lisäksi jos etäisyys laskettaisiin "x+y" niin sen voisi suorittaa suoraan kokonaisluvuilla.
