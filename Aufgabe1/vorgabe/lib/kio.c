//
// Created by gabolein on 22.10.19.
//

#include "kio.h"
#include "serial.h"
#include <stdarg.h>

void kprintf(char* format, ...) {
    va_list arg;
    va_start(arg, format);

    for(char* traverse = format; *traverse != '\0'; traverse++) {
        if (*traverse == '%') {
            traverse++; // move to next character

            switch(*traverse) {
                case '%' : ; // not so pretty, do we wanna keep it?
                    kputchar('%');
                    break;
                case 'c' : ;
                    char character = va_arg(arg, int);
                    kputchar(character);
                    break;
                case 's' : ;
                    char* string = va_arg(arg, char*);
                    kprintf(string);
                    break;
                case 'x' : ;
                    unsigned int u_integer = va_arg(arg, unsigned int);
                    kprintf(itoa(u_integer, 16));
                    break;
                case 'i' : ;
                    int integer = va_arg(arg, int);
                    if(integer == -2147483647) { // edge case
                        kprintf("-2147483647");
                    } else {
                        if(integer < 0) {
                            kputchar('-');
                            integer *= -1;
                        }
                        kprintf(itoa(integer, 10));
                    }
                    break;
                case 'u' :
                    u_integer = va_arg(arg, unsigned int);
                    kprintf(itoa(u_integer, 10));
                    break;
                case 'p' :
                    unsigned int address = va_arg(arg, unsigned int);
                    kprintf(itoa(address, 16));
                    break;
                default: // if unknown definer, behave like printf
                    kputchar('%');
                    kputchar(*traverse);
                    break;
            }

        } else {
            kputchar(*traverse);
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
    } while(value&&i);
    return &c_buffer[i+1];

}

