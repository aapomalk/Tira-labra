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

klo 13:30-19:50

## Miten ohjelma on edistynyt?
Hyvin. Ohjelma toimii, paitsi että alueisiin jako estää välillä pidempien reittien löytymisen.

## Mitä opin tällä viikolla / tänään?

## Mikä jäi epäselväksi tai tuottanut vaikeuksia?

## Mitä teen seuraavaksi?
