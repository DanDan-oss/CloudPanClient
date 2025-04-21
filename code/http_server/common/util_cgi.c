#include "util_cgi.h"
#include "make_log.h"

#define UTIL_LOG_MODULE     "util_cgi"
/**
 * @brief  去掉一个字符串两边的空白字符
 *
 * @param inbuf确保inbuf可修改
 *
 * @returns   
 *      0 成功
 *      -1 失败
 */
int trim_space(char *inbuf)
{
    int count = 0, i = 0, j = strlen(inbuf-1);

    char *str = inbuf;
    if(NULL == str)   { LOG_ERROR(UTIL_LOG_MODULE, "inbuf  == NULL"); return -1; }
    while (isspace(str[i]) && str[i] != '\0')   { i++;}
    while (isspace(str[j]) && j > i)    { j--; }
    count = j-i+1;
    strncpy(inbuf, str + i, count);
    inbuf[count] = '\0';
    return 0;
}

/**
 * @brief  在字符串full_data中查找字符串substr第一次出现的位置
 *
 * @param full_data 	源字符串首地址
 * @param full_data_len 源字符串长度
 * @param substr        匹配字符串首地址
 *
 * @returns   
 *      成功: 匹配字符串首地址
 *      失败：NULL
 */
char* memstr(char* full_data, int full_data_len, char* substr)
{
    if( NULL==full_data || full_data_len<=0 || NULL==substr)   return;
    if(*substr == '\0')   return;
    
    // 匹配子串的长度
    int sublen = strlen(substr); 
    char* cur = full_data;
    int last_possible = full_data_len - sublen + 1;     // 减去匹配子串后的长度
    for (int i = 0; i < last_possible; i++)
    {
        if(*cur != *substr)   { cur++; continue; }
        if(0 == memcmp(cur, substr, sublen))    return cur;
    }
}

//通过文件名file_name， 得到文件后缀字符串, 保存在suffix 如果非法文件后缀,返回"null"
int get_file_suffix(const char *file_name, char *suffix)
{
    const char *p = file_name;
    const char *q=NULL, *k= NULL;
    int len = 0;

    if (p == NULL)   return -1;
    q = p;

    //mike.doc.png
    //             ↑
    while (*q != '\0')   { q++; }
    k = q;  // q=文件末尾
    while (*k != '.' && k != p)   { k--; }  // 从末尾往前找,找到倒数第一个.赋值给k

    if (*k == '.')
    {
        k++;  len = q - k;
        if (len != 0)    { strncpy(suffix, k, len); suffix[len] = '\0'; }
        else    { strncpy(suffix, "null", 5); }
    }
    else   { strncpy(suffix, "null", 5); }
    return 0;
}