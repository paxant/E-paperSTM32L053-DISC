# E-paperSTM32L053-DISC
Библиотека была взята из стандартной библиотеки BSP от ST для микроконтроллера STM32L053.
Для использования библиотеки необходимо настроить проект таким образом:

#define EPD_RESET_PIN_Pin GPIO_PIN_2
#define EPD_RESET_PIN_GPIO_Port GPIOB
#define EPD_PWR_PIN_Pin GPIO_PIN_10
#define EPD_PWR_PIN_GPIO_Port GPIOB
#define EPD_DC_PIN_Pin GPIO_PIN_11
#define EPD_DC_PIN_GPIO_Port GPIOB
#define EPD_BUSY_PIN_Pin GPIO_PIN_8
#define EPD_BUSY_PIN_GPIO_Port GPIOA
#define EPD_CS_PIN_Pin GPIO_PIN_15
#define EPD_CS_PIN_GPIO_Port GPIOA

SPI1 настроенный на пины PB3, PB4, PB5.

В main.h необходимо добавить 
extern SPI_HandleTypeDef hspi1;

В main.c подключить библиотеки

#include "../../Drivers/BSP/stm32l0538_discovery_epd.h"
#include "../../Drivers/BSP/stm32l0538_discovery.h"
#include "../../Drivers/BSP/menu_res.c"

Инициализация:

BSP_EPD_Init();

Пример вывода изображения (например в цикле)
 
BSP_EPD_Clear(EPD_COLOR_WHITE);
BSP_EPD_SetFont(&Font12);
BSP_EPD_DrawImage(0, 0, 72, 172, Background_pic);
BSP_EPD_DisplayStringAt(0, 13, (uint8_t*)"STM32L053", RIGHT_MODE);
BSP_EPD_SetFont(&Font12);
BSP_EPD_DisplayStringAt(87, 8, (uint8_t*)"DISCOVERY", LEFT_MODE);
BSP_EPD_DisplayStringAt(71, 3, (uint8_t*)"KGRHFERIFG", LEFT_MODE);
BSP_EPD_DisplayStringAt(94, 0, (uint8_t*)"V1.0.0", LEFT_MODE);
BSP_EPD_RefreshDisplay();
HAL_Delay(2000);
