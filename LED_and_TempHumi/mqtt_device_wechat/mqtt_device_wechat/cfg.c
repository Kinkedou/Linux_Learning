#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "cfg.h"
#include <string.h>
#include "cJSON.h"

//{"URI":"mqtts.heclouds.com",
// "ClientID":"phone"
// "username":"0XrranWU9A"
// "password":"version=2018-10-31&res=products%2F0XrranWU9A%2Fdevices%2Fphone&et=1800526241&method=md5&sign=qVve7AJKzfArq1a%2BYi8vvw%3D%3D"
// "ProductID":"0XrranWU9A"
// "DeivceName":"phone"
// }

int read_cfg(char *URI, char *ClientID, char *user, char *passwd, char *ProductID, char *DeviceName)
{
	char buf[1000];
	cJSON *ptTemp;

	int fd = open(CFG_FILE,O_RDONLY);
	if (fd < 0)
		return -1;

	int ret=read(fd, buf, sizeof(buf));
	if(ret <= 0 )
	{
		return -1;
	}

	cJSON *root = cJSON_Parse(buf);
	ptTemp = cJSON_GetObjectItem(root, "URI");
	if (ptTemp)
	{
		strcpy(URI, ptTemp->valuestring);
	}

	ptTemp = cJSON_GetObjectItem(root, "ClientID");
	if (ptTemp)
	{
		strcpy(ClientID, ptTemp->valuestring);
	}

	ptTemp = cJSON_GetObjectItem(root, "username");
	if (ptTemp)
	{
		strcpy(user, ptTemp->valuestring);
	}

	ptTemp = cJSON_GetObjectItem(root, "password");
	if (ptTemp)
	{
		strcpy(passwd, ptTemp->valuestring);
	}

	ptTemp = cJSON_GetObjectItem(root, "ProductID");
	if (ptTemp)
	{
		strcpy(ProductID, ptTemp->valuestring);
	}

	ptTemp = cJSON_GetObjectItem(root, "DeivceName");
	if (ptTemp)
	{
		strcpy(DeviceName, ptTemp->valuestring);
	}

	cJSON_Delete(root);
	return 0;
}
