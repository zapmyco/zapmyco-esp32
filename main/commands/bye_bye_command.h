/**
 * @file bye_bye_command.h
 * @brief 拜拜命令类定义
 */

#pragma once

#include "command_base.h"

extern "C" {
#include "bsp_board.h"
#include "../assets/voices/byebye.h"
}

/**
 * @brief 拜拜命令类
 */
class ByeByeCommand : public CommandBase {
private:
    static const int COMMAND_ID = 314;
    static const char* PINYIN;
    static const char* DESCRIPTION;

public:
    /**
     * @brief 构造函数
     */
    ByeByeCommand();

    /**
     * @brief 析构函数
     */
    ~ByeByeCommand() override = default;

    /**
     * @brief 获取命令配置信息
     * @return 命令配置结构体
     */
    command_config_t get_config() const override;

    /**
     * @brief 执行拜拜命令
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
