COLTAN CRISTIAN, C 111 D, TEMA 1, PROBLEMA 5, TANKS
/*
HOW TO PLAY ==============================================

FIECARE TANC ARE UN COMBUSTIBIL.
TANCUL VERDE ARE 7, IAR CEL ROSU 5
SE MISCA STANGA DE PE A, RESPECTIV DREAPTA DE PE D.
DUPA CE COMBUSTIBILUL S A TERMINAT PUTETI SETA PUTEREA.
(SE AFISEAZA PE ECRAN "OUT OF FUEL")

PENTRU SETAREA PUTERII APASATI P(CRESTE) O(SCADE)

APASATI SPACE DACA ATI AJUNS LA VALOAREA DORITA.

PENTRU SETAREA UNGHIULUI APASATI U(CRESTE) Y(SCADE)
AVETI DE ALES INTRE 4 UNGHIURI (0,30,60,90)
0 trage orizontal(bara este goala), 1 trage la 30*(bara are 1 bit), 
2 trage la 60*(bara are 2 biti), iar 90 vertical(bara este plina, 3 biti)

APASATI SPACE DACA ATI AJUNS LA VALOAREA DORITA

PENTRU SETAREA MUNITIEI APASATI M(CRESTE) N(SCADE)
BARA GOALA = PROIECTIL MIC (DAMAGE REDUS (DACA NIMERESTE TANCUL -40))
BARA 1 CARACTER = PROIECTIL MARE (DAMAGE RIDICAT (DACA NIMERESTE TANCUL -80))
BARA 2 CARACTERE = ATAC AERIAN (DAMAGE FOARTE REDUS (DACA NIMERESTE -20))

APASATI SPACE DACA ATI AJUNS LA VALOARE DORITA PENTRU A TRAGE.
ATENTIE!
DACA UTILIZATI ATACUL AERIAN, APASATI ENTER PENTRU A TRAGE!
(INTRODUCETI COLOANA PE CARE SA FIE LANSATA RACHETA)

ATENTIE!
PUTEREA TREBUIE SA FIE MINIM 1 (BARA DE PUTERE NU POATE FI GOALA), 
ALTFEL TANCUL EXPLODEAZA.
*/

Greutati intampinate:
- colorarea tancurilor si a hartii
- miscarea tancurilor
- miscarea proiectilului

Am inceput prin a desena si colora harta.
Pentru teren am folosit o distributie gaussiana, printand pamantul cu galben iar cerul cu albastru.
Am initializat si colorat tancurile.
Am folosit 2 matrici pentru fiecare tank (o matrice de coordonate x si una de coordonate y)
Pentru miscare am luat o functie de keyboard hit impreuna cu 2 miscari ( stanga si dreapta), tancul poate urca si coborî relieful
Pentru miscarea la dreapta ca sa urce pe teren mai inalt am verificat daca in fata senilei exista pamant.
Pentru miscarea la stanga ca sa coboare am verificat daca in spatele senilei este liber.
Dupa ce am implementat miscarile am trecut la proiectil.
Am gandit miscarea proiectilului pe 4 unghiuri diferite(0,30,60,90)
0 - miscare rectilinie
30 - miscare in parabola
60 - miscare in parabola
90 - miscare pe verticala, unde tancul isi poate lua damage singur
Apoi am trecut la tipurile de proiectil.
Am realizat 3 tipuri de proiectil
- proiectil mic ( damage de -40) (bara de munitie este goala)
- proiectil mare ( damage de -80) (bara de munitie are 1 bit)
- atac aerian (damage -20) (bara de munitie este plina, 2 biti)
Daca folositi proiectil apasati space pentru a trage, altfel dupa ce setati coloana pe care sa coboare racheta de la atacul aerian apasati enter.
Jocul se incheie atunci cand un tanc ramane fara puncte de viata.


