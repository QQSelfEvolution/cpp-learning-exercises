// 小龙 Day1练习 #3: STL容器
// C++基础 - 标准模板库容器
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>

// 打印分隔线
void printLine(const std::string& title) {
    std::cout << "\n=== " << title << " ===" << std::endl;
}

int main() {
    std::cout << "=== 小龙 C++学习 Day1 - STL容器 ===" << std::endl;
    
    // 1. vector - 动态数组
    printLine("Vector 动态数组");
    std::vector<int> numbers = {10, 20, 30, 40, 50};
    std::cout << "原始: ";
    for (int n : numbers) std::cout << n << " ";
    
    numbers.push_back(60);
    std::cout << "\n添加后: ";
    for (int n : numbers) std::cout << n << " ";
    
    std::cout << "\n大小: " << numbers.size();
    std::cout << ", 容量: " << numbers.capacity();
    
    // 2. list - 双向链表
    printLine("List 双向链表");
    std::list<std::string> names = {"阿代码", "小匠", "小龙"};
    std::cout << "成员: ";
    for (const auto& name : names) std::cout << name << " -> ";
    std::cout << "end";
    
    names.push_front("队长");  // 头部插入
    names.push_back("小暗");   // 尾部插入
    std::cout << "\n扩展后: ";
    for (const auto& name : names) std::cout << name << " -> ";
    std::cout << "end";
    
    // 3. map - 键值对
    printLine("Map 键值对");
    std::map<std::string, int> scores;
    scores["语文"] = 95;
    scores["数学"] = 88;
    scores["英语"] = 92;
    scores["编程"] = 100;
    
    std::cout << "考试成绩:" << std::endl;
    for (const auto& pair : scores) {
        std::cout << "  " << pair.first << ": " << pair.second << "分" << std::endl;
    }
    
    // 查找
    auto it = scores.find("编程");
    if (it != scores.end()) {
        std::cout << "找到编程成绩: " << it->second << "分" << std::endl;
    }
    
    // 4. set - 不重复集合
    printLine("Set 不重复集合");
    std::set<int> unique_numbers = {1, 2, 3, 2, 1, 4, 5, 3};
    std::cout << "去重后的数字: ";
    for (int n : unique_numbers) std::cout << n << " ";
    
    // 添加元素
    unique_numbers.insert(6);
    unique_numbers.erase(3);  // 删除元素
    std::cout << "\n修改后: ";
    for (int n : unique_numbers) std::cout << n << " ";
    
    // 5. 算法操作
    printLine("STL 算法");
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};
    
    std::cout << "排序前: ";
    for (int n : nums) std::cout << n << " ";
    
    std::sort(nums.begin(), nums.end());
    
    std::cout << "\n排序后: ";
    for (int n : nums) std::cout << n << " ";
    
    // 查找最大值最小值
    auto max_it = std::max_element(nums.begin(), nums.end());
    auto min_it = std::min_element(nums.begin(), nums.end());
    std::cout << "\n最大值: " << *max_it << ", 最小值: " << *min_it;
    
    std::cout << "\n=== Day1练习3完成 ===" << std::endl;
    return 0;
}
