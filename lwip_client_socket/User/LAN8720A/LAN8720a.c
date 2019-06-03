
/* Includes ------------------------------------------------------------------*/
#include "LAN8720A.h"
#include "debug.h"

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/**
  * @brief  ������̫���ӿ�
  * @param  None
  * @retval None
  */
void ETH_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* ʹ�ܶ˿�ʱ�� */
    ETH_MDIO_GPIO_CLK_ENABLE();
    ETH_MDC_GPIO_CLK_ENABLE();
    ETH_RMII_REF_CLK_GPIO_CLK_ENABLE();
    ETH_RMII_CRS_DV_GPIO_CLK_ENABLE();
    ETH_RMII_RXD0_GPIO_CLK_ENABLE();
    ETH_RMII_RXD1_GPIO_CLK_ENABLE();
    ETH_RMII_TX_EN_GPIO_CLK_ENABLE();
    ETH_RMII_TXD0_GPIO_CLK_ENABLE();    
    ETH_RMII_TXD1_GPIO_CLK_ENABLE();
	
	/* ������̫������*/
    /*
    ETH_MDIO -------------------------> PA2
    ETH_MDC --------------------------> PC1
    ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
    ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
    ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
    ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
    ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PB11
    ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
    ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14
	*/
	
    /* ����ETH_MDIO���� */
    GPIO_InitStructure.Pin = ETH_MDIO_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = ETH_MDIO_AF;
    HAL_GPIO_Init(ETH_MDIO_PORT, &GPIO_InitStructure);

    /* ����ETH_MDC���� */
    GPIO_InitStructure.Pin = ETH_MDC_PIN;
    GPIO_InitStructure.Alternate = ETH_MDC_AF;
    HAL_GPIO_Init(ETH_MDC_PORT, &GPIO_InitStructure);

    /* ����ETH_RMII_REF_CLK���� */
    GPIO_InitStructure.Pin = ETH_RMII_REF_CLK_PIN;
    GPIO_InitStructure.Alternate = ETH_RMII_REF_CLK_AF;
    HAL_GPIO_Init(ETH_RMII_REF_CLK_PORT, &GPIO_InitStructure);

    /* ����ETH_RMII_CRS_DV���� */
    GPIO_InitStructure.Pin = ETH_RMII_CRS_DV_PIN;
    GPIO_InitStructure.Alternate = ETH_RMII_CRS_DV_AF;
    HAL_GPIO_Init(ETH_RMII_CRS_DV_PORT, &GPIO_InitStructure);

    /* ����ETH_RMII_RXD0���� */
    GPIO_InitStructure.Pin = ETH_RMII_RXD0_PIN;
    GPIO_InitStructure.Alternate = ETH_RMII_RXD0_AF;
    HAL_GPIO_Init(ETH_RMII_RXD0_PORT, &GPIO_InitStructure);

    /* ����ETH_RMII_RXD1���� */
    GPIO_InitStructure.Pin = ETH_RMII_RXD1_PIN;
    GPIO_InitStructure.Alternate = ETH_RMII_RXD1_AF;
    HAL_GPIO_Init(ETH_RMII_RXD1_PORT, &GPIO_InitStructure);

    /* ����ETH_RMII_TX_EN���� */
    GPIO_InitStructure.Pin = ETH_RMII_TX_EN_PIN;
    GPIO_InitStructure.Alternate = ETH_RMII_TX_EN_AF;
    HAL_GPIO_Init(ETH_RMII_TX_EN_PORT, &GPIO_InitStructure);

    /* ����ETH_RMII_TXD0���� */
    GPIO_InitStructure.Pin = ETH_RMII_TXD0_PIN;
    GPIO_InitStructure.Alternate = ETH_RMII_TXD0_AF;
    HAL_GPIO_Init(ETH_RMII_TXD0_PORT, &GPIO_InitStructure);

    /* ����ETH_RMII_TXD1���� */
    GPIO_InitStructure.Pin = ETH_RMII_TXD1_PIN;
    GPIO_InitStructure.Alternate = ETH_RMII_TXD1_AF;
    HAL_GPIO_Init(ETH_RMII_TXD1_PORT, &GPIO_InitStructure);	
	
	/* ʹ����̫��ȫ���ж� */
	HAL_NVIC_SetPriority(ETH_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(ETH_IRQn);
}


extern ETH_HandleTypeDef EthHandle;
void ETH_IRQHandler(void)
{
  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
  HAL_ETH_IRQHandler(&EthHandle);
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
extern xSemaphoreHandle s_xSemaphore;
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
//  LED2_TOGGLE;
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
  ;
}

void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *heth)
{
    PRINT_ERR("eth err\n");
}






