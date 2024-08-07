
#ifndef TASK_UI_H_
#define TASK_UI_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
typedef enum
{
  BUTTON_TYPE_NONE,
  BUTTON_TYPE_PULSE,
  BUTTON_TYPE_SHORT,
  BUTTON_TYPE_LONG,
} button_type_t;

typedef struct
{
	QueueHandle_t hqueue_btn;
} ao_btn_handle_t;
/********************** external data declaration ****************************/
/********************** external functions declaration ***********************/

void task_ui(void* argument);
bool oa_ui_send_msg(ao_btn_handle_t* hao, button_type_t msg);
void ao_btn_init(ao_btn_handle_t* hao);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_UI_H_ */
/********************** end of file ******************************************/

