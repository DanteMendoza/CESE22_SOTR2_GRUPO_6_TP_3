
#ifndef TASK_LED_H_
#define TASK_LED_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
typedef enum
{
  LED_COLOR_RED,
  LED_COLOR_GREEN,
  LED_COLOR_BLUE,
} led_color_t;

typedef struct
{
	QueueHandle_t hqueue_led;
} ao_led_handle_t;
/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
void task_led_(void *argument);
bool oa_led_send_msg(ao_led_handle_t* hao , led_color_t msg);
void ao_led_init(ao_led_handle_t* hao);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H_ */
/********************** end of file ******************************************/

