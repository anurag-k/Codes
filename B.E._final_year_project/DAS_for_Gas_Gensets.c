unsigned get_temp1(unsigned int); // for temp sensor type 1
long get_temp2(unsigned int); // for temp sensor type 1
long get_pr(unsigned int);//for pressure sensor
long get_v(unsigned int);
void checkfault();
void disp();
void disp_fault();

long temp1_arr[93]={70000,70875,71775,72625,73500,74350,75250,76125,77000,78000,79000,80000,
81000,81950,82900,83850,84000, 85775,86750,87725,88700,89725,90750,91750,92800,93850,94900,95950,
97000,98050,99100,100150,101200,102300,103400,104500,105600,106700,107800,108900,110000,111200,112400,
113600,114800,115975,117150,118325,119500,120750,122000,123250,124500,125750,127000,128250,129500,
130800,132100,133400,134700,136025,137350,138675,140000,141375,142750,144125,145500,146925,148350,
149775,151200,152650,154100, 155550,157000,158375,159750,161125,162500,164000,165500,167000,168500,
170000,171500,173000,174500,175975,177450,178925,180400};

char temp_bat[4],temp_ct[4],temp_lop[4],temp_rpm[4],temp_cot[4],temp_imt[4];
unsigned ct1,egt1,lop1,ct_neg=0,ack_flag=0;
unsigned bat_temp,ct_temp,ct,ct_int,ct_d1,ct_d2,lop_temp,lop_int,lop_d1,lop_d2;
unsigned egt_temp,bat_d1,bat_d2,ack=0,disp_del=24;
unsigned bat_int,cot_temp,cot,cot_int,cot_d1,cot_d2,imt_temp,imt,imt_int,imt_d1,imt_d2;
long bat,egt,lop;
long lop_thresh1_l=1800,lop_thresh2_l=1600,lop_thresh1_h=2800,lop_thresh2_h=3000,egt_thresh1=780,egt_thresh2=800;
unsigned cot_thresh1=7600,cot_thresh2=8000,imt_thresh1=5500,imt_thresh2=6000;
unsigned freq, freq_th=64,ct_thresh_l=3000,ct_thresh1=9500,ct_thresh2=9800,bat_thresh=0; //fault condition
unsigned disp_time_count=0,c1=2,cf=0,estop=0;
unsigned flag=0,fault=0,fault_code[7]={0,0,0,0,0,0,0},i,shutdown=0,fault_count=0,lop_case=2;
void main()
{
    TRISC.f1=0;    //rw
    TRISC.f3=0; //warning output
    TRISD.f5=0;  //acknowledge ip
    TRISC.f0=0; //rs
    TRISC.f2=0; //en
    TRISB=0x01; //PORTB as output port for LCD
    Lcd_Custom_Config(&PORTB,7,6,5,4,&PORTC,0,1,2); //PORTD configured for LCD
    TRISD.f4=1;   // for emergency stop
    INTCON.f7=1; //enabling global interrupt
    INTCON.f6=1; //enabling peripheral interrupt
    INTCON.f4=1;  //enabling int0 interrupt

    INTCON2.f6=0; //interrupt on falling edge
    PIR2.f1=0; //when 1, timer 3 overflow interrupt has occured
    PIE2.f1=1;//enabling timer 3 overflow interrupt
    TMR3H=0x00; //loading timer 3 registers with 0 initial value
    TMR3L=0x00; // timer 3 counts until FFFF or 65536
    T3CON=0xB0;//16 bit readable; prescaled by 8; input clock source=Fosc/4

  //timer3 to be used for display timing

    PIE1=0x01; // enable timer 1 interrupt
    PIR1.f0=0; //timer 1 interrupt flag=0(no interrupt has occured)
    TMR0L=0X00;//timer 0 as counter to count pulses with initial value=0
    TMR0H=0X00;
    TMR1H=0X15; //for 1 ms delay
    TMR1L=0XA0;
    T0CON=0b00111000; //stop,16bit,ext clk,H to L,bypass prescalar
    T1CON=0X00;// dont start timer1 yet

  //timer0 & 1 engaged in rpm measurement

    TRISA=0xFF; //all pins set as inputs
    TRISE.f0=1;


    T0CON.f7=1; //start counting pulses
    T1CON.f0=1; //start timer for 1ms
    T3CON.f0=1;//start timer3 for display
  comeback:  while(1)
    {
      estop=0;
      if(PORTD.f4==0)
      {
        delay_ms(2000);
        if(PORTD.f4==0)
        {
        estop=1;
        shutdown=1;
        break;
        }
      }


      for(i=0;i<7;i++)
      {
      if(fault_code[i]==0)

      fault_count++;
      if(fault_code[i]==2)
      {
      shutdown=1;
       break;
       }
      }
      if(fault_count<7)
       fault=1;   // fault is either a warning or shutdown
       else
       fault=0;
       fault_count=0;


       if(shutdown==1)
       break;
       if(fault==1)  // now the possible fault is a warning
       {
       shutdown=0;
       if(ack_flag==1)
       {
        ack_flag=0;
        ack=1;
       }

        if(ack==0)  //fault not yet acknowledged
       {
       PORTC.f3=1;  // give warning output
       disp_del=10;
       }
       else if(ack==1)
       {
       disp_del=24;
      // PORTC.f3=0;
       }
       }
       else if(fault==0)
       {
       shutdown=0;
       disp_del=24;
       ack=0;
       PORTC.f3=0;        // warning output
      }
      if(flag==1)       // flag is for specified delay
     {
      flag=0;
      if((fault==0) || ((fault==1)&&(ack==1)))
      disp();
      else if((fault==1) && (ack==0))
      disp_fault();

      }
     //1. battery voltage

       ADCON1=0b11000000;//right justified; conversion time=16Tosc and
                                 //all pins of PORTA as analog inputs
       ADCON0=0b01000001;//channel 0; ADC enabled
       delay_us(20);
       ADCON0.f2=1;// start conversion
       while(ADCON0.f2);
       bat_temp=(256*ADRESH)+ADRESL;


       if(bat_temp>0)
       {
       if(bat_temp>614)
       bat_thresh=716; //21V
       else
       bat_thresh=307; // 9V
       if(bat_temp<bat_thresh)
       fault_code[0]=1;
       else
       fault_code[0]=0;
        }


       ADRESL=0x00; // clearing ADRES resgisters
       ADRESH=0x00;

      //2. coolant temperature

       ADCON1=0b11000000;//right justified; conversion time=16Tosc and
                      //all pins of PORTA as analog inputs
       ADCON0=0b01001001;//channel 1; ADC enabled
       delay_us(20);
       ADCON0.f2=1;// start conversion
       while( ADCON0.f2);

       ct_temp=(256*ADRESH)+ADRESL;
       ct=get_temp1(ct_temp);
       ct=ct*250;  //to deg F
       if(ct>=3200)
       {
       ct=ct-3200;
       ct_neg=0;

       }
       else
       {
        ct=3200-ct;
        ct_neg=1;

      }

        //to deg C
       ct=(ct/9)*5;


       if (ct>ct_thresh1)
       {
       fault_code[1]=1;
        if(ct>ct_thresh2)
        fault_code[1]=2; }
       else
       fault_code[1]=0;

        if(ct<ct_thresh_l)
        {
        //if(PORTC.f1!=1)
        PORTC.f1=1;
        }     //low coolant temp  warning;
        else if(ct>ct_thresh_l)
        PORTC.f1=0;


       ADRESL=0x00; // clearing ADRES resgisters
       ADRESH=0x00;

      //3. lubricant oil pressure

       ADCON1=0b11000000;//right justified; conversion time=16Tosc and
                      //all pins of PORTA as analog inputs
       ADCON0=0b01010001;//channel 2; ADC enabled
       delay_us(20);
       ADCON0.f2=1;// start conversion
       while( ADCON0.f2);
       lop_temp=(256*ADRESH)+ADRESL;
       lop=get_pr(lop_temp);

       if(lop_temp<184 || lop_temp>286)
       {
        fault_code[2]=1;
        if(lop_temp<184)
        lop_case=0;
        else if(lop_temp>286)
        lop_case=1;
        if(lop_temp<163 || lop_temp>307)


        fault_code[2]=2;
        }
        else
        {
        lop_case=2;
       fault_code[2]=0;
       }

       ADRESL=0x00; // clearing ADRES resgisters
       ADRESH=0x00;

      //4. exhaust gas temperature

       ADCON1=0b11000000;//right justified; conversion time=16Tosc and
                      //all pins of PORTA as analog inputs
       ADCON0=0b01011001;//channel 3; ADC enabled
       delay_us(20);
       ADCON0.f2=1;// start conversion
       while( ADCON0.f2);
       egt_temp=(256*ADRESH)+ADRESL;
       egt=get_temp2(egt_temp);
      if(egt>egt_thresh1)
      {
      fault_code[3]=1;
      if(egt>egt_thresh2)
      fault_code[3]=2;
      }
       else
       fault_code[3]=0;



       ADRESL=0x00; // clearing ADRES resgisters
       ADRESH=0x00;

      //5. rpm



      //6. compressor outlet temperature

       ADCON1=0b11000000;//right justified; conversion time=16Tosc and
                      //all pins of PORTA as analog inputs
       ADCON0=0b01100001;//channel 5; ADC enabled
       delay_us(20);
       ADCON0.f2=1;// start conversion
       while( ADCON0.f2);
       cot_temp=(256*ADRESH)+ADRESL;
       cot=get_temp1(cot_temp);
       cot=cot*250;  //to deg F
       if(cot>=3200)
        {
       cot=cot-3200; //to deg C

       cot=(cot/9)*5;

     if(cot>cot_thresh1)
     {
       fault_code[5]=1;
       if(cot>cot_thresh2)
       fault_code[5]=2;
       }
       else
       fault_code[5]=0;
       }

       ADRESL=0x00; // clearing ADRES resgisters
       ADRESH=0x00;

      //7. intake manifold temperature

       ADCON1=0b11000000;//right justified; conversion time=16Tosc and
                      //all pins of PORTA as analog inputs
       ADCON0=0b01101001;//channel 5; ADC enabled
       delay_us(20);
       ADCON0.f2=1;// start conversion
       while( ADCON0.f2);
       imt_temp=(256*ADRESH)+ADRESL;
       imt=get_temp1(imt_temp);
       imt=imt*250;  //to deg F
       if(imt>=3200)
        {
       imt=imt-3200; //to deg C

       imt=(imt/9)*5;
       if(imt>imt_thresh1)
       {
       fault_code[6]=1;
       if(imt>imt_thresh2)
       fault_code[6]=2;
       }
       else
       fault_code[6]=0;
        }


       ADRESL=0x00; // clearing ADRES resgisters
       ADRESH=0x00;
  }
  if(shutdown==0)
  {
  goto comeback;
  }
  else if(shutdown==1)
  {
  PORTC.f3=0;
  Lcd_Custom_Cmd(LCD_CLEAR);
  Lcd_Custom_Cmd(LCD_CURSOR_OFF);
  Lcd_Custom_Chr(1,2,' ');
  for(i=0;i<7;i++)
  Lcd_Custom_Chr_cp(fault_code[i]+48);
  Lcd_Custom_Out_Cp("Shutdown due to:");
  Lcd_Custom_Chr(3,4,' ');
  if(estop==1)
  Lcd_Custom_Out_Cp("Emergency Stop");
  for(i=0;i<7;i++)
  if(fault_code[i]==2)
  {
  switch(i)
  {
  case 1:
  Lcd_Custom_Out_Cp("high coolant temp");
  break;

  case 2:
  if(lop_case==0)
  Lcd_Custom_Out_Cp("low L O P");
  else
  Lcd_Custom_Out_Cp("high L O P");
  break;

  case 3:
  Lcd_Custom_Out_Cp("high E G T");
  break;
  case 4:
  Lcd_Custom_Out_Cp("Overspeed");
  break;

  case 5:
  Lcd_Custom_Out_Cp("high C O Temp");
  break;

  case 6:
  Lcd_Custom_Out_Cp("high I M Temp");
  break;
  }

}

  TRISB=0x00;     // drive fuel shut off relay
  PORTD.f5=1;
}

}
void interrupt()
{
  if(INTCON.f1==1)     // if there's an interrupt on INT0 pin
  {
      INTCON.f1=0;
      if(PORTB.f0==0)
      {
      delay_ms(1);    // key debouncing
      if(PORTB.f0==0)
      ack_flag=1;
      }
}

  if(PIR1.f0) //timer 1 overflow
  {

      T1CON.f0=0; //disable timer1
      T0CON.f7=0;  // stop counter

      freq=256*TMR0H + TMR0L;
      if(freq>freq_th)
      fault_code[4]=2;
      else
      fault_code[4]=0;
      freq=freq*50;



      PIR1.f0=0; //clear timer 1 interrupt flag
      TMR1H=0X15; //reload timer 1 for 20ms delay
      TMR1L=0XA0;
      TMR0H=0x00;
      TMR0L=0x00;
      T1CON.f0=1; //restart timer 1
      T0CON.f7=1; //restart counter (timer0)


  }


  if(PIR2.f1) // if interrupt is due to timer3 overflow
  {
      PIR2.f1=0; // clear timer3 interrupt flag
      T3CON.f0=0;//stop timer 3

      if(disp_time_count==disp_del) //for generating x seconds delay
      {
        flag=1;
        disp_time_count=0;
        if(c1==1)
        c1=2;
        else if(c1==2)
        c1=1;

      }
      else
      disp_time_count++;

      TMR3H=0x00; // reload timer 3 with 0 value
      TMR3L=0x00;
      T3CON.f0=1;  //restart timer 3
  }
}


unsigned get_temp1(unsigned int ct1)
{
  long v,r;
  unsigned i;
  v=(5000*(long)ct1)/1023;
  //r=(100000*v)/(5000-v);
  r=v*50;
  for(i=0;i<93;i++)
  {
   if(temp1_arr[i]>r)
   break;
  }
  return(i);
}

long get_temp2(unsigned int egt1)
{
  long v,r;
  long t2;
  v=(5000*(long)egt1)/1023;

  r=v*50;
  if(r<=13850)
  t2=(r-10000)/38;
  else if(r>13850 && r<=17580)
  t2=(r-10105)/37;
  else if(r>17580 && r<=24710)
  t2=(r-10450)/36;
  else if(r>24710)
  t2=(r-11850)/32;
  return(t2);
}

long get_pr(unsigned int lop1)
{
   long v,p;
   v=(5000*(long)lop1)/1023;
   p=2*v;
   return(p);
}
long get_v(unsigned int bat1)
{
 long v;
 v=(5000*(long)bat1)/1023;
 return(v);
}
void disp()
{
Lcd_Custom_Cmd(LCD_CURSOR_OFF);
Lcd_Custom_Cmd(LCD_CLEAR);
 switch(c1)
        {
          case 1:
          bat=get_v(bat_temp);
          bat=bat*6;
          bat_int=bat/1000;
          bat=bat/10;// integer part
        //  bat_d2=bat%10;  // fractional part upto 2 dec places
          bat=bat/10;
          bat_d1=bat%10;
          temp_bat[0]=(bat_int/10)+48;
          temp_bat[1]=bat_int%10 + 48;
         /* Lcd_Custom_Cmd(LCD_CLEAR);
          Lcd_Custom_Cmd(LCD_CURSOR_OFF); */
          Lcd_Custom_Out(1,1," Batt V  :");
          Lcd_Custom_Out(1,12,temp_bat);
          Lcd_Custom_Chr_CP('.');
          Lcd_Custom_Chr_CP(48+bat_d1);
         // Lcd_Custom_Chr_CP(48+bat_d2);
          Lcd_Custom_Chr_CP('V');


          ct_int=ct/100;// integer part
       //   ct_d2=ct%10;  // fractional part upto 2 dec places
          ct=ct/10;
          ct_d1=ct%10;
          if(ct_neg==1)
          Lcd_Custom_Chr(2,11,'-');

          temp_ct[0]=(ct_int/100)+48;
          temp_ct[1]=(ct_int/10)%10 + 48;
          temp_ct[2]=ct_int%10 + 48;

          Lcd_Custom_Out(2,1," Coolant :");
          Lcd_Custom_Out(2,12,temp_ct);


          Lcd_Custom_Chr_CP('.');
          Lcd_Custom_Chr_CP(48+ct_d1);
         // Lcd_Custom_Chr_CP(48+ct_d2);
          Lcd_Custom_Chr_CP(223);
          Lcd_Custom_Chr_CP('C');




           lop_int=lop/100;// integer part
          // lop_d2=lop%10;  // fractional part upto 2 dec places
           lop=lop/10;
           lop_d1=lop%10;
           //converting to ASCII for display
           temp_lop[0]=(lop_int/100)+48;
           temp_lop[1]=(lop_int/10)%10 + 48;
           temp_lop[2]=lop_int%10 + 48;


           Lcd_Custom_Out(3,1," L O P   :");
           Lcd_Custom_Out(3,12,temp_lop);
           Lcd_Custom_Chr_CP('.');
           Lcd_Custom_Chr_CP(48+lop_d1);
          // Lcd_Custom_Chr_CP(48+lop_d2);
           Lcd_Custom_Chr_CP('p');
           Lcd_Custom_Chr_CP('s');
           Lcd_Custom_Chr_CP('i');



           Lcd_Custom_Out(4,1," E G Temp: ");

           Lcd_Custom_Chr_CP((egt/100)+48);
           Lcd_Custom_Chr_CP((egt/10)%10 + 48);
           Lcd_Custom_Chr_CP(egt%10 + 48);

           Lcd_Custom_Chr_CP(223);
           Lcd_Custom_Chr_CP('C');
        break;

           case 2:
          // c1=0;
           Lcd_Custom_Cmd(LCD_CLEAR);
           Lcd_Custom_Cmd(LCD_CURSOR_OFF);
           Lcd_Custom_Out(1,1," E RPM   : ");

           Lcd_Custom_Chr_CP((freq/1000)+48);
           Lcd_Custom_Chr_CP((freq/100)%10 + 48);
           Lcd_Custom_Chr_CP((freq/10)%10 + 48);
           Lcd_Custom_Chr_CP(freq%10 + 48);


                       //displaying parameter values
          cot_int=cot/100;// integer part
       //   cot_d2=cot%10;  // fractional part upto 2 dec places
          cot=cot/10;
          cot_d1=cot%10;
          //converting to ASCII for display
          temp_cot[0]=(cot_int/100)+48;
          temp_cot[1]=(cot_int/10)%10 + 48;
          temp_cot[2]=cot_int%10 + 48;

          Lcd_Custom_Out(2,1," C O Temp:");
          Lcd_Custom_Out(2,12,temp_cot);
          Lcd_Custom_Chr_CP('.');
          Lcd_Custom_Chr_CP(48+cot_d1);
         // Lcd_Custom_Chr_CP(48+cot_d2);
          Lcd_Custom_Chr_CP(223);
          Lcd_Custom_Chr_CP('C');


                       //displaying parameter values
          imt_int=imt/100;// integer part
       //   imt_d2=imt%10;  // fractional part upto 2 dec places
          imt=imt/10;
          imt_d1=imt%10;
          //converting to ASCII for display
          temp_imt[0]=(imt_int/100)+48;
          temp_imt[1]=(imt_int/10)%10 + 48;
          temp_imt[2]=imt_int%10 + 48;
          Lcd_Custom_Out(3,1," I M Temp:");
          Lcd_Custom_Out(3,12,temp_imt);
          Lcd_Custom_Chr_CP('.');
          Lcd_Custom_Chr_CP(48+imt_d1);
       //   Lcd_Custom_Chr_CP(48+imt_d2);
          Lcd_Custom_Chr_CP(223);
          Lcd_Custom_Chr_CP('C');
          break;
        }
 }
    void disp_fault()
    {
    Lcd_Custom_Cmd(LCD_CLEAR);
    Lcd_Custom_Cmd(LCD_CURSOR_OFF);


    if(fault_code[cf]==1)
    {
    switch(cf)
    {
    case 0:

    Lcd_Custom_Out(1,7,"WARNING!");
    Lcd_Custom_Out(3,2,"Low Battery Voltage");
    cf++;
    break;

    case 1:

    Lcd_Custom_Out(1,7,"WARNING!");
    Lcd_Custom_Out(3,2,"Coolant temp high");
    cf++;
    break;

    case 2:

    Lcd_Custom_Out(1,7,"WARNING!");
    if(lop_case==0)

    Lcd_Custom_Out(3,5,"L O P low");

    else
    Lcd_Custom_Out(3,5,"L O P high");
    cf++;
    break;

    case 3:

    Lcd_Custom_Out(1,7,"WARNING!");
    Lcd_Custom_Out(3,4,"E G Temp high");
    cf=cf+2;
    break;

    case 5:

    Lcd_Custom_Out(1,7,"WARNING!");
    Lcd_Custom_Out(3,4,"C O Temp high");
    cf++;
    break;

    case 6:

    Lcd_Custom_Out(1,7,"WARNING!");
    Lcd_Custom_Out(3,4,"I M Temp high");
    cf=0;
    break;



    }
    }
    else
    {
    while((fault_code[cf]!=1) && (cf<=6))
    cf++;

     if(cf>6)

    cf=0;
    }

 }



