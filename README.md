Authors: Gabriel Behrendt, Thomas Fock

# Kernel for the Raspberry Pi 2 Model B

Implemented features:
* Drivers for [UART](driver/serial.c) and [timer](driver/timer.c)
* [Multithreading](system/thread.c) with round robin [scheduler](lib/scheduler.c)
* [Heap allocator](user/src/allocator.c)
* [Memory virtualization](system/memory.c)
* [Interrupt handling](system/irqHandler.c) and [user/kernel interface](user/src/swiInterface.c)
