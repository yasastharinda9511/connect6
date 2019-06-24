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
 26 else{Dir_h=6;Dir_v=-6;initial_v=-1;step=-1;}
 27 for(h=initial_h;h<Dir_h;h++){
 28 if (outer_break){outer_break=false;break;}
 29 if (v<Dir_v){
 30 for(v=initial_v;v<Dir_v;v=v+step){
 31 short R=x-v;
 32 short C=y-h;
 33 if ((d==2 || d==3) && abs(h)!=abs(v)){continue;}
 34 if(R>18 || R<0 || C>18 || C<0){break;}
 35 if (grid[R][C]==O){outer_break=true;break;}
 36 else if(grid[R][C]=='-'){Dir_Inf+=e;}
 37 else if(grid[R][C]==X){
 38 if(d==0 || d==1){
 39 Dir_Inf+=(21-abs(h))*initial_h;
 40 Dir_Inf+=(21-abs(v))*initial_v;
 41 }else{Dir_Inf+=(21-abs(h));}}}}
 42 else{for(v=initial_v;v>Dir_v;v=v+step){
 43 short R=x-v;
 44 short C=y-h;
 45 if ((d==2 || d==3) && abs(h)!=abs(v)){continue;}
 46 if(R>18 || R<0 || C>18 || C<0){break;}
 47 if (grid[R][C]==O){outer_break=true;break;
 48 }else if(grid[R][C]=='-'){Dir_Inf+=e;}
 49 else if(grid[R][C]==X){
 50 if(d==0 || d==1){
 51 Dir_Inf+=(21-abs(h))*initial_h;
 52 Dir_Inf+=(21-abs(v))*initial_v;
 53 }else{Dir_Inf+=(21-abs(h));}}}}}
 54 for(h=initial_h;h<Dir_h;h++){
 55 if (outer_break){outer_break=false;break;}
 56 if(v<Dir_v){
 57 for(v=initial_v;v<Dir_v;v=v+step){
 58 short R=x+v;
 59 short C=y+h;
 60 if ((d==2 || d==3) && abs(h)!=abs(v)){\continue;}
 61 if(R>18 || R<0 || C>18 || C<0){outer_break=true;break;}
 62 if (grid[R][C]==O){outer_break=true;break;
 63 }else if(grid[R][C]=='-'){Dir_Inf+=e;
 64 }else if(grid[R][C]==X){
 65 if(d==0 || d==1){
 66 Dir_Inf+=(21-abs(h))*initial_h;
 67 Dir_Inf+=(21-abs(v))*initial_v;
 68 }else{Dir_Inf+=(21-abs(h));}}}
 69 }else{for(v=initial_v;v>Dir_v;v=v+step){
 70 short R=x+v;
 71 short C=y+h;
 72 if ((d==2 || d==3) && abs(h)!=abs(v)){continue;}
 73 if(R>18 || R<0 || C>18 || C<0){outer_break=true;break;}
 74 if (grid[R][C]==O){outer_break=true;break;
 75 }else if(grid[R][C]=='-'){Dir_Inf+=e;
 76 }else if(grid[R][C]==X){
 77 if(d==0 || d==1){
 78 Dir_Inf+=(21-abs(h))*initial_h;
 79 Dir_Inf+=(21-abs(v))*initial_v;
 80 }else{Dir_Inf+=(21-abs(h));}}}}}
 81 Inf+=Dir_Inf*Dir_Inf;}
 82 return Inf;}
 83 bool duplicate(short List[],short l, char side){
 84 char X,O;int a,b;short i = 0;short max = 0;short r = 0;
 85 short m = 0;short count;bool dup = false;
 86 if(side =='W'){
 87 O='B';//Opponent side
 88 X='W';//Our side
 89 }else if(side =='B'){
 90 O='W';//Opponent side
 91 X='B';}//Our side
 92 for(m=0;m<2;m++){dup_array[m] = -1;}
 93 for (i = 0; i < l; i=i+2){count = 0;
 94 for (r = i; r < l; r=r+2){
 95 if ((List[r] == List[i])&&(List[r+1]==List[i+1])){
 96 count += 1;}}
 97 if (count > max){
 98 max = count;
 99 dup_array[0] = List[i];
100 dup_array[1] = List[i+1];
101 dup = true;
102 }else if(count == max){
103 a = influence(List[i],List[i+1],grid,O);
104 b = influence(dup_array[0],dup_array[1],grid,O);
105 if (a>b){
106 dup_array[0] = List[i];
107 dup_array[1] = List[i+1];
108 dup = true;}}}
109 return dup;}
110 void win(char grid[19][19], short a[2],short b[2],char side){
111 short x[2], y[2];char O,X;short i,r;
112 for(r = 0; r<len(List);r++){List[r] = -1;}
113 k = 0;
114 if(side =='W'){
115 O='B';//Opponent side
116 X='W';}//Our side
117 else if(side =='B'){
118 O='W';//Opponent side
119 X='B';}//Our side
120 x[0] = a[0];x[1] = b[0];y[0] = a[1];y[1] = b[1];
121 for(i=0;i<2;i++){int r;//Horizontal win check
122 for(r = 0;r<6;r++){
123 int z;
124 int emphori[10];
125 int countHoriW=0;
126 int countemphori=0;
127 for(z=0;z<6;z++){
128 if (x[i]+z-r<0 || x[i]+z-r>18 ){break;}
129 if (grid[x[i]+z-r][y[i]] == X){countHoriW+=1;}
130 if (grid[x[i]+z-r][y[i]]=='-'){
131 emphori[countemphori]=x[i]+z-r;
132 emphori[countemphori+1]=y[i];
133 countemphori+=2;}}
134 if (countemphori==4 && countHoriW==4){
135 List[k] = emphori[0];
136 List[k+1] = emphori[1];
137 k += 2;
138 List[k] = emphori[2];
139 List[k+1] = emphori[3];
140 k += 2;}
141 else if(countemphori==2 && countHoriW==5){
142 List[k] = emphori[0];
143 List[k+1] = emphori[1];
144 k += 2;}}
145 for(r=0;r<6;r++){//Vertical win check
146 int countBackho=0;
147 int empback[10];
148 int countempback=0;
149 int z;
150 for(z=0;z<6;z++){
151 if(y[i]+z-r<0 || y[i]+z-r>18){break;}
152 if (grid[x[i]][y[i]+z-r]==X){countBackho+=1;}
153 if (grid[x[i]][y[i]+z-r]=='-'){
154 empback[countempback]=x[i];
155 empback[countempback+1]=y[i]+z-r;
156 countempback+=2;}}
157 if (countempback==4 && countBackho==4){
158 List[k] = empback[0];
159 List[k+1] = empback[1];
160 k += 2;
161 List[k] = empback[2];
162 List[k+1] = empback[3];
163 k += 2;}
164 else if(countempback==2 && countBackho==5){
165 List[k] = empback[0];
166 List[k+1] = empback[1];
167 k += 2;}}
168 for(r=0;r<6;r++){//Diagonal 1 win check
169 int countdia=0;
170 int countempdia=0;
171 static int empdia[10];
172 int z;
173 for(z=0;z<6;z++){
174 if(x[i]-z+r<0 || y[i]+z-r<0 || x[i]-z+r>18 || y[i]+z-r>18 ){break;}
175 if (grid[x[i]-z+r][y[i]+z-r]==X){countdia+=1;}
176 if (grid[x[i]-z+r][y[i]+z-r]=='-'){
177 empdia[countempdia]=x[i]-z+r;
178 empdia[countempdia+1]=y[i]+z-r;
179 countempdia+=2;}}
180 if (countempdia==4 && countdia==4){
181 List[k] = empdia[0];
182 List[k+1] = empdia[1];
183 k += 2;
184 List[k] = empdia[2];
185 List[k+1] = empdia[3];
186 k += 2;}
187 else if(countempdia==2 && countdia==5){
188 List[k] = empdia[0];
189 List[k+1] = empdia[1];
190 k += 2;}}
191 for(r=0;r<6;r++){//Diagonal 2 win check
192 int countdiagW=0;
193 int countempdiag=0;
194 static int empdiag[10];
195 int z;
196 for(z=0;z<6;z++){
197 if(x[i]+z-r<0 || y[i]+z-r<0 || x[i]+z-r>18 || y[i]+z-r>18 ){break;}
198 if (grid[x[i]+z-r][y[i]+z-r]==X){countdiagW+=1;}
199 if (grid[x[i]+z-r][y[i]+z-r]=='-'){
200 empdiag[countempdiag]=x[i]+z-r;
201 empdiag[countempdiag+1]=y[i]+z-r;
202 countempdiag+=2;}}
203 if (countempdiag==4 && countdiagW==4){
204 List[k] = empdiag[0];
205 List[k+1] = empdiag[1];
206 k += 2;
207 List[k] = empdiag[2];
208 List[k+1] = empdiag[3];
209 k += 2;}
210 else if(countempdiag==2 && countdiagW==5){
211 List[k] = empdiag[0];
212 List[k+1] = empdiag[1];
213 k += 2;}}}}
214 short* empyCoords(short a[2], char side){
215 bool EmptySpace=false;
216 char O,X;short i=0;short r=0;int influ=0;
217 int max=0;short x,y;static short EmptyArray[2];
218 if(side =='W'){
219 O='B';//Opponent side
220 X='W';}//Our side
221 else if(side =='B'){
222 O='W';//Opponent side
223 X='B';}//Our side
224 x = a[0]; y = a[1];
225 for(i=-5;i<6;i++){
226 for (r=-5;r<6;r++){
227 if (x+i<0 || x+i>18){break;}
228 if (y+r<0 || y+r>18){continue;}
229 if (grid[x+i][y+r]=='-'){
230 influ=influence(x+i,y+r,grid,O)+influence(x+i,y+r,grid,X);
231 if (max<influ){
232 EmptyArray[0]=x+i;
233 EmptyArray[1]=y+r;
234 max=influ;
235 EmptySpace=true;}}}}
236 if (!EmptySpace){
237 for (i=0;i<19;i++){
238 for (r=0;r<19;r++){
239 if (grid[i][r]=='-'){
240 influ=influence(i,r,grid,O)+influence(i,r,grid,X);
241 if (max<influ){
242 EmptyArray[0]=i;
243 EmptyArray[1]=r;
244 max=influ;}}}}}
245 grid[EmptyArray[0]][EmptyArray[1]] = X;
246 return EmptyArray;}
247 short* Con6player(short oppent_array1[2],short oppent_array2[2],char grid[19][19],char side){
248 char O,X;short i;short r;short *p;static short main_array[4];
249 if(side =='W'){O='B';X='W';}
250 else if(side =='B'){O='W';X='B';}
251 if (turn==1 && side=='W' ){
252 grid[oppent_array1[0]][oppent_array1[1]]=O;
253 p = empyCoords(oppent_array1,X);
254 our_array1[0]=*p;
255 our_array1[1]=*(p+1);
256 p = empyCoords(oppent_array1,X);//changed
257 our_array2[0]=*p;
258 our_array2[1]=*(p+1);
259 main_array[0]=our_array1[0];
260 main_array[1]=our_array1[1];
261 main_array[2]=our_array2[0];
262 main_array[3]=our_array2[1];
263 turn+=1;
264 }else{
265 grid[oppent_array1[0]][oppent_array1[1]]=O;
266 grid[oppent_array2[0]][oppent_array2[1]]=O;
267 win(grid,our_array1,our_array2,X);//our win
268 if(k!=0){short j;int inf1=0;int inf2=0;short num1, num2;
269 for (j =0; j < k; j=j+2){int t;
270 t = influence(List[j],List[j+1],grid,X);
271 if((inf1 < t) || (inf2 < t)){
272 if (inf1 < t){inf2 = inf1;inf1 = t;num2 = num1;num1 = j;}
273 else{inf2 = t;num2 = j;}}}
274 grid[List[num1]][List[num1+1]]=X;
275 our_array1[0] = List[num1];
276 our_array1[1] = List[num1+1];
277 grid[List[num2]][List[num2+1]]=X;
278 our_array2[0] = List[num2];
279 our_array2[1] = List[num2+1];
280 main_array[0]=our_array1[0];
281 main_array[1]=our_array1[1];
282 main_array[2]=our_array2[0];
283 main_array[3]=our_array2[1];
284 }else{short s=0;short chance=2;
285 for(s=0;s<2;s++){win(grid,oppent_array1,oppent_array2,O);//opponent win
286 if(k!=0){
287 if (duplicate(List,k,side)){
288 grid[dup_array[0]][dup_array[1]]=X;
289 if (chance==2){
290 main_array[0]=dup_array[0];
291 main_array[1]=dup_array[1];
292 chance+=-1;
293 }else{
294 main_array[2]=dup_array[0];
295 main_array[3]=dup_array[1];}
296 }else{short j;int inf=0;int t;short num;
297 for (j =0; j < k; j=j+2){
298 t = influence(List[j],List[j+1],grid,O);
299 if (inf < t){inf = t;num = j;}}
300 grid[List[num]][List[num+1]]=X;
301 if (chance==2){
302 main_array[0]=List[num];
303 main_array[1]=List[num+1];
304 chance+=-1;
305 }else{
306 main_array[2]=List[num];
307 main_array[3]=List[num+1];}}
308 }else{if (chance==2){
309 p = empyCoords(our_array1,X);//Changed
310 our_array1[0]=*p;
311 our_array1[1]=*(p+1);
312 p = empyCoords(our_array1,X);
313 our_array2[0]=*p;
314 our_array2[1]=*(p+1);
315 main_array[0]=our_array1[0];
316 main_array[1]=our_array1[1];
317 main_array[2]=our_array2[0];
318 main_array[3]=our_array2[1];
319 break;
320 }else{
321 p = empyCoords(oppent_array2,X);//Changed
322 our_array2[0]=*p;
323 our_array2[1]=*(p+1);
324 main_array[2]=our_array2[0];
325 main_array[3]=our_array2[1];}}}}}
326 return main_array;}
327 //Algorithm for the game
