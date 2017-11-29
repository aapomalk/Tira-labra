# Vertaisarviointi

Paketti ladattu 10:38 23.11.2017

Mielenkiintoinen aihe.

(koodia luettu 27.11. klo 21:30-22:30)

Hyvältä vaikuttaa.
Haet eri kierrosten askeleita samassa listassa, jolloin paras reitti löytyy luonnollisesti ensimmäisenä (kunhan toteutat askeleiden järjestämisen).
Hyvän oloinen rakenne, missä askeleet ja kierrokset on eritelty omiin luokkiinsa.

## AStarStepComparator.java (katsottu netistä 27.11.)

**heuristic:** Mielestäni ei ole hyötyä ottaa karteesista etäisyyttä `sqrt(x*x+y*y)`,
kun luolastossa voi liikkua vain vierekkäisiin ruutuihin ruutu kerrallaan.
Todellinen pienin etäisyys kohteeseen siis olisi `x+y`.

**comparator:** lisäksi karteesisen etäisyyden tarkkuus menetetään kun muutetaan double -> int.
Tässä etäisyyden voisi kertoa jollain vakiolla, jolloin saataisiin tarkempi vertailu.
Tai esimerkiksi palauttaa 1 jos `val1 > val2`, 0 jos `val1 == val2` ja -1 jos `val1 < val2`.
Tähän on tärkeä keskittyä, sillä jos järjestys on epätäsmällinen niin ohjelma ei tuota lyhintä/parasta polkua luolaston läpi.

## Cycle.java

**visited:** ovelaa käyttää taulukkoa, joka ei kopioidu sijoitusoperaatiossa (vaan on eri askeleille sama)
säästäen muistia ja helpottaa informaation jakamista.
Jos haluaisi säästää enemmän tilaa niin jokaiselle luolan ruudulle voisi olla oma taulukko/lista,
johon lisättäisiin millä kierroksella siinä on jo käyty.
Tämä sillä oletuksella, että kukin kierros koluaa vain osan luolastosta.
Jos kaikki kierrokset koluavat käytännössä koko luolaston niin tilaa ei säästy (mutta tällöin maali on luultavasti jo löydetty).

## Forarger.java

Vanhassa versiossa et vielä järjestä askeleita.
Oletettavasti tätä varten olet lisännyt PriorityQueue luokan.