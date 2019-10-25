//
// Created by gabolein on 22.10.19.
//

#include "kio.h"
#include "serial.h"
#include <stdarg.h>

#define MIN_INT -2147483647

__attribute__((format(printf, 1, 2)))
void kprintf(char* format, ...) {

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
                    break;
                case 'c' :
                    char_rep = va_arg(arg, int);
                    kputChar(character);
                    break;
                case 's' :
                    str = va_arg(arg, char*);
                    kprintf(str);
                    break;
                case 'x' :
                    u_int_num = va_arg(arg, unsigned int);
                    kprintf(itoa(u_int_num, 16));
                    break;
                case 'i' :
                    int_num = va_arg(arg, int);
                    if(int_num == MIN_INT) { // edge case (zweier komplement who?)
                        kprintf("-2147483647");
                    } else {
                        if(int_num < 0) {
                            kputChar('-');
                            int_num *= -1;
                        }
                        kprintf(itoa(int_num, 10));
                    }
                    break;
                case 'u' :
                    u_int_num = va_arg(arg, unsigned int);
                    kprintf(itoa(u_int_num, 10));
                    break;
	            case 'p' :
                    address = va_arg(arg, unsigned int);
                    kprintf(itoa(address, 16));
                    break;
                default: // if unknown definer, behave like printf
                    kputChar('%');
                    kputChar(*traverse);
                    break;
            }

        } else {
            kputChar(*traverse);
        }
    }
    va_end(arg);
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

