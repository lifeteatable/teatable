#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <regex>

// ��������
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text);
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2);

int main() {
    // �����û�������ļ�·��
    std::string filePath1, filePath2, filePath3;
    std::cout << "�������һƪ���ĵ��ļ�·��: ";
    std::cin >> filePath1;
    std::cout << "������ڶ�ƪ���ĵ��ļ�·��: ";
    std::cin >> filePath2;
    std::cout << "��������ؽ�����ļ�·��: ";
    std::cin >> filePath3;
    // ���ļ��ж�ȡ��ƪ���ĵ�����
    std::ifstream file1(filePath1);
    std::ifstream file2(filePath2);
    std::stringstream buffer1, buffer2;//�ַ�����
    try{
        buffer1 << file1.rdbuf();//���ļ����ݴ����ַ�������
        buffer2 << file2.rdbuf();
        std::string paper1 = buffer1.str();
        std::string paper2 = buffer2.str();

        // ������ת��Ϊ��Ƶ����
        std::unordered_map<std::string, int> vector1 = createWordFrequencyVector(paper1);
        std::unordered_map<std::string, int> vector2 = createWordFrequencyVector(paper2);

        // �����������ƶ�
        double similarity = calculateCosineSimilarity(vector1, vector2);

        // �������ƶ���ֵ
        double threshold = 0.7;

        std::cout << similarity << std::endl;
        printf("%.2f\n",similarity);
        std::ofstream outfile(filePath3);
        if (outfile.is_open()) 
        {
            // ����ȡ������������д���ļ�
            outfile << similarity << std::endl; 
            outfile.close(); // �ر�����ļ�
        } else {
            std::cout << "�޷�������ļ�" << std::endl;
        }


        // ��������
        if (similarity >= threshold)
        {
            std::cout << "����ƪ���Ŀ��ܴ��������Ի�Ϯ��" << std::endl;
        }
        else
        {
            std::cout << "����ƪ���Ĳ�̫���ܴ��������Ի�Ϯ��" << std::endl;
        }
    }catch (const std::exception &e)
    {
        std::cerr << "�ļ���ʧ�ܣ�" << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// ������Ƶ����
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text)
{
    std::unordered_map<std::string, int> vector;
    std::regex pattern(R"([\w'-]+)");//��������ʽƥ���ַ���������ȥ���ո�ͱ�����
    std::sregex_iterator iter(text.begin(), text.end(), pattern);//�õ������������ַ��������ڱ����ַ��������з��ϵ���
    std::sregex_iterator end;
    std::string word;
    while (iter != end) 
    {
        std::smatch match = *iter;
        std::string word = match.str();
        std::cout << word << std::endl;
        // ת��ΪСд�Ժ��Դ�Сд
        for (char& c : word) {
            c = tolower(c);
        }
        vector[word]++;
        ++iter;
    }
    return vector;
}

// �����������ƶ�

//����ֵ=(�����������ڻ�)/[(����һ��ģ��)*(��������ģ��)]
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2)
{
    double dotProduct = 0.0; //���ڴ�����������ĵ��
    double norm1 = 0.0; //����һ��ģ����ƽ��
    double norm2 = 0.0; //��������ģ����ƽ��

    for (const auto& entry : vector1)
    {
        const std::string& word = entry.first;//������һ��ü�ֵ������õ���
        int freq1 = entry.second; //��øõ��ʵĴ�Ƶ
        if (vector2.count(word))
        { //�������������Ҳ�иõ���
            int freq2 = vector2.at(word); //��øõ��ʵĴ�Ƶ
            dotProduct += freq1 * freq2; //���ӵ��ֵ
        }
        norm1 += freq1 * freq1; //��������һ��ģ����ƽ��ֵ
    }

    for (const auto& entry : vector2)
    {
        int freq2 = entry.second; //�����������ÿһ�����ʵĴ�Ƶ
        norm2 += freq2 * freq2; //��ǿ��������ģ����ƽ��ֵ
    }

    if (norm1 == 0.0 || norm2 == 0.0)
    {
        return 0.0; // ��������㵼�¼������
    }

    return dotProduct / (std::sqrt(norm1) * std::sqrt(norm2));  //��ƽ����ʹ֮Ϊ����ֵ����Ϊģ��
}
