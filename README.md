# Tira-labra

Tietorakenteiden harjoitustyö periodi 2, syksy 2017

Ohjelma on kirjoitettu C:llä. Projektin tarkempi dokumentaatio löytyy dokumentaatio -kansiosta.
Alla ohjeita C-koodin kääntämiseen.

src -kansio sisältää lähdekoodin.

tests -kansio testit. Testit tarvitsevat toimiakseen [Unity](http://www.throwtheswitch.org/unity/) -yksikkötestaus paketin C:lle.

xtc_reader -kansio sisältää testiversion xtc -tiedostojen lukemisesta (tarvitsee toimiakseen GROMACS kirjaston) 
ja yhden xtc aika-askeleen auki kirjoitettuna sekä vastavan pdb tiedoston verkon luomista varten.

Dokumentaatio -kansio sisältää viikkoraportit ja muun kurssin vaatimuksiin kuuluvat dokumentaatiot ohjelmasta ja ohjelman tekemisestä/testaamisesta jne.

Muut kansiot ovat vertaisarviointeja varten ladattuja github -projekteja.

## Yleisiä ohjeita C-koodin kääntämiseen ja ajamiseen

1. Tarvitset kääntäjän, ks. esimerkiksi [GCC, the GNU Compiler Collection](https://gcc.gnu.org/).

2. Itse olen käyttänyt [make](https://www.gnu.org/software/make/) -ohjelmaa kääntämisen helpottamiseen.
Make ohjelmaa käytetään ajamaan makefile -tiedostoja.

3. Linux käyttöjärjestelmässä gcc ja make löytyvät luultavasti valmiina.
Windows ympäristössä tarvitset jonkin ohjelman tjtn, joka sisältää edelliset ohjelmat.
Esimerkiksi (Cygwin)[https://www.cygwin.com/] projekti
(gcc ja make täytyy erikseen lisätä Cygwiniin, seuraa Cygwinin ohjeita).

4. Käytän projektissa valmiin ohjelman päätteenä .out -päätettä.
Näitä ohjelmia voi ajaa terminaalista (esimerkiksi Cygwin -terminaali) komennolla:
`./ohjelman_nimi.out` ja tarvittavat komentoriviparametrit kuten: `./ohjelman_nimi.out parametri_1 parametri_2`.
`./` osa tarkoittaa osoitetta kyseiseen ohjelmaan, kun oma sijaintimme on terminaalissa sama kuin ohjelman sijainti.
Jos ohjelma sijaitsee jossain muussa kuin nykyisessä kansiossa käytetään esimerkiksi komentoa:
`/polku/kyseiseen/tiedostoon/ohjelman_nimi.out parametrit` tai `../../suhteellinen/polku/ohjelman_nimi.out parametrit`.
Lisätietoa saat luultavasti kurssilta [Shell scripting](https://courses.helsinki.fi/fi/csm13501) (en itse ole käynyt sitä),
tai muulta Linux:n käyttöön opastavalta kurssilta.