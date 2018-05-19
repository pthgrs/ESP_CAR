#include<stdio.h>                                                               
#include<unistd.h>                                                              
#include<pigpio.h>                                                              
                                                                                
#define SDAPIN 4                                                                
#define TRIG_PINNO 16                                                           
#define ECHO_PINNO 6                                                            
                                                                                
void cb_func_DHT22(int gpio, int level, uint32_t tick);                         
                                                                                
void trigger(void);                                                             
void cb_func_echo(int gpio, int level, uint32_t tick);                          
                                                                                
uint32_t start_tick_tempC;                                                      
uint32_t start_tick,dist_tick;                                                  
                                                                                
uint8_t data_[5];                                                               
int call_count_ = 0;                                                            
                                                                                
int main(){                                                                     
    float tempC;                                                                
    //float humidity;                                                           
                                                                                
    float distance;                                                             
    gpioCfgClock(2, 1, 1);                                                      
                                                                                
    if(gpioInitialise() < 0) return 1;                                          
                                                                                
    gpioSetMode(SDAPIN , PI_OUTPUT);                                            
    gpioSetMode(TRIG_PINNO, PI_OUTPUT);                                         
    gpioSetMode(ECHO_PINNO, PI_INPUT);                                          
                                                                                
                                                                                
    start_tick_tempC = gpioTick();                                              
    gpioWrite(SDAPIN, 1); //init.                                               
    time_sleep(1);                                                              
    gpioSetAlertFunc(SDAPIN, cb_func_DHT22);                                    
                                                                                
    gpioWrite(SDAPIN, 0);                                                       
    time_sleep(0.0008);                                                         
                                                                                
    gpioSetPullUpDown(SDAPIN, PI_PUD_UP);                                       
    gpioSetMode(SDAPIN, PI_INPUT);                                              
                                                                                
    usleep(10000);                                                              
                                                                                
    if(call_count_ == 43 && data_[4] == ((data_[0] + data_[1] + data_[2] + data{
        //humidity = (data_[0] * 256 + data_[1])/10.0f;                         
        tempC = ((data_[2] & 0x7f) * 256 +data_[3]) / 10.0f;                    
        if(data_[2] & 0x80)                                                     
            tempC *= -1.0f;                                                     
    }else                                                                       
        printf("Data Invalid!\n");                                              
                                                                                
    printf("Temperature : %.1fC \n", tempC);                                    
                                                                                
                                                                                
    gpioSetAlertFunc(ECHO_PINNO, cb_func_echo);                                 
    gpioWrite(TRIG_PINNO, PI_OFF);                                              
    gpioDelay(1000000); //delay 1 second                                        
                                                                                
    while(1){                                                                   
        start_tick = dist_tick = 0;                                             
        gpioTrigger(TRIG_PINNO, 10, PI_HIGH);                                   
        gpioDelay(50000);                                                       
                                                                                
        if(dist_tick && start_tick){                                            
            distance = dist_tick / 1000000. * 340 / 2 * 100;                    
            if(distance <2 || distance > 400)                                   
                printf("range error \n");                                       
            else                                                                
                printf("interval : %6dus, Distance : %6.1f cm\n", dist_tick, di 
            }                                                                   
        else{                                                                   
            printf("senser error\n");                                           
        }                                                                       
        gpioDelay(1000000);                                                     
    }                                                                           
                                                                                
    gpioTerminate();                                                            
    return 0;                                                                   
}                                                                               
                                                                                
void cb_func_DHT22(int gpio, int level, uint32_t tick){                         
    static int data_bit_offset = 0;                                             
                                                                                
    if(level != 0) return ; //check falling edge.                               
                                                                                
    int duration = tick - start_tick_tempC ;                                    
    start_tick_tempC = tick;                                                    
    call_count_ ++;                                                             
                                                                                
   // printf("%2d duration : %3d   ", call_count_ , duration);                  
                                                                                
    if(call_count_ == 1){                                                       
     //   printf("init");                                                       
        data_bit_offset = 0;                                                    
    }else if(call_count_ == 2) ;// printf("Tbe+Tgo");                           
    else if(call_count_ ==3)  ;//printf("Trel+Treh");                           
    else{                                                                       
        data_[data_bit_offset /8 ] <<= 1;//shift                                
        data_[data_bit_offset /8 ] |= (duration > 100 ? 1 : 0);                 
      //  printf("%d", (duration > 100 ? 1 : 0));                               
        data_bit_offset ++;                                                     
    }                                                                           
   // printf("\n");                                                             
}                                                                               
                                                                                
void cb_func_echo(int gpio, int level, uint32_t tick)                           
{                                                                               
    if(level == PI_HIGH)                                                        
        start_tick = tick ;                                                     
    else if(level == PI_LOW)                                                    
        dist_tick = tick - start_tick;                                          
} 
