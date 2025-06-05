/**
 * @file command_manager.cc
 * @brief 命令管理器类实现
 */

#include "command_manager.h"
#include "light_on_command.h"
#include "light_off_command.h"
#include "bye_bye_command.h"

static const char *TAG = "命令管理器";

// 静态成员初始化
CommandManager* CommandManager::instance_ = nullptr;

CommandManager::CommandManager() {
    // 私有构造函数
}

CommandManager* CommandManager::get_instance() {
    if (instance_ == nullptr) {
        instance_ = new CommandManager();
    }
    return instance_;
}

void CommandManager::initialize() {
    ESP_LOGI(TAG, "正在初始化命令管理器...");
    
    // 注册所有命令
    register_command(std::make_unique<LightOnCommand>());
    register_command(std::make_unique<LightOffCommand>());
    register_command(std::make_unique<ByeByeCommand>());
    
    ESP_LOGI(TAG, "✓ 命令管理器初始化完成，共注册 %zu 个命令", commands_.size());
}

esp_err_t CommandManager::configure_commands(esp_mn_iface_t *multinet, model_iface_data_t *mn_model_data) {
    ESP_LOGI(TAG, "开始配置自定义命令词...");

    // 首先尝试从sdkconfig加载默认命令词配置
    esp_mn_commands_update_from_sdkconfig(multinet, mn_model_data);

    // 清除现有命令词，重新开始
    esp_mn_commands_clear();

    // 分配命令词管理结构
    esp_err_t ret = esp_mn_commands_alloc(multinet, mn_model_data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "命令词管理结构分配失败: %s", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    // 添加所有注册的命令词
    int success_count = 0;
    int fail_count = 0;

    for (const auto& command : commands_) {
        command_config_t config = command->get_config();
        
        ESP_LOGI(TAG, "添加命令词 [%d]: %s (%s)",
                 config.command_id, config.description, config.pinyin);

        // 添加命令词
        esp_err_t ret_cmd = esp_mn_commands_add(config.command_id, config.pinyin);
        if (ret_cmd == ESP_OK) {
            success_count++;
            ESP_LOGI(TAG, "✓ 命令词 [%d] 添加成功", config.command_id);
        } else {
            fail_count++;
            ESP_LOGE(TAG, "✗ 命令词 [%d] 添加失败: %s",
                     config.command_id, esp_err_to_name(ret_cmd));
        }
    }

    // 更新命令词到模型
    ESP_LOGI(TAG, "更新命令词到模型...");
    esp_mn_error_t *error_phrases = esp_mn_commands_update();
    if (error_phrases != NULL && error_phrases->num > 0) {
        ESP_LOGW(TAG, "有 %d 个命令词更新失败:", error_phrases->num);
        for (int i = 0; i < error_phrases->num; i++) {
            ESP_LOGW(TAG, "  失败命令 %d: %s",
                     error_phrases->phrases[i]->command_id,
                     error_phrases->phrases[i]->string);
        }
    }

    // 打印配置结果
    ESP_LOGI(TAG, "命令词配置完成: 成功 %d 个, 失败 %d 个", success_count, fail_count);

    // 打印激活的命令词
    ESP_LOGI(TAG, "当前激活的命令词列表:");
    multinet->print_active_speech_commands(mn_model_data);

    // 打印支持的命令列表
    print_supported_commands();

    return (fail_count == 0) ? ESP_OK : ESP_FAIL;
}

command_result_t CommandManager::execute_command(int command_id) {
    CommandBase* command = find_command(command_id);
    if (command == nullptr) {
        ESP_LOGW(TAG, "⚠️  未知命令ID: %d", command_id);
        return COMMAND_RESULT_NOT_FOUND;
    }

    // 特殊处理拜拜命令
    if (command_id == 314) { // ByeByeCommand::COMMAND_ID
        esp_err_t result = command->execute();
        return (result == ESP_OK) ? COMMAND_RESULT_EXIT_REQUESTED : COMMAND_RESULT_EXECUTE_FAILED;
    }

    // 执行其他命令
    esp_err_t result = command->execute();
    return (result == ESP_OK) ? COMMAND_RESULT_SUCCESS : COMMAND_RESULT_EXECUTE_FAILED;
}

const char* CommandManager::get_command_description(int command_id) {
    CommandBase* command = find_command(command_id);
    if (command != nullptr) {
        return command->get_description();
    }
    return "未知命令";
}

size_t CommandManager::get_command_count() const {
    return commands_.size();
}

void CommandManager::print_supported_commands() const {
    ESP_LOGI(TAG, "支持的语音命令:");
    for (const auto& command : commands_) {
        command_config_t config = command->get_config();
        ESP_LOGI(TAG, "  ID=%d: '%s'", config.command_id, config.description);
    }
}

void CommandManager::register_command(std::unique_ptr<CommandBase> command) {
    if (command != nullptr) {
        commands_.push_back(std::move(command));
    }
}

CommandBase* CommandManager::find_command(int command_id) {
    for (const auto& command : commands_) {
        if (command->get_command_id() == command_id) {
            return command.get();
        }
    }
    return nullptr;
}
