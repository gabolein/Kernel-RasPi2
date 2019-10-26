#include "led.h"
#include "serial.h"
#include "kio.h"

void start_kernel(void)
{
  char receivedChar;
  while(1){
    kputChar('G');
    kputChar('\n');
      
  }
}
