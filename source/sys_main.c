#include "sys_common.h"
#include "sys_core.h"
#include "system.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* USER CODE BEGIN (1) */
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "os_task.h"


/* USER CODE BEGIN (1) */
#include "sci.h"

xTaskHandle xTask1Handle;


#define  TSIZE1 10
uint8  TEXT1[TSIZE1]= {'H','E','R','C','U','L','E','S',' ',' '};
#define  TSIZE2 18
uint8  TEXT2[TSIZE2]= {'M','I','C','R','O','C','O','N','T','R','O','L','L','E','R','S',' ',' '};
#define  TSIZE3 19
uint8  TEXT3[TSIZE3]= {'T','E','X','A','S',' ','I','N','S','T','R','U','M','E','N','T','S','\n','\r'};
uint8 TX_Data_Slave[8]={0xF0,0xF0,0XF0,0XF0,0XF0,0xF0,0xF0,0xF0};
static uint8_t command;

void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 length);
void wait(uint32 time);


#define UART scilinREG
/* USER CODE END */

/* USER CODE BEGIN (2) */
/* USER CODE END */

void vTask1(void *pvParameters)
{
    for(;;)
    {
        sciReceive(UART, 1, (uint8_t*)&command);
        vTaskDelay(100);
    }
}

void main(void)
{
/* USER CODE BEGIN (3) */

    sciInit();      /* initialize sci/sci-lin    */
                    /* even parity , 2 stop bits */
    _enable_IRQ();
    //_enable_interrupt_();
    //in rtos
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
    vTaskStartScheduler();
    while(1);
    //in rtos

//in normal without rtos
//    while(1)        /* continious desplay        */
//    {
////      sciDisplayText(UART,&TEXT1[0],TSIZE1);   /* send text code 1 */
////      sciDisplayText(UART,&TEXT2[0],TSIZE2);   /* send text code 2 */
////      sciDisplayText(UART,&TEXT3[0],TSIZE3);   /* send text code 3 */
//        sciReceive(UART, 1, (uint8_t*)&command);
//      wait(1000);
//    };
    //in normal without rtos

/* USER CODE END */
}

void sciNotification(sciBASE_t *sci,unsigned flags){
    // Respond with the received byte
    sciSend(scilinREG, 1, (uint8_t*)&command);

    // Enable listen to SCI port in interrupt mode
    sciReceive(scilinREG, 1, (uint8_t*)&command);
    // commenting out sciReceive() gives the same result on the console: this
    // function is called in a loop from sciNotification() i.e. the interrupt
    // is not cleared
    sciSend(scilinREG, 8, TX_Data_Slave);

}
/* USER CODE BEGIN (4) */
void sciDisplayText(sciBASE_t *sci, uint8 *text,uint32 length)
{
    while(length--)
    {
        while ((UART->FLR & 0x4) == 4); /* wait until busy */
        sciSendByte(UART,*text++);      /* send out text   */
    };
}


void wait(uint32 time)
{
    time--;
}
/* USER CODE END */



