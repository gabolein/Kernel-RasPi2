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
                    if(integer < 0) {
                        kputchar('-');
                        integer *= -1;
                    }
                    kprintf(itoa(integer, 10));
                    break;
                case 'u' :
                    u_integer = va_arg(arg, unsigned int)
                    kprintf(itoa(u_integer, 10));
                    break;
                case 'p' : // what type is it, how much do i have to print out?
                    size_t lulu = va_arg(arg, void);
                    kprintf(itoa(lululu, 16))
                    break;





            }
        } else {
            kputchar(*traverse);
        }
    }
    va_end(arg);
}



char* itoa(unsigned int val, int base){ // IST CPY PASTED
    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

