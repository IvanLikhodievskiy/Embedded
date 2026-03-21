#include "main.h"

TIM_HandleTypeDef    TimHandle;
uint32_t uwPrescalerValue = 0;

static uint8_t  freq_index  = 0;
static volatile uint32_t timer_count = 0;
static uint32_t on_ticks  = 500;
static uint32_t off_ticks = 500;
static uint8_t  led_state = 1;

static void MPU_Config(void);
void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void UpdateLedTiming(void);

int main(void)
{
  MPU_Config();
  CPU_CACHE_Enable();
  HAL_Init();
  SystemClock_Config();

  BSP_LED_Init(LED1);
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  GPIO_InitTypeDef gpio = {0};
  gpio.Pin  = KEY_BUTTON_PIN;
  gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
  gpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY_BUTTON_GPIO_PORT, &gpio);

  uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

  TimHandle.Instance               = TIMx;
  TimHandle.Init.Period            = 10 - 1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    Error_Handler();

  if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    Error_Handler();

  BSP_LED_On(LED1);

  while (1)
  {
  }
}

static void UpdateLedTiming(void)
{
  const uint32_t period_ms[5] = {1000, 500, 250, 125, 62};
  uint32_t T = period_ms[freq_index];
  on_ticks  = T / 2;
  off_ticks = T / 2;
  timer_count = 0;
  led_state   = 1;
  BSP_LED_On(LED1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIMx)
    return;

  timer_count++;

  if (led_state == 1 && timer_count >= on_ticks)
  {
    BSP_LED_Off(LED1);
    led_state   = 0;
    timer_count = 0;
  }
  else if (led_state == 0 && timer_count >= off_ticks)
  {
    BSP_LED_On(LED1);
    led_state   = 1;
    timer_count = 0;
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin != KEY_BUTTON_PIN)
    return;

  if (BSP_PB_GetState(BUTTON_KEY) == RESET)
  {
    freq_index = (freq_index + 1U) % 5U;
    UpdateLedTiming();
  }
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK) { while(1); }

  ret = HAL_PWREx_EnableOverDrive();
  if (ret != HAL_OK) { while(1); }

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if (ret != HAL_OK) { while(1); }
}

static void Error_Handler(void)
{
  BSP_LED_On(LED1);
  while (1) {}
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  HAL_MPU_Disable();

  MPU_InitStruct.Enable            = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress       = 0x00;
  MPU_InitStruct.Size              = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission  = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable      = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable       = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable       = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number            = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField      = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable  = 0x87;
  MPU_InitStruct.DisableExec       = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void CPU_CACHE_Enable(void)
{
  SCB_EnableICache();
  SCB_EnableDCache();
}
