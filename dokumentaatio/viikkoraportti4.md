# Viikkopalaute 4

## Viikon työlista

+Ensimmäinen vertaisarviointi tehtynä (Löydät linkin katselmoitavaan repoon tiralabtoolista)

+Dokumentaatio: Koodi kommentoitua. Aloitettu kirjoittamaan toteutus- ja testausdokumentaatiota.

+Viikkoraportti numero 4

+Ohjelma: Ohjelman ydintoiminta valmis. Omia tietorakenteita aloitettu.

+Testaus: Koodin kattava yksikkötestaus. Aloitettu suorituskyky- tai muu aiheeseen sopiva testaus (kirjoita näistä testausdokumenttiin)

## Mitä olen tehnyt tällä viikolla?

### 21.11.
Dokumentaation valmistelua (raportti 4, toteutus- ja testausdokumentaatio). A* algoritmia muokattu, koska ensimmäisenä löydetty yhteys ei välttämättä ole lyhin. Testaus vielä kesken.

Algoritmi on nyt toimiva.

Dokumentaatiota lisätty. Suorituskykytestaus pitäisi aloittaa mahdollisimman pian.

klo 10-11:15 21:15-22 23-24

### 22.11.
Vertaisarviointia, josta tosin ei tunnu tulevan mitään. Neuraali-verkot ovat toistaiseksi liian vaikea ymmärtää, ja JavaScript ei helpota asiaa.

21:30-22, 22:30-0:30

### 23.11.
Uuteen vertaisarviointi -projektiin tutustumista.

10:40-11:15

### 24.11.
Suorituskyky testejä. A* ja Dijkstra algoritmeissa ei näytä olevan merkittävää eroa.
Lisäksi suurilla syötteillä tulee segmentation fault, ja välillä myös muutenkin.

klo 7:30-10

### 25.11.
Tein uuden "luokan" allocation, tarkoituksena virheellisten allokaatioitten käsitteleminen.

10-11

### 26.11.
Mysteerinen segmentation fault jatkuu, mutta se ei näytä olevan täysin riippuvainen verkon koosta.

Mysteeri on ratkaistu osittain: kyseessä on indeksi numero 10. Syytä en ymmärrä, mutta jos kyseisen indeksin ohittaa niin segfaultia ei tule...

Linuxilla kyseistä ongelmaa indeksin 10 kanssa ei ole, ilmeisesti jokin virhe Cygwin gcc kääntäjässä.

Suoritustestejä. Lopultakin sain Dijkstra -version toimimaan.

14-16:15, 18:45-23:45

## Miten ohjelma on edistynyt?

Ohjelma on odistynyt jotenkin.
Enimmäkseen kuitenkin oheisasioita tehty (kuten suoritustestejä, debuggaamista ja vertaisarviointia).

## Mitä opin tällä viikolla / tänään?

Opin että kääntäjällä voi olla äärimmäisen suuri merkitys.
Cygwin kääntäjä tuottaa erikoisia virheitä, joita Linux:n vastaava ei tuota.

## Mikä jäi epäselväksi tai tuottanut vaikeuksia?

Kääntäjän ongelmat ovat hämäriä.
Lisäksi kesti pitkään ennen kuin sain toteutettua heuristic_zero muokkauksen, 
joka siis muuttaa A* algoritmin Dijkstra algoritmiksi
(aluksi muutin koko funktion palauttamaan nollan, lopulta tajusin että muutos täytyy tehdä ainoastaan a_star.c tiedostossa).

## Mitä teen seuraavaksi?

Seuraavaksi annan vertaispalautetta ja katson mitä seuraavalla viikolla täytyy tehdä.
Ohjelman suhteen yritän edetä tiedostojen lukemiseen ja tämän jälkeen pyrin viimeistelemään ohjelman.
Lopuksi testaan ohjelmaa oikeisiin xtc -tiedostoihin.