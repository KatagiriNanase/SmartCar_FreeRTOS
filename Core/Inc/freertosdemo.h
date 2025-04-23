#ifndef __FREERTOSDEMO_H__
#define __FREERTOSDEMO_H__

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
/*---User Include ---*/
#include "headfile.h"

/*---User Definiton ---*/
#define EVENT_DIS_UPDATE_BIT (1<<0)

/*---Extern Variale ---*/
extern QueueHandle_t HcsrDisMutex;

/*---Function Prototype ---*/
void FreeRTOS_Start(void);


#endif /* __FREERTOSDEMO_H__ */
