//#include "led.h"
#include "serial.h"
#include "kio.h"

void start_kernel(void)
{ 
  kputChar('G');
  kputChar('\n');
  kprintf("Ich bin ein Brot\n");
  kprintf("Ich bin ein Brot\n");
  
  while(1){
  }
}
