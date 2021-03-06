#ifndef HARDWARE_MANAGEMENT_H
#define HARDWARE_MANAGEMENT_H

/*
 * L298N Driver
 *  You can drive a 2-fase stepper motor, a 4-fase stepper motor or 2 DC motors.
 *  Double H bridge drive Chip: L298N (ST NEW)
 *  Driver voltage: 5V - 35V
 *  Driver current: 2A (MAX single bridge)
 *  Max power: 25W
 *  
 * Encoder Library is found here:
 * https://github.com/PaulStoffregen/Encoder
 * 
 * Usage:
 * Set your hardware configuration below 
 *  
 */



#include <Stepper.h> // for using stepper motors
#include <Encoder.h> // for using rotary encoders

class Hardware_mgmt {
  public:
    Hardware_mgmt();  // constructor
    ~Hardware_mgmt(); // destructor

    void setup();     // setup hardware
    void process();   // check sensors and set motors one time

  private:
  //////////////////////////////////////////////////////////////
  //  USE THIS SECTION TO SELECT YOUR HARDWARE CONFIGURATION  //
  //////////////////////////////////////////////////////////////
  
  // sensors:
    // azimuth potentiometer sensor
    bool   azi_pot_enable =        true;             // enable or disable feature
    int    azi_potentiometer_pin = A0;               // connect the middle pin 
    double azi_pot_min_degree    = 0;                // minimum angle of potentiometer
    int    azi_pot_min_value     = 0;                // minimum analogue reading of potentiometer
    double azi_pot_max_degree    = 340;              // maximum angle of potentiometer
    int    azi_pot_max_value     = 1024;             // maximum analogue reading of potentiometer, 1024 = 3.3V
    
    // elevation potentiometer sensor
    bool   ele_pot_enable =        true;             // enable or disable feature
    // int    ele_potentiometer_pin = A1;               // connect the middle pin 
    double ele_pot_min_degree    = 0;                // minimum angle of potentiometer
    int    ele_pot_min_value     = 0;                // minimum analogue reading of potentiometer
    double ele_pot_max_degree    = 340;              // maximum angle of potentiometer
    int    ele_pot_max_value     = 1024;             // maximum analogue reading of potentiometer, 1024 = 3.3V
        
    // azimuth rotary sensor
    bool   azi_rotary_enable =     false;             // enable or disable feature
    int    azi_rotary_pins[2] =    {5, 6};           // set pins here
    int    azi_rotary_pulses =     3600;             // set nr of pulses per full azi rotation (360 degree)

    // elevation rotary sensor
    bool   ele_rotary_enable =     false;             // enable or disable feature
    int    ele_rotary_pins[2] =    {7, 8};           // set pins here
    int    ele_rotary_pulses =     3600;             // set nr of pulses per full ele rotation (360 degree)

  // actuators:
    // azimuth stepper motor
    bool   azi_stepper_enable =    false;             // enable or disable feature
    int    azi_steps_rev =         200;              // set the number of steps per revolution
    int    azi_stepper_pins[4] =   {9, 10, 11, 12};   // set pins here

    // elevation stepper motor
    bool   ele_stepper_enable =    false;              // enable or disable feature
    int    ele_steps_rev =         200;               // number of steps per revolution
    int    ele_stepper_pins[4] =   {13, 14, 15, 16};  // set pins here

    // azimuth DC motor
    bool   azi_dc_enable =         true;              // enable or disable feature
    int    azi_dc_pin =            17;                // pin to drive motor, PWM for speed control
    bool   azi_dc_pwm =            true;              // true for pwm driver, false if only on/off
    int    azi_dc_bridge_pins[2] = {18, 19};          // set pins here

    // elevation DC motor
    bool   ele_dc_enable =         true;              // enable or disable feature
    int    ele_dc_pin =            20;                // pin to drive motor, PWM for speed control
    bool   ele_dc_pwm =            true;              // true for pwm driver, false if only on/off
    int    ele_dc_bridge_pins[2] = {21, 22};          // set pins here

  //////////////////////////////////////////////////////////////
  //  //  //  //  //  END OF HARDWARE CONFIGURATION   //  //  //
  ////////////////////////////////////////////////////////////// 

    Encoder *azi_encoder = nullptr;
    Encoder *ele_encoder = nullptr;

    Stepper *azi_stepper = nullptr;
    Stepper *ele_stepper = nullptr;
    
    // rotating speed setting is equal for azi and ele
    int    azi_current_speed; // percentage of the rotating speed
    int    ele_current_speed; // percentage of the rotating speed
    
    int32_t azi_current_enc; // current reading of azimuth encoder
    int32_t ele_current_enc; // current reading of elevation encoder

    int azi_pot_value;       // current reading of azimuth potentiometer
    int ele_pot_value;       // current reading of elevation potentiometer
};

Hardware_mgmt::Hardware_mgmt() {
    azi_encoder = nullptr;
    ele_encoder = nullptr;
    azi_stepper = nullptr;
    ele_stepper = nullptr;
}

Hardware_mgmt::~Hardware_mgmt() {
  delete azi_encoder;
  delete ele_encoder;
  delete azi_stepper;
  delete ele_stepper;
}

void Hardware_mgmt::setup() {
  // remove old allocations
  delete azi_encoder;
  delete ele_encoder;
  delete azi_stepper;
  delete ele_stepper;

  // enable new objects
  if(azi_rotary_enable) {
    azi_encoder = new Encoder(azi_rotary_pins[0], azi_rotary_pins[1]);
  }
  if(ele_rotary_enable) {
    ele_encoder = new Encoder(ele_rotary_pins[0], ele_rotary_pins[1]);
  }
  
  if(azi_stepper_enable) {
    azi_stepper = new Stepper(azi_steps_rev, azi_stepper_pins[0], azi_stepper_pins[1], azi_stepper_pins[2], azi_stepper_pins[3]);
  }
  if(ele_stepper_enable) {
    ele_stepper = new Stepper(ele_steps_rev, ele_stepper_pins[0], ele_stepper_pins[1], ele_stepper_pins[2], ele_stepper_pins[3]);
  }

  // connect sensors
  if(azi_pot_enable) {
    
  }

  // calculate settings
  
  
}

void Hardware_mgmt::process() {
  /*
  azi_current_enc = azi_encoder->read();
  ele_current_enc = ele_encoder->read();
  
  azi_stepper->setSpeed(60);        // set speed to 60 rpm
  azi_stepper->step(azi_steps_rev); // rotate 1 rev. positive is CW, negative is CCW
  */
}

#endif // HARDWARE_MANAGEMENT_H
