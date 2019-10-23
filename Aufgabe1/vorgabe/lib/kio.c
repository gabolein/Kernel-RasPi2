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
                case '%' : ;// should be neato
                    kputchar('%');
                    break;
                case 'c' : ;// ask whether it is ascii value we are receiving
                    char character = va_arg(arg, int);
                    kputchar(character);
                    break;
                case 's' : ;// if kprintf works, this does too
                    char* string = va_arg(arg, char*);
                    kprintf(string);
                    break;
                case 'x' :  ;// itoa should work, but everything will have a plus, not ideal
                    unsigned int u_integer = va_arg(arg, unsigned int);
                    kprintf(itoa(u_integer, 16));
                    break;
                case 'i' : ;// vorzeichen solution in itoa might not be ideal, doesnt work for negatives
                    int integer = va_arg(arg, int);
                    kprintf(itoa(integer, 10));
                    break;
                case 'u' : // same as for case 'x'
                    u_integer = va_arg(arg, unsigned int)
                    kprintf(itoa(u_integer, 10));
                    break;
                case 'p' : // what type is it, how much do i have to print out?

                    break;





            }
        } else {
            kputchar(*traverse);
        }
    }
    va_end(arg);
}



char* itoa(int val, int base){ // IST CPY PASTED
    static char buf[33] = {0};

    int i = 30;

    // solution doesnt even work
    if (val < 0) { // problems: not sure if it works. Every positive integer will have a +. 0 will have a plus. to be reviewed
        buf[0] = '-';
    } else {
        buf[0] = '+';
    }



    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

