/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Louis Adams 
 * Date: 2019-08-30
 */

#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
    
/* Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char * nextWord(FILE* file){
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
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
 * This function validates the word entered as input by the user
 */
char * validateWord(char *input){
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = input[length];
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
        else if(length > 0 || input[length] == '\0'){
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
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXME: implement
    char *word = nextWord(file);
    while(word != NULL){
        hashMapPut(map, word, 0); 
    
        // Free the word after putting it in the hash table 
        free(word);

        word = nextWord(file);
    }

/*    hashMapPrint(map);
    
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n", hashMapTableLoad(map));
*/
}

/**
 * This function swaps 2 HashLink pointers
 */
void swap(HashLink **a, HashLink **b){
    HashLink *temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * This function calculates the Levenshtein distance between two strings and returns that value as an int
 * Adapted from en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance
 */
int levenDist(char *s1, char *s2){
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    
    for(y = 1; y <= s1len; y++){
        column[y] = y;
    }
    for(x = 1; x <= s2len; x++){
        column[0] = x;
        for(y = 1, lastdiag = x-1; y <= s1len; y++){
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
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
    HashMap* map = hashMapNew(1000);
    
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    
    char inputBuffer[256];
    char *word;
    int quit = 0;
    while (!quit){
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        word = validateWord(inputBuffer);
        if(strcmp(inputBuffer, "quit") == 0){
            quit = 1;
        }

        while(word == NULL){
            printf("Invalid input. Please enter a valid word: ");
            scanf("%s", inputBuffer);
            word = validateWord(inputBuffer);
            if(strcmp(inputBuffer, "quit") == 0){
                quit = 1;
            }
        }
        
        if(quit == 1){
            free(word);
            break;
        }

        // Implement the spell checker code here..
        if(hashMapContainsKey(map, word) == 1){
            printf("The inputted word \"%s\" is spelled correctly.\n", word);
        }
        // Otherwise find the 5 closest matches to the inputted word
        else{
            HashLink *cur;
            int dist;               // The Levenshtein distance between two words
            HashLink *matches[5];   // An array of the closest matches to the inputted word

            // Traverse through all the links in all the buckets to add Levenshtein distances as values
            for(int i = 0; i < map->capacity; i++){
                cur = map->table[i];
                if(cur != NULL){
                    while(cur != NULL){
                        dist = levenDist(word, cur->key);
                        hashMapPut(map, cur->key, dist);
                        cur = cur->next;
                    }
                }
            }

            // Set matches array to a link
            HashLink link;
            link.value = 256;
            for(int i = 0; i < 5; i++){
                matches[i] = &link;
            }

            // Traverse through all the links in all the buckets to get the closest 5 matches
            for(int i = 0; i < map->capacity; i++){
                cur = map->table[i];
                if(cur != NULL){
                    while(cur != NULL){
                        if(*hashMapGet(map, cur->key) < matches[4]->value){
                            matches[4] = cur;
                            if(matches[4]->value < matches[3]->value){
                                swap(&matches[3], &matches[4]);
                                if(matches[3]->value < matches[2]->value){
                                    swap(&matches[2], &matches[3]);
                                    if(matches[2]->value < matches[1]->value){
                                        swap(&matches[1], &matches[2]);
                                        if(matches[1]->value < matches[0]->value){
                                            swap(&matches[0], &matches[1]);
                                        }
                                    }
                                }
                            }
                        }
                        cur = cur->next;
                    }
                }
            }
            printf("The inputted word \"%s\" is spelled incorrectly. Did you mean...?", word);
            printf("\n1. %s\n2. %s\n3. %s\n4. %s\n5. %s\n", matches[0]->key, matches[1]->key, 
                matches[2]->key, matches[3]->key, matches[4]->key);
        }
        free(word);
    }
    hashMapDelete(map);

    return 0;
}
