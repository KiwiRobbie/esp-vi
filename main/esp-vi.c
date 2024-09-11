/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdint.h>
#include <math.h>
#include "esp_log.h"
#include "esp_random.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"

#include "sdkconfig.h"
#include "mcurses.h"

#include "cl_main.h"
#include "esp_attr.h"
#include "esp_vfs.h"

#include <errno.h>

static const char *TAG = "example";

#define MAX_FILES 10

struct myvfs_file_t
{
    char *path;
    size_t size;
    void *data;
};

static struct myvfs_file_t files[MAX_FILES];

static ssize_t myfs_write(int fd, const void *data, size_t size)
{
    ESP_LOGI("myfs", "write %d", fd);

    struct myvfs_file_t *file = &files[fd];
    file->data = malloc(size);
    assert(file->data);
    memcpy(file->data, data, size);
    file->size = size;

    return size;
}

static ssize_t myfs_read(int fd, void *dst, size_t size)
{
    ESP_LOGI("myfs", "read %d bytes from fd=%d", size, fd);

    struct myvfs_file_t *file = &files[fd];

    ssize_t src_size = file->size;
    ssize_t dst_size = size;
    ssize_t read_size = src_size > dst_size ? dst_size : src_size;

    memcpy(dst, file->data, read_size);
    return read_size;
}

static ssize_t myfs_pread(int fd, void *dst, size_t size, off_t offset)
{
    ESP_LOGI("myfs", "pread fd=%d %ld to +%d bytes", fd, offset, size);

    struct myvfs_file_t *file = &files[fd];

    ssize_t src_size = file->size - offset;
    ssize_t dst_size = size;
    ssize_t read_size = src_size > dst_size ? dst_size : src_size;

    if (read_size > 0)
    {
        memcpy(dst, file->data + offset, size);
        return read_size;
    }
    else
    {
        return 0;
    }
}

static int myfs_open(const char *path, int flags, int mode)
{
    ESP_LOGI("myfs", "open %s", path);

    int available = -1;
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].path != NULL && 0 == strcmp(files[i].path, path))
        {
            ESP_LOGI("myfs", "file '%s' exists", files[i].path);
            return i;
        }
        if (available == -1 && files[i].path == NULL)
        {
            ESP_LOGI("myfs", "found available slot %d", i);
            available = i;
        }
    }
    if (available == -1)
    {
        ESP_LOGI("myfs", "no available slot");
        return -1;
    }

    ESP_LOGI("myfs", "creating new file");
    char *owned_path = malloc(sizeof path);
    strcpy(owned_path, path);
    ESP_LOGI("myfs", "creating new file %s", owned_path);
    files[available].path = owned_path;
    return available;
}
static int myfs_fstat(int fd, struct stat *st)
{
    ESP_LOGI("myfs", "fstat fd=%d", fd);
    struct myvfs_file_t *file = &files[fd];
    memset(st, 0, sizeof(*st));
    // st->st_dev = 0;
    st->st_mode = S_IRWXU | S_IRWXG | S_IRWXO | S_IFREG;
    // st->st_mtim = 0;
    // st->st_atim = 0;
    // st->st_ctim = 0;
    st->st_size = file->size;
    st->st_blksize = 1024;
    return 0;
}
static int myfs_close(int fd)
{
    ESP_LOGI("myfs", "close fd=%d", fd);

    return 0;
}

void esp_putchar(uint8_t c)
{
    uart_write_bytes(UART_NUM_0, &c, 1);
    // fputc(c, stdout);
    // tinyusb_cdcacm_write_queue_char(0, c);
}

void app_main(void)
{
    esp_vfs_t myfs = {
        .flags = ESP_VFS_FLAG_DEFAULT,
        .write = &myfs_write,
        .open = &myfs_open,
        .fstat = &myfs_fstat,
        .close = &myfs_close,
        .read = &myfs_read,
        .pread = &myfs_pread,
    };

    ESP_ERROR_CHECK(esp_vfs_register("/tmp", &myfs, NULL));

    struct stat sb;
    FILE *file = fopen("/tmp/test", "r+");
    // const char *buffer = malloc(1024 * 17);
    // int file = fmemopen(buffer, 1024 * 17, "r+");
    int fd = fileno(file);

    ESP_LOGI(TAG, "fprintf result: %d", (int)fprintf(file, "hello world"));
    ESP_LOGI(TAG, "pread err: %s", strerror(errno));

    ESP_LOGI(TAG, "fstat result: %d", (int)fstat(fd, &sb));
    ESP_LOGI(TAG, "pread err: %s", strerror(errno));
    ESP_LOGI(TAG, "%ld %ld", sb.st_size, sb.st_blksize);

    char buf[100];
    ESP_LOGI(TAG, "fgets result: %d", (int)fgets(buf, 100, file));
    ESP_LOGI(TAG, "pread err: %s", strerror(errno));
    ESP_LOGI(TAG, "fgets buffer: %s", buf);

    ESP_LOGI(TAG, "pread result: %d", esp_vfs_pread(fd, buf, 8, 2));
    ESP_LOGI(TAG, "pread err: %s", strerror(errno));

    ESP_LOGI(TAG, "fstat result: %d", (int)fstat(fd, &sb));
    ESP_LOGI(TAG, "pread err: %s", strerror(errno));

    ESP_LOGI(TAG, "%ld %ld", sb.st_size, sb.st_blksize);
    // esp_log_level_set("*", ESP_LOG_ERROR);
    setFunction_putchar(esp_putchar); // tell the library which output channel shall be used

    // {
    const int uart_buffer_size = (1024 * 2);
    const uart_port_t uart_num = UART_NUM_0;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, 4, 5, 18, 19));

    // QueueHandle_t uart_queue ;
    ESP_ERROR_CHECK(
        uart_driver_install(UART_NUM_0, uart_buffer_size,
                            uart_buffer_size, 10, NULL, 0));
    //     while (1)
    //     {
    char *message = "Hello World\n";
    //         ESP_LOGI(TAG, "Wrote to UART");

    uart_write_bytes(uart_num, (const char *)message, strlen(message));
    //     }
    // }

    ESP_LOGI(TAG, "Waiting to start VI main()");

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    char *argv[] = {"vi\0"};

    main(1, argv);
    ESP_LOGI(TAG, "VI main() returned");
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}
