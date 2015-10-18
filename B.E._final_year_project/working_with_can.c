unsigned long pgn;
void init();
void data(int);
void main()
 {
 init();
 }

void init()
{
 TRISB = 0x08; // Set CAN input and output
 TRISD=0X00; //PORT D OUTPUT FOR LCD
 PIE3=0X01;
 Lcd_Init(&PORTD);
 CANCON = 0X80; // request to enter CAN configuration mode REQOP = 100
 while (CANSTAT!=0X80); //Wait to enter CAN configuration mode OPMODE = 100
 BRGCON1 = 0X03; // set SJW and BRP, SJW = 1TQ, BRP = 01H
 BRGCON2 = 0X90; // set Phase_Seg1 = 3TQ and Prog_Seg = 1TQ
 BRGCON3 = 0X42; // set Phase_Seg2 = 3TQ / / Set send mail 0 symbols and data
 RXB0SIDH = 0XFF; // set the receive buffer identifier 0
 RXB0SIDL = 0XFB;   //srr=1, EXID=1
 RXB0EIDH=0XFF;    //HIGH EXTENDED IDENTIFIER SET AS 1
 RXB0EIDL=0XFF;    //LOW EXTENDED IDENTIFIER SET AS 1
 RXB0CON = 0X40; // just to receive valid MSG DEPENDING of EXTENDED identifier, FILHIT0 = 0 means RXB0 uses filter0
                        //NOT USING FILTERS
 RXB0DLC = 0X08; // set the length of data area for receive buffer 0=8BYTES
 RXB0D0 = 0X00; // initialize data from receive buffer 0 data zone
 RXB0D1 = 0X00;
 RXB0D2 = 0X00;
 RXB0D3 = 0X00;
 RXB0D4 = 0X00;
 RXB0D5 = 0X00;
 RXB0D6 = 0X00;
 RXB0D7 = 0X00;
 RXM0SIDH = 0X00;   //FILTER DISABLED
 RXM0SIDL = 0X00;
 RXM0EIDH= 0X00;  //EXTENDED MASK DISABLED
 RXM0EIDL= 0X00;
 CIOCON = 0X00;
 CANCON = 0X00; // enable CAN enter normal operating mode
 while (CANSTAT!= 0) ;   //WAIT FOR NORMAL MODE TO ESTABLISH
 }

void interrupt()
{
  if(PIR3.f0==1)
  {
   unsigned char temph,temp1,templ,temp2;
   PIR3.f0=0;
   temph=RXB0SIDH;
   temph<<5;
   temp1=RXB0SIDL;
   temp1>>3;
   temph=temph|temp1;
   temp2=RXB0SIDL & 00000011;
   temph=temph | temp2;    //PDU and PF obtained in temph
   templ=RXB0EIDH;         //PDU specific obtained in templ
   pgn=(256*temph)+templ;
   switch(pgn)
   {
    case 61444:data(1);        //pgn 64870 for coolant temp dl=1 byte
               break;
    case 65262:data(2);        //pgn 64851 for exhaust gas temp, dl=2byte
               break;
    case 65263:data(3);
               break;         //for lube oil pressure, dl=1 byte
    default: data(4);
             break;
 }
 }
 }

 void data(int n)
 {
   int speedl,speedh,speed,ct,lop;
   char sp[7],chct[7],chlop[7];
   switch(n)
   {
  case 1: speedl=RXB0D4;
          speedh=RXB0D3;
          speed=256*speedh+speedl;
          Lcd_Cmd(Lcd_Clear);
          Lcd_Cmd(Lcd_Cursor_Off);
          IntToStr(speed,sp);
          Lcd_Out(1,5,"rpm");
          Lcd_Out(2,8,sp);
          break;
  case 2: ct=RXB0D0;
          Lcd_Cmd(Lcd_Clear);
          Lcd_Cmd(Lcd_Cursor_Off);
          IntToStr(speed,sp);
          Lcd_Out(1,5,"coolant temp");
          Lcd_Out(2,8,chct);
          break;
  case 3: lop=RXB0D3;
          Lcd_Cmd(Lcd_Clear);
          Lcd_Cmd(Lcd_Cursor_Off);
          IntToStr(speed,sp);
          Lcd_Out(1,5,"lube oil pressure");
          Lcd_Out(2,8,chlop);
          break;
  case 4: Lcd_Cmd(Lcd_Clear);
          Lcd_Cmd(Lcd_Cursor_Off);
          Lcd_Out(1,5,"invalid data");
          break;
   }
  }
