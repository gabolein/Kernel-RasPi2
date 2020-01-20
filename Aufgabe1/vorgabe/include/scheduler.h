#include <stdint.h>
#include "thread.h"

#ifndef SCHEDULER_H
#define SCHEDULER_H

struct thcStruct* rrSchedule(struct thcStruct*, uint16_t);
void adjustSleptTime();

#endif //SCHEDULER_H
