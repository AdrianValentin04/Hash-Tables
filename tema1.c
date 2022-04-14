// GHEORGHE Adrian-Valentin - 331CB

#include "tema1.h"

int main(int argc, char *argv[])
{
    int sizeHash = 'Z' - 'A' + 1;
    TH *hash = (TH *)InitTH(sizeHash, codHash);
    FILE *inputfile = fopen(argv[1], "r");

    char *token, line[MAX_LINE_LEN], *sentence;
    char *word;
    int length = 0;

    // consola de comenzi
    while (fgets(line, MAX_LINE_LEN, inputfile)) {
        token = strtok(line, " \n\0");

        if (!token) {
            break;
        } else if (!strcmp(token, "insert")) {
           
            // comanda de insert
            token = strtok(NULL, " ,\n\0");
            
            // luam doar cuvintele care contin litere si '-'
            while(token) {
                word = strdup(token);
                if (strlen(word) >= 3 && IsLetter(word) == 1) {
                    
                    length = strlen(word);
                    InsertCommand(hash, word, CmpLen, equalLen);
                
                }
               
                token = strtok(NULL, " ,.\n\0");
            }

        } else if (!strcmp(token, "print")) {
            
            // comanda de print
            int ok = 0;
            sortAll(hash);

            // printeaza toate elementele
            token = strtok(NULL, " ,\n\0");
            if (!token) {
                PrintZeroArg(hash, AfiLen, AfiAllWords);
                ok = 1;
            }

            if (ok == 0) {
                
                // printeaza doar elementele care au un anumit nr de aparitii
                char *chr = strdup(token);
                token = strtok(NULL, " ,\n\0");
                
                if (!token) {

                    int input = atoi(chr);
                    PrintOneArg(hash, input);
                    ok = 1;

                }

                if(ok == 0) {

                    // printeaza cuvintele care incep cu anumita litera si
                    // au o anumita lungime
                    char *chr2 = strdup(token);
                    int in = atoi(chr2);
                    int code = hash->fh(chr);
                    
                    PrintTwoArg(hash, code, in);
                    
                    // daca se cer cuvinte cu lungime < 3 atunci nu se afiseaza
                    // si \n de la finalul afisarii
                    if (in >= 3)
                        printf("\n");
                }
            }
        }
    }

    fclose(inputfile);
    DistrTH(&hash, free);

    return 0;
}