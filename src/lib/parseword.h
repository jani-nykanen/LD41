/**
 * libparseword library (header)
 * 
 * @author Jani Nykänen
 * @version 1.0.0
 */

#ifndef __LIB__PARSEWORD__
#define __LIB__PARSEWORD__

/** Word data type */
typedef struct
{
    char* data;
    int* wordPos;
    int* wordLength;
    int wordCount; 
    int size;
}
PW_WORDDATA;

/**
 * Parse a file and return word data
 * @param path File path
 * @return Word data object pointer
 */
PW_WORDDATA* pw_parse_file(const char* path);

/**
 * Free word data object
 * @param w Word data object pointer
 */
void pw_destroy_word_data(PW_WORDDATA* w);

/**
 * Get word in index
 * @param w Word data
 * @param index Word index
 * @return Word pointer
 */
char* pw_get_word(PW_WORDDATA* w, int index);

#endif // __LIB__PARSEWORD__