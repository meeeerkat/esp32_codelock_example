/* This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this software is
   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
 */

#include "esp_system.h"
#include "nvs_flash.h"
#include "codelock.h"
#include "validation_leds.h"

static const char *TAG = "BHOCKZ";

void codelock_success_callback(void) {
    blink_validation_led(SUCCESS_LED);
}
void codelock_failure_callback(void) {
    blink_validation_led(FAILURE_LED);
}

void app_main(void)
{
    esp_err_t ret;
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    init_validation_leds();
    init_codelock("ABCD");
    codelock_set_on_success_callback(codelock_success_callback);
    codelock_set_on_failure_callback(codelock_failure_callback);
}
