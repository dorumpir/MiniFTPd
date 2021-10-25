#include "parseconf.h"
#include "common.h"
#include "tunable.h"
#include "str.h"

void parseconf_load_file(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (NULL == fp)
		ERR_EXIT("fopen");

	char setting_line[1024] = {0};
	while (NULL != fgets(setting_line, sizeof(setting_line), fp))
	{
		if (0 == strlen(setting_line) || 
			'#' == setting_line[0] ||
			str_all_space(setting_line))
			continue;

		str_trim_crlf(setting_line);
		str_trim_ends_space(setting_line);
		parseconf_load_setting(setting_line);
		memset(setting_line, 0, sizeof(setting_line));
	}
	fclose(fp);
}
void parseconf_load_setting(const char* setting)
{
	char key[128] = {0};
	char value[128] = {0};
	str_split(setting, key, value, '=');
	if (0 == strlen(value))
	{
		fprntf(stderr, "Missing value in config file for : %s\n", key);
		exit(EXIT_FAILURE);
	}

	const struct parseconf_str_setting* p_str_setting = parseconf_str_array;
	while(NULL != p_str_setting->p_settng_name)
	{
		if (0 != strcmp(key, p_str_setting->p_settng_name))
		{
			
		}
	}	
}