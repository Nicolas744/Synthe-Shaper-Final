#include <F28x_Project.h>
#include <stdlib.h>


struct Note
{
    bool valid;         // lets us know if this note is still being pressed
    int note_num;       // the MIDI value of the note
    int velocity;       // the velocity of the note
    float freq;         // the actual frequency of the note
    uint32_t envCount;  // a counter used in calculating the envelope
    int squareCutoff;   // used in calculating the square wave
};


void initUART(){

    EALLOW;
    // set GPIO MUX values

    GpioCtrlRegs.GPEGMUX1.bit.GPIO139 = 1;
    GpioCtrlRegs.GPEMUX1.bit.GPIO139 = 2;
    GpioCtrlRegs.GPEDIR.bit.GPIO139 = 0;
    GpioCtrlRegs.GPEPUD.bit.GPIO139 = 0;

    // set SPICCR values

    ScicRegs.SCICCR.all = 0x07; // set parity, stop bits, and number of bits in a frame

    // set SCICTL 1 and 2 values

    ScicRegs.SCICTL1.all = 0x41; // enable RX and RX interrupt also perform software reset
    ScicRegs.SCICTL1.bit.SWRESET = 1; // unsure on if I need this or not

    ScicRegs.SCICTL2.all = 0x02; // enable RX interrupt

    // set Baud rate

    ScicRegs.SCIHBAUD.all = 0x03;
    ScicRegs.SCILBAUD.all = 0x1C; // 199 (C7) gives a baud rate of 31250

    // enable ints and PIE
    SysCtl_selectCPUForPeripheral(SYSCTL_CPUSEL5_SCI , 3, SYSCTL_CPUSEL_CPU2);
}

float noteToFreq(int note_num){
    float freq = note_num-69;
    freq = freq/12;
    freq = pow(2,freq);
    freq = freq*440;
    return freq;
}
