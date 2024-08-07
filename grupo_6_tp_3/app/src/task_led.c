
/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "task_led.h"
#include "task_ui.h"

/********************** macros and definitions *******************************/
#define TASK_PERIOD_LED_MS_                      (5000)

#define QUEUE_LENGTH_            (20)
#define QUEUE_ITEM_SIZE_         (sizeof(led_color_t))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/
extern ao_led_handle_t ao_led;
/********************** internal functions definition ************************/


/********************** external functions definition ************************/

void task_led_(void *argument)
{
	ao_led_handle_t* hao = (ao_led_handle_t*)argument;
	while (true)
	{
		led_color_t msg;
		while (pdPASS == xQueueReceive(hao->hqueue_led, (void*)&msg, 0))
		{
			switch (msg)
			{
			case LED_COLOR_RED:
				LOGGER_INFO("led red");
				HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
				vTaskDelay((TickType_t)(TASK_PERIOD_LED_MS_ / portTICK_PERIOD_MS));
				HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
				break;
			case LED_COLOR_GREEN:
				LOGGER_INFO("led green");
				HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
				vTaskDelay((TickType_t)(TASK_PERIOD_LED_MS_ / portTICK_PERIOD_MS));
				HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
				break;
			case LED_COLOR_BLUE:
				LOGGER_INFO("led blue");
				HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_SET);
				vTaskDelay((TickType_t)(TASK_PERIOD_LED_MS_ / portTICK_PERIOD_MS));
				HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
				break;
			default:
				break;
			}
		}
	}
}

/********************** external functions definition ************************/
bool oa_led_send_msg(ao_led_handle_t* hao , led_color_t msg)
{
	if( uxQueueMessagesWaiting( hao->hqueue_led ) != 20 )
	{
		return(pdPASS == xQueueSend(hao->hqueue_led, (void*)&msg, 0));
	}
	LOGGER_INFO("No hay más lugar para recibir msg de estado del led");
	return false;
}

void ao_led_init(ao_led_handle_t* hao)
{
	hao->hqueue_led = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
	while(NULL == hao->hqueue_led)
	{
		// error
	}

	vQueueAddToRegistry(hao->hqueue_led, "Queue Handle led");

	BaseType_t status;
	status = xTaskCreate(task_led_, "task_led", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
	if(pdPASS != status)
	{
		// error
	}

}

/********************** end of file ******************************************/
