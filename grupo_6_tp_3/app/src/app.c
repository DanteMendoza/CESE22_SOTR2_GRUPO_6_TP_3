
/********************** inclusions *******************************************/

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"

#include "app.h"
#include "task_button.h"
#include "task_ui.h"
#include "task_led.h"
#include "priority_queue.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration *****************************/

ao_btn_handle_t ao_btn;
ao_priority_handle_t ao_priority;
ao_led_handle_t ao_led;

/********************** external functions definition ************************/
void app_init(void)
{
  ao_btn_init(&ao_btn); //inicializo objeto activo para el boton
  ao_priority_init(&ao_priority); //inicializo objeto activo para la priority queue
  ao_led_init(&ao_led); //inicializo el objeto activo para el led

  BaseType_t status;

  status = xTaskCreate(task_button, "task_button", 128, NULL, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status)
  {
    // error
  }


  LOGGER_INFO("app init");

  cycle_counter_init();
}

/********************** end of file ******************************************/
