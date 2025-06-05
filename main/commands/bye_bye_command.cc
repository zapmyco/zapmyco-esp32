/**
 * @file bye_bye_command.cc
 * @brief 拜拜命令类实现
 */

#include "bye_bye_command.h"

// 静态成员定义
const char* ByeByeCommand::PINYIN = "bai bai";
const char* ByeByeCommand::DESCRIPTION = "拜拜";

static const char *TAG = "拜拜命令";

ByeByeCommand::ByeByeCommand() {
    // 构造函数中可以进行初始化工作
}

command_config_t ByeByeCommand::get_config() const {
    return {
        .command_id = COMMAND_ID,
        .pinyin = PINYIN,
        .description = DESCRIPTION
    };
}

esp_err_t ByeByeCommand::execute() {
    ESP_LOGI(TAG, "👋 执行拜拜命令");
    
    // 播放再见音频
    ESP_LOGI(TAG, "播放再见音频...");
    esp_err_t audio_ret = bsp_play_audio(byebye, byebye_len);
    if (audio_ret == ESP_OK) {
        ESP_LOGI(TAG, "✓ 再见音频播放成功");
    } else {
        ESP_LOGE(TAG, "再见音频播放失败: %s", esp_err_to_name(audio_ret));
    }

    return ESP_OK;
}

const char* ByeByeCommand::get_description() const {
    return DESCRIPTION;
}

int ByeByeCommand::get_command_id() const {
    return COMMAND_ID;
}

const char* ByeByeCommand::get_pinyin() const {
    return PINYIN;
}
