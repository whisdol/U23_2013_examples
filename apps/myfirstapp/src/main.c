#include <System.h>
#include <Accelerometer.h>

#include <stdio.h>

int main()
{
	// Do some basic initialization tasks
	InitializeSystem();

	// Initialize pins for LEDs
	InitializeAccelerometer();
	CalibrateAccelerometer();
	iprintf("Accelerometer calibrated.\n\r");

	// Enable printf via trace macrocell (get output with 'make trace')
	EnableDebugOutput(DEBUG_ITM);


	int8_t data[3];
	

	//Turn on all LEDs
	while(1)
	{
		ReadCalibratedAccelerometerData(data);
		iprintf("%i; %i; %i;\r\n", data[0], data[1], data[2]);
		Delay(10);
	}
}
