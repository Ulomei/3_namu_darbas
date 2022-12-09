//Varianto nr. - 11
//Salyga - Parasyti funkcija, kuri atspausdina kiekvienos eilutes ilgiausia zodi.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define WORD_SIZE 255
#define FILE_NAME_LENGTH 128

    #define MAIN_LINE "Programa atspausdins kiekvienos eilutes ilgiausia zodi is duomenu failo i rezultatu faila.\n\
Duomenu failas irasomas programos parametruose pirmas, o rezultatu failas antras.\n\
Viena eile skaitoma iki perkelimo i kita eilute zenklo. \n\
Zodziai ilgesni nei 255 zenkai nebus skaitomi.\n\
------------------------------------------------------\n\n"
    #define MAIN_ARGC_1 "Parametruose duomenu failas buvo nepateiktas. Suveskite ji i ekrana. Duomenu failas privalo pasibaigti \".txt\" ir nebuti ilgesnis nei 128 zenklai.\n"
    #define DO_VALIDATION_NR1_LINE "Ivestis netinkama! Pavadinimas per ilgas. Iveskite failo pavadinima eskite darkart.\n"
    #define DO_VALIDATION_NR2_LINE "Netinkama ivestis! Neresta \".txt\".\n"
    #define GET_DATA_FILE_NAME_NULL "Nepavyko nuskaityti duomenu failo. Programa baigia darba"
    #define MAIN_NUM_0 "Nepavyko nuskiatyti failo"
    #define MAIN_NOT_FOPEN "Nepavyko atidaryti failo"

typedef struct Stack
{
    char *array;
    int size;
} Stack;

void initStack(Stack *stack) // sukuria struktura
{
    stack -> array = NULL;
    stack -> size = 0;
}

void printStack(Stack *stack, FILE *output) // atspausdina masyva
{
    if(stack -> array != NULL)
    {
        fwrite (stack -> array, (stack -> size), sizeof (char), output);
    }
}

void push(Stack *stack, char *word ) // ideda zodi i masyva
{
    int j = (stack -> size);
    (stack -> size) += strlen (word)+1;
    stack -> array = (char*) realloc (stack -> array, sizeof (char) * (stack -> size));
    for(int i = 0; i < strlen(word); ++i)
    {
        stack -> array[j+i] = word[i];
    }
        stack -> array[(stack -> size)-1] = '\n';
}

void destroyStack (Stack *stack) // sunakinina struktura
{
    free(stack -> array);
    stack -> size = 0;
}

void doValidationNr1 (char *fname) //gauna varda ir patikrina ar jis neperzengia sinboliu limito
{
    while (scanf("%128s", fname) != 1 || getchar() != '\n')
    {
        printf("%s", DO_VALIDATION_NR1_LINE);
        while (getchar() != '\n')
            ;
    }
}

int doValidationNr2 (char *fname) // patikrina ar vardas turi .txt jo gale
{
    int capacity;
    capacity = strlen (fname);
    for (int i=0; i<(capacity)-3; ++i)
    {
        if  ((fname[i] == '.') && (fname[i+1] == 't') && (fname[i+2] == 'x') && (fname[i+3] == 't'))
            return 0;
    }
    printf ("%s", DO_VALIDATION_NR2_LINE);
    return 1;
}

void doValidationCombo (char *fname) // apjungia validacijas
{
    do
    {
        doValidationNr1 (fname);
    } while (doValidationNr2 (fname) == 1);

}

char *getDataFileName () // pagrindine vardo gavimo funkcija
{
    char *fname = NULL;
    printf ("%s", MAIN_ARGC_1);
    fname = (char*) realloc (fname, sizeof(char) * FILE_NAME_LENGTH);
    if (fname == NULL)
    {
        printf ("%s", GET_DATA_FILE_NAME_NULL);
        return fname;
    }
    else
    {
        doValidationCombo (fname);
    }
    return fname;
}

void findWords (char *line, int lenght, Stack *out) // randa visus zodius eiluteje su tam tikru skimoliu skaiciumi (didziausiu toke eilueje)
{
    char *word;
    while ((word = strtok_r(line, " ", &line)))
    {
        if (strlen (word) == lenght)
        {
            push(out, word);
        }
    }
}

int findWordLenght (char *line) //randa kokio didziausio ilgio yra zodis
{
    int max_lenght = 0;
    char *word_tok;
    while ((word_tok = strtok_r(line, " ", &line)))
    {
        if (strlen (word_tok) > max_lenght && strlen (word_tok) < WORD_SIZE)
        {
            max_lenght = strlen (word_tok);
        }
    }
    return max_lenght;
}

int main (int argc, char *argv[])
{
    printf ("%s", MAIN_LINE);
    char* data_file_name;

    if (argc == 1) // jei parametruose daugiau nieko nera parasyta
    {
        data_file_name = getDataFileName ();
    }
    else // jei parametruose yra parasyta kazkas daugiau
    {
        data_file_name = argv[1];
        if (doValidationNr2 (data_file_name) == 1) //jie parametruose esantis failas netinkamas
            data_file_name = getDataFileName ();
    }

    if (data_file_name == NULL)  //patikrina ar failo pavadinimas egzistuoja
    {
        free (data_file_name);
        return 0;
    }

    char buffer [BUFFER_SIZE];
    char *tok;
    char *tok2;
    char *extra_tok;
    int max_w_lenght;
    int extra_lenght = 0;
    int tok_ilgis;
    int num;
    Stack out;
    initStack(&out);
    FILE *input;
                                                //gauna ilgiausius zodius eiluteje ir jous suraso i bufferi
    if (( input = fopen(data_file_name, "r"))!= NULL )
    {
        do
        {
            num = fread (buffer, sizeof(char), BUFFER_SIZE, input);

            if (num) // nuskaite kazka
            {
                tok = strtok(buffer, "\n");
                while (tok != 0)
                {
                    tok_ilgis = strlen (tok);
                    tok2 = malloc (tok_ilgis+1);
                    memcpy (tok2, tok, tok_ilgis+1);
                    max_w_lenght = findWordLenght (tok);
                    tok = strtok (0, "\n");

                    if (tok == 0 && num == BUFFER_SIZE)
                    {
                        extra_tok = malloc (tok_ilgis+1);
                        memcpy (extra_tok, tok2, tok_ilgis+1);
                        extra_lenght = max_w_lenght;
                    }
                    else
                    {
                        if (extra_lenght > max_w_lenght)
                        {
                            findWords (extra_tok, extra_lenght, &out);
                        }
                        else if (extra_lenght < max_w_lenght)
                        {
                            findWords (tok2, max_w_lenght, &out);
                        }
                        else
                        {
                            findWords (extra_tok, extra_lenght, &out);
                            findWords (tok2, max_w_lenght, &out);
                        }

                        if (extra_lenght > 0)
                        {
                            free (extra_tok);
                            extra_lenght = 0;
                        }
                    }
                }
            }
            else
            {
                if (ferror(input))
                {
                    printf ("%s", MAIN_NUM_0);
                    destroyStack(&out);
                    free (tok);
                    free (tok2);
                    free (extra_tok);
                    return 0;
                }
            }
        }while (num == BUFFER_SIZE);
    }
  else
    printf ("%s",MAIN_NOT_FOPEN);
    fclose (input);
                    //gauna rezultatu failo pavadinima ir iraso bufferio duomenis i faila
    if (argc > 2)
    {
        data_file_name = argv[2];
        if (data_file_name == NULL)  //patikrina ar failo pavadinimas egzistuoja
        {
            free (data_file_name);
            destroyStack(&out);
            free (tok);
            free (tok2);
            free (extra_tok);
            return 0;
        }
        if (doValidationNr2(data_file_name) == 0)
        {
            FILE *output;
            if (( output = fopen(data_file_name, "w"))!= NULL )
            {
                printStack(&out, output);
                fclose (output);
            }
            else
                printf ("%s",MAIN_NOT_FOPEN);
        }
        else
            printStack(&out, stdout);
    }
    else
        printStack(&out, stdout);

    destroyStack(&out);
    free (tok);
    free (tok2);
    free (extra_tok);

    return 0;
}
