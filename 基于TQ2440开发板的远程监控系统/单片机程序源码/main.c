#include <reg52.h>


void send_str(char *p)
{
	while(*p)
	{
		SBUF=*p;
		while(TI==0);
		TI=0;
		p++;
	}
}

void int0() interrupt 0
{
	char *com = "a\n";
	send_str(com);
}

void init()
{
   SCON = 0x50;            	   
   TMOD|= 0x20;                   
   PCON|= 0x80;                                                          
	
	TH1 = 0xF3;	  //12M���񣬲�����4800			
	TL1 = 0xF3;         
	TR1  = 1;                                                             
	ES   = 1;               
	EA   = 1;     
	
    EX0 = 1;	 //�ⲿ�ж�0
	IT0 = 1;	 //��Ե����
}
void main()
{
	 init();
	 while(1);
}