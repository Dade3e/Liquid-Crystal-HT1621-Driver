#define CS	 6  //Pin 13 as chip selection output
#define WR	 7  //Pin 12 as read clock	output
#define DATA 8	//Pin 7 as Serial data output
 
#define CS1    digitalWrite(CS, HIGH) 
#define CS0    digitalWrite(CS, LOW)
#define WR1    digitalWrite(WR, HIGH) 
#define WR0    digitalWrite(WR, LOW)
#define DATA1  digitalWrite(DATA, HIGH) 
#define DATA0  digitalWrite(DATA, LOW)
 
 
#define sbi(x, y)  (x |= (1 << y))   
#define cbi(x, y)  (x &= ~(1 <<y ))  
#define uchar   unsigned char 
#define uint   unsigned int 
 
//定义HT1621的命令 
#define  ComMode    0x52  //4COM,1/3bias  1000    010 1001  0  
#define  RCosc      0x30  //1000 0011 0000 
#define  LCD_on     0x06  //1000     0000 0 11 0 
#define  LCD_off    0x04  
#define  Sys_en     0x02  //1000   0000 0010 
#define  CTRl_cmd   0x80  
#define  Data_cmd   0xa0  
 
 
/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,H,h,L,n,N,o,P,r,t,U,-, ,*/
 
const char num_old[]={0x7D,0x60,0x3E,0x7A,0x63,0x5B,0x5F,0x70,0x7F,0x7B,0x77,0x4F,0x1D,0x0E,0x6E,0x1F,0x17,0x67,0x47,0x0D,0x46,0x75,0x37,0x06,0x0F,0x6D,0x02,0x00,};
                 //0      1      2      3        4      5      6       7       8       9
char dispnum[6]={0x00,0x00,0x00,0x00,0x00,0x00};

//                0     1     2     3     4     5     6     7     8     9     ^o    o
const char num[]={0x3f, 0x09, 0x67, 0x4f, 0x59, 0x5e, 0x7e, 0x0b, 0x7f, 0x5f, 0x53, 0x6C};

 
void SendBit_1621(uchar sdata,uchar cnt)
{ 
	//data cnt HT1621
	uchar i; 
	for(i=0;i<cnt;i++) 
	{ 
		WR0;
		if(sdata&0x80) DATA1; 
		else DATA0;
		WR1;
		sdata<<=1; 
	} 
}
 
void SendCmd_1621(uchar command) 
{ 
	CS0; 
	SendBit_1621(0x80,4);  
	SendBit_1621(command,8); 
	CS1;                     
}
 
void Write_1621(uchar addr,uchar sdata)
{ 
	addr<<=2; 
	CS0; 
	SendBit_1621(0xa0,3);     
	SendBit_1621(addr,6);     
	SendBit_1621(sdata,8);   
	CS1; 
}
 
void HT1621_all_off(uchar num) 
{
	uchar i; 
	uchar addr=0; 
	for(i=0;i<num;i++) 
	{ 
		Write_1621(addr,0x00); 
		addr+=2; 
	} 
}
 
void HT1621_all_on(uchar num)
{
	uchar i; 
	uchar addr=0; 
	for(i=0;i<num;i++) 
	{
		Write_1621(addr,0xff); 
		addr+=2; 
	} 
}
 
void Init_1621(void)
{
	SendCmd_1621(Sys_en);
	SendCmd_1621(RCosc);    
	SendCmd_1621(ComMode);  
	SendCmd_1621(LCD_on);
}    
 
void displaydata(int p)
{
	uchar i=0;
	switch(p)
	{
		case 1:
		sbi(dispnum[0],7);
		break;
		case 2:
		sbi(dispnum[1],7);
		break;
		case 3:
		sbi(dispnum[2],7);
		break;
		default:break;
	}
	for(i=0;i<=5;i++) 
	{
		Write_1621(i*2,dispnum[i]);
	}
}

void draw_n(uint8_t dig, uint8_t value){
  
  Write_1621(6,bitRead(value, 6) * (16 * dig) + bitRead(value, 7)*dig);
  Write_1621(4,bitRead(value, 5) * (16 * dig) + bitRead(value, 4)*dig);
  Write_1621(2,bitRead(value, 3) * (16 * dig) + bitRead(value, 2)*dig);
  Write_1621(0,bitRead(value, 1) * (16 * dig) + bitRead(value, 0)*dig);
}

void draw(uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4){

  // --2--
  //5     1
  //|--7--|
  //6     4
  // --3--
  // numero 5 = B01011110 = 0x54 = 94
  
  uint8_t raw6 = ((bitRead(dig1, 6) * 16 *8 ) + (bitRead(dig2, 6) * 16*4 ) + 
  (bitRead(dig3, 6) * 16*2) + (bitRead(dig4, 6) * 16) +
  bitRead(dig1, 7)*8 + bitRead(dig2, 7)*4 + bitRead(dig3, 7)*2 + bitRead(dig4, 7));

  uint8_t raw4 = ((bitRead(dig1, 5) * 16 *8) + (bitRead(dig2, 5) * 16*4 ) + 
  (bitRead(dig3, 5) * 16*2) + (bitRead(dig4, 5) * 16) +
  bitRead(dig1, 4)*8 + bitRead(dig2, 4)*4 + bitRead(dig3, 4)*2 + bitRead(dig4, 4));
  
  uint8_t raw2 = ((bitRead(dig1, 3) * 16 *8) + (bitRead(dig2, 3) * 16*4 ) + 
  (bitRead(dig3, 3) * 16*2) + (bitRead(dig4, 3) * 16) +
  bitRead(dig1, 2)*8 + bitRead(dig2, 2)*4 + bitRead(dig3, 2)*2 + bitRead(dig4, 2));

  uint8_t raw0 = ((bitRead(dig1, 1) * 16 *8) + (bitRead(dig2, 1) * 16*4 ) + 
  (bitRead(dig3, 1) * 16*2) + (bitRead(dig4, 1) * 16) +
  bitRead(dig1, 0) * 8 + bitRead(dig2, 0)* 4 + bitRead(dig3, 0)*2 + bitRead(dig4, 0));
  Write_1621(6,raw6);
  Write_1621(4,raw4);
  Write_1621(2,raw2);
  Write_1621(0,raw0);
}

void setup() {
  pinMode(CS, OUTPUT); // 
  pinMode(WR, OUTPUT); // 
  pinMode(DATA, OUTPUT); //
  CS1;
  DATA1;
  WR1;
  delay(50);
  Init_1621();
  HT1621_all_on(16); //1621
  delay(100);
  HT1621_all_off(16); //1621  
  delay(100);
 
  //displaydata(1);//light on the first decimal point starting from the right side
  dispnum[5]=num[5];
  dispnum[4]=num[4];
  dispnum[3]=num[3];
  dispnum[2]=num[2];
  dispnum[1]=num[1];
  dispnum[0]=num[0];
  
  sbi(dispnum[5],7);   
  //cbi(dispnum[5],7); 
  sbi(dispnum[4],7);  
  //cbi(dispnum[4],7); 
  sbi(dispnum[3],7);   
  //cbi(dispnum[3],7); 
  
  //frog face
  Write_1621(0,0x66);
  Write_1621(2,0x1f);
  Write_1621(4,0x86);
  Write_1621(6,0x60);

  HT1621_all_off(16);

  byte b = B01111111;
  draw_n(1, b);

  
  b = B01011110;
  draw(num[0],num[1],num[2],num[3]);  
  
 
}
 
void loop() {
 
 // you can place your main code here to run repeatedly
 
}