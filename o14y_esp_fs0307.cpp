#include <string.h>
#include <driver/ledc.h>
#include <esp_err.h>
#include <esp_log.h>
#include <o14y_esp_fs0307.h>
#define UNPACK(p) ((o14y::esp::fs0307*)p)
#define ENSURE(ec) { esp_err_t e = (ec); if (e != ESP_OK){ return e; }}
#define CENTER 307
static const char* app = "o14y_esp_fs0307";
namespace o14y { namespace esp {
class fs0307 {
 public:
    fs0307() {}
    esp_err_t init(int gpio_num, ledc_timer_t t, ledc_channel_t ch){
        ledc_timer_config_t timer = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_12_BIT, // 19.5us per bit
            .timer_num = t,
            .freq_hz = 50, // 20ms
            .clk_cfg = LEDC_AUTO_CLK,
        };
        ENSURE(ledc_timer_config(&timer));
        ledc_channel_config_t channel = {
            .gpio_num       = gpio_num,
            .speed_mode     = LEDC_LOW_SPEED_MODE,
            .channel        = ch,
            .intr_type      = LEDC_INTR_DISABLE,
            .timer_sel      = t,
            .duty           = CENTER, // center, 1499us
            .hpoint         = 0
        };
        memmove(&channel_, &channel, sizeof(channel));
        ENSURE(ledc_channel_config(&channel));
        ch_ = ch;
        return ESP_OK;
    }
    esp_err_t set(float angle_in_deg){
        uint32_t us_per_angle = 10; // 600 us for 60deg
        #define ticks_per_us 4096/20'000
        uint32_t duty = CENTER + angle_in_deg * us_per_angle * ticks_per_us;
        ENSURE(ledc_set_duty(LEDC_LOW_SPEED_MODE, ch_, duty));
        // Update duty to apply the new value
        ENSURE(ledc_update_duty(LEDC_LOW_SPEED_MODE, ch_)); 
        ESP_LOGI(app, "set duty: %d for angle: %.2f\n", duty, angle_in_deg);
        return ESP_OK;        
    }
 private:
    ledc_channel_config_t channel_;
    ledc_channel_t ch_;
};
}}
esp_err_t o14y_esp_fs0307_make(o14y_esp_fs0307_t* context, int gpio_num, ledc_timer_t t, ledc_channel_t ch) {
    auto obj = new o14y::esp::fs0307();
    ENSURE(obj->init(gpio_num, t, ch));
    *context = obj;
    return ESP_OK;
}
esp_err_t o14y_esp_fs0307_clear(o14y_esp_fs0307_t context) {
    delete UNPACK(context);
    return ESP_OK;
}
esp_err_t o14y_esp_fs0307_set(o14y_esp_fs0307_t context, float angle_in_deg) {
    auto obj = UNPACK(context);
    ENSURE(obj->set(angle_in_deg));
    return ESP_OK;
}

