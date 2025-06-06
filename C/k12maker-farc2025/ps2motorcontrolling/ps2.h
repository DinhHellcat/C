#include <PS2X_lib.h>

PS2X ps2x; // create PS2 Controller Class object

#define PS2_DAT 12 // MISO  19
#define PS2_CMD 13 // MOSI  23
#define PS2_SEL 15 // SS     5
#define PS2_CLK 14 // SLK   18

bool driving_speed = 0;

void PS2control()
{
    if(ps2x.ButtonPressed(PSB_L2) || ps2x.ButtonPressed(PSB_R2))
    {
        Serial.println("Changing speed");
        driving_speed = !driving_speed;
        speed = driving_speed ? max_speed : half_speed;
    }
    //Serial.println(speed);

    if(ps2x.Button(PSB_L1))
    {
        if(ps2x.Analog(PSS_LY) == 0)
        {
            if(ps2x.Analog(PSS_LX)>150)
                forward_right();
            else if(ps2x.Analog(PSS_LX)>50)
                forward();
            else
                forward_left();
        }
        else if(ps2x.Analog(PSS_LY) > 150)
        {
            if(ps2x.Analog(PSS_LX)>150)
                backward_right();
            else if(ps2x.Analog(PSS_LX)>50)
                backward();
            else
                backward_left();
        }
        else
        {
            if(ps2x.Analog(PSS_LX)>150)
                right();
            else if(ps2x.Analog(PSS_LX)>50)
                stop();
            else
                left();
        }
    }
    else if(ps2x.Button(PSB_R1))
    {
        if(ps2x.Analog(PSS_RY) == 0)
        {
            if(ps2x.Analog(PSS_RX)>150)
                forward_right();
            else if(ps2x.Analog(PSS_RX)>50)
                forward();
            else
                forward_left();
        }
        else if(ps2x.Analog(PSS_RY) > 150)
        {
            if(ps2x.Analog(PSS_RX)>150)
                backward_right();
            else if(ps2x.Analog(PSS_RX)>50)
                backward();
            else
                backward_left();
        }
        else
        {
            if(ps2x.Analog(PSS_RX)>150)
                right();
            else if(ps2x.Analog(PSS_RX)>50)
                stop();
            else
                left();
        }
    }
    else
        stop();
    
    delay(50);
}
// #define BEBUG_CTRL

// calibration for different kinds of PS2 controller, this value only suitable for the PS2 controller comes with VRC2023 K12 Maker kit 
// #define X_JOY_CALIB 127
// #define Y_JOY_CALIB 128

// #define TOP_SPEED 4095
// #define NORM_SPEED 2048
// #define TURNING_FACTOR 1

// #define SINGLE_HAND_DRIVING 0
// #define TWO_HAND_DRIVING 1
// bool driving_mode = SINGLE_HAND_DRIVING;

// bool PS2control()
// {
//   int speed = NORM_SPEED;
//   if (ps2x.Button(PSB_R2))
//     speed = TOP_SPEED;
//    if (ps2x.ButtonPressed(PSB_SELECT))
//     driving_mode =! driving_mode;
  

//   int nJoyX = X_JOY_CALIB - ps2x.Analog(PSS_RX); // read x-joystick
//   int nJoyY = Y_JOY_CALIB - (driving_mode ? ps2x.Analog(PSS_LY) :ps2x.Analog(PSS_RY)); // read y-joystick
//   int nMotMixL;                          // Motor (left) mixed output
//   int nMotMixR;                          // Motor (right) mixed output

//   if(nJoyX == -1 && nJoyY == 0) // in case of lost connection with the wireless controller, only used in VRC2023 PS2 wireless controller 
//   {
//     setPWMMotors(0, 0, 0, 0);
//     return 0;
//   }

//   bool temp = (nJoyY * nJoyX > 0);
//   if (nJoyX) // Turning
//   {
//     nMotMixL = -nJoyX + (nJoyY * temp);
//     nMotMixR = nJoyX + (nJoyY * !temp);
//   }
//   else // Forward or Reverse
//   {
//     nMotMixL = nJoyY;
//     nMotMixR = nJoyY;
//   }
//   #ifdef BEBUG_CTRL
//   Serial.print(F("Calculated value from joystick: "));
//   Serial.print(nMotMixL);
//   Serial.print("\t");
//   Serial.println(nMotMixR);
//   #endif
//   int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

//   if (nMotMixR > 0)
//   {
//     c3 = nMotMixR;
//     c3 = map(c3, 0, 128, 0, speed);
//   }

//   else if (nMotMixR < 0)
//   {
//     c4 = abs(nMotMixR) + 1;
//     c4 = map(c4, 0, 128, 0, speed);
//   }

//   if (nMotMixL > 0)
//   {
//     c1 = nMotMixL;
//     c1 = map(c1, 0, 128, 0, speed);
//   }
//   else if (nMotMixL < 0)
//   {
//     c2 = abs(nMotMixL);
//     c2 = map(c2, 0, 128, 0, speed);
//   }
//   setPWMMotors(c1, c2, c3, c4);
//   return 1;
// }