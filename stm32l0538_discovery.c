/**
  ******************************************************************************
  * @file    stm32l0538_discovery.c
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on STM32L0538-DISCO Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0538_discovery.h"
#include "main.h"
/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32L0538_DISCOVERY
  * @{
  */

/** @defgroup STM32L0538_DISCOVERY_LOW_LEVEL
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32L0538-Discovery Kit from STMicroelectronics.
  * @{
  */

/** @defgroup STM32L0538_DISCOVERY_LOW_LEVEL_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32L0538_DISCOVERY_LOW_LEVEL_Private_Defines
  * @{
  */

  /**
  * @brief STM32L0538 DISCOVERY BSP Driver version number
  */
#define __STM32L0538_DISCO_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32L0538_DISCO_BSP_VERSION_SUB1   (0x03) /*!< [23:16] sub1 version */
#define __STM32L0538_DISCO_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32L0538_DISCO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32L0538_DISCO_BSP_VERSION        ((__STM32L0538_DISCO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32L0538_DISCO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32L0538_DISCO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32L0538_DISCO_BSP_VERSION_RC))
   
/**
  * @}
  */

/** @defgroup STM32L0538-DISCOVERY LOW_LEVEL_Private_Macros
  * @{
  */
/**
  * @}
  */


static SPI_HandleTypeDef SpiHandle;

/**
  * @}
  */

/** @defgroup STM32L0538_DISCOVERY LOW_LEVEL_Private_FunctionPrototypes
  * @{
  */
static void               SPIx_Init(void);
static void               SPIx_Write(uint8_t Value);
static uint32_t           SPIx_Read(void);
static void               SPIx_Error(void);
static void               SPIx_MspInit(SPI_HandleTypeDef *hspi);

/* Link functions for EPD peripheral */
void                      EPD_IO_Init(void);
void                      EPD_IO_WriteData(uint16_t RegValue);
void                      EPD_IO_WriteReg(uint8_t Reg);
uint16_t                  EPD_IO_ReadData(void);
void                      EPD_Delay(uint32_t delay);
void                      EPD_ReadBusy(void);
void                      EPD_IO_Reset_Low(void);
void                      EPD_IO_Reset_High(void);
/**
  * @}
  */

/** @defgroup STM32L0538_DISCOVERY LOW_LEVEL_Private_Functions
  * @{
  */

/**
  * @brief  This method returns the STM32L0538 DISCO BSP Driver revision
  * @param  None
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32L0538_DISCO_BSP_VERSION;
}



/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* SPI Routines *********************************/
/**
  * @brief  SPIx Bus initialization
  * @param  None
  * @retval None
  */
static void SPIx_Init(void)
{

	SpiHandle = hspi1;
/*
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
	/*
    SpiHandle.Instance = DISCOVERY_SPIx;
    
    /* On STM32L0538-DISCO, EPD ID cannot be read then keep a common configuration */
    /* for EPD (SPI_DIRECTION_2LINES) */
    /* Note: To read a register a EPD, SPI_DIRECTION_1LINE should be set */
	/*
    SpiHandle.Init.Mode               = SPI_MODE_MASTER;
    SpiHandle.Init.Direction          = SPI_DIRECTION_2LINES;
    SpiHandle.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_8;
    SpiHandle.Init.DataSize           = SPI_DATASIZE_8BIT;
    SpiHandle.Init.CLKPhase           = SPI_PHASE_2EDGE;
    SpiHandle.Init.CLKPolarity        = SPI_POLARITY_HIGH;
    SpiHandle.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS                = SPI_NSS_SOFT;
    SpiHandle.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial      = 7;
    SpiHandle.Init.TIMode             = SPI_TIMODE_DISABLE;
    
    SPIx_MspInit(&SpiHandle);
    HAL_SPI_Init(&SpiHandle);

  }*/
}

/**
  * @brief  SPI Read 4 bytes from device.
  * @param  ReadSize Number of bytes to read (max 4 bytes)
  * @retval Value read on the SPI
  */
static uint32_t SPIx_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0;
  uint32_t writevalue = 0xFFFFFFFF;
  
  status = HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SPIx_TIMEOUT_MAX);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the BUS */
    SPIx_Error();
  }

  return readvalue;
}

/**
  * @brief  SPI Write a byte to device.
  * @param  Value: value to be written
  * @retval None
  */
static void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&SpiHandle, (uint8_t*) &Value, 1, SPIx_TIMEOUT_MAX);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the BUS */
    SPIx_Error();
  }
}

/**
  * @brief  SPI error treatment function.
  * @param  None
  * @retval None
  */
static void SPIx_Error (void)
{
  /* De-Initialize the SPI communication BUS */
  HAL_SPI_DeInit(&SpiHandle);

  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}

/**
  * @brief  SPI MSP Init
  * @param  hspi: SPI handle
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{

	/*
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
	/*
  DISCOVERY_SPIx_GPIO_CLK_ENABLE();

  /* Configure SPI SCK */
	/*
  GPIO_InitStruct.Pin = DISCOVERY_SPIx_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DISCOVERY_SPIx_AF;
  HAL_GPIO_Init(DISCOVERY_SPIx_GPIO_PORT, &GPIO_InitStruct);

  /* Configure SPI MOSI */
	/*
  GPIO_InitStruct.Pin = DISCOVERY_SPIx_MOSI_PIN;
  GPIO_InitStruct.Alternate = DISCOVERY_SPIx_AF;
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
  HAL_GPIO_Init(DISCOVERY_SPIx_GPIO_PORT, &GPIO_InitStruct);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */

  //DISCOVERY_SPIx_CLK_ENABLE();
}

/********************************* LINK EPD ***********************************/

/**
  * @brief  Configures the EPD SPI interface.
  * @param  None
  * @retval None
  */
void EPD_IO_Init(void)
{
  /* Enable Display */
  EPD_PWR_LOW();

  /* Set or Reset the control line */
  EPD_CS_LOW();
  EPD_CS_HIGH();

  /* EPD reset pin management */
  EPD_RESET_HIGH();
  EPD_Delay(10);

  /* SPI Configuration */
  SPIx_Init();
}

/**
  * @brief  Write register value.
  * @param  None
  * @retval None
  */
void EPD_IO_WriteData(uint16_t RegValue) 
{
  /* Reset EPD control line CS */
  EPD_CS_LOW();

  /* Set EPD data/command line DC to High */
  EPD_DC_HIGH();

  /* Send Data */
  SPIx_Write(RegValue);

  /* Deselect: Chip Select high */
  EPD_CS_HIGH();
}

/**
  * @brief  Writes command to selected EPD register.
  * @param  Reg: Address of the selected register.
  * @retval None
  */
void EPD_IO_WriteReg(uint8_t Reg)
{
  /* Reset EPD control line CS */
  EPD_CS_LOW();
  
  /* Set EPD data/command line DC to Low */
  EPD_DC_LOW();

  /* Send Command */
  SPIx_Write(Reg);

  /* Deselect: Chip Select high */
  EPD_CS_HIGH();
}

/**
  * @brief  Reads data from EPD data register.
  * @param  None
  * @retval Read data.
  */
uint16_t EPD_IO_ReadData(void)
{
  /* Reset EPD control line CS */
  EPD_CS_LOW();

  /* Deselect: Chip Select high */
  EPD_CS_HIGH();

  /* Send Data */
  return SPIx_Read();
}

/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  * @retval None
  */
void EPD_Delay (uint32_t Delay)
{
  HAL_Delay (Delay);
}

/**
  * @brief  wait for the EPD to be ready.
  * @param  None.
  * @retval None
  */
void EPD_ReadBusy(void)
{ 
  while(1)
  {	
    /* BUSY = 1 */
    if(isEPD_BUSY() == 0)
    {
      break;
    }
  }
}

/**
  * @brief  EPD Reset Low.
  * @param  None.
  * @retval None
  */
void EPD_IO_Reset_Low()
{
  EPD_RESET_LOW();
}

/**
  * @brief  EPD Reset High.
  * @param  None.
  * @retval None
  */
void EPD_IO_Reset_High()
{
  EPD_RESET_HIGH();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */
