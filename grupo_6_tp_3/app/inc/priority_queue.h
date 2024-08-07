
#ifndef INC_PRIORITY_QUEUE_H_
#define INC_PRIORITY_QUEUE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef enum
{
  NO_PRIORITY,
  LOW_PRIORITY,
  MEDIUM_PRIORITY,
  HIGH_PRIORITY,
} led_priority_t;

typedef struct
{
	QueueHandle_t hqueue_priority;
} ao_priority_handle_t;
/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
void task_priority_(void *argument);
bool priority_msg(ao_priority_handle_t* hao , led_priority_t msg);
void ao_priority_init(ao_priority_handle_t* hao);
/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* INC_PRIORITY_QUEUE_H_ */
/********************** end of file ******************************************/
