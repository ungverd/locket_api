#include "eventHandlers.h"
#include <stdio.h>
#include "qhsm.h"
#include "Glue.h"



void ChargeTime_Update(Ability *me, unsigned int Time) {
	me->ChargeTime = Time;
	ChargeTime_Save(Time);
}