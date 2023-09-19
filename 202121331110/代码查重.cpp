#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <regex>

// 函数声明
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text);
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2);

int main() {
    // 接收用户输入的文件路径
    std::string filePath1, filePath2, filePath3;
    std::cout << "请输入第一篇论文的文件路径: ";
    std::cin >> filePath1;
    std::cout << "请输入第二篇论文的文件路径: ";
    std::cin >> filePath2;
    std::cout << "请输入查重结果的文件路径: ";
    std::cin >> filePath3;
    // 从文件中读取两篇论文的内容
    std::ifstream file1(filePath1);
    std::ifstream file2(filePath2);
    std::stringstream buffer1, buffer2;//字符串流
    try{
        buffer1 << file1.rdbuf();//把文件内容存入字符串流中
        buffer2 << file2.rdbuf();
        std::string paper1 = buffer1.str();
        std::string paper2 = buffer2.str();

        // 将论文转换为词频向量
        std::unordered_map<std::string, int> vector1 = createWordFrequencyVector(paper1);
        std::unordered_map<std::string, int> vector2 = createWordFrequencyVector(paper2);

        // 计算余弦相似度
        double similarity = calculateCosineSimilarity(vector1, vector2);

        // 设置相似度阈值
        double threshold = 0.7;

        std::cout << similarity << std::endl;
        printf("%.2f\n",similarity);
        std::ofstream outfile(filePath3);
        if (outfile.is_open()) 
        {
            // 将读取到的论文内容写入文件
            outfile << similarity << std::endl; 
            outfile.close(); // 关闭输出文件
        } else {
            std::cout << "无法打开输出文件" << std::endl;
        }


        // 输出检测结果
        if (similarity >= threshold)
        {
            std::cout << "这两篇论文可能存在相似性或抄袭！" << std::endl;
        }
        else
        {
            std::cout << "这两篇论文不太可能存在相似性或抄袭。" << std::endl;
        }
    }catch (const std::exception &e)
    {
        std::cerr << "文件打开失败：" << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// 创建词频向量
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text)
{
    std::unordered_map<std::string, int> vector;
    std::regex pattern(R"([\w'-]+)");//用正则表达式匹配字符串，用于去除空格和标点符号
    std::sregex_iterator iter(text.begin(), text.end(), pattern);//用迭代器来迭代字符串，用于遍历字符串中所有符合的项
    std::sregex_iterator end;
    std::string word;
    while (iter != end) 
    {
        std::smatch match = *iter;
        std::string word = match.str();
        std::cout << word << std::endl;
        // 转换为小写以忽略大小写
        for (char& c : word) {
            c = tolower(c);
        }
        vector[word]++;
        ++iter;
    }
    return vector;
}

// 计算余弦相似度

//余弦值=(两个向量的内积)/[(向量一的模长)*(向量二的模长)]
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2)
{
    double dotProduct = 0.0; //用于存放两个向量的点积
    double norm1 = 0.0; //向量一的模长的平方
    double norm2 = 0.0; //向量二的模长的平方

    for (const auto& entry : vector1)
    {
        const std::string& word = entry.first;//在向量一获得键值，即获得单词
        int freq1 = entry.second; //获得该单词的词频
        if (vector2.count(word))
        { //如果在向量二中也有该单词
            int freq2 = vector2.at(word); //获得该单词的词频
            dotProduct += freq1 * freq2; //增加点积值
        }
        norm1 += freq1 * freq1; //增加向量一的模长的平方值
    }

    for (const auto& entry : vector2)
    {
        int freq2 = entry.second; //获得向量二中每一个单词的词频
        norm2 += freq2 * freq2; //增强向量二的模长的平方值
    }

    if (norm1 == 0.0 || norm2 == 0.0)
    {
        return 0.0; // 避免除以零导致计算错误
    }

    return dotProduct / (std::sqrt(norm1) * std::sqrt(norm2));  //开平方根使之为绝对值，即为模长
}
