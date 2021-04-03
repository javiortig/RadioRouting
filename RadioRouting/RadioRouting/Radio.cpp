#include "Radio.h"

bool Radio::messageCmp(Message m, bool _anyCommand, bool _anyEmisor, bool _anyData){
				int c = 0;
				
				//command
				if ((!_anyCommand) && (m.command != message.command)){
								return false;
				}
				//command
				if ((!_anyEmisor) && (m.emisor != message.emisor)){
								return false;
				}
				//command
				if ((!_anyData) && (m.data != message.data)){
								return false;
				}
				
				return true;
}

void Radio::sendMessage(Message m){
				String message = START_C + m.command + m.emisor + m.dataLen + m.data + END_C;
				
				hc12->print(message);
				delay(100);
}

void Radio::_setMessageDefault(){
				message.command = -1;
				message.emisor = -1;
				message.dataLen = -1;
				message.data = "";
}

bool Radio::readMessage(bool clear){
				if (clear){
								buffer = _readhc12();
				}
				else{
								buffer += _readhc12();
				}
				
				
				
				if (!_searchInBuffer()){//nothing found
								_setMessageDefault();
								return false;
				}
				else{
								return true;
				}
}

void Radio::clearBuffer(){
				buffer = "";
}

void Radio::_clearhc12(){
				while(hc12->available()){
								hc12->read();
				}
}

String Radio::_readhc12(){
				String temp = "";
				while(hc12->available()){
								temp += hc12->read();
				}
				
				return temp;
}

bool Radio::changeChannel(int channel){ //true if succesful
				 clearBuffer();
				_clearhc12();
				String smsg = "AT+C0";
				String temp;
				if (channel <1 || channel > 99) return false;
				if (channel < 10) {
								smsg += '0';
				}
				smsg += String(channel);
				
				digitalWrite(set, LOW);
				delay(60);
				hc12->print(smsg);
				delay(60);
				temp = _readhc12();
				
				if(temp != smsg) {
								digitalWrite(set, HIGH);
								delay(60);
								return false;
				}
				
				digitalWrite(set, HIGH);
				delay(80);
				currentChannel = channel;
				return true;
}


bool Radio::_isDigit(char c){
				if (c >= '0' && c <='9'){
								return true;
				}
				else{
								return false;
				}
}

int Radio::_pow(int b, int e){
				int n=1;
				for (int i=0; i<e; i++){
								n *= b;
				}
				
				return n;
}

int Radio::_stoi(String s){
				int n =0;
				if (s.length() == 0){
								return -1;
				}
				
				for (int i= 0; i<s.length(); i++){
								if (!_isDigit(s[i])){
												return -1;
								}
								n += _pow(10,s.length() - i -1)*(s[i]-'0');
				}
				return n;
}

bool Radio::_searchInBuffer(){
				int i, bufferLen, c;
				int com, em, dl;
				String cs, es, dls, ds;
				cs="";
				es = "";
				dls = "";
				ds = "";
				
				bufferLen = buffer.length();
				
				for(i=0; i <bufferLen; i++){
								if(buffer[i] == START_C){
												break;
								}
				}
				
				
				//fields extraction
				for(c=0; i< bufferLen && c <COMMAND_LEN; i++, c++){
								if (!_isDigit(buffer[i])){
												return false;
								}
								else{
												cs += buffer[i];
								}
				}
				com = _stoi(cs);
				
				for(c=0; i< bufferLen && c <EMISOR_LEN; i++, c++){
								if (!_isDigit(buffer[i])){
												return false;
								}
								else{
												es += buffer[i];
								}
				}
				em = _stoi(es);
				
				for(c=0; i< bufferLen && c <DATALEN_LEN; i++, c++){
								if (!_isDigit(buffer[i])){
												return false;
								}
								else{
												dls += buffer[i];
								}
				}
				dl = _stoi(dls);
				
				for(c=0; i< bufferLen && c <dl; i++, c++){
			     	ds += buffer[i];
				}
				
				//out of index
				if (i >= bufferLen){
								return false;
				}
				
				message.command = com;
				message.emisor = em;
				message.dataLen = dl;
				message.data = ds;
				
				buffer = buffer.substring(i);
				
				return true;
}

void Radio::debug(String s, bool line = true){
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

void Radio::debug(int i, bool line = true){
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

void Radio::_starthc12(){
				digitalWrite(vcc, HIGH);
				digitalWrite(set, HIGH);
				delay(50);
				hc12 = new SoftwareSerial(rd, td);
				delay(50);
				hc12->begin(bRate);
				delay(50);
}

void Radio::_stophc12(){
				hc12->end();
				delay(50);
				digitalWrite(vcc, LOW);
				digitalWrite(set, LOW);
				
				delete(hc12);
				hc12 = NULL;
}

Radio::Radio(int _id,short int _vcc, short int _td, short int _rd, short int _set, int _bRate){
				this->vcc = _vcc;
				this->td = _td;
				this->rd = _rd;
				this->set = _set;
				this->bRate = _bRate;
				this->buffer = "";
				this->id = _id;
				this->sensorCount= 0;
				_setMessageDefault();
				
				
				pinMode(vcc, OUTPUT);
				pinMode(td, OUTPUT);
				pinMode(rd, OUTPUT);
				pinMode(set, OUTPUT);
				delay(50);
				_starthc12();
}

Radio::~Radio(){
				_stophc12();
}