#ifndef _UTILS_H
#define _UTILS_H
char* itoa10(unsigned int val);
char* itoa16(unsigned int val);
char* itoa10buffer(unsigned int val, char* c_buffer);
char* itoa16buffer(unsigned int val, char* c_buffer);
void setEmptyString(char c_buffer[], int buf_len);
#endif //_UTILS_H
