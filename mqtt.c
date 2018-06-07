#include "project.h"

#define ADDRESS     "192.168.219.115:1883"
//#define ADDRESS     "192.168.0.9:1883"
#define CLIENTID    "esp"
#define TOPIC       "TOPI/#"
#define QOS         1
#define TIMEOUT     10000L

//volatile MQTTClient_deliveryToken deliveredtoken;
MQTTClient_deliveryToken deliveredtoken;

void createMQTTClient()
{
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    
    // connect the client to the daemon
    if((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    // subscribe to the topic
    MQTTClient_subscribe(client, TOPIC, QOS);

    // loop
    while(1);

    // disconnect the client from the daemon
    MQTTClient_disconnect(client, 10000);

    // free up memory used by the client and end the program
    MQTTClient_destroy(&client);
}

void delivered(void *context, MQTTClient_deliveryToken dt) 
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

// when msg arrive from the server, the MQTTClient_messageArrived funtion is started
int msgarrvd(void *context, char *topicName
            , int topicLen, MQTTClient_message *message) 
{
    int i;
    char* msg;
    char *topic;

    printf("%s\t", topicName);
    printf("%s\n", (char *)message->payload);

    topic = strtok(topicName, "/");
    topic = strtok(NULL, " ");

    msg = message->payload;

    msgController(topic, msg);

    // free the memory used to store the msg
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("\tcause:%s\n", cause);
}

void publish(char *topic, char *msg)
{
    printf("in publish %s, %s\n", topic, msg);

    MQTTClient_message pubmsg;
    MQTTClient_deliveryToken token;
    int rc;

    strcpy(pubmsg.payload, msg);
    pubmsg.payloadlen = strlen(msg);
    pubmsg.qos = 2;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
}
