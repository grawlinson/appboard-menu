// Defines & Includes
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>	// Used for _delay_ms() function.
#include "LabBoard.h"	//" - external from base installation
// Function Declarations
void setup(void);
char getInput(void);
	// LCD/Drawing
void drawMenu(void);
void drawMainMenu(void);
void drawRGBControlMenu(void);
void drawRGBMenu(void);
void drawPeripheralMenu(void);
void moveCursor(char position);
void updateValue(void);
	// RGB
void increaseRed(void);
void decreaseRed(void);
void increaseGreen(void);
void decreaseGreen(void);
void increaseBlue(void);
void decreaseBlue(void);
	// Lamp
void lampInit(void);
void lampDisable(void);
void lampSetBrightness(char brightness);
void increaseLampBrightness(void);
void decreaseLampBrightness(void);
	// Motor
void motorInit(void);
void motorDisable(void);
void motorSetSpeed(char speed);
void increaseMotorSpeed(void);
void decreaseMotorSpeed(void);
	// Speaker
void speakerInit(void);
void speakerDisable(void);
void speakerSetFrequency(char frequency);
void increaseSpeakerFrequency(void);
void decreaseSpeakerFrequency(void);
// Enums
enum inputs {NONE, UP, DOWN, LEFT, RIGHT};
enum states {TOP1, TOP2, TOP3, TOP4, MOTOR, LAMP, SPEAKER, RGB1, RGB2, RGB3, RED, GREEN, BLUE};
// Variables
enum inputs input;
enum states state;
char line1[20], line2[20];
char redCurrent, greenCurrent, blueCurrent;
char motorSpeed, lampBrightness, speakerFrequency;
// Main Function
int main(void)
{
	setup();	// Setup microcontroller IO/peripherals/etc.

	while(1)
    {
		_delay_ms(100);	// Cheap method of debouncing the inputs.

		input = getInput();

		/*
		State machine that performs some action(s) based on menu state & user input.

		Menus:
		TOP1-4: Main menu that links to the various peripherals.
		RGB1-3: RGB control menu that links to individual RGB current.

		Peripheral menus:
		MOTOR/LAMP/SPEAKER
		RED/GREEN/BLUE (RGB LED)

		Typical input action (apart from changing state & display):
		UP/DOWN: Move cursor or increment/decrement value.
		LEFT/RIGHT: Enter/exit menu.
		*/
		switch(state)
		{
			case TOP1:
				switch(input)
				{
					case UP:
						state = TOP4;

						moveCursor(3);
					break;
					case DOWN:
						state = TOP2;

						moveCursor(1);
					break;
					case RIGHT:
						state = MOTOR;

						drawMenu();
					break;
				}
			break;
			case TOP2:
				switch (input)
				{
					case UP:
						state = TOP1;

						moveCursor(0);
					break;
					case DOWN:
						state = TOP3;

						moveCursor(2);
					break;
					case RIGHT:
						state = LAMP;

						drawMenu();
					break;
				}
			break;
			case TOP3:
				switch (input)
				{
					case UP:
						state = TOP2;

						moveCursor(1);
					break;
					case DOWN:
						state = TOP4;

						moveCursor(3);
					break;
					case RIGHT:
						state = SPEAKER;

						drawMenu();
					break;
				}
			break;
			case TOP4:
				switch (input)
				{
					case UP:
						state = TOP3;

						moveCursor(2);
					break;
					case DOWN:
						state = TOP1;

						moveCursor(0);
					break;
					case RIGHT:
						state = RGB1;

						drawMenu();
					break;
				}
			break;
			case MOTOR:
				switch (input)
				{
					case UP:
						increaseMotorSpeed();

						updateValue();
					break;
					case DOWN:
						decreaseMotorSpeed();

						updateValue();
					break;
					case LEFT:
						state = TOP1;

						drawMenu();
					break;
				}
			break;
			case LAMP:
				switch (input)
				{
					case UP:
						increaseLampBrightness();

						updateValue();
					break;
					case DOWN:
						decreaseLampBrightness();

						updateValue();
					break;
					case LEFT:
						state = TOP2;

						drawMenu();
					break;
				}
			break;
			case SPEAKER:
				switch (input)
				{
					case UP:
						increaseSpeakerFrequency();

						updateValue();
					break;
					case DOWN:
						decreaseSpeakerFrequency();

						updateValue();
					break;
					case LEFT:
						state = TOP3;

						drawMenu();
					break;
				}
			break;
			case RGB1:
				switch (input)
				{
					case UP:
						state = RGB3;

						moveCursor(3);
					break;
					case DOWN:
						state = RGB2;

						moveCursor(2);
					break;
					case LEFT:
						state = TOP4;

						drawMenu();
					break;
					case RIGHT:
						state = RED;

						drawMenu();
					break;
				}
			break;
			case RGB2:
				switch (input)
				{
					case UP:
						state = RGB1;

						moveCursor(1);
					break;
					case DOWN:
						 state = RGB3;

						moveCursor(3);
					break;
					case LEFT:
						state = TOP4;

						drawMenu();
					break;
					case RIGHT:
						state = GREEN;

						drawMenu();
					break;
				}
			break;
			case RGB3:
				switch (input)
				{
					case UP:
						state = RGB2;

						moveCursor(2);
					break;
					case DOWN:
						state = RGB1;

						moveCursor(1);
					break;
					case LEFT:
						state = TOP4;

						drawMenu();
					break;
					case RIGHT:
						state = BLUE;

						drawMenu();
					break;
				}
			break;

			case RED:
				switch (input)
				{
					case UP:
						increaseRed();

						updateValue();
					break;
					case DOWN:
						decreaseRed();

						updateValue();
					break;
					case LEFT:
						state = RGB1;

						drawMenu();
					break;
				}
			break;

			case GREEN:
				switch (input)
				{
					case UP:
						increaseGreen();

						updateValue();
					break;
					case DOWN:
						decreaseGreen();

						updateValue();
					break;
					case LEFT:
						state = RGB2;

						drawMenu();
					break;
				}
			break;
			case BLUE:
				switch (input)
				{
					case UP:
						increaseBlue();

						updateValue();
					break;
					case DOWN:
						decreaseBlue();

						updateValue();
					break;
					case LEFT:
						state = RGB3;

						drawMenu();
					break;
				}
		}
	}
}
void setup(void)
{
	// Setup LCD & Display
	SLCDInit();
	SLCDDisplayOn();
	drawMenu();

	// Setup IO
	DDRE = 0x03;	// Multiplexer: Buttons
	PORTE = 0x01;
	DDRA = 0x00;	// Buttons

	// Setup Peripherals
	lampInit();
	motorInit();
	speakerInit();

	// Setup RGB LED
	RGBInit();
	RGBDisable(RGB_RED|RGB_GREEN|RGB_BLUE);
}
// Captures button input & passes selected button to input enum.
char getInput(void)
{
	char input = NONE;	// Default: NONE

	// If UP/DOWN/LEFT/RIGHT selected, input variable changed.
	if(!(PINA & _BV(PINA3)))
	{
		input = UP;
	}
	else if(!(PINA & _BV(PINA5)))
	{
		input = DOWN;
	}
	else if(!(PINA & _BV(PINA7)))
	{
		input = LEFT;
	}
	else if(!(PINA & _BV(PINA4)))
	{
		input = RIGHT;
	}

	return input;
}
// Draws the required menu on the display when required.
void drawMenu(void)
{
	SLCDTurnOffBlinkingCursor();	// Turned off to prevent blinking cursor jumping all over the place when display is drawn. Also not required in some menus.
	SLCDClearScreen();	// Screen cleared before next re-draw.

	switch(state)
	{
		case TOP1:
		drawMainMenu();
		moveCursor(0);	// User input: Cursor only moved & blinking when in TOP1-4 or RGB1-3 menus.
		SLCDTurnOnBlinkingCursor();
		break;
		case TOP2:
		drawMainMenu();
		moveCursor(1);
		SLCDTurnOnBlinkingCursor();
		break;
		case TOP3:
		drawMainMenu();
		moveCursor(2);
		SLCDTurnOnBlinkingCursor();
		break;
		case TOP4:
		drawMainMenu();
		moveCursor(3);
		SLCDTurnOnBlinkingCursor();
		break;
		case MOTOR:
		drawPeripheralMenu();
		break;
		case LAMP:
		drawPeripheralMenu();
		break;
		case SPEAKER:
		drawPeripheralMenu();
		break;
		case RGB1:
		drawRGBControlMenu();
		moveCursor(1);
		SLCDTurnOnBlinkingCursor();
		break;
		case RGB2:
		drawRGBControlMenu();
		moveCursor(2);
		SLCDTurnOnBlinkingCursor();
		break;
		case RGB3:
		drawRGBControlMenu();
		moveCursor(3);
		SLCDTurnOnBlinkingCursor();
		break;
		case RED:
		drawRGBMenu();
		break;
		case GREEN:
		drawRGBMenu();
		break;
		case BLUE:
		drawRGBMenu();
		break;
	}
}
// Template for the main menu.
void drawMainMenu(void)
{
	SLCDSetCursorPosition(0,0);
	SLCDWriteString(" 1) Motor");
	SLCDSetCursorPosition(1,0);
	SLCDWriteString(" 2) Lamp");
	SLCDSetCursorPosition(2,0);
	SLCDWriteString(" 3) Speaker");
	SLCDSetCursorPosition(3,0);
	SLCDWriteString(" 4) RGB LED");
}
// Template for the RGB control menu.
void drawRGBControlMenu(void)
{
	SLCDSetCursorPosition(0,0);
	SLCDWriteString("RGB LED Control");
	SLCDSetCursorPosition(1,0);
	SLCDWriteString(" 1) Set Red mA");
	SLCDSetCursorPosition(2,0);
	SLCDWriteString(" 2) Set Green mA");
	SLCDSetCursorPosition(3,0);
	SLCDWriteString(" 3) Set Blue mA");
}
// Template  for the RED/GREEN/BLUE RGB LED menus. Could be combined with peripheralMenu.
void drawRGBMenu(void)
{
	// Write to line1/2 depending on RGB state.
	switch(state)
	{
		case RED:
			sprintf(line1, "RGB Red LED");
			sprintf(line2, "%2dmA", redCurrent);
		break;
		case GREEN:
			sprintf(line1, "RGB Green LED");
			sprintf(line2, "%2dmA", greenCurrent);
		break;
		case BLUE:
			sprintf(line1, "RGB Blue LED");
			sprintf(line2, "%2dmA", blueCurrent);
		break;
	}

	// Draw menu.
	SLCDSetCursorPosition(0,0);
	SLCDWriteString(line1);
	SLCDSetCursorPosition(1,0);
	SLCDWriteString(line2);
}
// Template for the peripheral (MOTOR/LAMP/SPEAKER) menus.
void drawPeripheralMenu(void)
{
	// Write to line1/2 depending on peripheral state.
	switch(state)
	{
		case MOTOR:
			sprintf(line1, "Motor Speed");
			sprintf(line2, "%3d%%", motorSpeed);
		break;
		case LAMP:
			sprintf(line1, "Lamp Brightness");
			sprintf(line2, "%3d%%", lampBrightness);
		break;
		case SPEAKER:
			sprintf(line1, "Speaker Frequency");
			sprintf(line2, "%2dkHz", speakerFrequency);
		break;
	}

	// Draw menu.
	SLCDSetCursorPosition(0,0);
	SLCDWriteString(line1);
	SLCDSetCursorPosition(1,0);
	SLCDWriteString(line2);
}
// Move the cursor based on user input.
void moveCursor(char position)
{
	SLCDSetCursorPosition(position, 0);
}
// Update display when variable changed.
void updateValue(void)
{
	// Write to line2 depending on which variable has changed.
	switch(state)
	{
		case MOTOR:
			sprintf(line2, "%3d%%", motorSpeed);
		break;
		case LAMP:
			sprintf(line2, "%3d%%", lampBrightness);
		break;
		case SPEAKER:
			sprintf(line2, "%2dkHz", speakerFrequency);
		break;
		case RED:
			sprintf(line2, "%2dmA", redCurrent);
		break;
		case GREEN:
			sprintf(line2, "%2dmA", greenCurrent);
		break;
		case BLUE:
			sprintf(line2, "%2dmA", blueCurrent);
		break;
	}

	SLCDSetCursorPosition(1,0);
	SLCDWriteString(line2);
}
// Increase Red RGB LED current.
void increaseRed(void)
{
	if(redCurrent == 0)	// 0 = RGB LED disabled, therefore it needs to be enabled. Reasoning: RGBWriteXCurrent(0) = 0.5mA.
	{
		// Enable RED RGB LED.
		//RGBEnable(RGB_X) disables the other LEDs if used. Therefore this if/else block is required to ensure other LEDs are kept on.
		if(greenCurrent > 0)
		{
			if(blueCurrent > 0)
			{
				RGBEnable(RGB_RED|RGB_GREEN|RGB_BLUE);
			}
			else
			{
				RGBEnable(RGB_RED|RGB_GREEN);
			}
		}
		else
		{
			if(blueCurrent > 0)
			{
				RGBEnable(RGB_RED|RGB_BLUE);
			}
			else
			{
				RGBEnable(RGB_RED);
			}
		}

		redCurrent++;
		RGBWriteRedCurrent(redCurrent);
	}
	else if(redCurrent < 32)	// 32mA limit.
	{
		redCurrent++;
		RGBWriteRedCurrent(redCurrent);
	}
}
// Decrease Red RGB LED current.
void decreaseRed(void)
{
	if(redCurrent <= 1) // Disable RGB LED. Reasoning: RGBWriteXCurrent(0) = 0.5mA.
	{
		redCurrent = 0;
		RGBDisable(RGB_RED);
	}
	else
	{
		redCurrent--;
		RGBWriteRedCurrent(redCurrent);
	}
}
// Increase Green RGB LED current.
void increaseGreen(void)
{
	if(greenCurrent == 0)	// 0 = RGB disabled, therefore it needs to be enabled. Reasoning: RGBWriteXCurrent(0) = 0.5mA.
	{
		// Enable GREEN RGB.
		//RGBEnable(RGB_X) disables the other LEDs if used. Therefore this if/else block is required to ensure other LEDs are kept on.

		if(redCurrent > 0)
		{
			if(blueCurrent > 0)
			{
				RGBEnable(RGB_RED|RGB_GREEN|RGB_BLUE);
			}
			else
			{
				RGBEnable(RGB_RED|RGB_GREEN);
			}
		}
		else
		{
			if(blueCurrent > 0)
			{
				RGBEnable(RGB_GREEN|RGB_BLUE);
			}
			else
			{
				RGBEnable(RGB_GREEN);
			}
		}

		greenCurrent++;
		RGBWriteGreenCurrent(greenCurrent);
	}
	else if(greenCurrent < 32)	// 32mA limit.
	{
		greenCurrent++;
		RGBWriteGreenCurrent(greenCurrent);
	}
}
// Decrease Green RGB LED current.
void decreaseGreen(void)
{
	if(greenCurrent <= 1)	// Disable RGB LED. Reasoning: RGBWriteXCurrent(0) = 0.5mA.
	{
		greenCurrent = 0;
		RGBDisable(RGB_GREEN);
	}
	else
	{
		greenCurrent--;
		RGBWriteGreenCurrent(greenCurrent);
	}
}
// Increase Blue RGB LED current.
void increaseBlue(void)
{
	if(blueCurrent == 0)	// 0 = RGB LED disabled, therefore it needs to be enabled. Reasoning: RGBWriteXCurrent(0) = 0.5mA.
	{
		// Enable BLUE RGB.
		//RGBEnable(RGB_X) disables the other LEDs if used. Therefore this if/else block is required to ensure other LEDs are kept on.

		if(redCurrent > 0)
		{
			if(greenCurrent > 0)
			{
				RGBEnable(RGB_RED|RGB_GREEN|RGB_BLUE);
			}
			else
			{
				RGBEnable(RGB_RED|RGB_BLUE);
			}
		}
		else
		{
			if(greenCurrent > 0)
			{
				RGBEnable(RGB_GREEN|RGB_BLUE);
			}
			else
			{
				RGBEnable(RGB_BLUE);
			}
		}
		blueCurrent++;
		RGBWriteBlueCurrent(blueCurrent);
	}
	else if(blueCurrent < 32)	// 32mA limit.
	{
		blueCurrent++;
		RGBWriteBlueCurrent(blueCurrent);
	}
}
// Decrease Blue RGB LED current.
void decreaseBlue(void)
{
	if(blueCurrent <= 1)	// Disable RGB LED. Reasoning: RGBWriteXCurrent(0) = 0.5mA.
	{
		blueCurrent = 0;
		RGBDisable(RGB_BLUE);
	}
	else
	{
		blueCurrent--;
		RGBWriteBlueCurrent(blueCurrent);
	}
}
// Initialise lamp.
void lampInit(void)
{
	TCCR1A |= (_BV(COM1B1) | _BV(WGM11));	// Clear OC1B@Match/Set OC1B@Top; Waveform Mode #14: Fast PWM
	TCCR1B |= (_BV(WGM13) | _BV(WGM12) | _BV(CS10));	// Waveform Mode #14; No prescaler
	ICR1 = 399;	// Set TOP at 400 for a 20kHz PWM wave
	OCR1B = 0;	// Initially set OFF
	DDRB |= _BV(DDRB6);	// Output: PB5
	PORTB &= ~_BV(PORTB6);	// Ensuring output is off.
	/*
		Note: Further steps could be taken, such as turning off the timer completely. This would be useful if power consumption was a concern.
		However, simply setting TOP to zero, resetting the counter & setting output to zero should be good enough.
		Therefore, a peripheralEnable() function is unneccessary.
	*/
}
// Disable lamp.
void lampDisable(void)
{
	OCR1B = 0;	// No PWM.
	PORTB &= ~_BV(PORTB6);	// Ensuring output is off.
}
// Set lamp brightness. Input: 0-100 [%]
void lampSetBrightness(char brightness)
{
	OCR1B = (399/100)*brightness;	// PWM % based on TOP value of 399.
}
// Increase lamp brightness.
void increaseLampBrightness(void)
{
	if(lampBrightness < 100)	// 100% PWM limit.
	{
		lampBrightness++;
		lampSetBrightness(lampBrightness);
	}
}
// Decrease lamp brightness.
void decreaseLampBrightness(void)
{
	if(lampBrightness <= 1)	// Disable lamp.
	{
		lampBrightness = 0;
		lampDisable();
	}
	else
	{
		lampBrightness--;
		lampSetBrightness(lampBrightness);
	}
}
// Initialise motor.
void motorInit(void)
{
	TCCR1A |= (_BV(COM1A1) | _BV(WGM11));	// Clear OC1A@Match/Set OC1A@Top; Waveform Mode #14: Fast PWM
	TCCR1B |= (_BV(WGM13) | _BV(WGM12) | _BV(CS10));	// Waveform Mode #14; No prescaler
	ICR1 = 399;	// Set TOP at 400 for a 20kHz PWM wave
	OCR1A = 0;	// Initially set OFF
	DDRB |= _BV(DDRB5);	// Output: PB5
	PORTB &= ~_BV(PORTB5);	// Ensuring output is off.
}
// Disable motor.
void motorDisable(void)
{
	OCR1A = 0;	// No PWM.
	PORTB &= ~_BV(PORTB5);	// Ensuring output is off.
}
// Set motor speed. Input: 0-100 [%]
void motorSetSpeed(char speed)
{
	OCR1A = (399/100)*speed;	// PWM % based on TOP value of 399.
}
// Increase motor speed.
void increaseMotorSpeed(void)
{
	if(motorSpeed < 100)	// 100% PWM limit.
	{
		motorSpeed++;
		motorSetSpeed(motorSpeed);
	}
}
// Decrease motor speed.
void decreaseMotorSpeed(void)
{
	if(motorSpeed <= 1)	// Disable motor.
	{
		motorSpeed = 0;
		motorDisable();
	}
	else
	{
		motorSpeed--;
		motorSetSpeed(motorSpeed);
	}
}
// Initialise speaker.
void speakerInit(void)
{
	TCCR2A |= (_BV(COM2A0) | _BV(WGM21));	// Toggle OC2A on Compare Match; Mode #2: CTC
	TCCR2B |= (_BV(CS21) | _BV(CS20));	// Prescaler of 32
	OCR2A = 0;	// Initially set OFF
	DDRB |= _BV(DDRB4);	// Output: PB4
	PORTB &= ~_BV(PORTB4);	// Ensuring output is off. A tad excessive.
}
// Disable speaker.
void speakerDisable(void)
{
	OCR2A = 0;	// TOP: 0.
	TCNT2 = 0;	// Reset counter.
	PORTB &= ~_BV(PORTB4);	// Ensuring output is off.
}
// Set speaker frequency. Input 0-25 [kHz]
void speakerSetFrequency(char frequency)
{
	OCR2A = ((125/frequency)-1);	// Simplified formula of OCR2A = (f_CPU/(2*N*f_OC2A))-1.
}
// Increase speaker frequency.
void increaseSpeakerFrequency(void)
{
	if(speakerFrequency < 25)	// 25kHz limit.
	{
		speakerFrequency++;
		speakerSetFrequency(speakerFrequency);
	}
}
// Decrease speaker frequency.
void decreaseSpeakerFrequency(void)
{
	if(speakerFrequency <= 1)	// Disable speaker.
	{
		speakerFrequency = 0;
		speakerDisable();
	}
	else
	{
		speakerFrequency--;
		speakerSetFrequency(speakerFrequency);
	}
}
