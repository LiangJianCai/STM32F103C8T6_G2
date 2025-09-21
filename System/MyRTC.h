#ifndef __MYRTC_H__
#define __MYRTC_H__

#include "sysconfig.h"
#include "stdint.h"
#include "error_handler.h"

extern int16_t MyRTC_Time[];

void MyRTC_Init(void);
ErrorCode_t MyRTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif
