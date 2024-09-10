/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdint.h>
#include <math.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "sdkconfig.h"
#include "mcurses.h"
#include "esp_random.h"
#include "cl_main.h"

#define PI 3.141592653589793
#define VT100_moveTo(x, y) move(y, x)

#define POINTS 100

#define YPOS 20
#define XPOS 20
#define RADIUS 15
#define SCALEX 2.3

static const char *TAG = "example";
static uint8_t rx_buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];

/**
 * @brief Application Queue
 */
static QueueHandle_t app_queue;
typedef struct
{
    uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1]; // Data buffer
    size_t buf_len;                                 // Number of bytes received
    uint8_t itf;                                    // Index of CDC device interface
} app_message_t;

/**
 * @brief CDC device RX callback
 *
 * CDC device signals, that new data were received
 *
 * @param[in] itf   CDC device index
 * @param[in] event CDC event type
 */
void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    /* initialization */
    size_t rx_size = 0;

    /* read */
    esp_err_t ret = tinyusb_cdcacm_read(itf, rx_buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);
    if (ret == ESP_OK)
    {

        app_message_t tx_msg = {
            .buf_len = rx_size,
            .itf = itf,
        };

        memcpy(tx_msg.buf, rx_buf, rx_size);
        xQueueSend(app_queue, &tx_msg, 0);
    }
    else
    {
        ESP_LOGE(TAG, "Read Error");
    }
}

/**
 * @brief CDC device line change callback
 *
 * CDC device signals, that the DTR, RTS states changed
 *
 * @param[in] itf   CDC device index
 * @param[in] event CDC event type
 */
void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event)
{
    int dtr = event->line_state_changed_data.dtr;
    int rts = event->line_state_changed_data.rts;
    ESP_LOGI(TAG, "Line state changed on channel %d: DTR:%d, RTS:%d", itf, dtr, rts);
}

void esp_putchar(uint8_t c)
{
    // fputc(c, stdout);
    tinyusb_cdcacm_write_queue_char(0, c);
}

int random_int(int start, int stop)
{

    uint32_t range = stop - start + 1;
    uint32_t offset = (esp_random() & INT_MAX) % range;
    return start + offset;
}

void sub_d(int p, int s, int x, int y)
{
    uint8_t r;
    uint8_t g;

    r = (p % 16) * 16;
    g = 180 - p;
    if (r < 10)
        attrset(F_BLACK); // forground color black
    else
    {
        if (g > 170)
            attrset(F_WHITE);
        else if (g > 169)
            attrset(F_GREEN);
    }

    if ((y >= 0) && (y < 24) && (x < 80))
    {
        int xx, yy;
        xx = x;
        yy = y;
        VT100_moveTo(xx, yy);
        char c;
        c = 33 + (x * y) % 200;
        addch(c);
    }
}

int t[80];

void app_main(void)
{

    // Create FreeRTOS primitives
    app_queue = xQueueCreate(5, sizeof(app_message_t));
    assert(app_queue);
    app_message_t msg;

    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = NULL,
        .external_phy = false,
#if (TUD_OPT_HIGH_SPEED)
        .fs_configuration_descriptor = NULL,
        .hs_configuration_descriptor = NULL,
        .qualifier_descriptor = NULL,
#else
        .configuration_descriptor = NULL,
#endif // TUD_OPT_HIGH_SPEED
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &tinyusb_cdc_rx_callback, // the first way to register a callback
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = NULL,
        .callback_line_coding_changed = NULL};

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    /* the second way to register a callback */
    ESP_ERROR_CHECK(tinyusb_cdcacm_register_callback(
        TINYUSB_CDC_ACM_0,
        CDC_EVENT_LINE_STATE_CHANGED,
        &tinyusb_cdc_line_state_changed_callback));

#if (CONFIG_TINYUSB_CDC_COUNT > 1)
    acm_cfg.cdc_port = TINYUSB_CDC_ACM_1;
    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    ESP_ERROR_CHECK(tinyusb_cdcacm_register_callback(
        TINYUSB_CDC_ACM_1,
        CDC_EVENT_LINE_STATE_CHANGED,
        &tinyusb_cdc_line_state_changed_callback));
#endif

    ESP_LOGI(TAG, "USB initialization DONE");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Waiting to start VI main()");

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    char **argv = {"vi\0"};

    main(1, argv);
    ESP_LOGI(TAG, "VI main() returned");

    vTaskDelay(3000 / portTICK_PERIOD_MS);
    setFunction_putchar(esp_putchar); // tell the library which output channel shall be used

    {
        initscr(); // initialize mcurses

        ESP_LOGI(TAG, "===================");
        ESP_LOGI(TAG, "VT100 graphics demo");
        ESP_LOGI(TAG, "===================");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        for (int i = 0; i < 80; i++)
        {
            esp_putchar(random_int('a', 'z'));
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        tinyusb_cdcacm_write_flush(0, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        curs_set(0); // set cursor invisible
        clear();
    }
    while (1)
    {

        int x = 0;
        int y = 0;

        for (int i = 0; i < 80; i++)
        {
            t[i] = random_int(-50, 0);
        }

        // sub_d(1,1,10,10);
        while (1)
        {
            for (int k = 1; k < 80; k++)
            {
                int i = random_int(0, 79);
                if (t[i] > 28)
                    t[i] = 0;
                t[i] = t[i] + 1;
                y = t[i];
                sub_d(0, 0, i, y - 6);
                sub_d(2 + x, 0, i, y - 5);
                sub_d(2 + x, 0, i, y - 4);
                sub_d(10 + x, 0, i, y - 3);
                sub_d(10 + x, 0, i, y - 2);
                sub_d(11 + x, 0, i, y - 1);
                sub_d(0, 2 + x, i, y);
            }
            vTaskDelay(30 / portTICK_PERIOD_MS);
        }

        // {
        //     static float n = 0;
        //     float x, y;

        //     x = (sin(2 * PI * n / POINTS) * RADIUS + XPOS) * SCALEX;
        //     y = cos(2 * PI * n / POINTS) * RADIUS + YPOS;
        //     n++;
        //     VT100_moveTo(x, y);
        //     fputc('+', stdout);

        //     vTaskDelay(100 / portTICK_PERIOD_MS);
        // }

        // if (xQueueReceive(app_queue, &msg, portMAX_DELAY))
        // {
        //     if (msg.buf_len)
        //     {

        //         /* Print received data*/
        //         ESP_LOGI(TAG, "Data from channel %d:", msg.itf);
        //         ESP_LOG_BUFFER_HEXDUMP(TAG, msg.buf, msg.buf_len, ESP_LOG_INFO);

        //         /* write back */
        //         tinyusb_cdcacm_write_queue(msg.itf, msg.buf, msg.buf_len);
        //         esp_err_t err = tinyusb_cdcacm_write_flush(msg.itf, 0);
        //         if (err != ESP_OK)
        //         {
        //             ESP_LOGE(TAG, "CDC ACM write flush error: %s", esp_err_to_name(err));
        //         }
        //     }
        // }
    }
}
