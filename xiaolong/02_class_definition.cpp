// 小龙 Day1练习 #2: 类定义
// C++基础 - 面向对象编程
#include <iostream>
#include <string>
#include <vector>

// DesktopController 类 - 桌面控制器
class DesktopController {
private:
    std::string device_id;
    bool connected;
    int command_count;

public:
    // 构造函数
    DesktopController(const std::string& id) 
        : device_id(id), connected(false), command_count(0) {
        std::cout << "控制器 " << device_id << " 创建" << std::endl;
    }
    
    // 析构函数
    ~DesktopController() {
        std::cout << "控制器 " << device_id << " 销毁" << std::endl;
    }
    
    // 连接设备
    bool connect() {
        if (connected) {
            std::cout << device_id << " 已经连接" << std::endl;
            return true;
        }
        connected = true;
        std::cout << device_id << " 连接成功" << std::endl;
        return true;
    }
    
    // 断开连接
    void disconnect() {
        connected = false;
        std::cout << device_id << " 已断开" << std::endl;
    }
    
    // 发送命令
    bool sendCommand(const std::string& cmd) {
        if (!connected) {
            std::cout << "错误: 设备未连接" << std::endl;
            return false;
        }
        command_count++;
        std::cout << device_id << " 执行命令 #" << command_count << ": " << cmd << std::endl;
        return true;
    }
    
    // 获取状态
    bool isConnected() const {
        return connected;
    }
    
    int getCommandCount() const {
        return command_count;
    }
};

// 继承示例 - 高级控制器
class AdvancedController : public DesktopController {
private:
    std::vector<std::string> log_history;

public:
    AdvancedController(const std::string& id) : DesktopController(id) {
        std::cout << "高级控制器 " << id << " 初始化" << std::endl;
    }
    
    // 重写发送命令 - 添加日志
    bool sendCommand(const std::string& cmd) override {
        log_history.push_back(cmd);
        std::cout << "[LOG] 命令已记录: " << cmd << std::endl;
        return DesktopController::sendCommand(cmd);
    }
    
    void showHistory() {
        std::cout << "\n--- 命令历史 ---" << std::endl;
        for (size_t i = 0; i < log_history.size(); i++) {
            std::cout << i + 1 << ". " << log_history[i] << std::endl;
        }
    }
};

int main() {
    std::cout << "=== 小龙 C++学习 Day1 - 类定义 ===" << std::endl;
    
    // 创建控制器
    DesktopController controller1("PC-001");
    controller1.connect();
    controller1.sendCommand("打开浏览器");
    controller1.sendCommand("打开终端");
    
    std::cout << "\n--- 创建高级控制器 ---" << std::endl;
    AdvancedController controller2("PC-002");
    controller2.connect();
    controller2.sendCommand("截图");
    controller2.sendCommand("录音");
    controller2.sendCommand("录像");
    
    // 显示历史
    controller2.showHistory();
    
    std::cout << "\n控制器2发送了 " << controller2.getCommandCount() << " 条命令" << std::endl;
    
    // 断开连接
    controller1.disconnect();
    controller2.disconnect();
    
    std::cout << "=== Day1练习2完成 ===" << std::endl;
    return 0;
}
