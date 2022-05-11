#include <msp430.h>
#include "buzzer.h"
#include "libTimer.h"

void buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}

void buzzer_set_period(short cycles)
{
  CCR0 = cycles;
  CCR1 = cycles >> 1;
}

void buzzer_off()
{
  buzzer_set_period(0);
}

static int note_count = 0;
void starwars_song()
{
  switch(note_count)
    {
    case 0:
      buzzer_set_period(950);
      note_count++;
      break;
    case 1:
    case 6:
    case 11:
      buzzer_set_period(950);
      note_count++;
      break;
    case 2:
    case 7:
    case 12:
    case 14:
      buzzer_set_period(710);
      note_count++;
      break;
    case 3:
    case 8:
    case 13:
      buzzer_set_period(840);
      note_count++;
      break;
    case 4:
    case 9:
    case 15:
      buzzer_set_period(840);
      if(note_count == 15)
	{
	  note_count = 0;
	}
      else
	{
	  note_count++;
	}
      break;
    case 5:
    case 10:
      buzzer_set_period(475);
      note_count++;
      break;
    }
}
