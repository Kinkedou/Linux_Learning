/*******************************************************************************
 * Copyright (c) 2012, 2023 IBM Corp., Ian Craggs
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "cJSON.h"
#include "rpc_client.h"
#include "cfg.h"
#include <unistd.h>


#define QOS         1
#define TIMEOUT     10000L


volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);

    cJSON *root = cJSON_Parse((char*)message->payload);
    cJSON *ptTemp = cJSON_GetObjectItem(root, "params");
    if (ptTemp)
    {
		cJSON* led1 = cJSON_GetObjectItem(ptTemp, "LED");
        if (led1)
        {
			rpc_led_control(led1->valueint);
        }
    }
    cJSON_Delete(root);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    if (cause)
    	printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    char broker[100];
    char address[1000];

    char URI[1000]; 
    char ClientID[1000];
    char user[1000]; 
    char passwd[1000]; 
    char ProductID[1000];
    char DeviceName[1000];
    
	char pub_topic[1000];
	char sub_topic[1000];
    
    if (0!=read_cfg(URI,ClientID,user,passwd,ProductID,DeviceName))
            {
        printf("read_cfg error\n");
        return -1;
	}

    sprintf(address, "tcp://%s:1883", URI);
    sprintf(sub_topic, "$sys/%s/%s/thing/property/set", ProductID,DeviceName);
    sprintf(pub_topic, "$sys/%s/%s/thing/property/post", ProductID, DeviceName);

    if (-1 == RPC_Client_Init())
    {
        printf("RPC_Client_Init error\n");
        return -1;
    }

    if ((rc = MQTTClient_create(&client, address, ClientID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto exit;
    }

    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = user;
    conn_opts.password = passwd;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", sub_topic, ClientID, QOS);

    if ((rc = MQTTClient_subscribe(client, sub_topic, QOS)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to subscribe, return code %d\n", rc);
    	rc = EXIT_FAILURE;
    }
    else
    {
    	int ch;
        int cnt = 0;
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        char buf[1000];
        MQTTClient_deliveryToken token;
        
    	while (1)
    	{
        	
			char humi, temp;
            while(0!=rpc_dht11_read(&humi, &temp));
            
            sprintf(buf, "{\
                \"id\": \"123\",\
                    \"version\" : \"1.0\",\
                    \"params\" : {\
                    \"humi_value\": {\
                        \"value\": %d\
                    },\
                        \"temp_value\" : {\
                        \"value\": %d\
                    }\
                }\
            }",temp,humi);
printf("%d",temp);
            pubmsg.payload = buf;
            pubmsg.payloadlen = (int)strlen(buf);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;

            if ((rc = MQTTClient_publishMessage(client, pub_topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
            {
                 printf("Failed to publish message, return code %d\n", rc);
                 continue;
            }
            
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            printf("Message with delivery token %d delivered\n", token);     

            sleep(5);
    	} 

        if ((rc = MQTTClient_unsubscribe(client, sub_topic)) != MQTTCLIENT_SUCCESS)
        {
        	printf("Failed to unsubscribe, return code %d\n", rc);
        	rc = EXIT_FAILURE;
        }
    }

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to disconnect, return code %d\n", rc);
    	rc = EXIT_FAILURE;
    }
destroy_exit:
    MQTTClient_destroy(&client);
exit:
    return rc;
}
