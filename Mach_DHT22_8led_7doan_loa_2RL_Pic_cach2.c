#include <16f877a.h>
#fuses      hs
#use delay(clock=20M)

#define mod     pin_e1
#define up      pin_e0
#define dw      pin_e2

#define loa     pin_a2
#define rl1     pin_c1
#define rl2     pin_c2
#define led11    pin_c3
#define led22    pin_c4

unsigned int32 tt_mod,dem;

unsigned int8 ma7doan[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};

signed int8 led0,led1,led2,led3,led4,led5,led6,led7;
UNSIGNED int8 ND2,ND1,DA2,DA1;
unsigned int32    nd,da;
signed int8    ghnd,ghda;
void quet_led();
void tt_mode();
void delay_quet_led(unsigned int8 h);
#include <dht22.c>

void tt_mode()
{
   if(tt_mod==0)
   {
      if(DHT_GetTemHumi(ND2,ND1,DA2,DA1)==1)
      {
         nd=ND2*256+ND1;
         nd=nd/10;
         da=DA2*256+DA1;
         da=da/10;
         
         if(nd>ghnd)
         {
            led0 = ma7doan[nd/10];
            led1 = ma7doan[nd%10]; 
            led2 = 0x9c;                              
            led3 = 0xc6; 
            
            led4 = ma7doan[ghnd/10];           
            led5 = ma7doan[ghnd%10];   
            led6 = 0x9c;        
            led7 = 0xc6;   
            output_high(rl1);
            output_low(rl2);
            output_high(loa);       
            output_high(led11);
            output_low(led22);
            delay_quet_led(65);
            output_low(loa);       
            output_low(led11);
            output_low(led22);
            delay_quet_led(65);
         }
         else if(da<ghda)
         {
            led0 = ma7doan[da/10];
            led1 = ma7doan[da%10]; 
            led2 = 0xaf;                              
            led3 = 0x89; 
            
            led4 = ma7doan[ghda/10];           
            led5 = ma7doan[ghda%10];   
            led6 = 0xaf;        
            led7 = 0x89;  
            output_high(rl2);
            output_low(rl1);
            output_high(loa);       
            output_high(led22);
            output_low(led11);
            delay_quet_led(65);
            output_low(loa);       
            output_low(led22);
            output_low(led11);
            delay_quet_led(65);
         }
         else
         {
            output_low(rl1);
            output_low(rl2);
            output_low(loa);       
            output_low(led22);
            output_low(led11);
            led0 = ma7doan[nd/10];
            led1 = ma7doan[nd%10]; 
            led2 = 0x9c;                              
            led3 = 0xc6; 
            
            led4 = ma7doan[da/10];           
            led5 = ma7doan[da%10];   
            led6 = 0xaf;        
            led7 = 0x89;   
         } 
      }    
   }
   else if(tt_mod==1)
   {
      led0 = ma7doan[5];
      led1 = 0x86;
      led2 = ma7doan[ghnd/10];
      led3 = ma7doan[ghnd%10]; 
      led4 = 0x9c;                              
      led5 = 0xc6;  
      led6 = 0xff;        
      led7 = 0xff;   
   }
   else if(tt_mod==2)
   {
      led0 = ma7doan[5];
      led1 = 0x89;
      led2 = ma7doan[ghda/10];
      led3 = ma7doan[ghda%10]; 
      led4 = 0xaf;                              
      led5 = 0x89;  
      led6 = 0xff;        
      led7 = 0xff;   
   }
}

void quet_led()   
{
   output_d(led0);      output_low(pin_b2);        delay_ms(1);         output_high(pin_b2);        // 100ms thi hien thi lam 5 lan 2 led
   output_d(led1);      output_low(pin_b3);        delay_ms(1);         output_high(pin_b3);
   output_d(led2);      output_low(pin_b4);        delay_ms(1);         output_high(pin_b4);        // 100ms thi hien thi lam 5 lan
   output_d(led3);      output_low(pin_b1);        delay_ms(1);         output_high(pin_b1);
   output_d(led4);      output_low(pin_b5);        delay_ms(1);         output_high(pin_b5);        // 100ms thi hien thi lam 5 lan
   output_d(led5);      output_low(pin_a0);        delay_ms(1);         output_high(pin_a0);      
   output_d(led6);      output_low(pin_a1);        delay_ms(1);         output_high(pin_a1);        // 100ms thi hien thi lam 5 lan            // 1000ms/8ms = 125 lan trong 1 giay
   output_d(led7);      output_low(pin_b0);        delay_ms(1);         output_high(pin_b0);         
}

void delay_quet_led(unsigned int8 h)
{
   unsigned int8 l;
   for(l=0;l<h;l++)
   {
      quet_led();   
   }
}

void phim_mod()         // kiem tra nut nhan mod
{
   if(input(mod)==0)      // kiem tra co nhan nut khong
   {
      delay_quet_led(3)
      if(input(mod)==0)   // con nhan nut khong
      {  
         if(tt_mod==0)
         {
            dem=0;
            do
            {
               dem++;
               quet_led();
            }
            while((input(mod)==0)&&(dem<625));           // cho nha phim

            if(dem>625)
            {
               tt_mod=1;
               tt_mode();
               while(input(mod)==0)
               {
                  quet_led();   
               }
            }   
         }
         else if(tt_mod==1)
         {
            tt_mod=2;
            tt_mode();
            while(input(mod)==0)
            {
               quet_led();   
            }
         }
         else if(tt_mod==2)
         {
            tt_mod=0;
            output_high(loa);       delay_quet_led(15); 
            output_low(loa);        delay_quet_led(15); 
            output_high(loa);       delay_quet_led(15); 
            output_low(loa);        delay_quet_led(15); 
            tt_mode();
            while(input(mod)==0)
            {
               quet_led();   
            }
         }
      }
   }
}

 void phim_up()
 {
   if(input(up)==0)
   {
     delay_quet_led(3);
      if (input(up)==0)
      {
         if(tt_mod==1)
         {  
            ghnd++;
            delay_quet_led(30);
            if(ghnd>80)          ghnd=80;
            write_eeprom(1,ghnd);
            ghnd=read_eeprom(1);
         }
         else if(tt_mod==2)
         {
            ghda++;
            if(ghda>99)          ghda=99;
            write_eeprom(2,ghda);
            ghda=read_eeprom(2);
         }
         tt_mode();
         delay_quet_led(10);
//!         while(input(up)==0)
//!         {
//!            quet_led();
//!         }
     }
   }
 }
 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void phim_dw()
 {
   if (input(dw)==0)
   {
      delay_quet_led(3);
      if (input(dw)==0)
      {
         if(tt_mod==1)
         {
               ghnd--;
               delay_quet_led(30);
            if(ghnd<0)          ghnd=0;
            write_eeprom(1,ghnd);
            ghnd=read_eeprom(1);
         }
         else if(tt_mod==2)
         {
            ghda--;
            if(ghda<0)          ghda=0;
            write_eeprom(2,ghda);
            ghda=read_eeprom(2);
         }
         tt_mode();
         delay_quet_led(10);


void main()
{
   set_tris_d(0x00);
   set_tris_c(0x00);
   set_tris_e(0xff);
   tt_mod=0;
   output_low(rl1);
   output_low(rl2);
   ghnd=read_eeprom(1);
   ghda=read_eeprom(2);
   while(true)
   {
      phim_mod();
      phim_up();
      phim_dw();
      tt_mode();
      quet_led();
   }
}


