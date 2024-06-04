#ifndef STRING_H
#define STRING_H

// Member APIs

/*! \brief Convert an input string into corresponding integer value
 *
 *  \param `s` string that has to be converted into corresponding integer
 *
 *  \return corresponding integer value*/
int str_to_int(char *s);

/*! \brief Convert an input integer into corresponding string value
 *
 *  \param `num` integer that has to be converted into corresponding string
 *  \param `buffer` buffer to store the corresponding string value (must contain
 * enough space)
 *
 *  \return corresponding string value*/
void int_to_str(int num, char *buffer);

/*! \brief Calculate the length of the input string
 *
 *  \param `s` string whose length has to be calculated
 *
 *  \return length of the input string*/
int str_length(const char *s);

/*! \brief Concatenate two strings; store the concatenated string in the source
 * string
 *
 *  \param `src` source string (must contain enough space to store concatenated
 * string)
 *  \param `dest` destination string*/
void str_concat(char *src, char *dest);

#endif // !STRING_H
