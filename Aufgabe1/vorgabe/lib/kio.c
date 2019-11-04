//
// Created by gabolein on 22.10.19.
//

#include "kio.h"
#include "serial.h"
#include <stdarg.h>
#include <stdint.h>

#define MIN_INT -2147483648

/*
 * Prints a string in the given format to screen
 */
__attribute__((format(printf, 1, 2)))
void kprintf(const char* format, ...) {

    va_list arg;
    va_start(arg, format);

    unsigned char char_rep;
    const char* str;
    unsigned int u_int_num;
    int int_num;
    unsigned int address;


    for (const char* traverse = format; *traverse != '\0'; traverse++) {
        if (*traverse == '%') {
            traverse++; // move to next character, we dont wanna print the '%'

            switch(*traverse) {

                case '%' :
                    kputChar('%');
                    break;
                case 'c' :
                    char_rep = va_arg(arg, int);
                    kputChar(char_rep);
                    break;
                case 's' :
                    str = va_arg(arg, const char*);
                    kprintf(str);
                    break;
                case 'x' :
                    u_int_num = va_arg(arg, unsigned int);
                    kprintf(itoa16(u_int_num));
                    break;
                case 'i' :
                    int_num = va_arg(arg, int);
                    if(int_num == MIN_INT) { // edge case (zweier komplement who?)
                        kprintf("-21474836478");
                    } else {
                        if(int_num < 0) { // if negative, print '-' and treat like unsigned int
                            kputChar('-');
                            int_num *= -1;
                        }
                        kprintf(itoa10(int_num));
                    }
                    break;
                case 'u' :
                    u_int_num = va_arg(arg, unsigned int);
                    kprintf(itoa10(u_int_num));
                    break;
	            case 'p' :
	                kprintf("0x");
                    address = va_arg(arg, uint32_t); // pointer -> 32Bit address
                    kprintf(itoa16(address));
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



char* itoa10(unsigned int value){

    static char c_buffer[10] = ""; // 10 -> maximal amount of characters needed to present 32 bits in decimal
    static char presentation[] = "0123456789";
    int i = 8; // start from the back
    do { // need do while in case value = 0
        c_buffer[i] = presentation[value % 10];
        --i;
        value /= 10;
    } while (value > 0);
    return &c_buffer[i+1];
}

char* itoa16(unsigned int value){

    static char c_buffer[8] = ""; // 8 -> maximal amount of characters needed to present 32 bits in hex
    static char presentation[] = "0123456789abcdef";
    int i = 6; // start from the back
    do { // need do while in case value = 0
        c_buffer[i] = presentation[value % 16];
        --i;
        value /= 16;
    } while (value > 0);
    return &c_buffer[i+1];
}

void kscanf(const char* format, ...) {

    char c_buffer[100] = "";
    char receivedChar;
    for (int i; i < 100; i++) {
        uartReceiveChar(&receivedChar);
        if (receivedChar == '\n') break;
        c_buffer[i] = receivedChar;
    }

    va_list arg;
    va_start(arg, format);
    for(format; format != '\0'; format++) {
        if(*format == '%'){
            format++;
            x = (arg, &char);
            uartReceiveChar(x);
            switch(*format) {
                case 'c' :
                    charHolder = x;
                    break;
                case 'i' :
                    *x = *x - '0';
                    break;
                case 'u' :
                    *x = *x - '0';
                case 'x':


            }
        }
    }
}

