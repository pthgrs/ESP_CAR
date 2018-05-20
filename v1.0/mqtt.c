#include "project.h"

#define ADDRESS     "192.168.219.115"
#define CLIENTID    "esp"
#define TOPIC       "direction"
#define QOS         1


volatile MQTTClient_deliveryToken deliveredtoken;

void createMQTTClient()
{
    MQTTClient client;
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

    printf("     topic: %s\n", topicName);
    printf("   message: ");

    msg = message->payload;

    if(strcmp(msg, "forward") == 0)
        go_forward();
    else if(strcmp(msg, "backward") == 0)
        go_backward();
    else if(strcmp(msg, "right") == 0)
        go_right();
    else if(strcmp(msg, "left") == 0)
        go_left();
    else if(strcmp(msg, "stop") == 0)
        stop();

    // free the memory used to store the msg
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    
    return 1;
}

void connlost(void *context, char *cause){
    printf("\nConnection lost\n");
    printf("\tcause:%s\n", cause);
}
