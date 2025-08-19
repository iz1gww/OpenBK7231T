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

#define RF2148_CMD_75P    0x02
#define RF2148_CMD_50P    0x03
#define RF2148_CMD_25P    0x04
#define RF2148_CMD_DOWN   0x07
#define RF2148_CMD_UP     0x09
#define RF2148_CMD_STOP   0x0A
#define RFRemote2148AMC_PACKET_SIZE (3)


const char* RFRemote2148AMc_GetCmdTypeString(int codeByte) {
	if (codeByte == RF2148_CMD_75P)
  {
    addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: 75% command received.\n");
    return "75P";
  }
	else if (codeByte == RF2148_CMD_50P)
  {
    addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: 50% command received.\n");
    return "50P";
  }
	else if (codeByte == RF2148_CMD_25P)
  {
    addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: 25% command received.\n");
    return "25P";
  }
  else if (codeByte == RF2148_CMD_DOWN)
  {
		addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: DOWN command received.\n");
    return "DWN";
  }
	else if (codeByte == RF2148_CMD_UP)
  {
    addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: UP command received.\n");
    return "UP";
  }
	else if (codeByte == RF2148_CMD_STOP)
  {
		addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: STOP command received.\n");
    return "STP";
  }
  else
  {
    addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "%s is not a valid codes command\n", codeByte);
    return "UNK";
  }
	
}

void RFRemote2148AMc_Init() {
  UART_InitUART(g_baudRate, 0, false);
  UART_InitReceiveRingBuffer(1024);
}

void RFRemote2148AMc_OnEverySecond() {

}
void RFRemote2148AMc_AppendInformationToHTTPIndexPage(http_request_t* request) {
  hprintf255(request, "<h4>RF Remote 2148Amc is running!</h4>");

}
void RFRemote2148AMc_QuickTick() {

}
void RFRemote2148AMc_StopDriver() {

}

void RFRemote2148AMc_ProcessIncoming(const byte* data, int len)
{
 if (sizeof(data) !=3)
 {
   	addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: packet data size is different from 3 bytes\n");
		return;
 }
 if (data[0] != 0x55 || data[1] != 0xA5) {
		addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "ProcessIncoming: header bytes values not correct %s\n", data);
		return;
	} 

  return RFRemote2148AMc_GetCmdTypeString(data[2]);
  
}



void RFRemote2148AMc_PrintPacket(byte *data, int len) {
	int i;
	char buffer_for_log[256];
	char buffer2[4];
	
			buffer_for_log[0] = 0;
			for (i = 0; i < len; i++) {
				snprintf(buffer2, sizeof(buffer2), "%02X ", data[i]);
				strcat_safe(buffer_for_log, buffer2, sizeof(buffer_for_log));
			}
			addLogAdv(LOG_INFO, LOG_FEATURE_RFREMOTE2148AMC, "Received: %s\n", buffer_for_log);
#if 1
			// redo sprintf without spaces
			buffer_for_log[0] = 0;
			for (i = 0; i < len; i++) {
				snprintf(buffer2, sizeof(buffer2), "%02X", data[i]);
				strcat_safe(buffer_for_log, buffer2, sizeof(buffer_for_log));
			}
			// fire string event, as we already have it sprintfed
			// This is so we can have event handlers that fire
			// when an UART string is received...
			EventHandlers_FireEvent_String(CMD_EVENT_ON_UART, buffer_for_log);
#endif
}

void RFRemote2148AMc_RunReceive() {
	byte data[192];
	int len;
	while (1)
	{
		len = UART_TryToGetNextTuyaPacket(data, sizeof(data));
		if (len > 0) {
			RFRemote2148AMc_PrintPacket(data,len);
			RFRemote2148AMc_ProcessIncoming(data, len);
		}
		else {
			break;
		}
	}
}

commandResult_t RFRemote2148AMc_SetBaudRate(const void* context, const char* cmd, const char* args, int cmdFlags) {
	Tokenizer_TokenizeString(args, 0);
	// following check must be done after 'Tokenizer_TokenizeString',
	// so we know arguments count in Tokenizer. 'cmd' argument is
	// only for warning display
	if (Tokenizer_CheckArgsCountAndPrintWarning(cmd, 1)) {
		return CMD_RES_NOT_ENOUGH_ARGUMENTS;
	}

	g_baudRate = Tokenizer_GetArgInteger(0);
	UART_InitUART(g_baudRate, 0, false);

	return CMD_RES_OK;
}
