#include "led.h"
#include "serial.h"
#include "kio.h"

void start_kernel(void)
{
  char receivedChar;
  while(1){
    if(uartReceiveChar(&receivedChar)){
      /* print received char in every possible way of interpretation */
      kprintf("****************************************\n");
      kprintf("gUt3N m0Rg3n <3 <3 UWU \n");
      kprintf("****************************************\n\n\n\n");
      kprintf("%c\n\n", receivedChar);
      kprintf("%i\n\n", receivedChar);
      //kprintf("%s\n\n", receivedChar);
      kprintf("%x\n\n", receivedChar);
      kprintf("%u\n\n", receivedChar);
      kprintf("%p\n\n", &receivedChar);
    }
  }
}
