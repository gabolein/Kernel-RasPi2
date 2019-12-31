#include "kioUser.h"
#include "swiInterface.h"
#include "threadUtil.h"
#include <stdarg.h>
#include <stdint.h>

#define MIN_INT -2147483648
#define DEC_BUFFER_SIZE 10
#define HEX_BUFFER_SIZE 11

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
                                        putChar('%');
                                        break;
                                case 'c' :
                                        char_rep = va_arg(arg, int);
                                        putChar(char_rep);
                                        break;
                                case 's' :
                                        str = va_arg(arg, const char*);
                                        kprintf(str);
                                        break;
                                case 'x' :
                                        u_int_num = va_arg(arg, unsigned int);
                                        {
                                        char c_buffer[HEX_BUFFER_SIZE] = "0x00000000";
                                        kprintf(itoa16(u_int_num, c_buffer));
                                        }
                                        break;
                                case 'i' :
                                        int_num = va_arg(arg, int);
                                        if(int_num == MIN_INT) { // edge case (zweier komplement who?)
                                                kprintf("-21474836478");
                                        } else {
                                                if (int_num < 0) { // if negative, print '-' and treat like unsigned int
                                                        putChar('-');
                                                        int_num *= -1;
                                                }
                                                {
                                                char c_buffer[DEC_BUFFER_SIZE] = "";
                                                kprintf(itoa10(int_num, c_buffer));
                                                }
                                        }
                                        break;
                                case 'u' :
                                        u_int_num = va_arg(arg, unsigned int);
                                        {
                                        char c_buffer[DEC_BUFFER_SIZE] = "";
                                        kprintf(itoa10(u_int_num, c_buffer));
                                        }
                                        break;
                                case 'p' :
                                        address = va_arg(arg, uint32_t); // pointer -> 32Bit address
                                        {
                                        char c_buffer[HEX_BUFFER_SIZE] = "0x00000000";
                                        kprintf(itoa16(address, c_buffer));
                                        }
                                        break;
                                default: // if unknown definer, behave like printf
                                        putChar('%');
                                        putChar(*traverse);
                                        break;
                        }

                } else {
                        putChar(*traverse);
                }
        }
        va_end(arg);
}
