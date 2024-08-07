
/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "task_ui.h"
#include "task_led.h"
#include "priority_queue.h"

/********************** macros and definitions *******************************/
#define QUEUE_LENGTH_            (10)
#define QUEUE_ITEM_SIZE_         (sizeof(button_type_t))
/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/
extern ao_btn_handle_t ao_btn;
extern ao_priority_handle_t ao_priority;
/********************** internal functions definition ************************/

void task_ui(void *argument)
{
	ao_btn_handle_t* hao = (ao_btn_handle_t*)argument;
	while (true)
	{
		button_type_t msg;
		led_priority_t priority;

		while(pdPASS == xQueueReceive(hao->hqueue_btn, &msg, 0))
		{
			switch (msg) {
			case BUTTON_TYPE_PULSE:
				priority = HIGH_PRIORITY;
				priority_msg(&ao_priority, priority);
				break;
			case BUTTON_TYPE_SHORT:
				priority = MEDIUM_PRIORITY;
				priority_msg(&ao_priority, priority);
				break;
			case BUTTON_TYPE_LONG:
				priority = LOW_PRIORITY;
				priority_msg(&ao_priority, priority);
				break;
			default:
				LOGGER_INFO("task_ui error");
				break;
			}

		}
	}
}

/********************** external functions definition ************************/
bool oa_ui_send_msg(ao_btn_handle_t* hao, button_type_t msg)
{
	if( uxQueueMessagesWaiting( hao->hqueue_btn ) != 10 )
	{
	return (pdPASS == xQueueSend(hao->hqueue_btn, (void*)&msg, 0));
	}
	LOGGER_INFO("No hay más lugar para recibir estados de los botones");
	return false;
}

void ao_btn_init(ao_btn_handle_t* hao)
{
	hao->hqueue_btn = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
	while(NULL == hao->hqueue_btn)
	{
		// error
	}

	vQueueAddToRegistry(hao->hqueue_btn, "Queue Handle btn");

	BaseType_t status;
	status = xTaskCreate(task_ui, "task_ui", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
	while (pdPASS != status)
	{
		// error
	}
}

/********************** end of file ******************************************/
