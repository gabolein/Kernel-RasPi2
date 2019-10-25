//
// Created by gabolein on 22.10.19.
//

#include "kio.h"
#include "serial.h"
#include <stdarg.h>
#include <stdint.h>

#define MIN_INT -2147483647

/*
 * Prints a string in the given format to screen. Returns the amount of printed characters.
 */
__attribute__((format(printf, 1, 2)))
int kprintf(char* format, ...) {

    int printed_chars = 0;

    va_list arg;
    va_start(arg, format);

    unsigned char char_rep;
    const char* str;
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
                    str = va_arg(arg, const char*);
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
                        if(int_num < 0) { // if negative, print '-' and treat like unsigned int
                            kputChar('-');
                            printed_chars++;
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
	                kprintf("0x");
                    address = va_arg(arg, uint32_t); // pointer -> 32Bit address
                    kprintf(itoa(address, 16));
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

