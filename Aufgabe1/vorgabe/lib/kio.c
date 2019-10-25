//
// Created by gabolein on 22.10.19.
//

#include "kio.h"
#include "serial.h"
#include <stdarg.h>

#define MIN_INT -2147483647

__attribute__((format(printf, 1, 2)))
int kprintf(char* format, ...) {

    int printed_chars = 0;

    va_list arg;
    va_start(arg, format);

    char char_rep;
    char* str;
    unsigned int u_int_num;
    int int_num;
    unsigned int address;


    for (char* traverse = format; *traverse != '\0'; traverse++) {
        if (*traverse == '%') {
            traverse++; // move to next character, we dont wanna print the '%'

            switch(*traverse) {

                case '%' :
                    kputChar('%');
                    printed_chars++;
                    break;
                case 'c' :
                    char_rep = va_arg(arg, int);
                    kputChar(char_rep);
                    printed_chars++;
                    break;
                case 's' :
                    str = va_arg(arg, char*);
                    kprintf(str);
                    printed_chars++;
                    break;
                case 'x' :
                    u_int_num = va_arg(arg, unsigned int);
                    kprintf(itoa(u_int_num, 16));
                    printed_chars++;
                    break;
                case 'i' :
                    int_num = va_arg(arg, int);
                    if(int_num == MIN_INT) { // edge case (zweier komplement who?)
                        kprintf("-2147483647");
                        printed_chars++;
                    } else {
                        if(int_num < 0) {
                            kputChar('-');
                            int_num *= -1;
                        }
                        kprintf(itoa(int_num, 10));
                        printed_chars++;
                    }
                    break;
                case 'u' :
                    u_int_num = va_arg(arg, unsigned int);
                    kprintf(itoa(u_int_num, 10));
                    printed_chars++;
                    break;
	            case 'p' :
                    address = va_arg(arg, unsigned int);
                    kprintf(itoa(address, 16));
                    printed_chars++;
                    break;
                default: // if unknown definer, behave like printf
                    kputChar('%');
                    printed_chars++;
                    kputChar(*traverse);
                    printed_chars++;
                    break;
            }

        } else {
            kputChar(*traverse);
            printed_chars++;
        }
    }
    va_end(arg);
    return printed_chars;
}



char* itoa(unsigned int value, int base){

    static char c_buffer[32] = "";
    static char presentation[] = "0123456789abcdef";
    int i = 30;
    do {
        c_buffer[i] = presentation[value % base];
        --i;
        value /= base;
    } while (value&&i);
    return &c_buffer[i+1];
}

