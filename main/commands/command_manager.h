/**
 * @file command_manager.h
 * @brief 命令管理器类定义
 * 
 * 负责管理所有语音命令，包括注册、查找和执行命令
 */

#pragma once

#include "command_base.h"
#include <vector>
#include <memory>

extern "C" {
#include "esp_err.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "esp_mn_speech_commands.h"
#include "esp_process_sdkconfig.h"
}

/**
 * @brief 命令执行结果枚举
 */
typedef enum {
    COMMAND_RESULT_SUCCESS = 0,     // 命令执行成功
    COMMAND_RESULT_NOT_FOUND,       // 命令未找到
    COMMAND_RESULT_EXECUTE_FAILED,  // 命令执行失败
    COMMAND_RESULT_EXIT_REQUESTED   // 请求退出
} command_result_t;

/**
 * @brief 命令管理器类
 * 
 * 单例模式，负责管理所有语音命令
 */
class CommandManager {
private:
    std::vector<std::unique_ptr<CommandBase>> commands_;
    static CommandManager* instance_;

    /**
     * @brief 私有构造函数（单例模式）
     */
    CommandManager();

public:
    /**
     * @brief 获取单例实例
     * @return CommandManager* 单例实例指针
     */
    static CommandManager* get_instance();

    /**
     * @brief 析构函数
     */
    ~CommandManager() = default;

    /**
     * @brief 初始化命令管理器
     * 注册所有可用的命令
     */
    void initialize();

    /**
     * @brief 配置命令词到语音识别模型
     * @param multinet 命令词识别接口指针
     * @param mn_model_data 命令词模型数据指针
     * @return esp_err_t 配置结果
     */
    esp_err_t configure_commands(esp_mn_iface_t *multinet, model_iface_data_t *mn_model_data);

    /**
     * @brief 根据命令ID执行命令
     * @param command_id 命令ID
     * @return command_result_t 执行结果
     */
    command_result_t execute_command(int command_id);

    /**
     * @brief 根据命令ID获取命令描述
     * @param command_id 命令ID
     * @return const char* 命令描述，如果未找到返回"未知命令"
     */
    const char* get_command_description(int command_id);

    /**
     * @brief 获取所有命令的数量
     * @return size_t 命令数量
     */
    size_t get_command_count() const;

    /**
     * @brief 打印所有支持的命令
     */
    void print_supported_commands() const;

private:
    /**
     * @brief 注册一个命令
     * @param command 命令对象的智能指针
     */
    void register_command(std::unique_ptr<CommandBase> command);

    /**
     * @brief 根据命令ID查找命令
     * @param command_id 命令ID
     * @return CommandBase* 命令对象指针，如果未找到返回nullptr
     */
    CommandBase* find_command(int command_id);
};
