# Testausdokumentti

## Mit� on testattu, miten t�m� tehtiin

Unity -C ohjelmointikielen testaus pakettia k�ytt�en ohjelman tietorakenteita ja algoritmeja on testattu.

Testeiss� on testattu olennaisia ja v�hemm�n olennaisia osia ohjelmasta. Yksi testien hy�ty on ollut tietorakenteiden ja algoritmien k�ytt�misen harjoittelu, mik� on hy�dytt�nyt ohjelmointia suuresti. T�m�n yhten� piirteen� on ollut ylip��t�ns� ohjelman k��ntymisen testaaminen. Varsinaisten algoritmien kohdalla my�s v��r�t tulokset on l�ydetty testien avulla (tulokset jotka eiv�t tuota segmentation fault:a tai ohjelman muuta keskeytyst�).

### test_graph_implementation.c

Sis�lt�� t�ll� hetkell� 12 eri testitapausta, joista kukin sis�lt�� useampia "TEST_ASSERT" (ja muita) -testej�.

Esimerkiksi "test_heuristic" testaa, ett� ohjelma laskee todellisen et�isyyden oikein. "test_vertex_graph" testaa VERTEX ja GRAPH rakenteiden yhteensopivuutta ja yhteistoimintaa. "test_insert_many_vertex_to_graph" oli my�s t�rke� testi. "test_periodicity" vastaa "test_heuristic" testi�, mutta periodisten reunaehtojen l�sn�ollessa.

### test_heap.c

A* -algoritmin j�rjest�misalgoritmiksi valikoitui kekoj�rjest�minen. Keolle on omistettu oma testaus ohjelma, joka sis�lt�� kuusi erilaista testitapausta, joista kukin sis�lt�� taas useampia testej�.

"test_insert_components" testaa keon kasvattamista ja "test_remove_first" testaa ett� keko pysyy kekona my�s poistettaessa alkioita. "test_parent_child" testaa varmuuden vuoksi, ett� vanhempi- ja lapsi- alkioiden indeksit l�ytyv�t oikein. Lis�ksi testataan alkioiden vaihtamista kesken��n (test_swap; sotkee keon v��rin k�ytettyn�) ja "test_heap_new_value" testaa alkion arvon vaihtamista (lopulta t�t� ei k�ytetty itse A* -algoritmissa).

### test_a_star.c

Kun keko ja verkko on todettu toimiviksi on hyv� testata itse algoritmia. Ohjelma sis�lt�� vain kolme testitapausta, mutta n�m� ovat sit�kin t�rke�mpi�. Tai oikeastaan vain yksi n�ist� on erityisen oleellinen.

T�m� oleellinen on "test_path_finding". Siin� ensin luodaan verkon testaamisen yhteydest� tuttu rakennelma, jossa jokaisella solmulla on yhteys kuuteen ymp�r�iv��n solmuun. Ensiksi kun solmujen enimm�iset�isyys asetetaan liian pieneksi ei kohde solmuun p��st�, mutta kun enimm�iset�isyytt� kasvatetaan l�ytyy kohdesolmu. My�s kun kohdesolmua vaihdetaan niin algoritmi l�yt�� sen.

## Mink�laisilla sy�tteill� testaus tehtiin (vertailupainotteisissa t�iss� t�rke��)

## Miten testit voidaan toistaa

Testit voidaan toistaa ensin k��nt�m�ll� testit (makefile -tiedosto test -kansiossa antaa esimerkkikomentoja, mutta helpompaa on k�ytt�� komentoa "make"; makefile my�s ajaa valitun testin). K��nt�misen j�lkeen ohjelman voi ajaa komentorivilt� esimerkiksi komennolla ./test_a_star.out

## Ohjelman toiminnan empiirisen testauksen tulosten esitt�minen graafisessa muodossa

## Testaus on ideaalitapauksessa suoritettava ohjelma. T�ll�in testi on helposti toistettavissa, mik� helpottaa toteutuksen tekoa jo varhaisessa vaiheessa. Javalla tehdyiss� t�iss� on eritt�in suositeltavaa k�ytt�� testaukseen JUnitia