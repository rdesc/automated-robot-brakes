//  motorsensor.c
#include "msp430.h"

// define all used ports on MSP430
#define     M1_1                  BIT1
#define     M1_2                  BIT2
#define     M2_1                  BIT7
#define     M2_2                  BIT1

#define     TRIG                  BIT4
#define     ECHO                  BIT5

#define     TXD                   BIT2                     

#define     BUTTON                BIT3

#define     BEEPER                BIT6

// define all variables
long sensor;
unsigned int distance;

unsigned int d;
unsigned int max;
unsigned int ind;

// define all methods
void beep(void);
void scan(int);
int measuredistance(void);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop WDT
    
    /* next three lines to use internal calibrated 1MHz clock: */
    BCSCTL1 = CALBC1_1MHZ;              // Set range
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);               // SMCLK = DCO = 1MHz
    
    // setup port for MOTORS, TRIG, BEEPER, ECHO, and BUTTON
    // set all ports to output except for P1.5 which is for ECHO and
    // P1.3 which is for push-button interrupt
    P1DIR = 0xDF;
    P1DIR &= ~BUTTON;
    P2DIR = 0xFF;
    
    // setup port for txd
    P1DIR |= TXD;
    P1OUT |= TXD;
    
    P1REN |= ECHO + BUTTON;             // Enable internal pull-up resistor on
    P1OUT &= ~(M1_1 + M2_1);
    P1OUT |= BUTTON;
    P2OUT &= ~(M1_2 + M2_2);
    P1IE |= BUTTON;                     //Enable input at P1.3 as an interrupt

    /* Configure hardware UART */
    P1SEL  |= TXD + BEEPER;     // P1.2=TXD and BEEPER to TA0.1
    P1SEL2 |= TXD;              // P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;       // Use SMCLK
    UCA0BR0 = 104;              // Set baud rate to 9600 with 1MHz clock
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;          // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;       // Initialize USCI state machine
    
    _BIS_SR (GIE);              // Enable general interrupt
}

// Port 1 interrupt service routine
void __attribute__ ((interrupt(PORT1_VECTOR)))  PORT1_ISR(void)
/* Main Application Loop */ {
    while(1){
            
       int d = (int)measuredistance(); // make distance measurement with sensor
        
        if(d > 20){                    // drive forward if distance > 20cm
            P1OUT &= ~(M1_1 + M2_1);
            P2OUT |= M1_2 + M2_2;
        }
        
        else{
            P2OUT &= ~(M1_2 + M2_2);   // otherwise drive in reverse
            P1OUT |= M1_1 + M2_1;      // while buzzer beeps twice
            beep();
            P1OUT &= ~(M1_1 + M2_1);
            __delay_cycles(500000);    // stop car
            scan(d);                   // start scanning environment
        }
        
        while (!(IFG2 & UCA0TXIFG));   // wait for TXD to be ready to send data
        UCA0TXBUF = d;                 // transmit data back to host computer for python to print
        __delay_cycles(100000);        // give the process some time to work
        
        P1IFG = 0x00;                  // clear any interrupt flags
        
    }
}

// do a 360 scan of environment and find direction
// in which obstacles are the furthest away
void scan(int distance)
{
    int arr[20] = {distance};          // initialize an array of size 20 with first element
                                       // in array being original distance measurement
    for (int i = 1; i < 20; i++)       // iterate over each item in array starting at index=1
    {
        P2OUT |= M1_2;                 // turn right for 0.14s (value found through rigorous testing)
        __delay_cycles(140000);        //***
        P2OUT &= ~(M1_2 + M2_2);       // stop
        int d = (int)measuredistance(); // take distance measurement and insert it into array
        arr[i] = d;
        __delay_cycles(200000);        // give a delay between each measurement
    }                                  // after for loop sensor would have taken 20 measurements
                                       // each at around 18 degree intervals for a full
                                       // 360 degree scan
    
    int max = 0;                       // initialize max distance = 0
    int ind = 0;                       // initialize array index = 0
    
    for(int i = 0; i < 20; i++)        // find element in array with greatest distance
    {
        if(max < arr[i]) {             // this will give the index of the array with the
            max = arr[i];              // greatest distance which will be used to find the
            ind = i;                   // direction in which the measurement was made
        }
    }
    
    if(ind < 10) {                      // if the index is less than 10 then turn right ind
        P2OUT |= M1_2;                  // number of times
        for(int i = 0; i < ind; i++)    // this will make the car face in the direction in
        {                               // which the max distance measurement was found
            __delay_cycles(140000);     //***delay here is the same when measurement was made***
        }
        P2OUT &= ~(M1_2 + M2_2);
    }
    
    else
    {                                   // if the index is greater or equal to 10 then turn left
        P2OUT |= M2_2;                  // (20-ind) number of times
        for(int i = 0; i < (20-ind); i++) // this will make the car face in the direction in
        {                               // which the max distance measurement was found
            __delay_cycles(140000);     //***delay here is the same when measurement was made***
        }
        P2OUT &= ~(M1_2 + M2_2);
    }
}

// measure distance in cm using sensor
int measuredistance(void)
{
    
    // setup timer to count the time ECHO is high
    TACCR0 = 0xFFFF;         // period
    TACTL = TASSEL_2 + MC_1; // TACLK = SMCLK, Up mode, start timer
    
    TACTL |= TACLR;          // clears the TAR value
    
    P1OUT |= TRIG;           // Start pulse
    __delay_cycles(10);      // Send pulse for 10 microsec.
    P1OUT &= ~TRIG;          // Stop pulse
    while(!(P1IN & ECHO));   // give time for pulse to trigger ECHO
    
    TAR = 0;                 // set timer = 0
    
    while(P1IN & ECHO);      // continue timer while ECHO is high
    
    sensor = (long)TAR;      // get data from TAR
    distance = sensor/56;    // convert data to distance in cm
    
    return distance;         // return distance measurement as an int type
    
}

// simulate back-up beeper using a PWM signal
void beep(void)
{
    CCTL1 = OUTMOD_7;              // CCR1 reset/set
    
    // beep twice
    for(int i = 0; i < 2; i++) {
        
        CCR0 = 1000-1;             // PWM Period
        CCR1 = 300;                // CCR1 PWM duty cycle
        TACTL = TASSEL_2 + MC_1;   // SMCLK, up mode
        __delay_cycles(400000);    // beep for 0.4s
        TACTL = TASSEL_2 + MC_0;
        CCR0 = 0;
        __delay_cycles(200000);    // pause for 0.2s
        
    }
}

