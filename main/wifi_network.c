

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include  "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>
#include "comm_server.h"
#include "lwip/sockets.h"
#include "driver/twai.h"
#include "config_server.h"
#include "realdash.h"
#include "slcan.h"
#include "can.h"
#include "ble.h"

static const char *WIFI_TAG = "wifi_network";
static esp_netif_t* ap_netif;
static esp_netif_t* sta_netif;


static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group = NULL;
#define WIFI_CONNECTED_BIT 			BIT0
#define WIFI_FAIL_BIT     			BIT1
#define WIFI_DISCONNECTED_BIT      	BIT2
#define WIFI_INIT_BIT     		 	BIT3
#define EXAMPLE_ESP_MAXIMUM_RETRY 	10
char sta_ip[20] = {0};

static const TickType_t connect_delay[] = {2000, 4000, 6000,8000,10000,10000};

static void wifi_network_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
	static int64_t last_try = 0;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
    	ESP_LOGI(WIFI_TAG, "WIFI_EVENT_STA_START");
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
    	ESP_LOGI(WIFI_TAG, "WIFI_EVENT_STA_DISCONNECTED");
//    	config_server_wifi_connected(0);

    	xEventGroupSetBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT);

		if((WIFI_INIT_BIT & xEventGroupGetBits(s_wifi_event_group)))
		{
	    	if(esp_timer_get_time() > (last_try + connect_delay[s_retry_num]*1000))
	    	{
	    		last_try = esp_timer_get_time();
				esp_wifi_connect();
				s_retry_num++;
				s_retry_num %= 6;
				ESP_LOGI(WIFI_TAG, "retry to connect to the AP");
	    	}
		}


    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
    	ESP_LOGI(WIFI_TAG, "IP_EVENT_STA_GOT_IP");
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(WIFI_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));

        sprintf(sta_ip, "%d.%d.%d.%d", IP2STR(&event->ip_info.ip));

        config_server_set_sta_ip(sta_ip);
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        xEventGroupClearBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT);
//        config_server_wifi_connected(1);
    }

    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
    	ESP_LOGI(WIFI_TAG, "WIFI_EVENT_AP_STACONNECTED");
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(WIFI_TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
    	ESP_LOGI(WIFI_TAG, "WIFI_EVENT_AP_STADISCONNECTED");
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(WIFI_TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if(event_id == WIFI_EVENT_AP_START)
    {
    	ESP_LOGI(WIFI_TAG, "WIFI_EVENT_AP_START");
    }
}

void wifi_network_deinit(void)
{
	xEventGroupClearBits(s_wifi_event_group, WIFI_INIT_BIT);
//	ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_network_event_handler));
//	ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_network_event_handler));
//	esp_netif_dhcps_stop(ap_netif);
//	esp_netif_destroy_default_wifi(ap_netif);
//	ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(sta_netif));
//	esp_netif_destroy(sta_netif);
//	esp_netif_destroy_default_wifi(sta_netif);
	ESP_LOGW(WIFI_TAG, "wifi deinit");
//    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_network_event_handler));
//    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_network_event_handler));
	esp_wifi_disconnect();
    esp_err_t err = esp_wifi_stop();

    if (err == ESP_ERR_WIFI_NOT_INIT)
    {
        return;
    }
//    ESP_ERROR_CHECK(err);
//    ESP_ERROR_CHECK(esp_wifi_deinit());
//
////    ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(ap_netif));
////    esp_netif_destroy(ap_netif);
//	esp_netif_dhcps_stop(ap_netif);
//	esp_netif_destroy_default_wifi(ap_netif);
//    ap_netif = NULL;
//
//    if(sta_netif != NULL)
//    {
//		ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(sta_netif));
//		esp_netif_destroy(sta_netif);
//		ap_netif = NULL;
//    }
}
void wifi_network_restart(void)
{
	xEventGroupSetBits(s_wifi_event_group, WIFI_INIT_BIT);
    esp_err_t err = esp_wifi_start();
    esp_wifi_disconnect();
    if (err == ESP_ERR_WIFI_NOT_INIT)
    {
        return;
    }
}
bool wifi_network_is_connected(void)
{
	EventBits_t ux_bits;
	if(s_wifi_event_group != NULL)
	{
		ux_bits = xEventGroupGetBits(s_wifi_event_group);

		return (ux_bits & WIFI_CONNECTED_BIT);
	}
	else return 0;
}

void wifi_network_init(void)
{
	if(s_wifi_event_group == NULL)
	{
		s_wifi_event_group = xEventGroupCreate();
	}
	xEventGroupClearBits(s_wifi_event_group, WIFI_INIT_BIT);
	ap_netif = esp_netif_create_default_wifi_ap();

	sta_netif = esp_netif_create_default_wifi_sta();

    int8_t channel = config_server_get_ap_ch();
	if(channel == -1)
	{
		channel = 6;
	}
	ESP_LOGE(WIFI_TAG, "AP Channel:%d", channel);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_network_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_network_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    static wifi_config_t wifi_config_sta = {
        .sta = {
            .ssid = "",
            .password = "",
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    static wifi_config_t wifi_config_ap =
    {
        .ap = {
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    wifi_config_ap.ap.channel = channel;
    if(config_server_get_wifi_mode() == APSTA_MODE)
    {
    	strcpy( (char*)wifi_config_sta.sta.ssid, (char*)config_server_get_sta_ssid());
    	strcpy( (char*)wifi_config_sta.sta.password, (char*)config_server_get_sta_pass());
    	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_sta) );
    }
    else
    {
    	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    }



    uint8_t derived_mac_addr[6] = {0};
    ESP_ERROR_CHECK(esp_read_mac(derived_mac_addr, ESP_MAC_WIFI_SOFTAP));
    sprintf((char *)wifi_config_ap.ap.ssid,"WiCAN_%02x%02x%02x%02x%02x%02x",
            derived_mac_addr[0], derived_mac_addr[1], derived_mac_addr[2],
            derived_mac_addr[3], derived_mac_addr[4], derived_mac_addr[5]);
    strcpy( (char*)wifi_config_ap.ap.password, (char*)config_server_get_ap_pass());

    esp_netif_ip_info_t ipInfo;
    IP4_ADDR(&ipInfo.ip, 192,168,80,1);
	IP4_ADDR(&ipInfo.gw, 192,168,80,1);
	IP4_ADDR(&ipInfo.netmask, 255,255,255,0);
	esp_netif_dhcps_stop(ap_netif);
	esp_netif_set_ip_info(ap_netif, &ipInfo);
	esp_netif_dhcps_start(ap_netif);

	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_ap));

    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_connect();
    ESP_LOGI(WIFI_TAG, "wifi_init finished.");
    xEventGroupSetBits(s_wifi_event_group, WIFI_INIT_BIT);
}
