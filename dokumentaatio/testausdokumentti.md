# Testausdokumentti

## Mit� on testattu, miten t�m� tehtiin

Unity -C ohjelmointikielen testaus pakettia k�ytt�en ohjelman tietorakenteita ja algoritmeja on testattu.

Testeiss� on testattu olennaisia ja v�hemm�n olennaisia osia ohjelmasta. Yksi testien hy�ty on ollut tietorakenteiden ja algoritmien k�ytt�misen harjoittelu, mik� on hy�dytt�nyt ohjelmointia suuresti. T�m�n yhten� piirteen� on ollut ylip��t�ns� ohjelman k��ntymisen testaaminen. Varsinaisten algoritmien kohdalla my�s v��r�t tulokset on l�ydetty testien avulla (tulokset jotka eiv�t tuota segmentation fault:a tai ohjelman muuta keskeytyst�).

My�s ohjelman tehokkuutta testattiin suhteessa solmujen m��r��n ja alueiden (domain) m��r��n.

### test_graph_implementation.c

Sis�lt�� t�ll� hetkell� 12 eri testitapausta, joista kukin sis�lt�� useampia "TEST_ASSERT" (ja muita) -testej�.

Esimerkiksi "test_heuristic" testaa, ett� ohjelma laskee todellisen et�isyyden oikein. "test_vertex_graph" testaa VERTEX ja GRAPH rakenteiden yhteensopivuutta ja yhteistoimintaa. "test_insert_many_vertex_to_graph" oli my�s t�rke� testi. "test_periodicity" vastaa "test_heuristic" testi�, mutta periodisten reunaehtojen l�sn�ollessa.

### test_heap.c

A* -algoritmin j�rjest�misalgoritmiksi valikoitui kekoj�rjest�minen. Keolle on omistettu oma testaus ohjelma, joka sis�lt�� kuusi erilaista testitapausta, joista kukin sis�lt�� taas useampia testej�.

"test_insert_components" testaa keon kasvattamista ja "test_remove_first" testaa ett� keko pysyy kekona my�s poistettaessa alkioita. "test_parent_child" testaa varmuuden vuoksi, ett� vanhempi- ja lapsi- alkioiden indeksit l�ytyv�t oikein. Lis�ksi testataan alkioiden vaihtamista kesken��n (test_swap; sotkee keon v��rin k�ytettyn�) ja "test_heap_new_value" testaa alkion arvon vaihtamista (lopulta t�t� ei k�ytetty itse A* -algoritmissa).

### test_a_star.c

Kun keko ja verkko on todettu toimiviksi on hyv� testata itse algoritmia. Ohjelma sis�lt�� vain kolme testitapausta, mutta n�m� ovat sit�kin t�rke�mpi�. Tai oikeastaan vain yksi n�ist� on erityisen oleellinen.

T�m� oleellinen on "test_path_finding". Siin� ensin luodaan verkon testaamisen yhteydest� tuttu rakennelma, jossa jokaisella solmulla on yhteys kuuteen ymp�r�iv��n solmuun. Ensiksi kun solmujen enimm�iset�isyys asetetaan liian pieneksi ei kohde solmuun p��st�, mutta kun enimm�iset�isyytt� kasvatetaan l�ytyy kohdesolmu. My�s kun kohdesolmua vaihdetaan niin algoritmi l�yt�� sen.

### test_performance.c

T�t� testi� varten k��nt�j� teki kaksi versiota: A* algoritmin ja Djikstra algoritmin.
N�iden v�lill� on n�ht�viss� selv� ero (ks. performance_test_output.txt, *2_output.txt).
My�s alueiden (domain) lukum��r�ll� oli hyvin suuri vaikutus.
Jos alueita oli yht� paljon kuin solmuja niin suoritusajat nousivat maltillisesti sy�tteen mukana,
mutta jos alueita oli vain yksi niin suoritusajan nousu oli hyvin jyrkk�� (vertaa performance_test_output.txt ja performance_test_output_one_domain.txt tiedostoja jne).

## Mink�laisilla sy�tteill� testaus tehtiin (vertailupainotteisissa t�iss� t�rke��)

## Miten testit voidaan toistaa

Testit voidaan toistaa ensin k��nt�m�ll� testit (makefile -tiedosto test -kansiossa antaa esimerkkikomentoja, mutta helpompaa on k�ytt�� komentoa "make"; makefile my�s ajaa valitun testin). K��nt�misen j�lkeen ohjelman voi ajaa komentorivilt� esimerkiksi komennolla ./test_a_star.out

Suoritusaikoja varten komentoriville (ainakin Linux k�ytt�j�rjestelmiss�) voi kirjoittaa seuraavanlaisen komennon:

`for i in {10..30..5} ; do ./test_performance2.out $i 1; ./test_performance_zero2_heuristic.out $i 1 ; done > performance_test2_output_one_domain.txt`

## Ohjelman toiminnan empiirisen testauksen tulosten esitt�minen graafisessa muodossa

Seuraavissa kuvaajissa (y-akselilla) aika on koneen oma aikayksikk�, l�hinn� vertailua varten.

X-akselilla on verkon koko.
Tarkemmin sanottuna kuution sivun pituus.
Solmuja verkossa on siis `x^3`.

Verkko on luotu siten, ett� vain vierekk�isiin solmuihin on yhteys
(ei vinottain) yl�s, alas ja nelj�lle sivulle.
Jokaisella solmulla on siis kuusi naapuria.

Yksi alue tarkoittaa sit�, ett� l�yt��kseen naapurinsa solmun t�ytyy
selvitt�� et�isyytens� jokaiseen verkon solmuun (ja l�hell�oleviin my�s kulma).
Monta aluetta tarkoittaa sit�, ett� l�himm�t solmut l�ytyv�t joko samasta tai l�heisist� alueista nopeuttaen merkitt�v�sti polun l�ytymist�.

### Helppo tapaus
Helpossa tapauksessa lyhin reitti l�ytyy etenem�ll� vain yhteen suuntaan.

#### Monta aluetta
![alt text](../tests/easy_case_many_domains.png "monta aluetta, helppo")
#### Vain yksi alue
![alt text](../tests/easy_case_one_domain.png "yksi alue, helppo")

### Vaikea tapaus
Vaikeassa tapauksessa lyhin reitti kulkisi vinottain,
mutta koska se ei ole mahdollista t�ytyy k��nty� v�hint��n kolme kerta.
Vaikean tapauksesta tekee se, ett� suunnilleen lyhimpi� reittej� on hyvin paljon.
Lis�ksi kun solmujen sijainteja on hieman satunnaistettu ei lyhimm�n polun l�ytyminen todellakaan ole helppoa.

#### Monta aluetta
![alt text](../tests/tough_case_many_domains.png "monta aluetta, vaikea")
#### Vain yksi alue
![alt text](../tests/tough_case_one_domain.png "yksi alue, vaikea")

## Tunnettuja ongelmia:

