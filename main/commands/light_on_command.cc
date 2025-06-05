/**
 * @file light_on_command.cc
 * @brief 开灯命令类实现
 */

#include "light_on_command.h"

// 静态成员定义
const char* LightOnCommand::PINYIN = "bang wo kai deng";
const char* LightOnCommand::DESCRIPTION = "帮我开灯";

static const char *TAG = "开灯命令";

LightOnCommand::LightOnCommand() {
    // 构造函数中可以进行初始化工作
}

command_config_t LightOnCommand::get_config() const {
    return {
        .command_id = COMMAND_ID,
        .pinyin = PINYIN,
        .description = DESCRIPTION
    };
}

esp_err_t LightOnCommand::execute() {
    ESP_LOGI(TAG, "💡 执行开灯命令");
    
    // 控制LED点亮
    gpio_set_level(LED_GPIO, 1);
    ESP_LOGI(TAG, "外接LED点亮");

    // 播放开灯确认音频
    esp_err_t audio_ret = bsp_play_audio(light_on, light_on_len);
    if (audio_ret == ESP_OK) {
        ESP_LOGI(TAG, "✓ 开灯确认音频播放成功");
    } else {
        ESP_LOGE(TAG, "开灯确认音频播放失败: %s", esp_err_to_name(audio_ret));
    }

    return ESP_OK;
}

const char* LightOnCommand::get_description() const {
    return DESCRIPTION;
}

int LightOnCommand::get_command_id() const {
    return COMMAND_ID;
}

const char* LightOnCommand::get_pinyin() const {
    return PINYIN;
}
