/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Louis Adams 
 * Date: 2019-08-30
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
//Fix me: Do the necessary change to make the implementation //case-insensitive  
   while (1) 
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')  
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }

            // Change upper case chars to lower case
            if(c >= 'A' && c <= 'Z'){
                c += 32;
            }

            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    FILE *fp;
    const char* fileName = "input1.txt";
    if (argc > 1)
    {
        fileName = argv[1];
    }
    printf("Opening file: %s\n", fileName);
    
    fp = fopen(fileName, "r");

    // Make sure file has been successfully opened
    if(fp != NULL){
        clock_t timer = clock();
        
        HashMap* map = hashMapNew(10);
        
        // --- Concordance code begins here ---
        char *word = nextWord(fp);
        while(word != NULL){
            if(hashMapContainsKey(map, word) == 1){
                hashMapPut(map, word, *(hashMapGet(map, word)) + 1); 
            }
            else{
                hashMapPut(map, word, 1);
            }
        
            // Be sure to free the word after you are done with it here.
            free(word);

            word = nextWord(fp);
        }

        // --- Concordance code ends here ---
        
        fclose(fp);
        
        hashMapPrint(map);
        
        timer = clock() - timer;
        printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
        printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
        printf("Number of links: %d\n", hashMapSize(map));
        printf("Number of buckets: %d\n", hashMapCapacity(map));
        printf("Table load: %f\n", hashMapTableLoad(map));
        
        hashMapDelete(map);
    }
    else{
        printf("There was an error opening the file.\n");
    }
    
    return 0;
}
