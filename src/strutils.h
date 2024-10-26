/** 
 * name: strutils.h
 * author: danyou
 * notes: 
 *   Insert `#define STRUTILS_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef strutils_h
#define strutils_h

struct Split {
    char** words;
    size_t nwords;
};

int split(char*, char*, struct Split*);
int join(char*, char**, size_t, char**);

#endif // strutils_h

// Implementation:

#ifdef STRUTILS_IMPL
#undef STRUTILS_IMPL

#include <stdlib.h>
#include <string.h>

/**
 * Splits a delimited, null-terminated string into an array of strings,
 * storing the array and the count in the struct pointed to by result.
 * Returns 1 for success and 0 for failure.
 * Remember to free the array after using it!
 */
int split(char* delim, char* string, struct Split* result)
{
    if (result == NULL || delim == NULL || string == NULL)
        return 0;
    size_t nwords = 0;
    char** words = NULL;
    char* string_copy = strdup(string);
    if (string_copy == NULL)
        return 0;
    char* string_ptr = string_copy;
    char* word;
    while ((word = strsep(&string_ptr, delim)) != NULL) {
        char* wordbuf = malloc(strlen(word) + 1);
        if (wordbuf == NULL) {
            free(string_copy);
            for (size_t i = 0; i < nwords; i++)
                free(words[i]);
            free(words);
            return 0;
        }
        strcpy(wordbuf, word);
        char** temp = realloc(words, (nwords + 1) * sizeof(char*));
        if (temp == NULL) {
            free(wordbuf);
            free(string_copy);
            for (size_t i = 0; i < nwords; i++)
                free(words[i]);
            free(words);
            return 0;
        }
        words = temp;
        words[nwords] = wordbuf;
        nwords++;
    }
    free(string_copy);
    result->nwords = nwords;
    result->words = words;
    return 1;
}

/**
 * Joins an array of strings into a single delimited string.
 * Assumes that delim and all strings in words are null-terminated,
 * and that nwords is the correct number of strings in words.
 * Stores the resulting string in the char* pointed to by result.
 * Returns 1 for success and 0 for failure.
 * Remember to free the string after using it!
 */ 
int join(char* delim, char** words, size_t nwords, char** result)
{
    if (result == NULL || delim == NULL || words == NULL)
        return 0;
    if (nwords == 0) {
        *result = strdup("");
        return *result != NULL;
    }
    size_t bufsize = 0;
    for (size_t i = 0; i < nwords; i++)
        bufsize += strlen(words[i]);
    bufsize += strlen(delim) * (nwords - 1);
    char* buf = malloc(bufsize + 1);
    if (buf == NULL)
        return 0;
    char* next = buf;
    for (size_t i = 0; i < nwords; i++) {
        next = stpcpy(next, words[i]);
        if (i < (nwords - 1))
            next = stpcpy(next, delim);
    }
    *result = buf;
    return 1;
}

#endif // STRUTILS_IMPL
