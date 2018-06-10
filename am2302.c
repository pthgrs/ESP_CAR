#include "project.h"

int call_count_ = 0;
uint32_t start_tick_;
uint8_t data_[5];

void *dht22_run(void *p)
{
    while(1){
        read_dht_data();
        time_sleep(1);
    }
}

void read_dht_data()
{
    float   tempC;
    float   humidity;
    char    str[10];
    pthread_t tid;

    call_count_ = 0;
    data_[0] = data_[1] = data_[2] = data_[3] = data_[4] = 0;
    
    set_mode(pi, SDAPIN, PI_OUTPUT);

    start_tick_ = get_current_tick(pi);
    gpio_write(pi, SDAPIN, 1);                      
    time_sleep(1);							
    callback(pi, SDAPIN, FALLING_EDGE, cb_func_dht22);    

    gpio_write(pi,SDAPIN, 0);
    time_sleep(0.0008);                             

    set_pull_up_down(pi, SDAPIN, PI_PUD_UP);        
    set_mode(pi, SDAPIN, PI_INPUT);                 
    usleep(10000);                                  

    if (call_count_ >= 43 && data_[4] == ((data_[0] + data_[1] + data_[2] + data_[3]) & 0xff)) {
        humidity = (data_[0] * 256 + data_[1]) / 10.0f;
        tempC = ((data_[2] & 0x7f) * 256 + data_[3]) / 10.0f;
        if (data_[2] & 0x80)
            tempC *= -1.0f;

        // printf("Temperature: %.1fC Humidity: %.1f%%\n", tempC, humidity);

        if(tempC > 20){
            sprintf(str, "%f", tempC);
            publish("TOANDROID/fire", str);
            pthread_create(&tid, NULL, alertLED, NULL);
        }

    } else 
        printf("Data Invalid!\n");

}
/* tick : 언제 발생했는지 */
void cb_func_dht22(int pi, unsigned user_gpio, unsigned level, uint32_t tick)
{
    static int data_bit_offset = 0;
    int duration = tick - start_tick_;
    if(duration < 0)
        return;

    start_tick_ = tick;
    call_count_++;


    if(call_count_ == 1) 
        data_bit_offset = 0;
    else if(call_count_ == 2) ;
    else if(call_count_ == 3) ;
    else {
        data_[data_bit_offset / 8] <<= 1;      // shift
        data_[data_bit_offset / 8] |= (duration > 100 ? 1 : 0);
//        printf("%d", (duration > 100?1:0));
        data_bit_offset++;
    }
}

void *alertLED(void *p)
{
    set_mode(pi, LED, PI_OUTPUT);

    for(int i = 0; i < 5; i++){
        gpio_write(pi, LED, 1);
        time_sleep(1);
        gpio_write(pi, LED, 0);
        time_sleep(1);
    }
}
