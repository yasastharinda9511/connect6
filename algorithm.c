  /*Appendix A - The Main Loop*/
 
  #include <p18f2550.h>
  #include <usart.h>
  #include <stdio.h>
  #include <delays.h>
  #include "config.h"
  typedef unsigned char bool;
  #define true 1
  #define false 0
  #define len(a) sizeof(a)/sizeof(a[0])
  #define EN LATAbits.LATA0 /* PORT for RS */
  #define TRIS_EN TRISAbits.TRISA0
  #define RS LATAbits.LATA1 /* PORT for RS */
  #define TRIS_RS TRISAbits.TRISA1 /* TRIS for RS*/
  #define DATA_PORT PORTB //Data Port for LCD
  #define TRIS_DATA_PORT TRISB

 short k = 0;
 short dup_array[2];
 short List[30];
 char LCDOpp[8]="Opp Move";
 char LCDMy[8]="My Moves";
 char LCDStart[7]="Welcome";
 char SecondMove[8];
 char FirstMove[4];
 char OppMoves[8];
 int Move1[2];
 int Move2[2];
 char PlayMove1[4],PlayMove2[4];
 int x0,y0,x1,y1,x2,y2;
 unsigned char side,Side[2];
 int r;
 unsigned char E='-';
 #pragma idata large_idata //Changed Memory Bank size to fit the grid


 char grid[19][19] = {{'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
{'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'},
 {'-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-', '-', '-','-', '-',
'-'}};

 #pragma idata
 int i=0;
 int j=0;
 short oppent_array1[2];
 short oppent_array2[2];
 short our_array1[2];
 short our_array2[2];
 short turn=1;
 short* ptr;
 short Moves[4];
 short *M;
 
 /*-----------------------------------Main Loop-------------------------------*/
  void main(void) {
  TRISA = 0x00;
  TRISB=0X00; //Sets A & B ports as outputs
  ADCON1 = 0x0F; //make RA digital
  OSCCON=0X76;
  OpenUSART(USART_TX_INT_OFF & //Initiate the USART Connection
  USART_RX_INT_OFF &
  USART_ASYNCH_MODE &
  USART_EIGHT_BIT &
  USART_CONT_RX &
  USART_BRGH_HIGH &
  USART_ADDEN_OFF, 26);
  lcd_init(); //Initiate the LCD
  while (BusyUSART());
  while (!DataRdyUSART());
  side=ReadUSART(); //Reads the side to play
    
  for (i=0;i<=3;i++){
  while (!DataRdyUSART());
  FirstMove[i]=ReadUSART(); //Reads the first move
  }
    
  x0=(FirstMove[0]-'0')*10+(FirstMove[1]-'0');
  x0=x0-1;
  y0=(FirstMove[2]-'0')*10+(FirstMove[3]-'0');
  y0=y0-1;
   //conversion of string to an array
    
 if (side=='B' && turn==1){
 grid[x0][y0]='B';
 our_array1[0]=x0;
 our_array1[1]=y0;
 display_my(FirstMove,FirstMove);
 }
else if (side=='W' && turn==1){
    x1=x0;
    y1=y0;
    x2=x1;
    y2=y1;
    display_opp(FirstMove);
      oppent_array1[0]=x1;
      oppent_array1[1]=y1;
      oppent_array2[0]=x2;
      oppent_array2[1]=y2;

      M=Con6player(oppent_array1,oppent_array2,grid,side);
      for (i=0;i<4;i++){
      Moves[i]=*(M+i);  
      } //Returns the moves to be played  
  
  PlayMove1[0]=((Moves[0]+1)/10)+'0';
  PlayMove1[1]=((Moves[0]+1)%10)+'0';
  PlayMove1[2]=((Moves[1]+1)/10)+'0';
  PlayMove1[3]=((Moves[1]+1)%10)+'0';
  PlayMove2[0]=((Moves[2]+1)/10)+'0';
  PlayMove2[1]=((Moves[2]+1)%10)+'0';
  PlayMove2[2]=((Moves[3]+1)/10)+'0';
  PlayMove2[3]=((Moves[3]+1)%10)+'0';
  //conversion of the array to a string
  
  for (i=0;i<=3;i++){
  while (BusyUSART());
  WriteUSART(PlayMove1[i]); //Sends the first move
  }
  
  for (i=0;i<=3;i++){
  while (BusyUSART());
  WriteUSART(PlayMove2[i]); //Sends the second move
   }
  
   display_my(PlayMove1,PlayMove2);
 }
    
   while(1){
   for (i=0;i<=7;i++){
   while (!DataRdyUSART()); //reads the Opponents moves
   SecondMove[i]=ReadUSART();
   }
     
   x1=(SecondMove[0]-'0')*10+(SecondMove[1]-'0');
   x1=x1-1;
   y1=(SecondMove[2]-'0')*10+(SecondMove[3]-'0');
   y1=y1-1;
   x2=(SecondMove[4]-'0')*10+(SecondMove[5]-'0');
   x2=x2-1;
   y2=(SecondMove[6]-'0')*10+(SecondMove[7]-'0');
   y2=y2-1;
 //conversion of string to array
   display_opp(SecondMove); //Displays opponent move on LCD
   oppent_array1[0]=x1;
   oppent_array1[1]=y1;
   oppent_array2[0]=x2;
   oppent_array2[1]=y2;
     
  M=Con6player(oppent_array1,oppent_array2,grid,side); //returns the moves to be sent       
  for (i=0;i<4;i++){
  Moves[i]=*(M+i);
  }
     
  PlayMove1[0]=((Moves[0]+1)/10)+'0'; //conversion of array to string
  PlayMove1[1]=((Moves[0]+1)%10)+'0';
  PlayMove1[2]=((Moves[1]+1)/10)+'0';
  PlayMove1[3]=((Moves[1]+1)%10)+'0';
     
  PlayMove2[0]=((Moves[2]+1)/10)+'0';
  PlayMove2[1]=((Moves[2]+1)%10)+'0';
  PlayMove2[2]=((Moves[3]+1)/10)+'0';
  PlayMove2[3]=((Moves[3]+1)%10)+'0';
     
for (i=0;i<=3;i++){
while (BusyUSART());
WriteUSART(PlayMove1[i]); //Sends the first move
 }
     
 for (i=0;i<=3;i++){
while (BusyUSART());
WriteUSART(PlayMove2[i]); //Sends the second move
 }
display_my(PlayMove1,PlayMove2); //displays move on LCD
 } 
    /*Appendix B - LCD Functions*/

  void delay() {
   Delay1KTCYx( 1 );
  } //Defining the delay function
  void longdelay() {
  Delay1KTCYx( 14 );
  } //Defining the delay function
  void dis_cmd(char cmd_value) {
  char cmd_value1;
  cmd_value1 = ((cmd_value >> 4) & 0x0F) ; //mask lower nibble because RD0-RD3 pins are used.
  DATA_PORT = cmd_value1; // send to LCD
  EN=1;
  RS=0;
  delay();
  EN =0;
  delay();
  cmd_value1 = (cmd_value & 0x0F) ; // mask higher nibble
  DATA_PORT = cmd_value1;// send to LCD
  EN=1;
  RS=0;
  delay();
 EN =0;
  delay();
  } //To send commands to the LCD, 4 bit mode
  void interface() {
 dis_cmd(0x28);
 longdelay();
 }
  void line_down(){
  dis_cmd(0xC0);
  }
  void DISPLAY_CURSOR() {
  dis_cmd(0x08);
  }
  void clear_display() {
  dis_cmd(0x01);
  longdelay();
  }
 void blink() {
  dis_cmd(0x0F);
  }
  void home () {
 dis_cmd(0x02);
  longdelay();
  }
  void dis_data(char data_value) {
  char data_value1;
  data_value1 = ((data_value >> 4) & 0x0F) ;
  DATA_PORT = data_value1;
  EN=1;
  RS=1;
  delay();
  EN =0;
  delay();
 data_value1 = (data_value & 0x0F) ;
  DATA_PORT = data_value1;
  EN=1;
  RS=1;
 delay();
 EN =0;
 delay();
 } //To send data to the LCD, 4 bit mode
 void lcd(unsigned char ch) {
 dis_data(ch);
  }
 void lcd_init() {
 for(i=0;i<3;i++) {
 dis_cmd( 0x02 ); // 4-bit mode
 interface();}
 DISPLAY_CURSOR();
 clear_display();
 blink();
 home ();
 dis_cmd( 0x06 ); // entry mode l-to-r, no shift
  for (i=0;i<7;i++){
 lcd(LCDStart[i]);
 longdelay();}
 } //initializing the LCD
  void display_opp(char OppMoves[8]) {
  clear_display();
  for (i=0;i<8;i++){
  lcd(LCDOpp[i]);}
  line_down();
  lcd('(');
  for (i=0;i<8;i++){
  if (i>0 && i%2==0 && i!=4){
  lcd(',');}
  if (i==4){
  lcd(')');
  lcd('&');
  lcd('(');}
  lcd(OppMoves[i]);}
 lcd(')');
  } //To display the Opposition Moves
  void display_my(char MyMove1[4],char MyMove2[4]) {
 clear_display();
  for (i=0;i<8;i++){
 lcd(LCDMy[i]);}
 line_down();
 lcd('(');
 for (i=0;i<4;i++){
 if (i>0 && i%2==0){
 lcd(',');}
 lcd(MyMove1[i]);}
 lcd(')');
 lcd('&');
  lcd('(');
   for (i=0;i<4;i++){
 if (i>0 && i%2==0){
 lcd(',');}
lcd(MyMove2[i]);}
lcd(')');
} //Display Our moves
 void display_my_win(void) {
unsigned char LCDWin[7]="We Win!";
clear_display();
 for (i=0;i<7;i++){
 lcd(LCDWin[i]);}
 } //Display Our win
 void display_opp_win(void) {
 unsigned char LCDLose[11]="We Lose! :(";
 clear_display();
 for (i=0;i<11;i++){
 lcd(LCDLose[i]);}
 } //Display Our loss
 /*Appendix C - Playing Algorithm*/

  short abs(short x){
  if(x>=0){return x;}
  else{return (-x);}}
  int influence(short x,short y,char grid[19][19],char side){
  char X,O;short Inf=0;short e=1;short d;
  if(side =='W'){
  O='B';//Opponent side
  X='W';//Our side
  }else if(side =='B'){
  O='W';//Opponent side
 X='B';}//Our side
  for(d=0;d<4;d++){
 short Dir_Inf=0;
 short initial_h=1;
 short initial_v=1;
 short step=1;
 short Dir_h,Dir_v;
 bool outer_break=false;
 short h=0;
 short v=0;
 if (d==0){Dir_h=6;Dir_v=1;initial_v=0;}//Horizontal Check
 else if (d==1){Dir_h=1;Dir_v=6;initial_h=0;}//Vertical Check
 else if(d==2){Dir_h=6;Dir_v=6;}//Diagonal 1 Check
 else{Dir_h=6;Dir_v=-6;initial_v=-1;step=-1;}
 for(h=initial_h;h<Dir_h;h++){
 if (outer_break){outer_break=false;break;}
 if (v<Dir_v){
 for(v=initial_v;v<Dir_v;v=v+step){
 short R=x-v;
 short C=y-h;
 if ((d==2 || d==3) && abs(h)!=abs(v)){continue;}
 if(R>18 || R<0 || C>18 || C<0){break;}
 if (grid[R][C]==O){outer_break=true;break;}
 else if(grid[R][C]=='-'){Dir_Inf+=e;}
 else if(grid[R][C]==X){
 if(d==0 || d==1){
 Dir_Inf+=(21-abs(h))*initial_h;
 Dir_Inf+=(21-abs(v))*initial_v;
 }else{Dir_Inf+=(21-abs(h));}}}}
  else{for(v=initial_v;v>Dir_v;v=v+step){
 short R=x-v;
 short C=y-h;
  if ((d==2 || d==3) && abs(h)!=abs(v)){continue;}
  if(R>18 || R<0 || C>18 || C<0){break;}
  if (grid[R][C]==O){outer_break=true;break;
  }else if(grid[R][C]=='-'){Dir_Inf+=e;}
  else if(grid[R][C]==X){
  if(d==0 || d==1){
  Dir_Inf+=(21-abs(h))*initial_h;
  Dir_Inf+=(21-abs(v))*initial_v;
 }else{Dir_Inf+=(21-abs(h));}}}}}
  for(h=initial_h;h<Dir_h;h++){
  if (outer_break){outer_break=false;break;}
  if(v<Dir_v){
  for(v=initial_v;v<Dir_v;v=v+step){
  short R=x+v;
  short C=y+h;
  if ((d==2 || d==3) && abs(h)!=abs(v)){\continue;}
  if(R>18 || R<0 || C>18 || C<0){outer_break=true;break;}
  if (grid[R][C]==O){outer_break=true;break;
  }else if(grid[R][C]=='-'){Dir_Inf+=e;
  }else if(grid[R][C]==X){
  if(d==0 || d==1){
  Dir_Inf+=(21-abs(h))*initial_h;
  Dir_Inf+=(21-abs(v))*initial_v;
  }else{Dir_Inf+=(21-abs(h));}}}
  }else{for(v=initial_v;v>Dir_v;v=v+step){
  short R=x+v;
  short C=y+h;
  if ((d==2 || d==3) && abs(h)!=abs(v)){continue;}
  if(R>18 || R<0 || C>18 || C<0){outer_break=true;break;}
  if (grid[R][C]==O){outer_break=true;break;
  }else if(grid[R][C]=='-'){Dir_Inf+=e;
  }else if(grid[R][C]==X){
 if(d==0 || d==1){
 Dir_Inf+=(21-abs(h))*initial_h;
  Dir_Inf+=(21-abs(v))*initial_v;
  }else{Dir_Inf+=(21-abs(h));}}}}}
  Inf+=Dir_Inf*Dir_Inf;}
 return Inf;}
  bool duplicate(short List[],short l, char side){
  char X,O;int a,b;short i = 0;short max = 0;short r = 0;
  short m = 0;short count;bool dup = false;
  if(side =='W'){
  O='B';//Opponent side
  X='W';//Our side
  }else if(side =='B'){
  O='W';//Opponent side
  X='B';}//Our side
  for(m=0;m<2;m++){dup_array[m] = -1;}
  for (i = 0; i < l; i=i+2){count = 0;
  for (r = i; r < l; r=r+2){
  if ((List[r] == List[i])&&(List[r+1]==List[i+1])){
  count += 1;}}
 if (count > max){
  max = count;
 dup_array[0] = List[i];
dup_array[1] = List[i+1];
 dup = true;
 }else if(count == max){
 a = influence(List[i],List[i+1],grid,O);
 b = influence(dup_array[0],dup_array[1],grid,O);
 if (a>b){
 dup_array[0] = List[i];
 dup_array[1] = List[i+1];
 dup = true;}}}
 return dup;}
  void win(char grid[19][19], short a[2],short b[2],char side){
 short x[2], y[2];char O,X;short i,r;
 for(r = 0; r<len(List);r++){List[r] = -1;}
 k = 0;
 if(side =='W'){
 O='B';//Opponent side
 X='W';}//Our side
 else if(side =='B'){
  O='W';//Opponent side
 X='B';}//Our side
 x[0] = a[0];x[1] = b[0];y[0] = a[1];y[1] = b[1];
 for(i=0;i<2;i++){int r;//Horizontal win check
 for(r = 0;r<6;r++){
 int z;
 int emphori[10];
 int countHoriW=0;
 int countemphori=0;
  for(z=0;z<6;z++){
 if (x[i]+z-r<0 || x[i]+z-r>18 ){break;}
 if (grid[x[i]+z-r][y[i]] == X){countHoriW+=1;}
 if (grid[x[i]+z-r][y[i]]=='-'){
 emphori[countemphori]=x[i]+z-r;
 emphori[countemphori+1]=y[i];
 countemphori+=2;}}
 if (countemphori==4 && countHoriW==4){
 List[k] = emphori[0];
 List[k+1] = emphori[1];
 k += 2;
List[k] = emphori[2];
 List[k+1] = emphori[3];
 k += 2;}
 else if(countemphori==2 && countHoriW==5){
 List[k] = emphori[0];
 List[k+1] = emphori[1];
 k += 2;}}
 for(r=0;r<6;r++){//Vertical win check
int countBackho=0;
 int empback[10];
 int countempback=0;
 int z;
 for(z=0;z<6;z++){
 if(y[i]+z-r<0 || y[i]+z-r>18){break;}
 if (grid[x[i]][y[i]+z-r]==X){countBackho+=1;}
 if (grid[x[i]][y[i]+z-r]=='-'){
 empback[countempback]=x[i];
 empback[countempback+1]=y[i]+z-r;
  countempback+=2;}}
 if (countempback==4 && countBackho==4){
 List[k] = empback[0];
 List[k+1] = empback[1];
 k += 2;
 List[k] = empback[2];
 List[k+1] = empback[3];
 k += 2;}
 else if(countempback==2 && countBackho==5){
 List[k] = empback[0];
 List[k+1] = empback[1];
 k += 2;}}
 for(r=0;r<6;r++){//Diagonal 1 win check
 int countdia=0;
 int countempdia=0;
 static int empdia[10];
 int z;
 for(z=0;z<6;z++){
 if(x[i]-z+r<0 || y[i]+z-r<0 || x[i]-z+r>18 || y[i]+z-r>18 ){break;}
 if (grid[x[i]-z+r][y[i]+z-r]==X){countdia+=1;}
 if (grid[x[i]-z+r][y[i]+z-r]=='-'){
 empdia[countempdia]=x[i]-z+r;
 empdia[countempdia+1]=y[i]+z-r;
 countempdia+=2;}}
 if (countempdia==4 && countdia==4){
 List[k] = empdia[0];
 List[k+1] = empdia[1];
 k += 2;
 List[k] = empdia[2];
 List[k+1] = empdia[3];
 k += 2;}
 else if(countempdia==2 && countdia==5){
 List[k] = empdia[0];
 List[k+1] = empdia[1];
 k += 2;}}
 for(r=0;r<6;r++){//Diagonal 2 win check
 int countdiagW=0;
  int countempdiag=0;
 static int empdiag[10];
 int z;
 for(z=0;z<6;z++){
 if(x[i]+z-r<0 || y[i]+z-r<0 || x[i]+z-r>18 || y[i]+z-r>18 ){break;}
 if (grid[x[i]+z-r][y[i]+z-r]==X){countdiagW+=1;}
 if (grid[x[i]+z-r][y[i]+z-r]=='-'){
 empdiag[countempdiag]=x[i]+z-r;
 empdiag[countempdiag+1]=y[i]+z-r;
 countempdiag+=2;}}
 if (countempdiag==4 && countdiagW==4){
 List[k] = empdiag[0];
 List[k+1] = empdiag[1];
 k += 2;
 List[k] = empdiag[2];
 List[k+1] = empdiag[3];
k += 2;}
 else if(countempdiag==2 && countdiagW==5){
 List[k] = empdiag[0];
 List[k+1] = empdiag[1];
 k += 2;}}}}
short* empyCoords(short a[2], char side){
 bool EmptySpace=false;
 char O,X;short i=0;short r=0;int influ=0;
 int max=0;short x,y;static short EmptyArray[2];
 if(side =='W'){
 O='B';//Opponent side
 X='W';}//Our side
 else if(side =='B'){
 O='W';//Opponent side
 X='B';}//Our side
x = a[0]; y = a[1];
 for(i=-5;i<6;i++){
 for (r=-5;r<6;r++){
 if (x+i<0 || x+i>18){break;}
 if (y+r<0 || y+r>18){continue;}
 if (grid[x+i][y+r]=='-'){
 influ=influence(x+i,y+r,grid,O)+influence(x+i,y+r,grid,X);
 if (max<influ){
 EmptyArray[0]=x+i;
 EmptyArray[1]=y+r;
 max=influ;
 EmptySpace=true;}}}}
 if (!EmptySpace){
 for (i=0;i<19;i++){
 for (r=0;r<19;r++){
 if (grid[i][r]=='-'){
 influ=influence(i,r,grid,O)+influence(i,r,grid,X);
 if (max<influ){
 EmptyArray[0]=i;
 EmptyArray[1]=r;
 max=influ;}}}}}
 grid[EmptyArray[0]][EmptyArray[1]] = X;
 return EmptyArray;}
 short* Con6player(short oppent_array1[2],short oppent_array2[2],char grid[19][19],char side){
 char O,X;short i;short r;short *p;static short main_array[4];
 if(side =='W'){O='B';X='W';}
 else if(side =='B'){O='W';X='B';}
 if (turn==1 && side=='W' ){
 grid[oppent_array1[0]][oppent_array1[1]]=O;
 p = empyCoords(oppent_array1,X);
 our_array1[0]=*p;
 our_array1[1]=*(p+1);
 p = empyCoords(oppent_array1,X);//changed
 our_array2[0]=*p;
 our_array2[1]=*(p+1);
 main_array[0]=our_array1[0];
 main_array[1]=our_array1[1];
 main_array[2]=our_array2[0];
 main_array[3]=our_array2[1];
 turn+=1;
 }else{
 grid[oppent_array1[0]][oppent_array1[1]]=O;
 grid[oppent_array2[0]][oppent_array2[1]]=O;
 win(grid,our_array1,our_array2,X);//our win
 if(k!=0){short j;int inf1=0;int inf2=0;short num1, num2;
 for (j =0; j < k; j=j+2){int t;
 t = influence(List[j],List[j+1],grid,X);
 if((inf1 < t) || (inf2 < t)){
 if (inf1 < t){inf2 = inf1;inf1 = t;num2 = num1;num1 = j;}
 else{inf2 = t;num2 = j;}}}
 grid[List[num1]][List[num1+1]]=X;
 our_array1[0] = List[num1];
 our_array1[1] = List[num1+1];
 grid[List[num2]][List[num2+1]]=X;
 our_array2[0] = List[num2];
 our_array2[1] = List[num2+1];
 main_array[0]=our_array1[0];
 main_array[1]=our_array1[1];
 main_array[2]=our_array2[0];
 main_array[3]=our_array2[1];
 }else{short s=0;short chance=2;
 for(s=0;s<2;s++){win(grid,oppent_array1,oppent_array2,O);//opponent win
 if(k!=0){
 if (duplicate(List,k,side)){
 grid[dup_array[0]][dup_array[1]]=X;
 if (chance==2){
 main_array[0]=dup_array[0];
 main_array[1]=dup_array[1];
 chance+=-1;
 }else{
 main_array[2]=dup_array[0];
 main_array[3]=dup_array[1];}
 }else{short j;int inf=0;int t;short num;
 for (j =0; j < k; j=j+2){
 t = influence(List[j],List[j+1],grid,O);
 if (inf < t){inf = t;num = j;}}
 grid[List[num]][List[num+1]]=X;
 if (chance==2){
 main_array[0]=List[num];
 main_array[1]=List[num+1];
 chance+=-1;
 }else{
 main_array[2]=List[num];
 main_array[3]=List[num+1];}}
 }else{if (chance==2){
 p = empyCoords(our_array1,X);//Changed
 our_array1[0]=*p;
 our_array1[1]=*(p+1);
 p = empyCoords(our_array1,X);
 our_array2[0]=*p;
 our_array2[1]=*(p+1);
 main_array[0]=our_array1[0];
 main_array[1]=our_array1[1];
 main_array[2]=our_array2[0];
 main_array[3]=our_array2[1];
 break;
}else{
 p = empyCoords(oppent_array2,X);//Changed
 our_array2[0]=*p;
 our_array2[1]=*(p+1);
 main_array[2]=our_array2[0];
 main_array[3]=our_array2[1];}}}}}
 return main_array;}
 //Algorithm for the game
