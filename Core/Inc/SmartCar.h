#ifndef __SMARTCAR_H__
#define __SMARTCAR_H__

#include "headfile.h"
#include "freertosdemo.h"

/*--- 智能车功能模式枚举 ---*/
typedef enum {
    MODE_STANDBY,
    MODE_AVOID,
    MODE_TRACE,
    MODE_FORWARD,
    MODE_BACK,
    MODE_LEFT,
    MODE_RIGHT
}FUNCTION_MODE;


/*--- 智能车避障模式枚举 ---*/
typedef enum {
    MODE_ORDINARY,
    MODE_OBSTACLE,
    MODE_AVOID_LEFT,
    MODE_AVOID_RIGHT,
    MODE_AVOID_BACK,
}SMARTCAR_MODE;

extern SMARTCAR_MODE uwSmartCar_Mode;





#endif /* __SMARTCAR_H__ */
