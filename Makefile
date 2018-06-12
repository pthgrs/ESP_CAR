project: 
	g++ -pthread -lrt -lpigpiod_if2 -lpaho-mqtt3c project.h direction.c mqtt.c hcsr04.c am2302.c record.c controller.c main.c -o test 

clean:
	rm test
