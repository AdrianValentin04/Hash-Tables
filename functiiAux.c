// GHEORGHE Adrian-Valentin - 331CB

#include <stdlib.h>
#include <string.h>
#include "tema1.h"

///////////////////// Functii tabela hash /////////////////////////

// calculeaza codul hash pentru inserare
int codHash(void * element)
{
	char *word = (char*) element;
    
    if (word[0] <= 90) 
        return *word - 'A';
    else return *word - 'a';
	
}

// initializeaza tabela hash
TH* InitTH(size_t M, TFHash fh)
{
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h) {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TSizes *) calloc(M, sizeof(TSizes));
    if(!h->v) {
        printf("eroare alocare vector de pointeri TLG in hash\n");
        free(h);
        return NULL;
    }


    h->M = M;
    h->fh = fh;
    return h;
}

// distruge tabela hash
void DistrTH(TH** ah, TF elib_elem)
{
    TSizes * p, el, aux;

    /* parcurgere cu pointeri */
    for (p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++) {
        /* daca exista elemente corespunzatoare acestui hash
         * eliberam info din celula si apoi eliberam celula */
        for(el = *p; el != NULL; ) {
            aux = el;
            el = el->urm;
            elib_elem(aux->info);
            free(aux);
        }
    }
    free((*ah)->v);
    free(*ah);
    *ah = NULL;
}

/////////////////////// Functii comparare ////////////////////////

// compara lungimile cuvintelor
int CmpLen(void *el1, void *el2)
{
    LENGTHS *l1 = (LENGTHS*)el1;
    LENGTHS *l2 = (LENGTHS*)el2;

    if (l1->len >= l2->len)
        return 0;
    else return 1;
}

// testeaza daca lungimile sunt egale
int equalLen(void* el1, void*el2)
{
    LENGTHS *l1 = (LENGTHS*)el1;
    LENGTHS *l2 = (LENGTHS*)el2;

    if (l1->len == l2->len)
        return 1;
    else return 0;
}

// returneaza daca 2 cuvinte sunt identice sau nu
int cmpWord(void *el1, void *el2)
{
    WORDS *w1 = (WORDS*)el1;
    WORDS *w2 = (WORDS*)el2;

    if (!strcmp(w1->word, w2->word)) 
        return 1;
    else return 0;
}

// compara lexicografic 2 cuvinte
int cmpLex(void *el1, void *el2)
{
    WORDS *w1 = (WORDS*)el1;
    WORDS *w2 = (WORDS*)el2;

    if (strcmp(w1->word, w2->word) < 0) 
        return 1;
    else if (strcmp(w1->word, w2->word) > 0) 
        return 0;
}

// compara nr de aparitii a 2 cuvinte
int cmpApp(void *el1, void *el2)
{
    WORDS *w1 = (WORDS*)el1;
    WORDS *w2 = (WORDS*)el2;

    if (w1->apps > w2->apps)
        return 1;
    else if (w1->apps < w2->apps)
        return 0;
}

// returneaza daca nr de aparitii al celor 2 cuvinte e egal
int appEqual(void *el, int in)
{
    WORDS *w = (WORDS*)el;
    if (w->apps <= in)
        return 1;
    else return 0;
}

// compara daca lungimea este egala cu un input
int cmpInt(void *el, int in)
{
    LENGTHS *l = (LENGTHS*)el;
    if (l->len == in)
        return 1;
    else return 0;
}

// returneaza faca un caracter este litera sau '-'
int IsLetter(void *el)
{
    char *w = (char*)el;

    if ((*w >= 65 && *w <= 90) ||
         (*w >= 97 && *w <= 122)) 
        return 1;
    else return 0;
}

// incrementeaza nr de aparitii
void* incApp(void *el)
{
    WORDS *w = (WORDS*)el;
    w->apps++;
    return (void*)w;
}

//////////////////////// Comanda INSERT ///////////////////////////

// comanda de inserare
int InsertCommand(TH *hash, void *elem, TFCmp cmp, TFCmp equal)
{
    int ok = 0;
    int ok2 = 0;
    WORDS *w = (WORDS*)malloc(sizeof(WORDS));
    if (!w) {
        return 0;
    }

    LENGTHS *l = (LENGTHS*)malloc(sizeof(LENGTHS));
    if (!l) {
        free(w);
        return 0;
    }

    w->word = malloc(MAX_WORD_LEN * sizeof(char));
    if (!w->word) {
        free(l);
        free(w);
        return 0;
    }

    strcpy(w->word, (char*)elem);
    w->apps = 1;

    l->len = strlen(w->word);

    TSizes sizeCell = malloc(sizeof(TCellSizes));
    if (!sizeCell) {
        free(l);
        free(w->word);
        free(w);
        return 0;
    }

    TWords wordCell = malloc(sizeof(TCellWords));
    if (!wordCell) {
        return 0;
    }

    // calculam codul hash al cuvantului de inserat
    int cod = hash->fh(w->word);

    sizeCell->info = (LENGTHS*)l;
    sizeCell->urm = NULL;

    wordCell->info = (WORDS*)w;
    wordCell->urm = NULL;

    // inserare pe prima pozitie in lista de lungimi
    if (!hash->v[cod]) {

        sizeCell->words = wordCell;
        hash->v[cod] = sizeCell;
        
    } else {
        TCellSizes *ant, *aux;

        // inserare in lista de lungimi
        for (aux = hash->v[cod], ant = NULL; aux!= NULL; ant = aux, aux = aux->urm) {
            if (equal(sizeCell->info, aux->info)) {
                ok = 1;
                break;
            }
            
            // verificam daca exista deja lungimea cuvantului de inserat
            if (cmp(sizeCell->info, aux->info) == 1) {
                break;
            } 
        }

        // inseram lungimea 
        if (ok == 0) {
            if (!aux) {
            ant->urm = sizeCell;
            }
        
            sizeCell->urm = aux;
            if (!ant)
                hash->v[cod] = sizeCell;
            else 
                ant->urm = sizeCell; 
            
            sizeCell->words = wordCell;

        } else {
            
            // trebuie inserat cuvantul in sublista
            if (!aux->words) {
                aux->words = wordCell;
            } else {
                TCellWords *wordAux, *wordAnt;

                // cautam daca vcuvantul exista deja in sublista
                for (wordAux = aux->words, wordAnt = NULL; wordAux != NULL; wordAnt = wordAux, wordAux = wordAux->urm) {
                    
                    if (cmpWord(wordCell->info, wordAux->info) == 1) {
                        ok2 = 1;
                        break;
                    }

                }

                // daca cuvantul exista deja, incrementam nr de aparitii
                if (ok2 == 1) {
                   
                    wordAux->info = incApp(wordAux->info);
                    ok2 = 0;

                } else {

                    // inseram cuvantul
                    wordCell->urm = wordAux;
                    
                    if (!wordAnt) 
                        aux->words = wordCell;
                    else
                        wordAnt->urm = wordCell;

                }
            }
            ok = 0;
        }
    }
    return 1;
}

//////////////////////////// Comanda PRINT ////////////////////////

// afiseaza lungimea cuvintelor din sublista
void AfiLen(void *el)
{
    LENGTHS* l = (LENGTHS*)el;

    printf("%d:", l->len);
}

// afiseaza un cuvant din sublista
void AfiOneWord(void *el)
{
    WORDS *w = (WORDS*)el;

    printf("%s/%d, ", w->word, w->apps);
}

// afiseaza ultimul cuvand din sublista
void AfiLastWord(void *el)
{
    WORDS *w = (WORDS*)el;

    printf("%s/%d", w->word, w->apps);
}

// afiseaza toata sublista
void AfiAllWords(void *el)
{
    TWords words = (TWords) el;
    TWords aux, last;

    for (aux = words; aux->urm != NULL; aux = aux->urm) {
        AfiOneWord(aux->info);
    }
    AfiLastWord(aux->info);
}

// printeaza cuvintele care au un anumit nr de aparitii
void AfiWordsByApps(void *el, int nrApps)
{
    TWords words = (TWords) el;
    TWords aux;

    // printam cuvintele cu maxim nrApps aparitii
    for (aux = words; aux->urm != NULL; aux = aux->urm) {
        
        if (appEqual(aux->info, nrApps)) 
            AfiOneWord(aux->info);
        
    }
    
    // printam ultimul cuvant - afisare separata ca sa scapam de 
    // spatiul si virgula de la final
    if (appEqual(aux->info, nrApps))
        AfiLastWord(aux->info);
}

// verifica daca exista cuvinte care au un anumit nr de aparitii
// intr-o sublista
int CheckIfExists(void *el, int nrApps)
{
    TSizes list = (TSizes)el;
    TSizes aux;
    
    for (aux = list; aux != NULL; aux = aux->urm) {
        
        // verificam daca exista cel putin un element cu nrApps aparitii
        // care incep cu o anumita litera
        if (FindWordWithApp(aux->words, nrApps) == 1) 
            return 1;

    }
    
    return 0;
}

// functie auxiliara pentru cea de mai sus
int FindWordWithApp(void *el, int nrApps)
{
    TWords words = (TWords) el;
    TWords aux;
    
    for (aux = words; aux != NULL; aux = aux->urm) {
       
        if (appEqual(aux->info, nrApps))
            return 1;
    }

    return 0;
}

// printeazza cuvintele ce incep cu o anumita litera si care au o anumita
// lungime
void PrintTwoArg(TH *hash, int hashCode, int len)
{
    TSizes p, el;
    p = hash->v[hashCode];
   
    if (p) {
       
        for(el = p; el != NULL; el = el->urm) {
            
            if (cmpInt(el->info, len) == 1) {
                
                printf("(");
                AfiLen(el->info);
                AfiAllWords(el->words);
                printf(")");

            } 
        }
    }
}

// printeaza elementele ce au un anumit nr de aparitii
void PrintOneArg(TH *hash, int nrApps)
{
    TSizes p, el;
    int i;
    
    for(i = 0; i < hash->M; i++) {
        p = hash->v[i];
        
        if(p) {
            
            if (CheckIfExists(p, nrApps)) {
                printf("pos%d: ",i);
                
                for(el = p; el != NULL; el = el->urm){
                    
                    if (FindWordWithApp(el->words, nrApps)) {
                        
                        printf("(");
                        AfiLen(el->info);
                        printf(" ");
                        AfiWordsByApps(el->words, nrApps);
                        printf(")");

                    }       
                } 
                printf("\n");
            }
        }
    }
}

// afiseaza toata tabela hash
void PrintZeroArg(TH *hash, TF printLen, TF printWord)
{
    TSizes p, el;
    int i;
   
    for(i = 0; i < hash->M; i++) {
        p = hash->v[i];
        
        if(p) {
            printf("pos %d: ",i);
            
            for(el = p; el != NULL; el = el->urm){
                
                printf("(");
                printLen(el->info);
                printWord(el->words);
                printf(")");

            } 

            printf("\n");
        }
    }
}

// functie de sortare a sublistelor de cuvinte 
void sortAll(TH *hash)
{
    TSizes aux;
    
    for (int i = 0; i <= hash->M; i++) {
        
        if (hash->v[i]) {
            
            for (aux = hash->v[i]; aux != NULL; aux = aux->urm) {
               
                // sortam intai lexicografic, apoi dupa aparitii
                sortWords(aux->words, cmpLex);
                sortWords(aux->words, cmpApp);
            
            }
        }
    }
}

// functie auxiliara pentru functia de mai sus
void sortWords(void *elem, TFCmp cmp)
{
    TWords words = (TWords)elem;
    TWords ptr1, lptr = NULL;
    int i, swapped;

    do 
    {
        swapped = 0;

        for (ptr1 = words; ptr1->urm != lptr;  ptr1 = ptr1->urm) {
            
            if (!cmp(ptr1->info, ptr1->urm->info)) {
                swapWords(ptr1, ptr1->urm);
                swapped = 1;
            }

        }

        lptr = ptr1;

    } while (swapped);
    
}

// interschimba 2 noduri
void swapWords(TWords a, TWords b)
{
    WORDS *tmp = a->info;
    a->info = b->info;
    b->info = tmp;
}