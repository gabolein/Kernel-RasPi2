#include "led.h"
#include "serial.h"
#include "kio.h"

void start_kernel(void)
{ 
  //kputChar('G');
  //kputChar('\n');
  //kprintf("123456789123456789123456789");
  //kprintf("Ich bin ein Brot\n");

  initUart();

  //yellow_on();
  //yellow_off();
  
  
  kputChar('a');
  kputChar('b');
  kputChar('c');
  kputChar('d');
  kputChar('e');
  kputChar('f');
  kputChar('g');
  kputChar('h');
  kputChar('i');
  kputChar('j');
  kputChar('k');
  kputChar('l');
  kputChar('m');
  kputChar('n');
  kputChar('o');
  kputChar('p');
  kputChar('q');
  kputChar('r');
  kputChar('s');
  kputChar('t');
  
  while(1){
  }
}
