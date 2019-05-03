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
 76 USART_ASYNCH_MODE &
 77 USART_EIGHT_BIT &
 78 USART_CONT_RX &
 79 USART_BRGH_HIGH &
 80 USART_ADDEN_OFF, 26);
 81 lcd_init(); //Initiate the LCD
 82 while (BusyUSART());
 83 while (!DataRdyUSART());
 84 side=ReadUSART(); //Reads the side to play
 85 for (i=0;i<=3;i++){
 86 while (!DataRdyUSART());
 87 FirstMove[i]=ReadUSART(); //Reads the first move
 88 }
 89 x0=(FirstMove[0]-'0')*10+(FirstMove[1]-'0');
 90 x0=x0-1;
 91 y0=(FirstMove[2]-'0')*10+(FirstMove[3]-'0');
 92 y0=y0-1;
 93 //conversion of string to an array
 94 if (side=='B' && turn==1){
 95 grid[x0][y0]='B';
 96 our_array1[0]=x0;
 97 our_array1[1]=y0;
 98 display_my(FirstMove,FirstMove);
 99 }
100 else if (side=='W' && turn==1){
101 x1=x0;
102 y1=y0;
103 x2=x1;
104 y2=y1;
105 display_opp(FirstMove);
106 oppent_array1[0]=x1;
107 oppent_array1[1]=y1;
108 oppent_array2[0]=x2;
109 oppent_array2[1]=y2;
110
111 M=Con6player(oppent_array1,oppent_array2,grid,side);
112 for (i=0;i<4;i++){
113 Moves[i]=*(M+i);
114 } //Returns the moves to be played
115
116 PlayMove1[0]=((Moves[0]+1)/10)+'0';
117 PlayMove1[1]=((Moves[0]+1)%10)+'0';
118 PlayMove1[2]=((Moves[1]+1)/10)+'0';
119 PlayMove1[3]=((Moves[1]+1)%10)+'0';
120 PlayMove2[0]=((Moves[2]+1)/10)+'0';
121 PlayMove2[1]=((Moves[2]+1)%10)+'0';
122 PlayMove2[2]=((Moves[3]+1)/10)+'0';
123 PlayMove2[3]=((Moves[3]+1)%10)+'0';
124 //conversion of the array to a string
125 for (i=0;i<=3;i++){
126 while (BusyUSART());
127 WriteUSART(PlayMove1[i]); //Sends the first move
128 }
129 for (i=0;i<=3;i++){
130 while (BusyUSART());
131 WriteUSART(PlayMove2[i]); //Sends the second move
132 }
133 display_my(PlayMove1,PlayMove2);
134 }
135 while(1){
136 for (i=0;i<=7;i++){
137 while (!DataRdyUSART()); //reads the Opponents moves
138 SecondMove[i]=ReadUSART();
139 }
140 x1=(SecondMove[0]-'0')*10+(SecondMove[1]-'0');
141 x1=x1-1;
142 y1=(SecondMove[2]-'0')*10+(SecondMove[3]-'0');
143 y1=y1-1;
144 x2=(SecondMove[4]-'0')*10+(SecondMove[5]-'0');
145 x2=x2-1;
146 y2=(SecondMove[6]-'0')*10+(SecondMove[7]-'0');
147 y2=y2-1;
148 //conversion of string to array
149 display_opp(SecondMove); //Displays opponent move on LCD
150 oppent_array1[0]=x1;
151 oppent_array1[1]=y1;
152 oppent_array2[0]=x2;
153 oppent_array2[1]=y2;
154
155 M=Con6player(oppent_array1,oppent_array2,grid,side); //returns the moves to be sent
156 for (i=0;i<4;i++){
157 Moves[i]=*(M+i);
158 }
159 PlayMove1[0]=((Moves[0]+1)/10)+'0'; //conversion of array to string
160 PlayMove1[1]=((Moves[0]+1)%10)+'0';
161 PlayMove1[2]=((Moves[1]+1)/10)+'0';
162 PlayMove1[3]=((Moves[1]+1)%10)+'0';
163
164 PlayMove2[0]=((Moves[2]+1)/10)+'0';
165 PlayMove2[1]=((Moves[2]+1)%10)+'0';
166 PlayMove2[2]=((Moves[3]+1)/10)+'0';
167 PlayMove2[3]=((Moves[3]+1)%10)+'0';
168
169 for (i=0;i<=3;i++){
170 while (BusyUSART());
171 WriteUSART(PlayMove1[i]); //Sends the first move
172 }
173 for (i=0;i<=3;i++){
174 while (BusyUSART());
175 WriteUSART(PlayMove2[i]); //Sends the second move
176 }
177 display_my(PlayMove1,PlayMove2); //displays move on LCD
178 }
179 
 1 /*Appendix B - LCD Functions*/
 2
 3 void delay() {
 4 Delay1KTCYx( 1 );
 5 } //Defining the delay function
 6 void longdelay() {
 7 Delay1KTCYx( 14 );
 8 } //Defining the delay function
 9 void dis_cmd(char cmd_value) {
 10 char cmd_value1;
 11 cmd_value1 = ((cmd_value >> 4) & 0x0F) ; //mask lower nibble because RD0-RD3 pins are used.
 12 DATA_PORT = cmd_value1; // send to LCD
 13 EN=1;
 14 RS=0;
 15 delay();
 16 EN =0;
 17 delay();
 18 cmd_value1 = (cmd_value & 0x0F) ; // mask higher nibble
 19 DATA_PORT = cmd_value1;// send to LCD
 20 EN=1;
 21 RS=0;
 22 delay();
 23 EN =0;
 24 delay();
 25 } //To send commands to the LCD, 4 bit mode
 26 void interface() {
 27 dis_cmd(0x28);
 28 longdelay();
 29 }
 30 void line_down(){
 31 dis_cmd(0xC0);
 32 }
 33 void DISPLAY_CURSOR() {
 34 dis_cmd(0x08);
 35 }
 36 void clear_display() {
 37 dis_cmd(0x01);
 38 longdelay();
 39 }
 40 void blink() {
 41 dis_cmd(0x0F);
 42 }
 43 void home () {
 44 dis_cmd(0x02);
 45 longdelay();
 46 }
 47 void dis_data(char data_value) {
 48 char data_value1;
 49 data_value1 = ((data_value >> 4) & 0x0F) ;
 50 DATA_PORT = data_value1;
 51 EN=1;
 52 RS=1;
 53 delay();
 54 EN =0;
 55 delay();
 56 data_value1 = (data_value & 0x0F) ;
 57 DATA_PORT = data_value1;
 58 EN=1;
 59 RS=1;
 60 delay();
 61 EN =0;
 62 delay();
 63 } //To send data to the LCD, 4 bit mode
 64 void lcd(unsigned char ch) {
 65 dis_data(ch);
 66 }
 67 void lcd_init() {
 68 for(i=0;i<3;i++) {
 69 dis_cmd( 0x02 ); // 4-bit mode
 70 interface();}
 71 DISPLAY_CURSOR();
 72 clear_display();
 73 blink();
 74 home ();
 75 dis_cmd( 0x06 ); // entry mode l-to-r, no shift
 76 for (i=0;i<7;i++){
 77 lcd(LCDStart[i]);
 78 longdelay();}
 79 } //initializing the LCD
 80 void display_opp(char OppMoves[8]) {
 81 clear_display();
 82 for (i=0;i<8;i++){
 83 lcd(LCDOpp[i]);}
 84 line_down();
 85 lcd('(');
 86 for (i=0;i<8;i++){
 87 if (i>0 && i%2==0 && i!=4){
 88 lcd(',');}
 89 if (i==4){
 90 lcd(')');
 91 lcd('&');
 92 lcd('(');}
 93 lcd(OppMoves[i]);}
 94 lcd(')');
 95 } //To display the Opposition Moves
 96 void display_my(char MyMove1[4],char MyMove2[4]) {
 97 clear_display();
 98 for (i=0;i<8;i++){
 99 lcd(LCDMy[i]);}
100 line_down();
101 lcd('(');
102 for (i=0;i<4;i++){
103 if (i>0 && i%2==0){
104 lcd(',');}
105 lcd(MyMove1[i]);}
106 lcd(')');
107 lcd('&');
108 lcd('(');
109 for (i=0;i<4;i++){
110 if (i>0 && i%2==0){
111 lcd(',');}
112 lcd(MyMove2[i]);}
113 lcd(')');
114 } //Display Our moves
115 void display_my_win(void) {
116 unsigned char LCDWin[7]="We Win!";
117 clear_display();
118 for (i=0;i<7;i++){
119 lcd(LCDWin[i]);}
120 } //Display Our win
121 void display_opp_win(void) {
122 unsigned char LCDLose[11]="We Lose! :(";
123 clear_display();
124 for (i=0;i<11;i++){
125 lcd(LCDLose[i]);}
126 } //Display Our loss
 1 /*Appendix C - Playing Algorithm*/
 2
 3 short abs(short x){
 4 if(x>=0){return x;}
 5 else{return (-x);}}
 6 int influence(short x,short y,char grid[19][19],char side){
 7 char X,O;short Inf=0;short e=1;short d;
 8 if(side =='W'){
 9 O='B';//Opponent side
 10 X='W';//Our side
 11 }else if(side =='B'){
 12 O='W';//Opponent side
 13 X='B';}//Our side
 14 for(d=0;d<4;d++){
 15 short Dir_Inf=0;
 16 short initial_h=1;
 17 short initial_v=1;
 18 short step=1;
 19 short Dir_h,Dir_v;
 20 bool outer_break=false;
 21 short h=0;
 22 short v=0;
 23 if (d==0){Dir_h=6;Dir_v=1;initial_v=0;}//Horizontal Check
 24 else if (d==1){Dir_h=1;Dir_v=6;initial_h=0;}//Vertical Check
 25 else if(d==2){Dir_h=6;Dir_v=6;}//Diagonal 1 Check
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
