#include <string>
#include <string.h>// strtok
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

std::vector<string> splitStr(const std::string& str, char tag) {
    std::vector<string>  splited_str;
    std::string sub_str;

    //遍历字符串，同时将i位置的字符放入到子串中，当遇到tag（需要切割的字符时）完成一次切割
    //遍历结束之后即可得到切割后的字符串数组
    for(size_t i = 0; i < str.length(); i++)
    {
        if(tag == str[i]) //完成一次切割
        {
            if(!sub_str.empty())
            {
                splited_str.push_back(sub_str);
                sub_str.clear();
            }
        }
        else //将i位置的字符放入子串
        {
            sub_str.push_back(str[i]);
        }
    }

    if(!sub_str.empty()) //剩余的子串作为最后的子字符串
    {
        splited_str.push_back(sub_str);
    }

    return splited_str;
}