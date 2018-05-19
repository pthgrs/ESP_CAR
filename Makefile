EXAMPLES = dht22_hcsr04.out \

all : $(EXAMPLES)

dht22_hcsr04.out : dht22_hcsr04.c
	gcc -Wall -W -pthread -lrt -lpigpio -Wno-unused-parameter $^ -o $@


clean : 
     rm $(EXAMPLES)


