#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "ble_codelock.h"
#include "codelock.h"
#include "potentiometer_lock.h"
#include "validation_leds.h"

static const char *TAG = "MAIN";

void codelock_success_callback(void) {
    ESP_LOGI(TAG, "RIGHT CODE");
    blink_validation_led(SUCCESS_LED);
}
void codelock_failure_callback(void) {
    ESP_LOGI(TAG, "WRONG CODE");
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

    char* code = "ABCDEF";

    init_validation_leds();
    init_codelock(code);
    codelock_set_on_success_callback(codelock_success_callback);
    codelock_set_on_failure_callback(codelock_failure_callback);

    init_ble_codelock(code);
    ble_codelock_set_on_success_callback(codelock_success_callback);
    ble_codelock_set_on_failure_callback(codelock_failure_callback);

    code_char c[4] = {3, 1, 4, 2};
    init_potentiometer_lock(c, 4);
    potentiometer_lock_set_on_success_callback(codelock_success_callback);
    potentiometer_lock_set_on_failure_callback(codelock_failure_callback);
}
