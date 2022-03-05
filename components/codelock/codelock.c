
#include "codelock.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"


#define KEYS_NB 5
static uint32_t KEYS_GPIOS[KEYS_NB] = {
    CONFIG_A_GPIO,
    CONFIG_B_GPIO,
    CONFIG_C_GPIO,
    CONFIG_D_GPIO,
    CONFIG_E_GPIO
};

#define KEYS_GPIO_MASK ((1ULL << CONFIG_A_GPIO) \
        | (1ULL << CONFIG_B_GPIO) \
        | (1ULL << CONFIG_C_GPIO) \
        | (1ULL << CONFIG_D_GPIO) \
        | (1ULL << CONFIG_E_GPIO));

#define CODE_LENGTH 4
static const char CODE[CODE_LENGTH] = {'A', 'D', 'E', 'C'};
static uint8_t input_code_length = 0;
static char input_code[CODE_LENGTH];

static xQueueHandle key_press_evt_queue = NULL;
static void (*on_success_callback) (void) = NULL;
static void (*on_failure_callback) (void) = NULL;


void codelock_set_on_success_callback(void (*callback) (void)) {
    on_success_callback = callback;
}
void codelock_set_on_failure_callback(void (*callback) (void)) {
    on_failure_callback = callback;
}

static void IRAM_ATTR key_press_isr_handler(void* arg)
{
    uint32_t key_gpio = (uint32_t) arg;
    xQueueSendFromISR(key_press_evt_queue, &key_gpio, NULL);
}

static char gpio_to_key(uint32_t key_gpio)
{
    for (int i=0; i < KEYS_NB; i++)
        if (KEYS_GPIOS[i] == key_gpio)
            return 'A' + i;
    return 0;
}

static void codelock_task(void* arg)
{
    ESP_LOGI("INPUTS", "Ready to take inputs");

    uint32_t key_gpio;
    while(1) {
        if(xQueueReceive(key_press_evt_queue, &key_gpio, portMAX_DELAY)) {
            char key = gpio_to_key(key_gpio);
            if (key != 0) {
                input_code[input_code_length++] = key;
                ESP_LOGI("CODELOCK", "input: %c", input_code[input_code_length-1]);
                if (input_code_length == CODE_LENGTH) {
                    input_code_length = 0;
                    ESP_LOGI("CODELOCK", "CODE ENTERED");
                    if(strncmp(input_code, CODE, CODE_LENGTH) == 0) {
                        ESP_LOGI("CODELOCK", "LOCKER OPENING");
                        if (on_success_callback)
                            on_success_callback();
                    }
                    else {
                        ESP_LOGI("CODELOCK", "WRONG CODE");
                        if (on_failure_callback)
                            on_failure_callback();
                    }
                }
            }
        }
    }
}

void init_codelock(void)
{
    gpio_config_t keys_io_conf = {};
    // interrupt on rising edge
    keys_io_conf.intr_type = GPIO_INTR_POSEDGE;
    keys_io_conf.pin_bit_mask = KEYS_GPIO_MASK;
    keys_io_conf.mode = GPIO_MODE_INPUT;
    keys_io_conf.pull_down_en = 0;
    keys_io_conf.pull_up_en = 1;
    gpio_config(&keys_io_conf);

    key_press_evt_queue = xQueueCreate(1, sizeof(uint32_t));
    xTaskCreate(codelock_task, "codelock_task", 2048, NULL, 10, NULL);

    // install gpio isr service for low and medium priority interrupts
    gpio_install_isr_service(ESP_INTR_FLAG_LOWMED);
    for (uint8_t i=0; i < KEYS_NB; i++)
        gpio_isr_handler_add(KEYS_GPIOS[i], key_press_isr_handler, (void*) KEYS_GPIOS[i]);
}

