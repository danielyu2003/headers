/** 
 * name: strutils.h
 * author: danyou
 * notes: 
 *   Insert `#define STRUTILS_IMPL` in only one unit before including this file.
 */

#ifndef strutils_h
#define strutils_h

// Definitions:

struct Split {
    char** words;
    size_t nwords;
};

struct Split split(char*, char*);
char* join(char*, char**, size_t);

#endif // strutils_h

#ifdef STRUTILS_IMPL
#undef STRUTILS_IMPL

// Implementation:

#include <stdlib.h>
#include <string.h>

/**
 * Splits a deliminated, null-terminated string into an array of strings, 
 * and returns a struct containing the array and the number of strings.
 * Remember to free the array after using it!
 */
struct Split split(char* delim, char* string)
{
    struct Split ret = {0};
    size_t nwords = 0;
    char** words = NULL;
    char* word = NULL;
    char* string_copy = strdup(string);
    char* string_ptr = string_copy;
    while ((word = strsep(&string_ptr, delim)) != NULL) {
        size_t i = nwords;
        words = realloc(words, (nwords + 1) * sizeof(char*));
        char* wordbuf = malloc(strlen(word) + 1);
        strcpy(wordbuf, word);
        words[i] = wordbuf;
        nwords++;
    }
    free(string_copy);
    ret.nwords = nwords;
    ret.words = words;
    return ret;
}

/**
 * Joins an array of strings into a single deliminated string.
 * Assumes that delim and all strings in words are null-terminated, and that
 * nwords is the correct number of strings in words.
 * Remember to free the string after using it!
 */ 
char* join(char* delim, char** words, size_t nwords)
{
    if (nwords == 0)
        return "";
    size_t bufsize = 0;
    for (size_t i = 0; i < nwords; i++)
        bufsize += strlen(words[i]);
    bufsize += (strlen(delim) * (nwords - 1));
    char* buf = malloc(bufsize+1);
    if (buf == NULL)
        return "";
    char* next = buf;
    for (size_t i = 0; i < nwords; i++) {
        next = stpcpy(next, words[i]);
        if (i < (nwords-1))
            next = stpcpy(next, delim);
    }
    return buf;
}

#endif // STRUTILS_IMPL
