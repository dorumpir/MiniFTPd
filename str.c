#include "str.h"
#include "common.h"

/**
 * Trim space in str's beg & end
 * 
 * @param str Input string
 */
void str_trim_ends_space(char* str)
{
	// copy from wiki
	size_t n;
  	n = 0;
  	while (str[n] != '\0' && isspace((unsigned char)str[n])) {
    	n++;
  	}
  	memmove(str, str + n, strlen(str) - n + 1);

	char* p = str + strlen(str) - 1;
	while(isspace(*p) && str < p)
		*p -- = '\0';  	
}

/**
 * Trim \r \n in str
 * 
 * @param str Input string
 */
void str_trim_crlf(char* str)
{
	char* p = str + strlen(str) - 1;
	while('\r' ==*p || '\n' == *p)
		*p -- = '\0';
}

void str_split(const char* str, char* left, char* right, char c)
{

}

/**
 * Judge if str is all space
 * 
 * @param  str Input string
 * @return     if is all space ,return 1. 
 *             else return 0
 */
int str_all_space(const char* str)
{
	while (*str)
	{
		if (!isspace(*str))
			return 0;
		str ++;
	}
	return 1;
}
/**
 * Change lower char to upper, change IN-SPACE
 * 
 * @param str Input string
 */
void str_upper(char* str)
{
	while (*str)
	{
		*str = toupper(*str);
		str ++;
	}
}

/**
 * Change str into long long number ( By Decimal)
 * Some system didn't support atoll(str) function
 * 
 * @param  str Input string, without \r\n
 * @return     If success return long long number of str
 *             Error: return number before error
 */
long long str_to_long_Long(const char* str)
{
	// return atoll(str);
	long long ret = 0;
	char* beg = (char*)str;
	char* end = beg + strlen(str);
	while (beg != end)
	{
		if (!isdigit(*beg))
			return ret;
		ret = (ret * 10) + (*beg - '0');
		beg ++;
	}
	return ret;
}
/**
 * Change Oct str into uisigned int number 
 * 
 * @param  str Input string(Oct), without \r\n
 * @return     If success, return long long number of str
 *             Error: return number before error
 */
unsigned int str_octal_to_uint(const char* str)
{
	unsigned int ret = 0;
	char* beg = (char*)str;
	char* end = beg + strlen(str);
	while (beg != end)
	{
		if (!isdigit(*beg))
			return ret;
		ret = (ret * 8) + (*beg - '0');
		beg ++;
	}
	return ret;
}