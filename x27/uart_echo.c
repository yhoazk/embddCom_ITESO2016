    //*****************************************************************************
//
// uart_echo.c - Example for reading data from and writing data to the UART in
//               an interrupt driven fashion.
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

//*****************************************************************************
//! This example application utilizes the UART to echo text.  The first UART
//! (connected to the USB debug virtual serial port on the evaluation board)
//! will be configured in 115,200 baud, 8-n-1 mode.  All characters received on
//! the UART are transmitted back to the UART.
//*****************************************************************************

#define ECU_ID (0x60)
#define TESTER_ID (0xF0)
volatile unsigned int ui32receivedByte = 0; // flag to let know that a new byte
                                            // has arrived
//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
// The UART interrupt handler.
//*****************************************************************************
void
UARTIntHandler(void)
{
    unsigned long ulStatus;

    // Get the interrrupt status.
    ulStatus = ROM_UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART0_BASE, ulStatus);

    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        // Read the next character from the UART and write it back to the UART.
        ROM_UARTCharPutNonBlocking(UART0_BASE, '-');
        //ROM_UARTCharPutNonBlocking(UART0_BASE, 'p');
        ui32receivedByte = ROM_UARTCharGetNonBlocking(UART0_BASE); //cosume the char in the buffer

        // Blink the LED to show a character transfer is occuring.
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        // Turn off the LED
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
}

//*****************************************************************************
// Send a string to the UART.
//*****************************************************************************

void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    // Loop while there are more characters to send.
    // char k = 'k';
    while(ulCount--)
    {
        // Write the next character to the UART.
        //ROM_UARTCharPutNonBlocking(UART0_BASE, k);
        //ROM_UARTCharPutNonBlocking(UART0_BASE, (*pucBuffer)+1);
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
        //pucBuffer++;
    }
}

//****************************************************************************
// My FSM
// add a state to send the request to authenticate also by secquence
// OR initate the request as soon as the device is ready
//****************************************************************************
typedef enum STATES_t{
    IDLE = 'A',
    CHECK_IF_HEADER,
    HEADER_FORMAT,
    HEADER_TGT,
    HEADER_SRC,
    CHECK_IF_SID,
    GET_DATA_BYTES,
    GET_KEY,
    CHECK_SUM,
    WAIT_5MS,
    SEND_HEADER,
    SEND_ECU_ID,
    SEND_TST_ID,
    SEND_ACC_RSID,
    SEND_ACC_SR,
    SEND_CRC,
    SEND_RND_SEED,
    SEND_ACC_KEY_VER_REQ,
    SEND_CALC_KEY,
    SEND
}STATES;
/* FSM request for seed */
STATES ui32TxReqSecSeed(void){
    static STATES currentState = CHECK_IF_HEADER;
    static STATES nextState = IDLE;
    switch(currentState){
        default:
        case SEND_HEADER:
            nextState = SEND_ECU_ID;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0x82);
            ROM_UARTCharPutNonBlocking(UART0_BASE, 'a');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            //UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            break;
        case SEND_ECU_ID:
            nextState = SEND_TST_ID;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, ECU_ID);
            ROM_UARTCharPutNonBlocking(UART0_BASE, 'b');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_TST_ID:
            nextState = SEND_ACC_RSID;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, TESTER_ID);
            ROM_UARTCharPutNonBlocking(UART0_BASE, 'c');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_ACC_RSID:
            nextState = SEND_ACC_SR;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0x27);
            ROM_UARTCharPutNonBlocking(UART0_BASE, 'd');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_ACC_SR:
            nextState = SEND_CRC;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0x01);
            ROM_UARTCharPutNonBlocking(UART0_BASE, 'e');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_CRC:
            nextState = SEND_HEADER;
            ROM_UARTCharPutNonBlocking(UART0_BASE, 'f');
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0xdb);
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
    }
    currentState = nextState;
    return currentState;
}

/* FSM processing the response  */
STATES ui32RxSecSeedProv(unsigned char ubMessage){
    static STATES currentState = CHECK_IF_HEADER;
    static STATES nextState = IDLE;

}

/* FSM processing the response  */
STATES ui32TxReqAuthSecKey(void){
    static STATES currentState = CHECK_IF_HEADER;
    static STATES nextState = IDLE;
    static unsigned int byteCntr = 0;
    switch(currentState){
        default:
        case SEND_HEADER:
            byteCntr = 0;
            nextState = SEND_ECU_ID;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0x83);
            ROM_UARTCharPutNonBlocking(UART0_BASE, '0');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            //UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            break;
        case SEND_ECU_ID:
            nextState = SEND_TST_ID;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, ECU_ID);
            ROM_UARTCharPutNonBlocking(UART0_BASE, '1');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_TST_ID:
            nextState = SEND_ACC_RSID;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, TESTER_ID);
            ROM_UARTCharPutNonBlocking(UART0_BASE, '2');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_ACC_RSID:
            nextState = SEND_ACC_KEY_VER_REQ;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0x27);
            ROM_UARTCharPutNonBlocking(UART0_BASE, '3');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_ACC_KEY_VER_REQ:
            nextState = SEND_CALC_KEY;
            byteCntr = 8;
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0x02);
            ROM_UARTCharPutNonBlocking(UART0_BASE, '4');
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;
        case SEND_CALC_KEY:
            if(byteCntr-- > 0){
                    nextState = SEND_CALC_KEY;
                    //byteCntr--;
                    ROM_UARTCharPutNonBlocking(UART0_BASE, 'x');
                    SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            }
            else{
                nextState = SEND_CRC;
            }
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0xdb);
            break;
        case SEND_CRC:
            nextState = SEND_HEADER;
            ROM_UARTCharPutNonBlocking(UART0_BASE, '.');
            // ROM_UARTCharPutNonBlocking(UART0_BASE, 0xdb);
            SysCtlDelay(SysCtlClockGet() / (1000 * 1));
            break;

    }
    currentState = nextState;
    return currentState;
}

/* FSM processing the response  */
STATES ui32RxAccGrnt(unsigned char ubMessage){
    static STATES currentState = CHECK_IF_HEADER;
    static STATES nextState = IDLE;
}

/* super fsm to control the other fsms */
STATES ui32x27ProcessFSM( unsigned char ubMessage){
    static STATES currentState = CHECK_IF_HEADER;
    static STATES nextState = IDLE;

    switch(currentState){
        default:
        case CHECK_IF_HEADER:
            if(ubMessage == 82){
                nextState = HEADER_FORMAT;
                UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            }
            else{
                //printf("Idle\n");
            }
            break;
        case HEADER_FORMAT:
            if(ubMessage == ECU_ID){
                nextState = HEADER_TGT;
                UARTSend((unsigned char *)"\033[2JECU_ID Correct\n", 18);
            }
            break;
        case HEADER_TGT:
            if(ubMessage == TESTER_ID){
                nextState = HEADER_SRC;
                UARTSend((unsigned char *)"\033[2JTESTER_ID Correct\n", 18);
            }
            break;
        case HEADER_SRC:
            if(ubMessage == 27){
                nextState = CHECK_IF_SID;
                UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            }
            break;
        case CHECK_IF_SID:
            if(ubMessage == 1){
                nextState = CHECK_SUM;
                UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            }
            break;
        case GET_DATA_BYTES:
            if(ubMessage == 82){
                nextState = HEADER_FORMAT;
                UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            }
            break;
        case GET_KEY:
            if(ubMessage == 82){
                nextState = HEADER_FORMAT;
                UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            }
            break;
        case CHECK_SUM:
            if(ubMessage == 82){
                nextState = SEND;
                UARTSend((unsigned char *)"\033[2JHeader Correct\n", 18);
            }
            break;
        case WAIT_5MS:
            break;
        case SEND:
            break;
    }
    currentState = nextState;
    return currentState;
}



//*****************************************************************************
// This example demonstrates how to send a string of data to the UART.
//*****************************************************************************
int
main(void)
{
    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable the GPIO pins for the LED (PF2).
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable processor interrupts.
    ROM_IntMasterEnable();

    // Set GPIO A0 and A1 as UART pins.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    // Enable the UART interrupt.
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    // Prompt for text to be entered.
    UARTSend((unsigned char *)"\033[2JAutenticate:", 16);

    // Loop forever echoing data through the UART.
    /* FSM for X27 */
    int msgLen = 0;
    while(1)
    {
        //ui32TxReqSecSeed();
        if (ui32receivedByte == '1'){
            // call the fnc the number of bytes to send
            if(msgLen == 0){
                msgLen = 6;
            } else if(msgLen == 1){
                ui32receivedByte = 0;
            }
            ui32TxReqSecSeed();
            msgLen--;

        }
        else if (ui32receivedByte == '2') {
            // call the fnc the number of bytes to send
            if(msgLen == 0){
                msgLen = 15;
            } else if(msgLen == 1){
                ui32receivedByte = 0;
            }
            ui32TxReqAuthSecKey();
            msgLen--;
        }

        
    }
}
