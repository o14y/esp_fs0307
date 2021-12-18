#pragma once
#include <hal/ledc_types.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef void* o14y_esp_fs0307_t;
esp_err_t o14y_esp_fs0307_make(o14y_esp_fs0307_t* context, int gpio_num, ledc_timer_t t, ledc_channel_t ch);
esp_err_t o14y_esp_fs0307_clear(o14y_esp_fs0307_t context);
esp_err_t o14y_esp_fs0307_set(o14y_esp_fs0307_t context, float angle_in_deg);
#define RGB(r,g,b) (((r&0xFF)<<8)|((g&0xFF)<<16)|(b&0xFF))
#ifdef __cplusplus
}
#endif