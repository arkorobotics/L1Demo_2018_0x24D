#include "xc.h"
#include "audio.h"
#include "music.h"

volatile uint32_t time_sec;		           // Demo Master Clock
volatile uint32_t time_subsec;

volatile unsigned short ch1_val = 0;       // Audio Channel 1

void audio_init(void) 
{
    PR1 = 0xFF;	// TODO: Add math for calculating PR1 based on sample freq (62.745kHz)
    _T1IP = 5;	// set interrupt priority
    _TON  = 1;	// turn on the timer
    _T1IF = 0;	// reset interrupt flag
    _T1IE = 1;	// turn on the timer1 interrupt

    time_sec = 0;       // Time Epoch
    time_subsec = 0; 
}

//_T1Interrupt() is the T1 interrupt service routine (ISR).
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    // Song index
	static unsigned int idx = 0;

	static unsigned short duration = 0;
    static unsigned int ch1_ncount = 0;

    // Do Channel 1 Stuff
    if (ch1_ncount < 0x7F80)
    {
        ch1_ncount += song_ch1f[idx];
    }
    else
    {
        ch1_ncount = 0;
    }

    ch1_val = sinetable[ch1_ncount>>6];
    
	// Duration
	if(duration < 0x7A1)
	{
	 	duration++;
	}
    else
    {
	 	idx++;

        // Loop it!
		if(idx == sizeof(song_ch1f) / sizeof(song_ch1f[0]) ) 
		{
			idx = 0;
		}

		duration = 0;
	}
	
	// Mix and set audio
    PORTB = (ch1_val<<8);

    // Time Keeping
    time_subsec++;

    if(time_subsec >= 62745)
    {
        time_sec++;
        time_subsec = 0;
    }

    // Clear Timer Flag
	_T1IF = 0;
}

