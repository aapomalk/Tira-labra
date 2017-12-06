# Koodikatselmointi

Projekti ladattu 6.12.2017 klo 13:07

Mielenkiintoinen aihe.

Yleisesti pohdin, ett� onko perusteltua laskea 300 kertaa A* pelin aikana (kerran per vuoro)?
Toisena vaihtoehtona voisi olla, ett� lasketaan aluksi
(tai taustalla, jos ensimm�isell� kierroksella ei ehdit�)
kaikkien pisteiden v�liset lyhimm�t et�isyydet (esimerkiksi n kertaa Dijkstra).
Sitten vain k�yt�isiin l�pi t�t� matriisia ja tarkistettaisiin onko toinen botti katkaissut polun,
jos polku on katki niin l�hist�lt� on helppo etsi� vaihtoehtoisia reittej�,
koska tiedet��n kaikkien solmujen v�liset lyhimm�t et�isyydet.
Tai sitten A* -algoritmia k�ytett�isiin tarvittaessa,
mik�li polku on poikki eik� l�hist�lt� l�ydy vaihtoehtoa
(my�s A* toimii nopeammin, kun tiedet��n valmiiksi solmujen v�liset et�isyydet ilman muita botteja,
t�ll� hetkell� A* k�y tarvittaessa 300 kertaa samassa umpikujassa etsiess��n lyhint� polkua).
T�ll� l�hestymisell� s��stett�isiin aikaa erityisesti loppupeliss�
ja voitaisiin keskitty� enemm�n itse botin teko�lyyn.

## Astar.java

Hyv�, Manhattan-et�isyytt� k�ytetty et�isyysarviossa (eik� euklidista et�isyytt�).

Itse antaisin solmulle, jossa vastustaja on, jonkin "sakon".
Sakko voisi olla ��ret�n (jolloin siit� ei tosiaan p��se l�pi - oleellisesti sama kuin nyt)
tai sitten se voisi riippua bottien terveyspisteist� (hp; health point) kuten silloin kun p��tet��n hy�k�t�.
Jos sakko ei olisi ��ret�n niin lyhin reitti kaivokselle/parantamispisteelle voisi kulkea toisen botin l�pi.

Sakko -menetelm�� voisi my�s hy�dynt�� siten, ett� botin viereisiss� solmuissa olisi my�s sakko, mutta pienempi.
T�ll�in botti v�lttelisi vihollisia mik�li taistelua on syyt� v�ltt��
(t�ll� hetkell� jos prioriteetti ei ole taistelu niin toisista boteista ei v�litet�).

Toisaalta sakon voisi my�s k��nt�� negatiiviseksi, jolloin botti voisi (kunhan koodia hieman muokataan)
peitota vastustajan matkalla esimerkiksi kaivokselle, kunhan toinen botti ei l�hde juoksemaan karkuun.

## CaptureDecision.java

Luokka valitsee l�himm�n kaivoksen, joka ei ole omassa omistuksessa.
T�ss� kohtaa voisi k�ytt�� pelisilm�� ja punnita my�s se,
ett� kannattaako mieluummin ry�st�� toiselta pelaajalta (ja kenelt�) kaivos kuin
vain valloittaa l�hin kaivos.

Ehk� kuitenkin kriittisempi ongelma t�ss� on se,
ett� kaivos valitaan et�isyysarvion perusteella.
T�ll�in on mahdollista, ett� valitaan todellisuudessa kaikista kauimmainen kaivos
(jos kaivos on heti sein�n takana, mutta sein�n toiselle puolelle p��see vain kiert�m�ll� pitk� matka).

Jos Astarin toteuttaa sopivalla tavalla niin uskoisin
ett� sit� voisi k�ytt�� my�s useamman kaivoksen kohdalla saman kierroksen aikana
(jo laskettuja lyhimpi� reittej� ei tarvitse hyl�t�).

Prioriteetti vakio 3 olisi selke�mp�� saada esimerkiksi koodin alkuun `int capture_priority_constant = 3;`
(t�ll� hetkell� se ei aukea, ellei tunne tarpeeksi muuta koodia).

## HealDecision.java

Taas sama, ett� l�hin parannuspaikka valitaan et�isyysarvion perusteella,
eik� todellisen et�isyyden perusteella.
Jos kaikki p��t�ksentekij�t ajaisivat samaa Astar -oliota
n�m� tosiaan voisivat hy�dynt�� edellisi� ajoja.
T�m� vaatisi sit�, ett� Astar:n solmuja j�rjest�v� tietorakenne sallisi muutokset loppuet�isyydess�.

Hyv� ett� parantamisen prioriteetti ei ole vakio.

## KillDecision.java

Rivill� 25: `!h.getPos().equals(state.getMe().getPos())` on hieman monimutkainen tapa sanoa `h != me`.
Koodin luettavuuden kannalta kannattaisin jotain j�lkimm�isen kaltaista.

Ja jos ei viel� tullut selv�ksi, niin et�isyysarvio ei kaikissa tapauksissa ole paras vaihtoehto.

## DecisionMaker.java

Tyylikk��mp�� olisi, jos p��t�kset j�rjestett�isiin listan avulla.
My�s jos jatkossa verrattavia p��t�ksi� tulee enemm�n niin koodia ei en�� tarvitsisi muuttaa niin paljoa,
jos j�rjest�miseen k�ytet��n listaa (tai taulukkoa jne).

## AdvancedGameState.java

Vaikuttaa hyv�lt�, erityisesti kun luolasto pidet��n vakiona
ja ainoastaan muuttuvat osiot p�ivitet��n vuorojen v�liss� (pelaajat, kaivokset).

## MyHashMap.java

Entry -luokkahan on linkitetty lista.
Aiotko korvata sen toisenlaisella linkitetyll� listalla, kunhan saat sen tehty�?
Vai toteutatko yhteens� kaksi linkitetty� listaa?
(joista toinen on vain hashmapin sis�ll�)

K�yt�t ilmeisesti Javan omaa hashCode() funktiota kullekkin luokalle.
Omasta puolestani t�m� on ok (en tosin ole tarkistanut miten kyseinen hashCode() -funktio toimii)
mutta kannattanee kysy� onko t�m� ok osana omaa tietorakennetta kurssin puitteissa.
Hash funktion toteutus on kuitenkin n�hd�kseni melko iso osa hashmap -tietorakennetta.

En ole perehtynyt koodiin niin syv�llisesti, ett� osaisin sanoa riitt��k� 6-paikkainen hashmap
(tai onko vakiom��r� hyv�, sit�h�n voi manuaalisesti kasvattaa/pienent�� tarvittaessa ennen koodin ajamista).
Mutta jos vakiom��r� osoittautuu huonoksi, niin oletko harkinnut ett� tekisit hashmapista dynaamisen?
Eli taulukkoa kasvatettaisiin jos alkioita on vaikka yli 3*taulukon koko.
Hashmapin etu menetet��n jos linkitettyjen listojen pituus kasvaa suureksi.

## Lopuksi

Ohjelma n�ytt�� edistyneen hyvin ja paljon hyvi� ratkaisuja on my�s tehty.