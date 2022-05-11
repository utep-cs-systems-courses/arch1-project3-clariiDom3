#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "switches.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

/*
void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}
*/

// axis zero for col, axis 1 for row
 //short drawPos[2] = {10,10}, controlPos[2] = {10,10};
 //short velocity[2] = {3,8}, limits[2] = {screenWidth-36, screenHeight-8};

short redrawScreen = 1;
//u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
    secCount = 0;
    redrawScreen = 1;
  }
}
  
//void update_shape();

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();
  enableWDTInterrupts();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  u_char width = screenWidth, height = screenHeight;
  clearScreen(COLOR_BLUE);
  while (1) {			/* forever */
    if (redrawScreen) {
      drawString5x7(42, 5, "PROVENZA", COLOR_WHITE, COLOR_BLUE);
      drawString5x7(37, 15, "By KAROL G", COLOR_WHITE, COLOR_BLUE);
      fillRectangle(42, 44, 50, 50, COLOR_WHITE);
      fillRectangle(18, 115, 92, 2, COLOR_WHITE);
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

    
unsigned int SHAPE_COLOR = COLOR_BLACK;    
void
update_shape()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static int colStep = 2;
  static int rowStep = 2;
  
  static char blue = 31, green = 0, red = 31;
  static unsigned char step = 0;

  if(step <= 60)
    {
      int startCol = col - step;
      int endCol = col + step;
      int width = 22;

      fillRectangle(18+step, 115, width, 4, SHAPE_COLOR);
      //fillRectangle(endCol-step, row+step, width, 1, SHAPE_COLOR);

      if(switches & SW1){
	//SHAPE_COLOR = COLOR_BLACK;
	fillRectangle(63, 130, 3, 10, COLOR_WHITE);
	fillRectangle(70, 130, 3, 10, COLOR_WHITE);
	buzzer_off();
      }
      if(switches & SW2){
	//	SHAPE_COLOR = COLOR_RED;
	drawChar5x7(65, 65, 'P', COLOR_HOT_PINK, COLOR_WHITE);
        buzzer_off();
      }
      if(switches & SW3){
	SHAPE_COLOR = COLOR_RED;
	drawPixel(10, 5, COLOR_HOT_PINK);
	drawPixel(120, 5,COLOR_WHITE);
	drawPixel(120, 150, COLOR_GREEN);
	drawPixel(10, 150, COLOR_RED);
	buzzer_off();
      }
      if(switches & SW4){
	starwars_song();
      }
      step++;
    }else{
    col += colStep*2;
    clearScreen(COLOR_BLUE);
    step = 0;
  }
}


/* Switch on S2 */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
