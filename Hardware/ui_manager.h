#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__
#include <stddef.h>
#include "stm32f10x.h"

// UI更新模式
typedef enum {
    UI_UPDATE_FULL,      // 全屏更新
    UI_UPDATE_PARTIAL,   // 部分更新
    UI_UPDATE_NONE       // 不更新
} UIUpdateMode_t;

// UI元素类型
typedef enum {
    UI_ELEMENT_TEXT,
    UI_ELEMENT_NUMBER,
    UI_ELEMENT_IMAGE,
    UI_ELEMENT_RECTANGLE,
    UI_ELEMENT_CIRCLE
} UIElementType_t;

// UI元素结构
typedef struct {
    UIElementType_t type;
    int16_t x, y;
    uint8_t width, height;
    void* data;  // 指向具体数据的指针
    uint8_t visible;
    uint8_t needUpdate;
} UIElement_t;

// UI页面结构
typedef struct {
    UIElement_t elements[32];  // 最多32个UI元素
    uint8_t elementCount;
    uint8_t needRefresh;
    UIUpdateMode_t updateMode;
} UIPage_t;

// UI管理器
typedef struct {
    UIPage_t currentPage;
    UIPage_t previousPage;
    uint8_t pageChanged;
    uint32_t lastUpdateTime;
    uint32_t updateInterval;  // 更新间隔(ms)
} UIManager_t;

// 函数声明
void UIManager_Init(void);
void UIManager_AddElement(UIElementType_t type, int16_t x, int16_t y, 
                         uint8_t width, uint8_t height, void* data);
void UIManager_ClearPage(void);
void UIManager_Update(void);
void UIManager_SetUpdateMode(UIUpdateMode_t mode);
void UIManager_SetUpdateInterval(uint32_t interval);
void UIManager_ForceRefresh(void);

// 便捷函数
void UIManager_ShowText(int16_t x, int16_t y, char* text, uint8_t fontSize);
void UIManager_ShowNumber(int16_t x, int16_t y, uint32_t number, uint8_t length, uint8_t fontSize);
void UIManager_ShowImage(int16_t x, int16_t y, uint8_t width, uint8_t height, const uint8_t* image);
void UIManager_ShowRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t filled);

#endif
