
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
#define ESPERAR_                 (3000)
#define QUEUE_LENGTH_            (10)
#define QUEUE_ITEM_SIZE_         (sizeof(button_type_t))
/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
int k1,k2,aux;
int m = 1;
int n = 10;
int lista[] = {0};
int estado[] = {0,0,0,0,0,0,0,0,0,0};

/********************** external data definition *****************************/
extern ao_led_handle_t ao_led;
/********************** internal functions definition ************************/

void task_priority_(void *argument)
{
	ao_priority_handle_t* hao = (ao_priority_handle_t*)argument;
	while (true)
	{
		led_priority_t msg;
		led_color_t led_color;

		while(pdPASS == xQueueReceive(hao->hqueue_priority, &msg, 0))
		{
			//ordenamos por prioridades los mensajes
			for(k1 = 0; k1<m; k1++)
			{
				lista[k1] = msg;
				for(k2 = 0; k2<n; k2++)
				{
					if(lista[k1]>estado[k2])
					{
						aux = lista[k1];
						lista[k1] = estado[k2];
						estado[k2] = aux;
					}
				}
			}

			//espera un tiempo y si no detecta que se envian mas pulsos se procede
			//a mandar los estados almacenados que se ordenaron por prioridad
			vTaskDelay((TickType_t)(ESPERAR_ / portTICK_PERIOD_MS));
			if( uxQueueMessagesWaiting( hao->hqueue_priority ) == 0)
			{
				LOGGER_INFO("priority queue activate");
				for(k2 = 0; k2<n; k2++)
				{
					switch (estado[k2]) {
					case NO_PRIORITY:
						break;
					case HIGH_PRIORITY:
						led_color = LED_COLOR_RED;
						if(oa_led_send_msg(&ao_led, led_color))
						{
							estado[k2] = 0;
						} else {
							LOGGER_INFO("Mensaje perdido led red");
							estado[k2] = 0;
						}
						break;
					case MEDIUM_PRIORITY:
						led_color = LED_COLOR_GREEN;
						if(oa_led_send_msg(&ao_led, led_color))
						{
							estado[k2] = 0;
						} else {
							LOGGER_INFO("Mensaje perdido led green");
							estado[k2] = 0;;
						}
						break;
					case LOW_PRIORITY:
						led_color = LED_COLOR_BLUE;
						if(oa_led_send_msg(&ao_led, led_color))
						{
							estado[k2] = 0;
						} else {
							LOGGER_INFO("Mensaje perdido led blue");
							estado[k2] = 0;
						}
						break;
					default:
						LOGGER_INFO("priority_queue error");
						break;
					}
				}
				LOGGER_INFO("se envio el paquete de datos para lectura");
			}
		}
	}
}

/********************** external functions definition ************************/
bool priority_msg(ao_priority_handle_t* hao , led_priority_t msg)
{
	if( uxQueueMessagesWaiting( hao->hqueue_priority ) != 10 )
	{
		return(pdPASS == xQueueSend(hao->hqueue_priority, (void*)&msg, 0));
	}
	LOGGER_INFO("No hay más lugar para recibir msg de prioridad");
	return false;
}

void ao_priority_init(ao_priority_handle_t* hao)
{
	hao->hqueue_priority = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
	while(NULL == hao->hqueue_priority)
	{
		// error
	}

	vQueueAddToRegistry(hao->hqueue_priority, "Queue Handle priority");

	BaseType_t status;
	status = xTaskCreate(task_priority_, "task_priority", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
	while (pdPASS != status)
	{
		// error
	}
}

/********************** end of file ******************************************/
