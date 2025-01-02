/*
 * ModuleTest.c
 *
 *	Provides the testing functions all of individual peripheral testing
 *	and full system testing
 *
 * Created on: September 3rd, 2023
 *		Author: Jackie Huynh
 *
 */
 
#include "ModuleTest.h"
#include "TCS34727.h"
#include "MPU6050.h"
#include "UART0.h"
#include "Servo.h"
#include "LCD.h"
#include "I2C.h"
#include "util.h"
#include "ButtonLED.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static char printBuf[100];
static char angleBuf[LCD_ROW_SIZE];
static char colorBuf[LCD_ROW_SIZE];
static char colorString[6];

char buffer[100];
int intValue = 10;
float floatValue = 3.14159;

/* RGB Color Struct Instance */
RGB_COLOR_HANDLE_t RGB_COLOR;
	
/* MPU6050 Struct Instance */
MPU6050_ACCEL_t Accel_Instance;
MPU6050_GYRO_t 	Gyro_Instance;
MPU6050_ANGLE_t Angle_Instance;

static void Test_Delay(void){
	LEDs=RED;
	DELAY_1MS(500);
	LEDs = DARK;
	DELAY_1MS(500);
}

static void Test_UART(void){
	/*CODE_FILL*/						
	// 1. Construct a string with letters, decimal numbers and floats using sprintf
	// 2. Send the string to PC serial terminal for display	
	// 3. Delay for 1s using ms delay function
	//UART0_OutString("Welcome to Bluetooth Controlled LED and DC Motor App");
 
   /* Format and print various data */
  sprintf(buffer, "Integer: %d, Float: %.2f", intValue, floatValue);
	UART0_OutString(buffer);
	UART0_OutCRLF();
	DELAY_1MS(1000);

}

static void Test_I2C(void){
	/*CODE_FILL*/						
	/* Check if RGB Color Sensor has been detected and display the ret value on PC serial terminal. */
	uint8_t ret = I2C0_Receive(TCS34727_ADDR, TCS34727_CMD|TCS34727_ID_R_ADDR);
    char buffer[20];  // Buffer to hold the formatted string
 // Format the value of ret as a string
    sprintf(buffer, "Received: %x\n", ret);
    
    // Print the formatted string to the serial terminal
    UART0_OutString(buffer);
    UART0_OutCRLF();
		DELAY_1MS(10);
}

static void Test_MPU6050(void){
	/* Accelerometer and Gyroscope Data Instances */
	MPU6050_ACCEL_t Accel_Instance;
	MPU6050_GYRO_t Gyro_Instance;
	MPU6050_ANGLE_t Angle_Instance;
	
	/* Grab Accelerometer and Gyroscope Raw Data*/
	MPU6050_Get_Accel(&Accel_Instance);
	MPU6050_Get_Gyro(&Gyro_Instance);
		
	/* Process Raw Accelerometer and Gyroscope Data */
	MPU6050_Process_Accel(&Accel_Instance);
	MPU6050_Process_Gyro(&Gyro_Instance);
		
	/* Calculate Tilt Angle */
	MPU6050_Get_Angle(&Accel_Instance, &Gyro_Instance, &Angle_Instance);
		
	/* Format buffer to print data and angle */
	char buffer[100];
  sprintf(buffer, "Accel: X=%.2f Y=%.2f Z=%.2f, Gyro: X=%.2f Y=%.2f Z=%.2f, Angle: X=%.2f Y=%.2f Z=%.2f",
                Accel_Instance.Ax, Accel_Instance.Ay, Accel_Instance.Az,
                Gyro_Instance.Gx, Gyro_Instance.Gy, Gyro_Instance.Gz,
                Angle_Instance.ArX, Angle_Instance.ArY, Angle_Instance.ArZ);

	
	/* Print to UART or any other output */
	UART0_OutString(buffer);
	UART0_OutCRLF();
	
	DELAY_1MS(500);
}


static void Test_TCS34727(void){
	/* Grab Raw Color Data From Sensor */
	uint16_t B_Raw;
	uint16_t G_Raw;
	uint16_t R_Raw;
	uint16_t C_Raw;
	
	B_Raw = TCS34727_GET_RAW_BLUE();
	G_Raw = TCS34727_GET_RAW_GREEN();
	R_Raw = TCS34727_GET_RAW_RED();
  C_Raw = TCS34727_GET_RAW_CLEAR();
	
	/* Create RGB_COLOR_HANDLE_t instance */
  RGB_COLOR_HANDLE_t RGB_COLOR_Instance;
  RGB_COLOR_Instance.B_RAW = B_Raw;
  RGB_COLOR_Instance.G_RAW = G_Raw;
  RGB_COLOR_Instance.R_RAW = R_Raw;
  RGB_COLOR_Instance.C_RAW = C_Raw;

  /* Process Raw Color Data to RGB Value */
  TCS34727_GET_RGB(&RGB_COLOR_Instance);
		
		
	/* Change Onboard RGB LED Color to Detected Color */
	switch(Detect_Color(&RGB_COLOR_Instance)){
		case RED_DETECT:
			LEDs = RED;
			break;
		case GREEN_DETECT:
			LEDs = GREEN;
			break;
		case BLUE_DETECT:
			LEDs = BLUE;
			break;
		case NOTHING_DETECT:
			LEDs = DARK;
			break;
		}
	

		
    /* Format String to Print */
char buffer[100]; // Assuming a sufficiently large buffer
float B_Float = (float)RGB_COLOR_Instance.B;
float G_Float = (float)RGB_COLOR_Instance.G;
float R_Float = (float)RGB_COLOR_Instance.R;
		
sprintf(buffer, "R: %.2f, G: %.2f, B: %.2f\n",
        R_Float, G_Float, B_Float);
//sprintf(buffer, "R: %u, G: %u, B: %u\n",
//        RGB_COLOR_Instance.B_RAW,
//		    RGB_COLOR_Instance.G_RAW,
//        RGB_COLOR_Instance.R_RAW);
//       // RGB_COLOR_Instance.C_RAW);

		
	/* Print String to Terminal through USB */
	UART0_OutString(buffer);
	UART0_OutCRLF();
	DELAY_1MS(1000);
}

static void Test_Servo(void){
	/*
	 * In this test, follow the series of steps below (each step requires a 1s delay after)
	 * 1. Drive Servo to 0 degree
	 * 2. Drive Servo to -45 degree
	 * 3. Drive Servo to 0 degree
	 * 4. Drive Servo to 45 degree
	 * 5. Drive Servo to 0 degree
	 * 6. Drive Servo to -90 degree
	 * 7. Drive Servo to 0 degree
	 * 8. Drive Servo to 90 degree
	 */ 
	
	
	Drive_Servo(0);       // Step 1
	DELAY_1MS(1000); // Wait 1 second
	
	Drive_Servo(-45);     // Step 2
	DELAY_1MS(1000); // Wait 1 second
	
	Drive_Servo(0);       // Step 3
	DELAY_1MS(1000); // Wait 1 second
	
	Drive_Servo(45);      // Step 4
	DELAY_1MS(1000); // Wait 1 second
	
	Drive_Servo(0);       // Step 5
	DELAY_1MS(1000); // Wait 1 second
	
	Drive_Servo(-90);     // Step 6
	DELAY_1MS(1000); // Wait 1 second
	
	Drive_Servo(0);       // Step 7
	DELAY_1MS(1000); // Wait 1 second
	
	Drive_Servo(90);      // Step 8
	DELAY_1MS(1000);; // Wait 1 second
}

static void Test_LCD(void){

	/* Print Name to LCD at Center Location */
	/*CODE_FILL*/	
  LCD_Clear();  // Clear the LCD before printing anything new
  DELAY_1MS(1000); // Short delay after clearing the LCD

  // Centering "Ever" on a 16 character wide LCD
  LCD_Set_Cursor(ROW1, 6);  // Set cursor to the first row, sixth column
  LCD_Print_Str((uint8_t*)"Rod");
	DELAY_1MS(1000);
  LCD_Set_Cursor(ROW2, 5);

  // Print "Ever" to the LCD
  LCD_Print_Str((uint8_t*)"Agatep");
  DELAY_1MS(1000); // Safety delay after writing to the LCD
}

static void Test_Full_System(void){
	uint16_t B_Raw;
	uint16_t G_Raw;
	uint16_t R_Raw;
	uint16_t C_Raw;
	
	/* Accelerometer and Gyroscope Data Instances */
	MPU6050_ACCEL_t Accel_Instance;
	MPU6050_GYRO_t Gyro_Instance;
	MPU6050_ANGLE_t Angle_Instance;
	
	/* Grab Accelerometer and Gyroscope Raw Data*/
	MPU6050_Get_Accel(&Accel_Instance);
	MPU6050_Get_Gyro(&Gyro_Instance);
		
	/* Process Raw Accelerometer and Gyroscope Data */
	MPU6050_Process_Accel(&Accel_Instance);
	MPU6050_Process_Gyro(&Gyro_Instance);
		
	/* Calculate Tilt Angle */
	MPU6050_Get_Angle(&Accel_Instance, &Gyro_Instance, &Angle_Instance);
	
	/* Drive Servo Accordingly to Tilt Angle on X-Axis*/
	Drive_Servo(Angle_Instance.ArX);
		
	/* Format buffer to print data and angle */
	char buffer2[100];
  sprintf(buffer2, "Gyro: X=%.2f, Angle: X=%.2f",
                Gyro_Instance.Gx,
                Angle_Instance.ArX);

	
	/* Print to UART or any other output */
	UART0_OutString(buffer2);
	UART0_OutCRLF();
		
	/* Grab Raw Color Data From Sensor */
	
	B_Raw = TCS34727_GET_RAW_BLUE();
	G_Raw = TCS34727_GET_RAW_GREEN();
	R_Raw = TCS34727_GET_RAW_RED();
  C_Raw = TCS34727_GET_RAW_CLEAR();
	
	/* Create RGB_COLOR_HANDLE_t instance */
  RGB_COLOR_HANDLE_t RGB_COLOR_Instance;
  RGB_COLOR_Instance.B_RAW = B_Raw;
  RGB_COLOR_Instance.G_RAW = G_Raw;
  RGB_COLOR_Instance.R_RAW = R_Raw;
  RGB_COLOR_Instance.C_RAW = C_Raw;

  /* Process Raw Color Data to RGB Value */
  TCS34727_GET_RGB(&RGB_COLOR_Instance);
		
	/* Change Onboard RGB LED Color to Detected Color */
	switch(Detect_Color(&RGB_COLOR_Instance)){
		case RED_DETECT:
			LEDs = RED;
			strcpy(colorString, "RED");
			break;
		case GREEN_DETECT:
			LEDs = GREEN;
			strcpy(colorString, "GREEN");
			break;
		case BLUE_DETECT:
			LEDs = BLUE;
			strcpy(colorString, "BLUE");
			break;
		case NOTHING_DETECT:
			LEDs = DARK;
			strcpy(colorString, "NA");
			break;
	}
		
	/* Format String to Print RGB value*/
char buffer[100]; // Assuming a sufficiently large buffer
float B_Float = (float)RGB_COLOR_Instance.B;
float G_Float = (float)RGB_COLOR_Instance.G;
float R_Float = (float)RGB_COLOR_Instance.R;
		
sprintf(buffer, "R: %.2f, G: %.2f, B: %.2f\n",
        R_Float, G_Float, B_Float);
	
	/* Print String to Terminal through USB */
	UART0_OutString(buffer);
	UART0_OutCRLF();
	//DELAY_1MS(1000);
	
	DELAY_1MS(200);
		
	/* Update LCD With Current Angle and Color Detected */
	
//	sprintf(angleBuf, "Angle:%0.2f\0", Angle_Instance.ArX);				//Format String to print angle to 2 Decimal Place
//	sprintf(colorBuf, "Color:%s\0", colorString);									//Format String to print color detected

	sprintf(angleBuf, "Angle:%0.2f", Angle_Instance.ArX);				//Format String to print angle to 2 Decimal Place
	sprintf(colorBuf, "Color:%s", colorString);									//Format String to print color detected
	
	/*CODE_FILL*/						//Clear LCD
  LCD_Clear();
	/*CODE_FILL*/						//Safety Delay of 2ms
	DELAY_1MS(2);
	/*CODE_FILL*/						//Set Cursor to Row 1 Column 0
	  LCD_Set_Cursor(ROW1, 0);  // Set cursor to the first row, sixth column
	/*CODE_FILL*/						//Print angleBuf String on LCD
	LCD_Print_Str((uint8_t*)angleBuf);
	/*CODE_FILL*/						//Safety Delay of 2ms
	DELAY_1MS(2);
	/*CODE_FILL*/						//Set Cursor to Row 2 Column 1
	LCD_Set_Cursor(ROW2, 1);
	/*CODE_FILL*/						//Print colorBuf String on LCD	
	LCD_Print_Str((uint8_t*)colorBuf);
		
	DELAY_1MS(20);
}

void Module_Test(MODULE_TEST_NAME test){
	
	switch(test){
		case DELAY_TEST:
			Test_Delay();
			break;
		
		case UART_TEST:
			Test_UART();
			break;
		
		case MPU6050_TEST:
			Test_MPU6050();
			break;
		
		case TCS34727_TEST:
			Test_TCS34727();
			break;
		
		case SERVO_TEST:
			Test_Servo();
			break;
		
		case LCD_TEST:
			Test_LCD();
			break;
			
		case FULL_SYSTEM_TEST:
			Test_Full_System();
			break;
		case I2C_TEST:
			Test_I2C();
			break;
		
		default:
			break;
	}
	
}

 