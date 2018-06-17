#include "project.h"

//#define ADDRESS     "192.168.0.18"
//#define ADDRESS     "192.168.43.12"
#define ADDRESS		"192.168.0.2"
//#define ADDRESS     "192.168.219.115:1883"
//#define ADDRESS     "192.168.0.9:1883
#define SUBID       "PI_SUB"
#define PUBID       "PI_PUB"
#define SUBTOPIC    "TOPI/#"
#define PUBTOPIC    "TOANDROID/"
#define QOS         1
#define TIMEOUT     10000L

MQTTClient_deliveryToken deliveredtoken;

void* sub(void *p)
{
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&subClient, ADDRESS, SUBID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(subClient, NULL, connlost, msgarrvd, delivered);
    
    // connect the client to the daemon
    if((rc = MQTTClient_connect(subClient, &conn_opts)) != MQTTCLIENT_SUCCESS){
        printf("subClient :Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    // subscribe to the topic
    MQTTClient_subscribe(subClient, SUBTOPIC, QOS);

    while(1);

    // disconnect the client from the daemon
    MQTTClient_disconnect(subClient, 10000);

    // free up memory used by the client and end the program
    MQTTClient_destroy(&subClient);
}

void initPub()
{
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&pubClient, ADDRESS, PUBID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // connect the client to the daemon
    if((rc = MQTTClient_connect(pubClient, &conn_opts)) != MQTTCLIENT_SUCCESS){
        printf("pubClient : Failed to connect, return code %d\n", rc);
    }
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
    char* msg;
    char *topic;

    printf("%s\t", topicName);
    printf("%s\n", (char *)message->payload);

    topic = strtok(topicName, "/");
    topic = strtok(NULL, " ");

    msg = (char *)message->payload;

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
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    printf("in publish %s, %s\n", topic, msg);

    pubmsg.payload = msg;
    pubmsg.payloadlen = strlen(msg);
    pubmsg.qos = 1;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(pubClient, topic, &pubmsg, &token);
//    printf("Waiting for up to %d seconds for publication of %s\n"
//            "on topic %s for client with ClientID: %s\n",
//            (int)(TIMEOUT/1000), msg, PUBTOPIC, PUBID);


    MQTTClient_waitForCompletion(pubClient, token, TIMEOUT);
}
