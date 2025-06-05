/**
 * @file light_off_command.cc
 * @brief 关灯命令类实现
 */

#include "light_off_command.h"

// 静态成员定义
const char* LightOffCommand::PINYIN = "bang wo guan deng";
const char* LightOffCommand::DESCRIPTION = "帮我关灯";

static const char *TAG = "关灯命令";

LightOffCommand::LightOffCommand() {
    // 构造函数中可以进行初始化工作
}

command_config_t LightOffCommand::get_config() const {
    return {
        .command_id = COMMAND_ID,
        .pinyin = PINYIN,
        .description = DESCRIPTION
    };
}

esp_err_t LightOffCommand::execute() {
    ESP_LOGI(TAG, "💡 执行关灯命令");
    
    // 控制LED熄灭
    gpio_set_level(LED_GPIO, 0);
    ESP_LOGI(TAG, "外接LED熄灭");

    // 播放关灯确认音频
    esp_err_t audio_ret = bsp_play_audio(light_off, light_off_len);
    if (audio_ret == ESP_OK) {
        ESP_LOGI(TAG, "✓ 关灯确认音频播放成功");
    } else {
        ESP_LOGE(TAG, "关灯确认音频播放失败: %s", esp_err_to_name(audio_ret));
    }

    return ESP_OK;
}

const char* LightOffCommand::get_description() const {
    return DESCRIPTION;
}

int LightOffCommand::get_command_id() const {
    return COMMAND_ID;
}

const char* LightOffCommand::get_pinyin() const {
    return PINYIN;
}
