/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/

#include <project.h>
#include <stdio.h>
#include "Systick.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "I2C_made.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "IR.h"
#include "Ambient.h"
#include "Beep.h"
#include <time.h>
#include <sys/time.h>
#include "music.h"
int rread(void);
float conv(float);
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 1
//battery level//
    
float conv(float input){
    float output = (input - 3500) / (24500 - 3500);
    return output;
}

float perCheck(float input) {
    if (input > 100){
        return 100;
    }
    else{ 
    return input;
    }
}

int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    
    ADC_Battery_Start(); //Start with Battery check 

    int16 adcresult =0;
    float volts = 0.0;
    
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    ADC_Battery_StartConvert();
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for get ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            volts = (float) adcresult / 4095 * 5 * (3/2);
            
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
            

            //playmusic(notation, 300);
            
            /*
            if(volts < 4){          //Battery Check
                        Notes(600, 164.81); 
                        Notes(300, 196.00);
                        Notes(600, 164.81); 
                        Notes(300, 196.00);
                        Notes(600, 164.81);
            }
            */      
            

        }
    
        
    CyDelay(3000);
    /*playNote(200, 196.00);
    playNote(200, 164.81);*/
    
        PWM_Start();
    
        //First Straight
        MotorDirLeft_Write(0);      // set LeftMotor forward mode
        MotorDirRight_Write(0);     // set RightMotor forward mode
    
    
    
    for(;;) 
    {
        struct sensors_ ref;

        Systick_Start();
        reflectance_start();
        

        // read raw sensor values
        reflectance_read(&ref);
        //printf("L2 %5d L1 %5d R1 %5d R2 %5d\r\n", ref.l2, ref.l1, ref.r1, ref.r2);       // print out each period of reflectance sensors
        
        float speed = 180;
        float leftSlow = 0; //will be in percentages  
        float rightSlow = 0;
        float L2 = 1;           
        float L1 = 1;
        float R1 = 1;
        float R2 = 0;
        float R3 = 0;
        float L3 = 0;
        
        L1 = ref.l1;  //practical sensor data ranges from 3500 to 24500 
        R1 = ref.r1;  //Output values will be 0 to 1 
        
        L2 = ref.l2;
        R2 = ref.r2;
        
        L3 = ref.l3;
        R3 = ref.r3;
        
        leftSlow = (conv(L1)*12 + conv(L2)*60 + conv(L3)*90);
        rightSlow = (conv(R1)*12 + conv(R2)*60 + conv(R3)*90);
        
        playNote(300, 196.00);
        
        leftSlow = perCheck(leftSlow);
        rightSlow = perCheck(rightSlow);
        
        printf("LS %.5f RS %.5f\n", leftSlow, rightSlow);
        printf("L2 %.5f L1 %.5f R1 %.5f R2 %.5f\n", L2, L1, R1, R2);
        
        PWM_WriteCompare1(speed * (1 - (leftSlow/100))); //left
        PWM_WriteCompare2(speed * (1 - (rightSlow/100))); //right
    
        float temp = ref.l2 - 3500;
        
        printf("%f\n",temp);
        
    } 
}   

#endif

#if 0
// button
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    for(;;)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            ShieldLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) CyDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif


#if 0
//ultrasonic sensor//
int main()
{
    /*
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    Ultra_Start();                          // Ultra Sonic Start function
    while(1) {
        int d = Ultra_GetDistance();
        //If you want to print out the value  
        printf("distance = %d\r\n", d);
        CyDelay(200);    
    }
    */
    CyGlobalIntEnable; 
    UART_1_Start();
    Systick_Start();
    
    printf("testtest\n");
    
    for(;;){
        float tempArray[50];
        char musicSheet[] = "C4, D4, E4, F4, G4, A4, B4, C5";
        playSong(musicSheet, tempArray);
        printf("%f, %f, %f, testtestTESTTEST\n", tempArray[0],tempArray[1],tempArray[2]);
        playNote(300, 196.00);
        playNote(300, tempArray[0]);
        setTempo(300, tempArray);
        
    CyDelay(2000);
    }
    printf("TestTwo");
}   
#endif


#if 0
//IR receiver//
int main()
{
    CyGlobalIntEnable; 
    UART_1_Start();
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    IR_wait(); // wait for IR command
    printf("IR command received\n");
    
    // print received IR pulses and their lengths
    for(;;)
    {
        if(IR_get(&IR_val)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
            //printf("%d %lu\r\n",IR_val & IR_SIGNAL_HIGH ? 1 : 0, (unsigned long) (IR_val & IR_SIGNAL_MASK));
        }
    }    
 }   
#endif


#if 0
//reflectance//
int main()
{
    struct sensors_ ref;
    struct sensors_ dig;

    Systick_Start();

    CyGlobalIntEnable; 
    UART_1_Start();
  
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    

    for(;;)
    {
        // read raw sensor values
        reflectance_read(&ref);
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       // print out each period of reflectance sensors
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig);      //print out 0 or 1 according to results of reflectance period
        printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);        //print out 0 or 1 according to results of reflectance period
        
        CyDelay(200);
    }
}   
#endif


#if 0
//motor//
int main() //TEST TRACK
{
    CyGlobalIntEnable; 
    UART_1_Start();

    float motorDiff = -7;
    float speed = 100;
    float curveConst = -50;
    
    CyDelay(3700);
    
    Notes(300, 164.81);
    Notes(600, 196.00);
    
    CyDelay(500);
    
    PWM_Start();
    
    //First Straight
    MotorDirLeft_Write(0);      // set LeftMotor forward mode
    MotorDirRight_Write(0);     // set RightMotor forward mode
    
    PWM_WriteCompare1(speed); //left
    PWM_WriteCompare2(speed + motorDiff); //right
    CyDelay(3600);
    
    //First Turn (Right)
    PWM_WriteCompare1(speed);  //turning right
    PWM_WriteCompare2(0);
    CyDelay(1470);
    
    //Second Straight
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed + motorDiff);
    CyDelay(2900);
    
    //Second Turn
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(0);
    CyDelay(1570);
    
    //Third Straight
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed + motorDiff);
    CyDelay(3000);
    
    //Third Turn
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(0);
    CyDelay(1900);
    
    //Fourth Curve
    PWM_WriteCompare1(speed); 
    PWM_WriteCompare2(speed + motorDiff + curveConst);
    CyDelay(5500);
    
    PWM_Stop();
    
    Notes(300, 196.00);
    Notes(600, 164.81);
    
    for(;;)
    {

    }
}
#endif


/* [] END OF FILE */
