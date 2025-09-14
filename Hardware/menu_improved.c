#include "menu_improved.h"
#include "ui_manager.h"
#include "app_state.h"
#include "error_handler.h"
static AppEvent_t ConvertKeyToAppEvent(KeyType_t keyEvent);
// 全局菜单管理器实例
static MenuManager_t g_menuManager;

/**
 * @brief 初始化菜单管理器
 */
void MenuManager_Init(void)
{
    uint8_t i, j;
    
    // 初始化管理器
    g_menuManager.pageCount = 0;
    g_menuManager.currentPage = 0;
    g_menuManager.needRefresh = 1;
    
    // 初始化页面
    for (i = 0; i < 4; i++) {
        g_menuManager.pages[i].itemCount = 0;
        g_menuManager.pages[i].currentIndex = 0;
        strcpy(g_menuManager.pages[i].title, "");
        
        // 初始化菜单项
        for (j = 0; j < 8; j++) {
            strcpy(g_menuManager.pages[i].items[j].name, "");
            g_menuManager.pages[i].items[j].type = MENU_ITEM_NORMAL;
            g_menuManager.pages[i].items[j].data = NULL;
            g_menuManager.pages[i].items[j].targetState = APP_STATE_CLOCK;
            g_menuManager.pages[i].items[j].enabled = 0;
        }
    }
}

/**
 * @brief 添加菜单页面
 */
void MenuManager_AddPage(const char* title)
{
    if (g_menuManager.pageCount >= 4) {
        REPORT_ERROR(ERROR_OUT_OF_RANGE, ERROR_LEVEL_ERROR, MODULE_ID_UI, "Too many menu pages");
        return;
    }
    
    MenuPage_t* page = &g_menuManager.pages[g_menuManager.pageCount];
    strncpy(page->title, title, sizeof(page->title) - 1);
    page->title[sizeof(page->title) - 1] = '\0';
    
    g_menuManager.pageCount++;
}

/**
 * @brief 添加菜单项
 */
void MenuManager_AddItem(const char* name, MenuItemType_t type, 
                        AppState_t targetState, void* data)
{
    if (g_menuManager.pageCount == 0) {
        REPORT_ERROR(ERROR_INVALID_PARAM, ERROR_LEVEL_ERROR, MODULE_ID_UI, "No menu page available");
        return;
    }
    
    MenuPage_t* currentPage = &g_menuManager.pages[g_menuManager.pageCount - 1];
    
    if (currentPage->itemCount >= 8) {
        REPORT_ERROR(ERROR_OUT_OF_RANGE, ERROR_LEVEL_ERROR, MODULE_ID_UI, "Too many menu items");
        return;
    }
    
    MenuItem_t* item = &currentPage->items[currentPage->itemCount];
    
    strncpy(item->name, name, sizeof(item->name) - 1);
    item->name[sizeof(item->name) - 1] = '\0';
    item->type = type;
    item->targetState = targetState;
    item->data = data;
    item->enabled = 1;
    
    currentPage->itemCount++;
}

/**
 * @brief 更新菜单
 */
void MenuManager_Update(void)
{
    if (g_menuManager.needRefresh) {
        MenuManager_RefreshDisplay();
        g_menuManager.needRefresh = 0;
    }
}

/**
 * @brief 处理按键
 */
void MenuManager_ProcessKey(KeyType_t key)
{
    switch (key) {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_CONFIRM:
            MenuManager_HandleNavigation(key);
            break;
        default:
            break;
    }
}

/**
 * @brief 显示菜单
 */
void MenuManager_Show(void)
{
    g_menuManager.needRefresh = 1;
    MenuManager_Update();
}

/**
 * @brief 刷新显示
 */
static void MenuManager_RefreshDisplay(void)
{
    MenuPage_t* page = &g_menuManager.pages[g_menuManager.currentPage];
    uint8_t i;
    
    // 清除UI
    UIManager_ClearPage();
    
    // 显示标题
    UIManager_ShowText(0, 0, page->title, OLED_8X16);
    
    // 显示菜单项
    for (i = 0; i < page->itemCount; i++) {
        MenuItem_t* item = &page->items[i];
        uint8_t y = 16 + i * 8;
        
        if (item->enabled) {
            // 显示菜单项名称
            UIManager_ShowText(8, y, item->name, OLED_6X8);
            
            // 高亮当前选中项
            if (i == page->currentIndex) {
                UIManager_ShowRectangle(0, y, 128, 8, OLED_UNFILLED);
            }
            
            // 根据类型显示额外信息
            switch (item->type) {
                case MENU_ITEM_TOGGLE: {
                    uint8_t* value = (uint8_t*)item->data;
                    UIManager_ShowText(100, y, *value ? "ON" : "OFF", OLED_6X8);
                    break;
                }
                case MENU_ITEM_VALUE: {
                    uint32_t* value = (uint32_t*)item->data;
                    UIManager_ShowNumber(100, y, *value, 3, OLED_6X8);
                    break;
                }
                default:
                    break;
            }
        }
    }
    
    // 更新显示
    UIManager_Update();
}

/**
 * @brief 处理导航
 */
static void MenuManager_HandleNavigation(KeyType_t key)
{
    MenuPage_t* page = &g_menuManager.pages[g_menuManager.currentPage];
    
    switch (key) {
        case KEY_UP:
            if (page->currentIndex > 0) {
                page->currentIndex--;
            } else {
                page->currentIndex = page->itemCount - 1;
            }
            g_menuManager.needRefresh = 1;
            break;
            
        case KEY_DOWN:
            if (page->currentIndex < page->itemCount - 1) {
                page->currentIndex++;
            } else {
                page->currentIndex = 0;
            }
            g_menuManager.needRefresh = 1;
            break;
            
        case KEY_CONFIRM:
            MenuManager_HandleSelection();
            break;
            
        default:
            break;
    }
}

/**
 * @brief 处理选择
 */
static void MenuManager_HandleSelection(void)
{
    MenuPage_t* page = &g_menuManager.pages[g_menuManager.currentPage];
    MenuItem_t* item = &page->items[page->currentIndex];
    
    if (!item->enabled) {
        return;
    }
    
    switch (item->type) {
        case MENU_ITEM_NORMAL:
        case MENU_ITEM_SUBMENU:
            // 切换到目标状态
            AppStateMachine_ProcessEvent(EVENT_KEY_CONFIRM);
            break;
            
        case MENU_ITEM_TOGGLE: {
            uint8_t* value = (uint8_t*)item->data;
            *value = !(*value);
            g_menuManager.needRefresh = 1;
            break;
        }
        
        case MENU_ITEM_VALUE: {
            // 这里可以实现数值调整逻辑
            uint32_t* value = (uint32_t*)item->data;
            // 简单的数值递增
            (*value)++;
            g_menuManager.needRefresh = 1;
            break;
        }
        
        default:
            break;
    }
}
