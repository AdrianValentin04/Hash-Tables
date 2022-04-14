// GHEORGHE Adrian-Valentin - 331CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1000
#define MAX_WORD_LEN 30

typedef int (*TFHash)(void*);
typedef void (*TF)(void*);
typedef int (*TFCmp)(void*, void*); 

typedef struct words {
    char* word;
    int apps;
} WORDS;

typedef struct lengths {
    int len;
} LENGTHS;

typedef struct sizes
{
    void *info;         
    struct sizes *urm;    
    struct wordsL *words;  
} TCellSizes, *TSizes;       

typedef struct wordsL
{
    void* info;  
    struct wordsL *urm;
} TCellWords, *TWords;

typedef struct
{
    size_t M;
    TFHash fh;
    TSizes *v;
} TH;

int codHash(void * element);
TH* InitTH(size_t M, TFHash fh);
void DistrTH(TH** ah, TF elib_elem);
int CmpLen(void *el1, void *el2);
int equalLen(void* el1, void*el2);
int cmpWord(void *el1, void *el2);
int cmpLex(void *el1, void *el2);
int cmpApp(void *el1, void *el2);
int appEqual(void *el, int in);
int cmpInt(void *el, int in);
int IsLetter(void *el);
void* incApp(void *el);
int InsertCommand(TH *hash, void *elem, TFCmp cmp, TFCmp equal);
void AfiLen(void *el);
void AfiOneWord(void *el);
void AfiLastWord(void *el);
void AfiAllWords(void *el);
void AfiWordsByApps(void *el, int nrApps);
int CheckIfExists(void *el, int nrApps);
int FindWordWithApp(void *el, int nrApps);
void PrintOneArg(TH *hash, int nrApps);
void PrintTwoArg(TH *hash, int hashCode, int len);
void PrintZeroArg(TH *hash, TF printLen, TF printWord);
void sortAll(TH *hash);
void sortWords(void *elem, TFCmp cmp);
void swapWords(TWords a, TWords b);