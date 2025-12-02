#include <stdio.h>
#include "cJSON.h"

int main(int argc, char **argv)
{
	char *str = " \
			{ \
			\"title\":\"JSON Example\", \
			\"author\": { \
					\"name\":\"John Doe\", \
					\"age\": 35, \
					\"isVerified\":true \
				}, \
			\"tags\":[\"json\", \"syntax\", \"example\"], \
			\"rating\": 4.5, \
			\"isPublished\":false, \
			\"comments\": null \
			}";

	cJSON *json;

	json = cJSON_Parse(str);
	if (!json)
	{
		printf("cJSON_Parse err\n");
		return 0;
	}

	cJSON *author = cJSON_GetObjectItem(json, "author");

	cJSON *age = cJSON_GetObjectItem(author, "age");
	if (age)
	{
		printf("age = %d\n", age->valueint);
	}

	cJSON *tags = cJSON_GetObjectItem(json, "tags");
	
	cJSON *item = cJSON_GetArrayItem(tags, 2);

	if (item)
	{
		printf("item = %s\n", item->valuestring);
	}

	return 0;
}

