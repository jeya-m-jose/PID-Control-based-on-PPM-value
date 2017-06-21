#include<avr/io.h>
#include<avr/interrupt.h>
#include <stdlib.h>


int rpm=150;
int count=0;
long int clock_ms=0;
long time0 = 0, time1 = 0;
int error=0,preverror=0;;
int x = 60;  //maintained rpm

int m=0,n=0;
void usart_int(void)
{
  UCSR0A=0x00;
  UCSR0B=(1<< RXEN0)|(1<< TXEN0);
  UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
  UBRR0L= 0x67 ; 
  
}

void usart_send(int ch )

{
  
  while(UCSR0A!=(UCSR0A|(1<<UDRE0)));
  
  UDR0= ch;

}
void dis(char a[100])
{int i=0;
  while(a[i])
  {usart_send(a[i]);
  i++;
}
}


void disp(int a)
{
  int flag=0;
  int t=a;
  char dis[10];
  while(t>0)
  {
    flag++;
    t/=10;
    }
    t=a;
    int g=1,l=0;
    while(t>0)
    {
      l=t%10;
      dis[flag-g]=l+48;
      t/=10;
      g++;
      
      }
    for(int i=0;i<flag;i++)
    usart_send(dis[i]);
  }

int main(void)
{
  sei();
  
  usart_int();
 
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  
  
  DDRC|=0b00000000;
  DDRD|=0b11000000;
  PORTD|=0b01000000;
  DDRB|=0b00000010;
  
   
  
  
  TCCR0A|=(1<<WGM01);
  TCCR0B|=(1<<CS01)|(1<<CS00);
  
  TCCR1A |= (1<<COM1A1)|(1<<WGM10);
  TCCR1B |= (1<<CS10) | (1<<WGM12);
  
  OCR0A=250;
  
  OCR1A=200;
  sei();
  TIMSK0=(1<<OCIE0A);
  
  EIMSK|=(1<<INT0);
  EICRA|=(1<<ISC01);
  
 
  
  while(1)
  {
    pinMode(11,INPUT);
  pinMode(12,INPUT);
    m=digitalRead(11);
    n=digitalRead(12);
   
  }
  
  return 0;
}
  ISR(TIMER0_COMPA_vect)
  {
    clock_ms++;
  
    
  }
  
  ISR(INT0_vect)
  {
    count++;
     _delay_ms(200);
    if(count==1)
    {
      
     time0 = clock_ms; 
     if(rpm>x)
     error+=(rpm-x);
     else
     error+=(x-rpm);
    }
    
    else if(count==2)
    {
     time1 = clock_ms;
   //Serial.println(time1-time0);
     float time = time1+200 - time0;
     
      rpm = 1000*(60 / time);
     
      if(rpm>200)
      rpm=150;
      
      count=0;
      //overf=0;
      clock_ms=0;
      dis("RPM is ");
      disp(rpm);
      usart_send('\n');
      
      
      
      
      
      
      
      
      
      
     if(m==0||n==0)
     {
     if(rpm>=x-3&&rpm<x)
     {
      //do nothing to OCR1A
    
      OCR1A+=1;
     }
      if(rpm>x&&OCR1A>10)
     {
      error+=(rpm-x);
      OCR1A-=(rpm-x)*0.2;
      OCR1A-=(error*time1*0.0002);
      OCR1A-=(error-preverror)/time1;
      preverror=error;
      error=0;
     }
     else if(rpm<x-3&&OCR1A<200)
     {
      error+=(x-rpm);
      OCR1A+=(rpm-x)*0.2;
      OCR1A+=(error*time1*0.0002);
      OCR1A+=(error-preverror)/time1;
      preverror=error;
      
   error=0;
    }
     }
  }
  }
  
      
  
  
  
  
  
