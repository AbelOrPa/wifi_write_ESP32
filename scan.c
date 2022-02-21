#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include "unity.h"
//#include "test_utils.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_partition.h"
#include "esp_rom_sys.h"

#define DEFAULT_SCAN_LIST_SIZE CONFIG_EXAMPLE_SCAN_LIST_SIZE

static const char *TAG = "scan";
char** networks;
char ssid[100];
int size = 50;
FILE *demo;

/* Initialize Wi-Fi as sta and set scan method */
static void wifi_scan(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
    networks = (char**)malloc(sizeof(char**)*size);
    
    if((demo = fopen("prueba.txt", "w")) == NULL){

        printf("Error al abrir el fichero");

    }
    else{

        printf("Entro bien");
        for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
            ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
            ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
            networks[i] = (char*)malloc(sizeof(char) * (strlen(ssid) + 1 ));
            strcpy(networks[i],&ap_info[i].ssid);
            fprintf(demo,"%s",networks[i]);
            ESP_LOGI(TAG, "Name of network %d is \t\t%s\n", i,networks[i]);
            free(networks[i]);
        }

        fclose(demo);
        printf("realizado con exito");
    }


}

void write_networks(){


    //ESP_LOGI(TAG,"nombre es %s",networks[1]);
    /*
    char* mode = "w";
    FILE* f;
    f = fopen("prueba1.txt", mode);
    //TEST_ASSERT_NOT_NULL(f);
    int count = sizeof(redes);
    //int count = 20;
    if(f == NULL){
        printf("Error al abrir el fichero");
        return -1;
    }
    else}{
         for (size_t i = 0; i < count; i++)
        {
            fprintf(f,"%s",redes[i]);
            //fputs("\n",f);    
        }

        fclose(f);
        printf("realizado con exito");
    }*/
   
    
}

/*void read_networks(){

    char* mode = "r";
    FILE* f;
    f = fopen("prueba1.txt", mode);
    char buf[20] = {0};
    fread(buf,1,sizeof(buf),f);
    ESP_LOGI(TAG,"El segundo valor es: \t\t%d\n", buf[1]);
    fclose(f);

}*/

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    wifi_scan();
    write_networks();
    //read_networks();


}