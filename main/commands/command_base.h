/**
 * @file command_base.h
 * @brief 语音命令基类定义
 * 
 * 定义了语音命令的基础接口，所有具体命令都应该继承此基类
 */

#pragma once

extern "C" {
#include "esp_err.h"
#include "esp_log.h"
}

/**
 * @brief 命令配置结构体
 */
typedef struct {
    int command_id;           // 命令ID
    const char *pinyin;       // 拼音表示
    const char *description;  // 中文描述
} command_config_t;

/**
 * @brief 语音命令基类
 * 
 * 所有具体的语音命令都应该继承此基类并实现相应的虚函数
 */
class CommandBase {
public:
    /**
     * @brief 构造函数
     */
    CommandBase() = default;
    
    /**
     * @brief 虚析构函数
     */
    virtual ~CommandBase() = default;

    /**
     * @brief 获取命令配置信息
     * @return 命令配置结构体
     */
    virtual command_config_t get_config() const = 0;

    /**
     * @brief 执行命令
     * @return esp_err_t 执行结果
     */
    virtual esp_err_t execute() = 0;

    /**
     * @brief 获取命令描述
     * @return 命令的中文描述
     */
    virtual const char* get_description() const = 0;

    /**
     * @brief 获取命令ID
     * @return 命令ID
     */
    virtual int get_command_id() const = 0;

    /**
     * @brief 获取命令拼音
     * @return 命令拼音
     */
    virtual const char* get_pinyin() const = 0;
};
