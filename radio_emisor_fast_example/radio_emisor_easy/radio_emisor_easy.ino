#include <SoftwareSerial.h>
#include <LowPower.h>


#define RD 4
#define TD 3
#define VCC 2

#define pinlength 2

int sensordown = A4;
int sensorup = A5;
int sleepcont = 1;
 
const int sensorpins[pinlength] = {sensordown, sensorup};

//SoftwareSerial HC12(RD, TD);

SoftwareSerial* hc12 = NULL;

String buffer;


void starthc12(){
				digitalWrite(VCC, HIGH);
				delay(50);
				hc12 = new SoftwareSerial(RD, TD);
				delay(50);
				hc12->begin(9600);
}

void stophc12(){
				hc12->end();
				delay(50);
				
				digitalWrite(VCC, LOW);
				
				delete(hc12);
				hc12 = NULL;
}

void sleep(){
				
				for(int i=0; i<pinlength;i++){
								digitalWrite(sensorpins[i], LOW);
				}
				
			stophc12();
				
     for(int i=0; i< sleepcont;i++){
								LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
				}
				
				for(int i=0; i<pinlength;i++){
								digitalWrite(sensorpins[i], HIGH);
				}
				
				starthc12();
}

void sendreads(){
				buffer = "00\n";
				
				if (digitalRead(sensordown)){
								buffer[0] = '0';
				}
				else{
								buffer[0] = '1';
				}
				
				if (digitalRead(sensorup)){
								buffer[1] = '0';
				}
				else{
								buffer[1] = '1';
				}
				if (hc12){
								hc12->print(buffer);
				}
				
}



void setup(){
				
				pinMode(VCC, OUTPUT);
				pinMode(sensordown, INPUT_PULLUP);
				pinMode(sensorup, INPUT_PULLUP);
				
				starthc12();
}

void loop(){
				sendreads();
				
				
				delay(50);
}