#include "ui_manager.h"
#include "OLED.h"
#include "Delay.h"

// 全局UI管理器实例
static UIManager_t g_uiManager;

// UI元素数据存储
typedef struct {
    char text[64];
    uint8_t fontSize;
} TextData_t;

typedef struct {
    uint32_t number;
    uint8_t length;
    uint8_t fontSize;
} NumberData_t;

typedef struct {
    const uint8_t* imageData;
} ImageData_t;

typedef struct {
    uint8_t filled;
} RectangleData_t;

/**
 * @brief 初始化UI管理器
 */
void UIManager_Init(void)
{
    uint8_t i;
    
    // 初始化管理器
    g_uiManager.currentPage.elementCount = 0;
    g_uiManager.currentPage.needRefresh = 1;
    g_uiManager.currentPage.updateMode = UI_UPDATE_FULL;
    g_uiManager.previousPage.elementCount = 0;
    g_uiManager.pageChanged = 0;
    g_uiManager.lastUpdateTime = 0;
    g_uiManager.updateInterval = 50;  // 默认50ms更新间隔
    
    // 初始化UI元素
    for (i = 0; i < 32; i++) {
        g_uiManager.currentPage.elements[i].type = UI_ELEMENT_TEXT;
        g_uiManager.currentPage.elements[i].x = 0;
        g_uiManager.currentPage.elements[i].y = 0;
        g_uiManager.currentPage.elements[i].width = 0;
        g_uiManager.currentPage.elements[i].height = 0;
        g_uiManager.currentPage.elements[i].data = NULL;
        g_uiManager.currentPage.elements[i].visible = 0;
        g_uiManager.currentPage.elements[i].needUpdate = 0;
    }
}

/**
 * @brief 添加UI元素
 */
void UIManager_AddElement(UIElementType_t type, int16_t x, int16_t y, 
                         uint8_t width, uint8_t height, void* data)
{
    if (g_uiManager.currentPage.elementCount >= 32) {
        return;  // 元素数量已达上限
    }
    
    UIElement_t* element = &g_uiManager.currentPage.elements[g_uiManager.currentPage.elementCount];
    
    element->type = type;
    element->x = x;
    element->y = y;
    element->width = width;
    element->height = height;
    element->data = data;
    element->visible = 1;
    element->needUpdate = 1;
    
    g_uiManager.currentPage.elementCount++;
    g_uiManager.currentPage.needRefresh = 1;
}

/**
 * @brief 清除当前页面
 */
void UIManager_ClearPage(void)
{
    g_uiManager.currentPage.elementCount = 0;
    g_uiManager.currentPage.needRefresh = 1;
    OLED_Clear();
}

/**
 * @brief 更新UI
 */
void UIManager_Update(void)
{
    uint32_t currentTime = Delay_GetTick();
    uint8_t i;
    
    // 检查是否需要更新
    if (!g_uiManager.currentPage.needRefresh && 
        (currentTime - g_uiManager.lastUpdateTime) < g_uiManager.updateInterval) {
        return;
    }
    
    // 检查页面是否改变
    if (g_uiManager.pageChanged) {
        OLED_Clear();
        g_uiManager.pageChanged = 0;
    }
    
    // 更新UI元素
    for (i = 0; i < g_uiManager.currentPage.elementCount; i++) {
        UIElement_t* element = &g_uiManager.currentPage.elements[i];
        
        if (!element->visible || !element->needUpdate) {
            continue;
        }
        
        switch (element->type) {
            case UI_ELEMENT_TEXT: {
                TextData_t* textData = (TextData_t*)element->data;
                OLED_ShowString(element->x, element->y, textData->text, textData->fontSize);
                break;
            }
            case UI_ELEMENT_NUMBER: {
                NumberData_t* numberData = (NumberData_t*)element->data;
                OLED_ShowNum(element->x, element->y, numberData->number, 
                           numberData->length, numberData->fontSize);
                break;
            }
            case UI_ELEMENT_IMAGE: {
                ImageData_t* imageData = (ImageData_t*)element->data;
                OLED_ShowImage(element->x, element->y, element->width, 
                             element->height, imageData->imageData);
                break;
            }
            case UI_ELEMENT_RECTANGLE: {
                RectangleData_t* rectData = (RectangleData_t*)element->data;
                OLED_DrawRectangle(element->x, element->y, element->width, 
                                 element->height, rectData->filled);
                break;
            }
            default:
                break;
        }
        
        element->needUpdate = 0;
    }
    
    // 更新OLED显示
    if (g_uiManager.currentPage.updateMode == UI_UPDATE_FULL) {
        OLED_Update();
    }
    
    g_uiManager.currentPage.needRefresh = 0;
    g_uiManager.lastUpdateTime = currentTime;
}

/**
 * @brief 设置更新模式
 */
void UIManager_SetUpdateMode(UIUpdateMode_t mode)
{
    g_uiManager.currentPage.updateMode = mode;
}

/**
 * @brief 设置更新间隔
 */
void UIManager_SetUpdateInterval(uint32_t interval)
{
    g_uiManager.updateInterval = interval;
}

/**
 * @brief 强制刷新
 */
void UIManager_ForceRefresh(void)
{
    g_uiManager.currentPage.needRefresh = 1;
    g_uiManager.pageChanged = 1;
}

/**
 * @brief 显示文本
 */
void UIManager_ShowText(int16_t x, int16_t y, char* text, uint8_t fontSize)
{
    static TextData_t textData;
    
    strncpy(textData.text, text, sizeof(textData.text) - 1);
    textData.text[sizeof(textData.text) - 1] = '\0';
    textData.fontSize = fontSize;
    
    UIManager_AddElement(UI_ELEMENT_TEXT, x, y, 0, 0, &textData);
}

/**
 * @brief 显示数字
 */
void UIManager_ShowNumber(int16_t x, int16_t y, uint32_t number, uint8_t length, uint8_t fontSize)
{
    static NumberData_t numberData;
    
    numberData.number = number;
    numberData.length = length;
    numberData.fontSize = fontSize;
    
    UIManager_AddElement(UI_ELEMENT_NUMBER, x, y, 0, 0, &numberData);
}

/**
 * @brief 显示图像
 */
void UIManager_ShowImage(int16_t x, int16_t y, uint8_t width, uint8_t height, const uint8_t* image)
{
    static ImageData_t imageData;
    
    imageData.imageData = image;
    
    UIManager_AddElement(UI_ELEMENT_IMAGE, x, y, width, height, &imageData);
}

/**
 * @brief 显示矩形
 */
void UIManager_ShowRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t filled)
{
    static RectangleData_t rectData;
    
    rectData.filled = filled;
    
    UIManager_AddElement(UI_ELEMENT_RECTANGLE, x, y, width, height, &rectData);
}
