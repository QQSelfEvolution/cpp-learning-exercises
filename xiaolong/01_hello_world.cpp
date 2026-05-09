// 小龙 Day1练习 #1: Hello World
// C++基础 - 第一个程序
#include <iostream>
#include <string>

int main() {
    std::cout << "=== 小龙 C++学习 Day1 ===" << std::endl;
    std::cout << "Hello World! 我是小龙!" << std::endl;
    std::cout << "开始学习C++!" << std::endl;
    
    // 基础输出
    std::string name = "小龙";
    int age = 26;
    std::cout << "我叫" << name << ", 今年" << age << "岁" << std::endl;
    
    // 变量类型演示
    double height = 180.5;
    char grade = 'A';
    bool isStudent = true;
    
    std::cout << "身高: " << height << "cm" << std::endl;
    std::cout << "评级: " << grade << std::endl;
    std::cout << "是学生: " << (isStudent ? "是" : "否") << std::endl;
    
    // 简单的计算
    int x = 10, y = 20;
    std::cout << x << " + " << y << " = " << (x + y) << std::endl;
    std::cout << x << " * " << y << " = " << (x * y) << std::endl;
    
    std::cout << "=== Day1练习1完成 ===" << std::endl;
    return 0;
}
