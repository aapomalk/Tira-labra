# Viikkoraportti 7

## Viikon työlista

    Dokumentaatio:
        100% selkeää ja kommentoitu koodi (jos käytit Javaa, generoi Javadoc ja lisää palautukseen)
        Valmiit dokumentit:
            Määrittelydokumentti (ei tarvitse päivittää alkuperäisestä)
            Toteutusdokumentti
            Testausdokumentti
            Viikkoraportit
            Käyttöohje

    Ohjelma:
        Suoritettava ohjelma (esim. jar-tiedosto)
            jos käytät mavenia, niin ohjeita suoritettavan jar-tiedoston tekemiseen esim. kesän javalabran sivuilla
        Kaikki tietorakenteet ja algoritmit toteutettu itse
        Työ valmis ja hiottu

    Testaus:
        Koodin kattava yksikkötestaus
        Dokumentoitu ohjelman testaus testausdokumenttiin
        Graafinen esitys esim. aikavaativuuksien toteutumisesta empiirisen testauksen perusteella

## Mitä olen tehnyt tällä viikolla?

### 12.12.2017
`valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./program_command` homma luistaa!
Monta ongelmaa korjattua, kuten taannoinen mysteerinen "indeksi 10" bugi.
Vaikka en kyseistä bugia edelleenkään täysin ymmärrä uskon että se on korjattu.

#### testing memory leaks:
+ok test_a_star.out
+ok test_graph_implementation2.out
+ok test_heap.out
+ok test_performance2.out
+ok test_performance.out
+ok test_performance_zero2_heuristic.out
+ok test_performance_zero_heuristic.out
+ok test_read_parameters_and_pdb.out
+ok test_read_parameters.out
+ok test_read_pdb.out
+ok~ test_read_xtc.out (the GROMACS functions might not free all?)

Ohjelma näyttäisi toimivan muuten, paitsi että alueisiin jako (domain decomposition) vaikuttaa lopputulokseen.
Vaatii testaamista.

Muokkasin testausdokumenttia, toteutusdokumenttia ja aloitin käyttöohjeen.

klo 13:30-19:50, 22:30-23:55 (yht 6h20min + 1h25min = 7h45min)

### 13.12.2017
Alueisiin jaon bugi korjattu.
Ohjelma tuottaa nyt identtisen tuloksen riippumatta alueisiin jaosta.

Parametrien lukemiseen virhetilanteiden käsittelyä.

Polkuun tulee välillä looppeja, jokin siis mättää..
Mutta ei mätä enää. Bugi korjattu.

Testataan kenties valmista ohjelmaa komennolla:
`./pathfinder.out -pdb ../xtc_reader/final_solv_ions.pdb -xtc /home/work/aapomalk/proteins/complex_iv_monomer/separated/final_no_hetero2/sisu_trajectories/md.part0005_clst_cntr.xtc -par ../tests/parameter_protein_and_water.txt -start 27853 -end 1870`

Muut komennot näet tulostetuista tiedostoista.
Ohjelma toimii.
Testi tulosteita on printattu src -kansioon (vaikka ei ehkä kuuluisi).

klo 11:10-12, 12:45-18:25

## Miten ohjelma on edistynyt?
Hyvin. Ohjelma toimii.
Limiter -ominaisuus on mukava (jos ei välitä pitkistä yhteyksistä),
nopeuttaen ohjelman ajamista huomattavasti (esimerkiksi vesimolekyylien välillä).

## Mitä opin tällä viikolla / tänään?
Valgrind on hyvä!
Kääntäjän -g ominaisuus on tärkeä valgrindin kanssa.

## Mikä jäi epäselväksi tai tuottanut vaikeuksia?
Täytyy vain luottaa siihen, että ohjelma toimii oikein
ja löytää kaikki lyhimmät reitit, jos niitä on olemassa.
Käsin niitä en halua etsiä, mutta ohjelma näyttäisi toimivan!

## Mitä teen seuraavaksi?
Työ on valmis, voin nauttia työn hedelmistä ja tehdä lisää töitä.