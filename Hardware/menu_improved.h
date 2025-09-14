#ifndef __MENU_IMPROVED_H__
#define __MENU_IMPROVED_H__

#include "stm32f10x.h"
#include "app_state.h"
#include "modelconfig.h"
// 菜单项类型
typedef enum {
    MENU_ITEM_NORMAL,    // 普通菜单项
    MENU_ITEM_TOGGLE,    // 开关菜单项
    MENU_ITEM_VALUE,     // 数值菜单项
    MENU_ITEM_SUBMENU    // 子菜单项
} MenuItemType_t;

// 菜单项结构
typedef struct {
    char name[16];           // 菜单项名称
    MenuItemType_t type;     // 菜单项类型
    void* data;              // 菜单项数据
    AppState_t targetState;  // 目标状态
    uint8_t enabled;         // 是否启用
} MenuItem_t;

// 菜单页面结构
typedef struct {
    MenuItem_t items[8];    // 最多8个菜单项
    uint8_t itemCount;
    uint8_t currentIndex;
    char title[16];          // 菜单标题
} MenuPage_t;

// 菜单管理器
typedef struct {
    MenuPage_t pages[4];     // 最多4个页面
    uint8_t pageCount;
    uint8_t currentPage;
    uint8_t needRefresh;
} MenuManager_t;

// 函数声明
void MenuManager_Init(void);
void MenuManager_AddPage(const char* title);
void MenuManager_AddItem(const char* name, MenuItemType_t type, 
                        AppState_t targetState, void* data);
void MenuManager_Update(void);
void MenuManager_ProcessKey(KeyType_t key);
void MenuManager_Show(void);

// 内部函数
static void MenuManager_RefreshDisplay(void);
static void MenuManager_HandleNavigation(KeyType_t key);
static void MenuManager_HandleSelection(void);

#endif
