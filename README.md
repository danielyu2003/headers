# headers

Single-file header libraries for personal projects. 

General Format:

```c
/** 
 * name: file.h
 * author: danyou
 * notes: 
 *   Insert `#define FILE_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef file_h
#define file_h

// ...

#endif // file_h

// Implementation:

#ifdef FILE_IMPL
#undef FILE_IMPL

// ...

#endif // FILE_IMPL

```