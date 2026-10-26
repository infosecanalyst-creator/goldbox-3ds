/* check-only stand-in for libctru synchronization */
#pragma once
#include <3ds/types.h>
typedef s32 LightLock;
typedef s32 CondVar;
#ifdef __cplusplus
extern "C" {
#endif
void LightLock_Init(LightLock* lock); void LightLock_Lock(LightLock* lock); void LightLock_Unlock(LightLock* lock);
void CondVar_Init(CondVar* cv); void CondVar_Wait(CondVar* cv, LightLock* lock); void CondVar_Signal(CondVar* cv); void CondVar_Broadcast(CondVar* cv);
#ifdef __cplusplus
}
#endif
