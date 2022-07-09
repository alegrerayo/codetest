// 과제명: Maze Runner
// 과제개요: ********************
// 사용한 하드웨어 (기능): GPIO, Joy-stick, KeyScan, LED
// 제출일:2022. 5. 25
// 제출자 클래스: 화요일반
//       학번:2020132024
//       이름: 이선우
#include "stm32f4xx.h"
#include "GLCD.h"

#define SW0_PUSH        0xFE00  //PH8  0b11111110
#define SW1_PUSH        0xFD00  //PH9 0b11111101
#define SW2_PUSH        0xFB00  //PH10 0b11111011
#define SW3_PUSH        0xF700  //PH11 0b11110111
#define SW4_PUSH        0xEF00  //PH12 0b11101111
#define SW5_PUSH        0xDF00  //PH13 0b11011111
#define SW6_PUSH        0xBF00  //PH14 0b10111111
#define SW7_PUSH        0x7F00  //PH15 0b01111111

//0001 0002 0004 0008 0010 0020 0040 0080
//FE00 FD00 FB00 F700 EF00 DF00 BF00 7F00

// NO Joy-Button : 0x03E0 : 0000 0011 1110 0000 
			//Bit No      FEDC BA98 7654 3210
#define NAVI_PUSH   0x03C0	//PI5 0000 0011 1100 0000 
#define NAVI_UP		0x03A0	//PI6 0000 0011 1010 0000 
#define NAVI_DOWN	0x0360	//PI7 0000 0011 0110 0000 
#define NAVI_RIGHT  0x02E0	//PI8 0000 0010 1110 0000 
#define NAVI_LEFT   0x01E0	//PI9 0000 0001 1110 0000 

void _GPIO_Init(void);
uint16_t KEY_Scan(void);
uint16_t JOY_Scan(void);

void BEEP(void);
void DisplayInitScreen(void);
void Draw_Maze(void);
void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);
void X_R_moveerase(void);
void X_L_moveerase(void);
void Y_U_moveerase(void);
void Y_D_moveerase(void);
void XcurYcur_UDLRcount(void);
void UCount_0_9(void);
void DCount_0_9(void);
void LCount_0_9(void);
void RCount_0_9(void);

uint8_t	SW0_Flag, SW7_Flag;
uint8_t	Ucount, Dcount, Lcount, Rcount;
int	Xcur=0, Ycur=0; // Mouse_X, Mouse_Y

int main(void)
{
    _GPIO_Init();       // GPIO (LED & SW) 초기화
    LCD_Init();         // LCD 모듈 초기화
    DelayMS(100);

    GPIOG->ODR = 0xFF00;	// LED 초기값: LED0~7 Off
    DisplayInitScreen();	// LCD 초기화면
    Draw_Maze();
    
   while(1)
	{
          switch(KEY_Scan())
		{
                  case SW0_PUSH  : 	//SW0
                    SW7_Flag=0;
                    Xcur=Ycur=0;
                    Ucount=Dcount=0;
                    Lcount=Rcount=0; 
                    _GPIO_Init();       // GPIO (LED & SW) 초기화 
                    LCD_Init();         // LCD 모듈 초기화
                    
                    GPIOG->ODR = 0xFF00;        // LED 초기값: LED0~7 Off
                    GPIOG->ODR |= 0x0001;       // GPIOG->ODR.0 'H'(LED0 ON)
                    
                    DisplayInitScreen();        // LCD 초기화면
                    Draw_Maze();
                    break;
                    
                  case SW7_PUSH:  	//SW7
                      SW7_Flag=1;
                      BEEP();
                      
                      XcurYcur_UDLRcount();
                      
                      GPIOG->ODR |= 0x0081; 	// GPIOG->ODR.0 'H'(LED0, LED7 ON) 
                      break;
                
                      
                      
              /*    //스틱 망가져서 스위치로 대체
                  //스틱 망가져서 스위치로 대체
                  case SW3_PUSH:	//Left stick
                    if(SW7_Flag==1) {
                     if (Xcur <= 0) {
                       Xcur = 1;
                      BEEP();
                      DelayMS(50);
                      BEEP(); 
                       Lcount=Lcount-1;
                      }
                     Xcur--;
                     Lcount++;
                     
                     GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                     GPIOG->ODR |= 0x0089;        //0, 7 LED 'H'유지 / SW3 누르면 LED 3 켜기
                     
                     X_L_moveerase();
                     Draw_Maze();
                     XcurYcur_UDLRcount();
                     BEEP();
                     }
                    break;
                    
                 case SW4_PUSH: 	//Right stick
                   if(SW7_Flag==1) {
                     if (Xcur >= 3) {
                       Xcur = 2;
                      BEEP();
                      DelayMS(50);
                      BEEP();
                       Rcount=Rcount-1;
                       }
                     Xcur++;
                     Rcount++;
                     
                     GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                     GPIOG->ODR |= 0x0091;       //0, 7 LED 'H'유지 / SW4 누르면 LED 4 켜기
                     
                     X_R_moveerase();
                     Draw_Maze();
                     XcurYcur_UDLRcount();
                     BEEP();
                     }
                   break;
                  
                case SW1_PUSH: 	//Up stick
                  if(SW7_Flag==1) {
                    if (Ycur <= 0) {
                      Ycur = 1;
                      BEEP();
                      DelayMS(50);
                      BEEP();
                      Ucount=Ucount-1;
                      }
                    Ycur--;
                    Ucount++;
                    
                    GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                    GPIOG->ODR |= 0x0083;         //0, 7 LED 'H'유지 / SW1 누르면 LED 1 켜기
                    
                    Y_U_moveerase();
                    Draw_Maze();
                    XcurYcur_UDLRcount();
                    BEEP();
                    }
                    break;
                   
                  case SW2_PUSH: 	//Down stick
                    if(SW7_Flag==1) {
                      if (Ycur >= 3) {
                        Ycur = 2;
                        BEEP();
                        DelayMS(50);
                        BEEP();
                        Dcount=Dcount-1;
                        } 
                      Ycur++;
                      Dcount++;
                      
                      GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                      GPIOG->ODR |= 0x0085;        //0, 7 LED 'H'유지 / SW2 누르면 LED 2 켜기
                      
                      Y_D_moveerase();
                      Draw_Maze();
                      XcurYcur_UDLRcount();
                      BEEP();
                     }
                    break;*/
                    }
                      
         switch(JOY_Scan()) 
            {
              case NAVI_LEFT:	//Left stick
                    if(SW7_Flag==1) {
                     if (Xcur <= 0) {
                       Xcur = 1;
                      BEEP();
                      DelayMS(50);
                      BEEP(); 
                      }
                     Xcur--;
                     Lcount++;
                     
                     GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                     GPIOG->ODR |= 0x0089;        //0, 7 LED 'H'유지 / SW3 누르면 LED 3 켜기
                     
                     X_L_moveerase();
                     Draw_Maze();
                     XcurYcur_UDLRcount();
                     UCount_0_9();
                     DCount_0_9();
                     LCount_0_9();
                     RCount_0_9();
                     BEEP();
                     }
                    break;
              
              case NAVI_RIGHT:  	//Right stick
                   if(SW7_Flag==1) {
                     if (Xcur >= 3) {
                       Xcur = 2;
                      BEEP();
                      DelayMS(50);
                      BEEP();
                       }
                     Xcur++;
                     Rcount++;
                     
                    GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                    GPIOG->ODR |= 0x0091;       //0, 7 LED 'H'유지 / SW4 누르면 LED 4 켜기
                    
                    X_R_moveerase();
                    Draw_Maze();
                    XcurYcur_UDLRcount();
                    UCount_0_9();
                     DCount_0_9();
                     LCount_0_9();
                     RCount_0_9();
                    BEEP();
                    }
                  break;
                  
                case NAVI_UP:  	//Up stick
                  if(SW7_Flag==1) {
                    if (Ycur <= 0) {
                      Ycur = 1;
                      BEEP();
                      DelayMS(50);
                      BEEP();
                      }
                    Ycur--;
                    Ucount++;
                    
                    GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                    GPIOG->ODR |= 0x0083;         //0, 7 LED 'H'유지 / SW1 누르면 LED 1 켜기
                    
                    Y_U_moveerase();
                    Draw_Maze();
                    XcurYcur_UDLRcount();
                    UCount_0_9();
                     DCount_0_9();
                     LCount_0_9();
                     RCount_0_9();
                    BEEP();
                    }
                    break;
                   
                  case NAVI_DOWN: 	//Down stick
                    if(SW7_Flag==1) {
                      if (Ycur >= 3) {
                        Ycur = 2;
                        BEEP();
                        DelayMS(50);
                        BEEP();
                        } 
                      Ycur++;
                      Dcount++;
                      
                      GPIOG->ODR = 0xFF00;    // LED 초기값: LED0~7 Off
                      GPIOG->ODR |= 0x0085;        //0, 7 LED 'H'유지 / SW2 누르면 LED 2 켜기
                      
                      Y_D_moveerase();
                      Draw_Maze();
                      XcurYcur_UDLRcount();
                      UCount_0_9();
                     DCount_0_9();
                     LCount_0_9();
                     RCount_0_9();
                      BEEP();
                     }
                    break;
                    }//switch(JOY_Scan())*/      
          
          if (Xcur==3 && Ycur ==3) {
            DelayMS(50);
            BEEP();
            DelayMS(50);
            BEEP();
            DelayMS(50);
            BEEP();
            break;
          }
	}//while(1)
}//int main(void)

void DisplayInitScreen(void)
{
    LCD_Clear(RGB_WHITE);       // 화면 클리어
    LCD_SetFont(&Gulim7);       // 폰트 : 굴림 7
    LCD_SetBackColor(RGB_YELLOW);       // 글자배경색 :   Yellow
    LCD_SetTextColor(RGB_BLUE);        // 글자색 : Blue
    LCD_DisplayText(0,0,"Maze Runner");       // Title

    LCD_SetTextColor(RGB_BLACK);	       //글자배경색 : Black
    LCD_SetBackColor(RGB_WHITE);	//글자배경색 : Whte
    
    LCD_SetBrushColor(RGB_BLUE);      //mouse color: Blue
    LCD_DrawFillRect(5+Xcur*20, 25+Ycur*20, 10, 10);    //mouse 초기화
}

void Draw_Maze(void)
{
    
    LCD_SetBackColor(RGB_WHITE);	//글자배경색 : Whte
    //미로판에 글자 입력
    LCD_SetTextColor(RGB_BLACK);	//글자색 : Black
    LCD_DisplayText(1,0," (0,0) ");
    LCD_DisplayText(1,9," (3,0) ");
    LCD_DisplayText(3,12," RESET (SW 0) ");
    LCD_DisplayText(4,12," START (SW 7) ");
    LCD_DisplayText(5,12," STATUS: (X,Y) ");
    LCD_SetTextColor(RGB_RED);	//글자색 : Red
    LCD_DisplayChar(5,22,'X');
    LCD_DisplayChar(5,24,'Y');
    LCD_SetTextColor(RGB_BLACK);	//글자색 : Black
    LCD_DisplayText(9,0," (0,3) ");
    LCD_DisplayText(9,9," (3,3) ");
    LCD_DisplayText(10,2," U:0, D:0, L:0, R:0 ");
    LCD_SetTextColor(RGB_RED);	//글자색 : Red
    LCD_DisplayChar(10,5,'0');
    LCD_DisplayChar(10,10,'0');
    LCD_DisplayChar(10,15,'0');
    LCD_DisplayChar(10,20,'0');
 
    LCD_SetPenColor(RGB_BLACK);        //펜색 : Black   //미로판 그리기
    for (int i=0; i<4; i++)
    {
      LCD_DrawLine(10, 30+i*20, 70, 30+i*20);   //수평선
      LCD_DrawLine(10+i*20, 30, 10+i*20, 90);   //수직선
    }
}

void X_R_moveerase(void)
{
    LCD_SetBrushColor(RGB_WHITE);       //mouse color: White
    LCD_DrawFillRect(5+(Xcur-1)*20, 25+Ycur*20, 10, 10);        //mouse 초기화
    
    LCD_SetBrushColor(RGB_BLUE);        //mouse color: Blue
    LCD_DrawFillRect(5+Xcur*20, 25+Ycur*20, 10, 10);           //mouse 초기화   
}

void X_L_moveerase(void)
{
    LCD_SetBrushColor(RGB_WHITE);       //mouse color: White
    LCD_DrawFillRect(5+(Xcur+1)*20, 25+Ycur*20, 10, 10);        //mouse 초기화
    
    LCD_SetBrushColor(RGB_BLUE);        //mouse color: Blue
    LCD_DrawFillRect(5+Xcur*20, 25+Ycur*20, 10, 10);            //mouse 초기화  
}

void Y_D_moveerase(void)
{
    LCD_SetBrushColor(RGB_WHITE);       //mouse color: White
    LCD_DrawFillRect(5+Xcur*20, 25+(Ycur-1)*20, 10, 10);        //mouse 초기화
    
    LCD_SetBrushColor(RGB_BLUE);        //mouse color: Blue
    LCD_DrawFillRect(5+Xcur*20, 25+Ycur*20, 10, 10);            //mouse 초기화   
}

void Y_U_moveerase(void)
{
    LCD_SetBrushColor(RGB_WHITE);       //mouse color: White
    LCD_DrawFillRect(5+Xcur*20, 25+(Ycur+1)*20, 10, 10);        //mouse 초기화
    
    LCD_SetBrushColor(RGB_BLUE);        //mouse color: Blue
    LCD_DrawFillRect(5+Xcur*20, 25+Ycur*20, 10, 10);    //mouse 초기화   
}

void XcurYcur_UDLRcount(void)
{
  LCD_SetBrushColor(RGB_WHITE);       //block color: White
  
    //Xcur, Ycur 위치에 흰색 박스
    LCD_DrawFillRect(145, 55, 5, 10);        //'X' 지우기
    LCD_DrawFillRect(133, 55, 5, 10);        //'Y' 지우기
    
    //U/D/L/R count 위치에 흰색박스
    LCD_DrawFillRect(31, 110, 5, 10);        //'U' 지우기
    LCD_DrawFillRect(61, 110, 5, 10);        //'D' 지우기
    LCD_DrawFillRect(91, 110, 5, 10);        //'L' 지우기
    LCD_DrawFillRect(121, 110, 5, 10);        //'R' 지우기
    
    LCD_SetTextColor(RGB_RED);	//글자색 : Red
    
    //X,Y좌표 입력
    LCD_DisplayChar(5,22,Xcur+0x30);      //0x30이 뭔 의미지...
    LCD_DisplayChar(5,24,Ycur+0x30);
    
    //U/D/L/Rcount 횟수 입력
    LCD_DisplayChar(10,5,Ucount+0x30);
    LCD_DisplayChar(10,10,Dcount+0x30);
    LCD_DisplayChar(10,15,Lcount+0x30);
    LCD_DisplayChar(10,20,Rcount+0x30);
}


void UCount_0_9(void)
{
	if (Ucount > 9)
      Ucount=0;
}

void DCount_0_9(void)
{
	if (Dcount > 9)
      Dcount=0;
}

void LCount_0_9(void)
{
	if (Lcount > 9)
      Lcount=0;
}

void RCount_0_9(void)
{
	if (Rcount > 9)
      Rcount=0;
}

void BEEP(void)			/* beep for 30 ms */
{ 	
	GPIOF->ODR |=  0x0200;	// PF9 'H' Buzzer on
	DelayMS(30);		// Delay 30 ms
	GPIOF->ODR &= ~0x0200;	// PF9 'L' Buzzer off
}

void DelayMS(unsigned short wMS)
{
    register unsigned short i;
    for (i=0; i<wMS; i++)
        DelayUS(1000);         		// 1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
    volatile int Dly = (int)wUS*17;
    for(; Dly; Dly--);
}

uint8_t joy_flag = 0;
uint16_t JOY_Scan(void)	// input joy stick NAVI_* 
{ 
	uint16_t key;
	key = GPIOI->IDR & 0XFF00;	// any key pressed ?
	if(key == 0XFF00)		// if no key, check key off
	{	if(joy_flag == 0)
			return key;
		else
		{	DelayMS(10);
			joy_flag = 0;
			return key;
		}
	}
	else				// if key input, check continuous key
	{	if(joy_flag != 0)	// if continuous key, treat as no key input
			return 0XFF00;
		else			// if new key,delay for debounce
		{	joy_flag = 1;
			DelayMS(10);
			return key;
		}
	}
}

/* Switch가 입력되었는지를 여부와 어떤 switch가 입력되었는지의 정보를 return하는 함수  */ 
uint8_t key_flag = 0;
uint16_t KEY_Scan(void)	// input key SW0 - SW7 
{ 
	uint16_t key;
	key = GPIOH->IDR & 0xFF00;	// any key pressed ?
	if(key == 0xFF00)		// if no key, check key off
	{	if(key_flag == 0)
			return key;
		else
		{	DelayMS(10);
			key_flag = 0;
			return key;
		}
	}
	else				// if key input, check continuous key
	{	if(key_flag != 0)	// if continuous key, treat as no key input
			return 0xFF00;
		else			// if new key,delay for debounce
		{	key_flag = 1;
			DelayMS(10);
 			return key;
		}
	}
}

void _GPIO_Init(void)
{
      	// LED (GPIO G) 설정
    	RCC->AHB1ENR	|=  0x00000040;	// RCC_AHB1ENR : GPIOG(bit#6) Enable							
	GPIOG->MODER 	|=  0x00005555;	// GPIOG 0~7 : Output mode (0b01)						
	GPIOG->OTYPER	&= ~0x00FF;	        // GPIOG 0~7 : Push-pull  (GP8~15:reset state)	
 	GPIOG->OSPEEDR 	|=  0x00005555;	// GPIOG 0~7 : Output speed 25MHZ Medium speed 
    
	// SW (GPIO H) 설정 
	RCC->AHB1ENR    |=  0x00000080;	        // RCC_AHB1ENR : GPIOH(bit#7) Enable							
	GPIOH->MODER 	&= ~0xFFFF0000;	// GPIOH 8~15 : Input mode (reset state)				
	GPIOH->PUPDR 	&= ~0xFFFF0000;	// GPIOH 8~15 : Floating input (No Pull-up, pull-down) :reset state

	// Buzzer (GPIO F) 설정 
    	RCC->AHB1ENR	|=  0x00000020;         // RCC_AHB1ENR : GPIOF(bit#5) Enable							
	GPIOF->MODER 	|=  0x00040000;	// GPIOF 9 : Output mode (0b01)						
	GPIOF->OTYPER 	&= ~0x0200;	        // GPIOF 9 : Push-pull  	
 	GPIOF->OSPEEDR 	|=  0x00040000;	// GPIOF 9 : Output speed 25MHZ Medium speed 

        //Joy Stick SW(PORT I) 설정
	RCC->AHB1ENR	|= 0X00000100;	// RCC_AHB1ENR GPIOI Enable
	GPIOI->MODER	&= 0X000FFC00;	// GPIOI 5~9 : Input mode (reset state)
	GPIOI->PUPDR	&= ~0x000FFC00;	// GPIOI 5~9 : Floating input (No Pull-up, pull-down) (reset state)
}

