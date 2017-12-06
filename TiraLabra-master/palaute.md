# Koodikatselmointi

Projekti ladattu 6.12.2017 klo 13:07

Mielenkiintoinen aihe.

Yleisesti pohdin, että onko perusteltua laskea 300 kertaa A* pelin aikana (kerran per vuoro)?
Toisena vaihtoehtona voisi olla, että lasketaan aluksi
(tai taustalla, jos ensimmäisellä kierroksella ei ehditä)
kaikkien pisteiden väliset lyhimmät etäisyydet (esimerkiksi n kertaa Dijkstra).
Sitten vain käytäisiin läpi tätä matriisia ja tarkistettaisiin onko toinen botti katkaissut polun,
jos polku on katki niin lähistöltä on helppo etsiä vaihtoehtoisia reittejä,
koska tiedetään kaikkien solmujen väliset lyhimmät etäisyydet.
Tai sitten A* -algoritmia käytettäisiin tarvittaessa,
mikäli polku on poikki eikä lähistöltä löydy vaihtoehtoa
(myös A* toimii nopeammin, kun tiedetään valmiiksi solmujen väliset etäisyydet ilman muita botteja,
tällä hetkellä A* käy tarvittaessa 300 kertaa samassa umpikujassa etsiessään lyhintä polkua).
Tällä lähestymisellä säästettäisiin aikaa erityisesti loppupelissä
ja voitaisiin keskittyä enemmän itse botin tekoälyyn.

## Astar.java

Hyvä, Manhattan-etäisyyttä käytetty etäisyysarviossa (eikä euklidista etäisyyttä).

Itse antaisin solmulle, jossa vastustaja on, jonkin "sakon".
Sakko voisi olla ääretön (jolloin siitä ei tosiaan pääse läpi - oleellisesti sama kuin nyt)
tai sitten se voisi riippua bottien terveyspisteistä (hp; health point) kuten silloin kun päätetään hyökätä.
Jos sakko ei olisi ääretön niin lyhin reitti kaivokselle/parantamispisteelle voisi kulkea toisen botin läpi.

Sakko -menetelmää voisi myös hyödyntää siten, että botin viereisissä solmuissa olisi myös sakko, mutta pienempi.
Tällöin botti välttelisi vihollisia mikäli taistelua on syytä välttää
(tällä hetkellä jos prioriteetti ei ole taistelu niin toisista boteista ei välitetä).

Toisaalta sakon voisi myös kääntää negatiiviseksi, jolloin botti voisi (kunhan koodia hieman muokataan)
peitota vastustajan matkalla esimerkiksi kaivokselle, kunhan toinen botti ei lähde juoksemaan karkuun.

## CaptureDecision.java

Luokka valitsee lähimmän kaivoksen, joka ei ole omassa omistuksessa.
Tässä kohtaa voisi käyttää pelisilmää ja punnita myös se,
että kannattaako mieluummin ryöstää toiselta pelaajalta (ja keneltä) kaivos kuin
vain valloittaa lähin kaivos.

Ehkä kuitenkin kriittisempi ongelma tässä on se,
että kaivos valitaan etäisyysarvion perusteella.
Tällöin on mahdollista, että valitaan todellisuudessa kaikista kauimmainen kaivos
(jos kaivos on heti seinän takana, mutta seinän toiselle puolelle pääsee vain kiertämällä pitkä matka).

Jos Astarin toteuttaa sopivalla tavalla niin uskoisin
että sitä voisi käyttää myös useamman kaivoksen kohdalla saman kierroksen aikana
(jo laskettuja lyhimpiä reittejä ei tarvitse hylätä).

Prioriteetti vakio 3 olisi selkeämpää saada esimerkiksi koodin alkuun `int capture_priority_constant = 3;`
(tällä hetkellä se ei aukea, ellei tunne tarpeeksi muuta koodia).

## HealDecision.java

Taas sama, että lähin parannuspaikka valitaan etäisyysarvion perusteella,
eikä todellisen etäisyyden perusteella.
Jos kaikki päätöksentekijät ajaisivat samaa Astar -oliota
nämä tosiaan voisivat hyödyntää edellisiä ajoja.
Tämä vaatisi sitä, että Astar:n solmuja järjestävä tietorakenne sallisi muutokset loppuetäisyydessä.

Hyvä että parantamisen prioriteetti ei ole vakio.

## KillDecision.java

Rivillä 25: `!h.getPos().equals(state.getMe().getPos())` on hieman monimutkainen tapa sanoa `h != me`.
Koodin luettavuuden kannalta kannattaisin jotain jälkimmäisen kaltaista.

Ja jos ei vielä tullut selväksi, niin etäisyysarvio ei kaikissa tapauksissa ole paras vaihtoehto.

## DecisionMaker.java

Tyylikkäämpää olisi, jos päätökset järjestettäisiin listan avulla.
Myös jos jatkossa verrattavia päätöksiä tulee enemmän niin koodia ei enää tarvitsisi muuttaa niin paljoa,
jos järjestämiseen käytetään listaa (tai taulukkoa jne).

## AdvancedGameState.java

Vaikuttaa hyvältä, erityisesti kun luolasto pidetään vakiona
ja ainoastaan muuttuvat osiot päivitetään vuorojen välissä (pelaajat, kaivokset).

## MyHashMap.java

Entry -luokkahan on linkitetty lista.
Aiotko korvata sen toisenlaisella linkitetyllä listalla, kunhan saat sen tehtyä?
Vai toteutatko yhteensä kaksi linkitettyä listaa?
(joista toinen on vain hashmapin sisällä)

Käytät ilmeisesti Javan omaa hashCode() funktiota kullekkin luokalle.
Omasta puolestani tämä on ok (en tosin ole tarkistanut miten kyseinen hashCode() -funktio toimii)
mutta kannattanee kysyä onko tämä ok osana omaa tietorakennetta kurssin puitteissa.
Hash funktion toteutus on kuitenkin nähdäkseni melko iso osa hashmap -tietorakennetta.

En ole perehtynyt koodiin niin syvällisesti, että osaisin sanoa riittääkö 6-paikkainen hashmap
(tai onko vakiomäärä hyvä, sitähän voi manuaalisesti kasvattaa/pienentää tarvittaessa ennen koodin ajamista).
Mutta jos vakiomäärä osoittautuu huonoksi, niin oletko harkinnut että tekisit hashmapista dynaamisen?
Eli taulukkoa kasvatettaisiin jos alkioita on vaikka yli 3*taulukon koko.
Hashmapin etu menetetään jos linkitettyjen listojen pituus kasvaa suureksi.

## Lopuksi

Ohjelma näyttää edistyneen hyvin ja paljon hyviä ratkaisuja on myös tehty.