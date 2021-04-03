#include <SoftwareSerial.h>
#include <LowPower.h>

SoftwareSerial* hc12 = NULL;

String buffer= "";
int currentch = 1;
bool debugging = false;

const int invalidint = -999;

#define responsetimeout 30000 
#define broadcastcount 4
#define sleepcont 1650

#define VCC 2
#define RD 4
#define TD 3
#define set 5

#define splitchar '/'
#define endchar ';'
#define commandlen 2
#define emisorlen 2
#define receptorlen 2
#define datalen 8

#define sensorlen 1


#define routerid "00"
#define comunitarioid "01"

#define syncc "00"
#define pozoc "01"

#define synd "00000000"
#define ackd "00000001"
#define gosleepd "99999999"

const String id = routerid;
//msg code: "command2/emisor2/receptor2/data8;"
//     ex:      ;09/02/07/10110011;
/*
commands:
00-establish communication
01-waterhole sensor data

ids:
00-receptor/router
01-pozo comunitario

99-broadcast
*/
const int msglen = commandlen + emisorlen + receptorlen + datalen + 5;
struct Message{
				bool updated;
				String command;
				String emisor;
				String receptor;
				String data;
};

Message messages[sensorlen];
const String idlist[sensorlen + 1] = {routerid, comunitarioid};
int synbytes[sensorlen];

void debug(String s, bool line = true){
				if (debugging){
								if (line){
								    Serial.println(s);
				     }
								else{
												Serial.print(s);
								}
								delay(100);
				}
}

void debug(int i, bool line = true, bool ischar = true){
				if (debugging){
								if (line){
								    Serial.println(i);
				     }
								else{
												Serial.print(Serial.println(i));
								}
								delay(100);
				}
}
				

void starthc12(){
				digitalWrite(VCC, HIGH);
				digitalWrite(set, HIGH);
				delay(50);
				hc12 = new SoftwareSerial(RD, TD);
				delay(50);
				hc12->begin(9600);
				delay(50);
}

void stophc12(){
				hc12->end();
				delay(50);
				
				digitalWrite(VCC, LOW);
				digitalWrite(set, LOW);
				
				
				delete(hc12);
				hc12 = NULL;
}

void sleep(int _sleepcont){
				
			stophc12();
			if (debugging){
								digitalWrite(LED_BUILTIN, LOW);
				}
 		 
				
     for(int i=0; i< _sleepcont;i++){
								LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
				}
				
				if (debugging){
								digitalWrite(LED_BUILTIN, HIGH);
				}
				starthc12();
}

void blink(int _cont){
				for(int i=0; i<_cont; i++){
								digitalWrite(LED_BUILTIN, LOW);
								delay(100);
								digitalWrite(LED_BUILTIN, HIGH);
				}
}

bool _isdigit(char c){
				if (c >= '0' && c <='9'){
								return true;
				}
				else{
								return false;
				}
}

int _validatebuffer(){ // ret index where msg starts in buffer
				int i, si, endsum, blen;
				
				blen = buffer.length();
				
				if (blen < msglen){
								return -1;
				} 
				//search for message start
				
				for (i=0; i<blen; i++){
								if ((buffer[i] == endchar) && ((i + msglen -1) < blen) && (buffer[i + msglen -1] == endchar)){
											 si= i;
												break;
								}
								
				}
				if (i >= blen){// nomsg start found
								return -1;
				}
				
				endsum = commandlen +si + 1;
				for(i = si+ 1; i<endsum; i++){
								if (!_isdigit(buffer[i])){
												return -1;
								}
				}
				if(buffer[i] != splitchar){
								return -1;
				}
				
				endsum = si +emisorlen+commandlen+2;
				for(i++; i<endsum; i++){
								if (!_isdigit(buffer[i])){
												return -1;
								}
				}
				if(buffer[i] != splitchar){
								return -1;
				}
				
				endsum = si+ receptorlen + emisorlen+commandlen+3;
				for(i++; i<endsum; i++){
								if (!_isdigit(buffer[i])){
												return -1;
								}
				}
				if(buffer[i] != splitchar){
								return -1;
				}
				
				endsum= si +receptorlen + emisorlen+commandlen+4+datalen;
				for(i++; i< endsum; i++){
								if (!_isdigit(buffer[i])){
												return -1;
								}
				}
				
				return si;
}

int _pow(int b, int e){
				int n=1;
				for (int i=0; i<e; i++){
								n *= b;
				}
				
				return n;
}

int _stoi(String s){
				int n =0;
				if (s.length() == 0){
								return invalidint;
				}
				
				for (int i= 0; i<s.length(); i++){
								if (!_isdigit(s[i])){
												return invalidint;
								}
								n += _pow(10,s.length() - i -1)*(s[i]-'0');
				}
				return n;
}

bool buffertomsg(int msgind){ //true if message format was valid
				int l, sind;
				
				sind = _validatebuffer();
				if(sind < 0){//check for a valid msg in buffer
								return false;
				}
				l = commandlen + sind +1;
				messages[msgind].command = buffer.substring(sind+1, l);
				l++;
				
				messages[msgind].emisor = buffer.substring(l, l+emisorlen);
				l += emisorlen+1;
				
				messages[msgind].receptor = buffer.substring(l, l+receptorlen);
				l += receptorlen+1;
				
				messages[msgind].data = buffer.substring(l, l+datalen);
				
				messages[msgind].updated = true;
				buffer = "";
				return true;
}



String readhc12(){
				String temp ="";
				
				
				while(hc12->available()){
								temp += char(hc12->read());
				}
				
				return temp;
}

void clearhc12(){
				readhc12();
}
				

bool changech(int channel){ //true if succesful
				clearhc12();
				String smsg = "AT+C0";
				String temp;
				if (channel <1 || channel > 99) return false;
				if (channel < 10) {
								smsg += '0';
				}
				smsg += channel;
				
				digitalWrite(set, LOW);
				delay(60);
				hc12->print(smsg);
				delay(60);
				temp = readhc12();
				
				if(temp != smsg) {
								digitalWrite(set, HIGH);
								delay(60);
								return false;
				}
				
				digitalWrite(set, HIGH);
				delay(80);
				currentch = channel;
				return true;
}


void sendmsg(String com, String em, String re, String data){
				String msg =  endchar + com + splitchar + em + splitchar + re + splitchar + data + endchar;
				hc12->print(msg);
				delay(100);
}

void clearmsg(int ind){
				
}

bool readmsg(int ind){
				readhc12();
				if (!buffertomsg(ind)){
								return false;
				}
				
				return true;
}

bool comparemsg(int ind, String com, String em, String re, String data){
				messages[ind].updated = false;
				
				if ((messages[ind].command == com) && 
				(messages[ind].emisor == em) &&
				(messages[ind].receptor == re) &&
				(messages[ind].data == data)){
								return true;
				}
				else{
								return false;
				}
}

//install setup, wont send all to sleep untill all slaves are sync
void routersetupsyn(){
//syn
				int c =0;
				//set all sensor syn to 0
				for(int i =0; i<sensorlen; i++){
								synbytes[i] = 0;
				}
				
				while (c <sensorlen){
								for (int i=0; i < sensorlen; i++){
												//if alrrady sync with this slave skip
												if (synbytes[i] == 0){
																changech(i+1);
											     	sendmsg(syncc, id, idlist[i+1], synd);
												     delay(responsetimeout);
												
											     	if (readmsg(i)){
															     	if (comparemsg(i, syncc, idlist[i+1], id, 
																        ackd)){          
																         c++;
												          }
											      }
												}		
			     	}			
				}
				
//sleep broadcast
				for (int k = 0; k < broadcastcount; k++){
								for (int i=0; i< sensorlen; i++){
								    changech(i+1);
								    sendmsg(syncc, id, idlist[i + 1], gosleepd);
			     	}
								delay(3000);
				}
				
}

void getdata(){

}


void wetup(){
				debugging = true;
				if (debugging == true ){
								pinMode(LED_BUILTIN, OUTPUT);
				     digitalWrite(LED_BUILTIN, HIGH);
				}
				
				pinMode(VCC, OUTPUT);
				pinMode(set, OUTPUT);
				
				starthc12();
				Serial.begin(9600);
				
				
				routersetupsyn();
}

void woop(){
			 sleep(sleepcont * 0.95);
				
				
}

void ZZZ(){
				
}