/* check-only stand-in for <3ds.h>: signatures as in libctru 2.x */
#pragma once
#include <3ds/types.h>
#include <3ds/synchronization.h>
#include <stddef.h>
#define U64_MAX UINT64_MAX
#define CUR_THREAD_HANDLE 0xFFFF8000
#define R_SUCCEEDED(res) ((res) >= 0)
typedef enum { GFX_TOP = 0, GFX_BOTTOM = 1 } gfxScreen_t;
typedef enum { GFX_LEFT = 0, GFX_RIGHT = 1 } gfx3dSide_t;
enum { KEY_A = 1, KEY_B = 2, KEY_SELECT = 4, KEY_START = 8, KEY_DRIGHT = 16, KEY_DLEFT = 32, KEY_DUP = 64, KEY_DDOWN = 128,
       KEY_R = 256, KEY_L = 512, KEY_X = 1024, KEY_Y = 2048, KEY_ZL = 1 << 14, KEY_ZR = 1 << 15, KEY_TOUCH = 1 << 20,
       KEY_CPAD_RIGHT = 1 << 28, KEY_CPAD_LEFT = 1 << 29, KEY_CPAD_UP = 1 << 30, KEY_CPAD_DOWN = (int)(1u << 31) };
typedef struct { u16 px, py; } touchPosition;
typedef struct PrintConsole { int dummy; } PrintConsole;
typedef enum { NDSP_OUTPUT_MONO = 0, NDSP_OUTPUT_STEREO = 1 } ndspOutputMode;
typedef enum { NDSP_INTERP_POLYPHASE = 0, NDSP_INTERP_LINEAR = 1, NDSP_INTERP_NONE = 2 } ndspInterpType;
enum { NDSP_FORMAT_MONO_PCM16 = 5 };
typedef struct ndspWaveBuf { union { s8* data_pcm8; s16* data_pcm16; u8* data_adpcm; const void* data_vaddr; }; u32 nsamples; void* adpcm_data; u32 offset; bool looping; u8 status; u16 sequence_id; struct ndspWaveBuf* next; } ndspWaveBuf;
typedef struct Thread_tag* Thread;
typedef void (*ThreadFunc)(void*);
#ifdef __cplusplus
extern "C" {
#endif
bool aptMainLoop(void);
void gfxInitDefault(void); void gfxExit(void); void gfxSetDoubleBuffering(gfxScreen_t screen, bool enable);
u8* gfxGetFramebuffer(gfxScreen_t screen, gfx3dSide_t side, u16* width, u16* height);
void gfxFlushBuffers(void); void gfxSwapBuffers(void); void gspWaitForVBlank(void);
PrintConsole* consoleInit(gfxScreen_t screen, PrintConsole* console); PrintConsole* consoleSelect(PrintConsole* console); void consoleClear(void);
Result romfsInit(void); Result romfsExit(void);
void hidScanInput(void); u32 hidKeysUp(void); u32 hidKeysDown(void); u32 hidKeysHeld(void); void hidTouchRead(touchPosition* pos);
Result ndspInit(void); void ndspExit(void); void ndspSetOutputMode(ndspOutputMode mode);
void ndspChnReset(int id); void ndspChnSetInterp(int id, ndspInterpType type); void ndspChnSetFormat(int id, u16 format);
void ndspChnSetMix(int id, float mix[12]); void ndspChnSetRate(int id, float rate); void ndspChnWaveBufClear(int id); void ndspChnWaveBufAdd(int id, ndspWaveBuf* buf);
void* linearAlloc(size_t size); Result DSP_FlushDataCache(const void* address, u32 size);
float osGet3DSliderState(void); void gfxSet3D(bool enable);
u64 osGetTime(void); void osSetSpeedupEnable(bool enable);
Thread threadCreate(ThreadFunc entrypoint, void* arg, size_t stack_size, int prio, int core_id, bool detached);
Result threadJoin(Thread thread, u64 timeout_ns); void threadFree(Thread thread);
Result svcGetThreadPriority(s32* out, Handle handle); void svcSleepThread(s64 ns);
#ifdef __cplusplus
}
#endif
