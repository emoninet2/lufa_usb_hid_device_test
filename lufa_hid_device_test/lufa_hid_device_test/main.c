/*
 * lufa_hid_device_test.c
 *
 * Created: 20-Sep-15 4:20:18 AM
 *  Author: emon1
 */ 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Descriptors.h"


#include "usart.h"
#include "serial_stdio.h"



// From module: LUFA Board Hardware Information Driver
#include <LUFA/Drivers/Board/Board.h>

// From module: LUFA Board LED Driver
#include <LUFA/Drivers/Board/LEDs.h>

// From module: LUFA Common Infrastructure
#include <LUFA/Common/Common.h>
#include <LUFA/Version.h>

// From module: LUFA Platform Specific Support - AVR8
#include <LUFA/Platform/Platform.h>
#include <LUFA/Drivers/USB/USB.h>

//////////////////////////////////////////////////////////////////////////

bool x = 0;



//////////////////////////////////////////////////////////////////////////

static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];


USB_ClassInfo_HID_Device_t Generic_HID_Interface =
{
	.Config =
	{
		.InterfaceNumber              = INTERFACE_ID_GenericHID,
		.ReportINEndpoint             =
		{
			.Address              = GENERIC_IN_EPADDR,
			.Size                 = GENERIC_EPSIZE,
			.Banks                = 1,
		},
		.PrevReportINBuffer           = PrevHIDReportBuffer,
		.PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
	},
};



/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

	USB_Device_EnableSOFEvents();

}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{

	uint8_t* Data        = (uint8_t*)ReportData;
	//uint8_t  CurrLEDMask = LEDs_GetLEDs();
	
	Data[0] = x;	
	printf("creating report LOL\n\r");
	printf("Data[0] : %x \n\r",Data[0]);
	printf("Data[1] : %x \n\r",Data[1]);
	printf("Data[2] : %x \n\r",Data[2]);
	printf("Data[3] : %x \n\r",Data[3]);

// 	Data[0] = ((CurrLEDMask & LEDS_LED1) ? 1 : 0);
// 	Data[1] = ((CurrLEDMask & LEDS_LED2) ? 1 : 0);
// 	Data[2] = ((CurrLEDMask & LEDS_LED3) ? 1 : 0);
// 	Data[3] = ((CurrLEDMask & LEDS_LED4) ? 1 : 0);

	*ReportSize = GENERIC_REPORT_SIZE;
	return false;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
	printf("you received some shit\n\r");

	uint8_t* Data       = (uint8_t*)ReportData;
	uint8_t  NewLEDMask = LEDS_NO_LEDS;

	printf("your report Data[0] is %X\n\r",Data[0]);
	printf("your report Data[1] is %X\n\r",Data[1]);
	printf("your report Data[2] is %X\n\r",Data[2]);
	printf("your report Data[3] is %X\n\r",Data[3]);


	if (Data[0])
	  NewLEDMask |= LEDS_LED1;

	if(Data[0]&0x01)
	{
		PORTC &= ~(1<<7);
	}
	else
	{
		PORTC |= (1<<7);
	}

	if (Data[1])
	  NewLEDMask |= LEDS_LED2;

	if (Data[2])
	  NewLEDMask |= LEDS_LED3;

	if (Data[3])
	  NewLEDMask |= LEDS_LED4;

	LEDs_SetAllLEDs(NewLEDMask);
}










int main(void)
{
	
	usart_set_baud_rate(9600);
	usart_setup(0,0,0,3,0);
	usart_enable();
	stdio_serial_initialize();
	
	printf("this is a test message to mark the start up of the USB testing\n\r");
	
	DDRC |= (1<<7);
	
	SetupHardware();
	GlobalInterruptEnable();
    while(1)
	
    {
        //TODO:: Please write your application code 
		HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();
		x ^= 1;
    }
}