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
#include "sdkconfig.h"
#include "mcurses.h"
#include "esp_random.h"
#include "cl_main.h"
#include "esp_vfs.h"

static const char *TAG = "example";


#define MAX_FILES 10

struct  myvfs_file_t {
    char* path; 
    size_t* size;
    void* data; 
};

static struct myvfs_file_t files[MAX_FILES];


ssize_t myfs_write(int fd, const void * data, size_t size){ 
    ESP_LOGI("myfs", "write %d", fd);

    struct myvfs_file_t* file = &files[fd];
    file-> data = malloc(size);
    assert(file-> data);
    memcpy(file->data, data, size);

    return size;
}

ssize_t myfs_read(int fd, void * dst, size_t size){ 
    ESP_LOGI("myfs", "read fd=%d", fd);

    struct myvfs_file_t* file = &files[fd];

    ssize_t src_size = *file->size;
    ssize_t dst_size = size;
    ssize_t read_size = src_size > dst_size ? dst_size : src_size;

    memcpy(dst, file->data, read_size);
    return read_size;
}

int myfs_open(const char * path, int flags, int mode){ 
    ESP_LOGI("myfs", "open %s", path);

    int available = -1; 
    for(int i = 0; i<MAX_FILES; i++) {
        if(files[i].path != NULL && 0==strcmp(files[i].path, path)){ 
            ESP_LOGI("myfs", "file '%s' exists", files[i].path);
            return i;
        }
        if (available==-1 && files[i].path==NULL) {
            ESP_LOGI("myfs", "found available slot %d", i);
            available = i;
        }
    }
    if (available == -1) {
        ESP_LOGI("myfs", "no available slot");
        return -1;
    }

    ESP_LOGI("myfs", "creating new file");
    char* owned_path = malloc(sizeof path);
    files[available].path = strcpy(owned_path, path);
    return available;
}
int myfs_fstat(int fd, struct stat * st){ 
    ESP_LOGI("myfs", "fstat fd=%d", fd);
    return 0;
}
int myfs_close(int fd){
    ESP_LOGI("myfs", "close fd=%d", fd);

    return 0;
 }




void app_main(void){
    ESP_LOGI(TAG, "Creating VFS");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    esp_vfs_t myfs = {
        .flags = ESP_VFS_FLAG_DEFAULT,
        .write = &myfs_write,
        .open = &myfs_open,
        .fstat = &myfs_fstat,
        .close = &myfs_close,
        .read = &myfs_read,
    };

    ESP_ERROR_CHECK(esp_vfs_register("/tmp", &myfs, NULL));
    
    struct stat sb;
    FILE* file = fopen("/tmp/test", "w");
    ESP_LOGI(TAG, "fprintf result: %d", (int)fprintf(file, "hello world"));
    ESP_LOGI(TAG, "fstat result: %d", (int)fstat(file, &sb));

    char buf[100]; 
    ESP_LOGI(TAG, "fgets result: %d", (int)fgets(buf, 100, file));
    ESP_LOGI(TAG, "fgets buffer: %s", buf);


    ESP_LOGI(TAG, "Waiting to start VI main()");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    char *argv[] = {"vi\0"};

    main(1, argv);
    ESP_LOGI(TAG, "VI main() returned");
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}
