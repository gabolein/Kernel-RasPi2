#include "led.h"
#include "serial.h"
#include "kio.h"

void start_kernel(void)
{
  char receivedChar;
  while(1){
    if(uartReceiveChar(&receivedChar)){
      /* print received char in every possible way of interpretation */
      kprintf()
    }
  }
}
