#include "../new_common.h"
#include "../new_pins.h"
#include "../new_cfg.h"
// Commands register, execution API and cmd tokenizer
#include "../cmnds/cmd_public.h"
#include "../mqtt/new_mqtt.h"
#include "../logging/logging.h"
#include "../hal/hal_pins.h"
#include "drv_public.h"
#include "drv_local.h"
#include "drv_uart.h"
#include "../httpserver/new_http.h"

void NTPSwitch_Init() {

}
void NTPSwitch_OnEverySecond() {

}
void NTPSwitch_AppendInformationToHTTPIndexPage(http_request_t* request) {
  hprintf255(request, "<h4> is running!</h4>");

}
void NTPSwitch_QuickTick() {

}
void NTPSwitch_StopDriver() {

}
