//
// Created by gabolein on 22.10.19.
//

#include "kio.h"
#include "serial.h"
#include "utils.h"
#include <stdarg.h>
#include <stdint.h>

#define MIN_INT -2147483648
#define LEN_DEC 10 // max chars needed to represent decimal
#define LEN_HEX 8 // max chars needed to represent hex


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
                                        {
                                        char c_buffer[11] = "0x00000000";
                                        kprintf(itoa16(u_int_num, c_buffer));
                                        }
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
                                        {
                                        char c_buffer[11] = "0x00000000";
                                        kprintf(itoa16(address, &c_buffer));
                                        }
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





/*
 * Prints a string in the given format to screen
 */
__attribute__((format(printf, 1, 2)))
void kprintfbuffer(const char* format, ...) {

        va_list arg;
        va_start(arg, format);

        unsigned char char_rep;
        const char* str;
        unsigned int u_int_num;
        int int_num;
        unsigned int address;
        char c_buffer10[LEN_DEC];
        char c_buffer16[LEN_HEX];


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
                                        setEmptyString(c_buffer16, LEN_HEX);
                                        kprintf(itoa16buffer(u_int_num, c_buffer16));
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
                                                setEmptyString(c_buffer10, LEN_DEC);
                                                kprintf(itoa10buffer(int_num, c_buffer10));
                                        }
                                        break;
                                case 'u' :
                                        u_int_num = va_arg(arg, unsigned int);
                                        setEmptyString(c_buffer10, LEN_DEC);
                                        kprintf(itoa10buffer(u_int_num, c_buffer10));
                                        break;
                                case 'p' :
                                        kprintf("0x");
                                        address = va_arg(arg, uint32_t); // pointer -> 32Bit address
                                        setEmptyString(c_buffer16, LEN_HEX);
                                        kprintf(itoa16buffer(address, c_buffer16));
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

