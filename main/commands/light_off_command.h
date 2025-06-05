/**
 * @file light_off_command.h
 * @brief 关灯命令类定义
 */

#pragma once

#include "command_base.h"

extern "C" {
#include "driver/gpio.h"
#include "bsp_board.h"
#include "../assets/voices/light_off.h"
}

/**
 * @brief 关灯命令类
 */
class LightOffCommand : public CommandBase {
private:
    static const int COMMAND_ID = 308;
    static const char* PINYIN;
    static const char* DESCRIPTION;
    static const gpio_num_t LED_GPIO = GPIO_NUM_21;

public:
    /**
     * @brief 构造函数
     */
    LightOffCommand();

    /**
     * @brief 析构函数
     */
    ~LightOffCommand() override = default;

    /**
     * @brief 获取命令配置信息
     * @return 命令配置结构体
     */
    command_config_t get_config() const override;

    /**
     * @brief 执行关灯命令
     * @return esp_err_t 执行结果
     */
    esp_err_t execute() override;

    /**
     * @brief 获取命令描述
     * @return 命令的中文描述
     */
    const char* get_description() const override;

    /**
     * @brief 获取命令ID
     * @return 命令ID
     */
    int get_command_id() const override;

    /**
     * @brief 获取命令拼音
     * @return 命令拼音
     */
    const char* get_pinyin() const override;
};
