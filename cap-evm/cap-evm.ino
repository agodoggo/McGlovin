//////////////// IMPORTANT PACKAGES TO DOWNLOAD ////////////////
/*
 * To be able to use this code, you need to download the custom library (slight-FDC1004-master) that's in a zip file in the GitHub repository.
 * To do this, click on Sketch at the top, then Include Library, then Add .ZIP Library and select the zip file from the repository.
 * You also need to download the slight_DebugMenu library. Again, to do this, go to Sketch, Include Library, Manage Libraries, then search for "slight_DebugMenu" and install the latest version.
 */


#include <LiquidCrystal.h>
#include <slight_DebugMenu.h>
#include <Wire.h>
#include <slight_FDC1004.h>

// initialize capacitive plate sensor
slight_FDC1004 mySensor;

void init_sensor(Print &out){
  out.println("connecting and setting up sensor!");
  bool ready = mySensor.begin();
  if (!ready){
    out.println("not yet ready!");
  }
  out.println("found and ready!");

  mySensor.update_interval_set(500);
  mySensor.sensor_event_set_callback(sensor_event);

  // start things
  mySensor_config(out);
  out.println("finished setting up!");
  out.println("-----------------------------------------------------------");
}

void sensor_event(slight_FDC1004 *instance){
  Print &out = Serial;
}


void mySensor_config(Print &out){
  out.println("set config: ");
  
  /////////// MEASUREMENT CONFIGURATIONS ///////////
  //// MESA 1 ////
  out.println("MESA_1: (chA = CIN1; chB = DISABLED;)");
  mySensor.measurement_config_chA_set(slight_FDC1004::MESA_1, slight_FDC1004::config_chA_CIN1);
  mySensor.measurement_config_chB_set(slight_FDC1004::MESA_1, slight_FDC1004::config_chB_DISABLED);
  mySensor.measurement_config_CAPDAC_set(slight_FDC1004::MESA_1,0);  
  mySensor.measurement_config_write(slight_FDC1004::MESA_1);

  //// MESA 2 ////
  out.println("MESA_2: (chA = CIN2; chB = DISABLED;)");
  mySensor.measurement_config_chA_set(slight_FDC1004::MESA_2, slight_FDC1004::config_chA_CIN2);
  mySensor.measurement_config_chB_set(slight_FDC1004::MESA_2, slight_FDC1004::config_chB_DISABLED);
  mySensor.measurement_config_CAPDAC_set(slight_FDC1004::MESA_2,0);
  mySensor.measurement_config_write(slight_FDC1004::MESA_2);

  //// MESA 3 ////
  out.println("MESA_3: (chA = CIN3; chB = DISABLED;)");
  mySensor.measurement_config_chA_set(slight_FDC1004::MESA_3, slight_FDC1004::config_chA_CIN3);
  mySensor.measurement_config_chB_set(slight_FDC1004::MESA_3, slight_FDC1004::config_chB_DISABLED);
  mySensor.measurement_config_CAPDAC_set(slight_FDC1004::MESA_3,0);
  mySensor.measurement_config_write(slight_FDC1004::MESA_3); 

  //// MESA 4 ////
  out.println("MESA_4: (chA = CIN3; chB = DISABLED;)");
  mySensor.measurement_config_chA_set(slight_FDC1004::MESA_4, slight_FDC1004::config_chA_CIN4);
  mySensor.measurement_config_chB_set(slight_FDC1004::MESA_4, slight_FDC1004::config_chB_DISABLED);
  mySensor.measurement_config_CAPDAC_set(slight_FDC1004::MESA_4,0);
  mySensor.measurement_config_write(slight_FDC1004::MESA_4);    

  out.println("written configurations to device");

  //// FDC CONFIG ////
  out.println("fdc config: (rate400S/s; repeat=true;");
  // set speed to 400S/s
  mySensor.measurement_rate_set(slight_FDC1004::repeate_rate_400Ss);
  // enable auto repeate
  mySensor.measurement_repeate_set(true);
  // enable measurement 1
  out.print("MESA_1");
  mySensor.measurement_init_set(slight_FDC1004::MESA_1,true);
  //enable measurement 2
  out.print(", MESA_2");
  mySensor.measurement_init_set(slight_FDC1004::MESA_2,true);
  // enable measurement 3
  out.print(", MESA_3");
  mySensor.measurement_init_set(slight_FDC1004::MESA_3,true);
  //enable measurement 4
  out.print(", MESA_4");
  mySensor.measurement_init_set(slight_FDC1004::MESA_4,true);  
  out.println(" init=enabled;");

  // WRITE CONFIG TO DEVICE //
  mySensor.fdc_config_write();
  out.println("write config to device.");  
}
void calc_capacitance(Print &out){
  // CALCULATING CAPACITANCES
  out.println(" ------------------------------- CALCULATING CAPACITANCES ------------------------------- ");

  uint16_t msb_regVal = 0;
  uint16_t lsb_regVal = 0;
  uint32_t unmasked_regVal = 0;
  uint32_t masked_regVal = 0;
  uint32_t mask = 0b11111111111111111111111100000000;
  long bit_shifts = 0;
  float cap_val = 0;
  
  // measurement 1
  msb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS1_MSB);
  lsb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS1_LSB);
  unmasked_regVal = ((uint32_t) msb_regVal << 16) | lsb_regVal;
  masked_regVal = unmasked_regVal & mask;;
  bit_shifts = ((long) masked_regVal) >> 8;
  if (bit_shifts < 0){
    bit_shifts = (~bit_shifts) + 1;
  }
  cap_val = ((float) bit_shifts / (float) 524288);
  out.print("MEAS1 CAPACITANCE (pF): ");
  out.print(cap_val);
  out.println(); 
  
  // measurement 2 
  msb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS2_MSB);
  lsb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS2_LSB);
  unmasked_regVal = ((uint32_t) msb_regVal << 16) | lsb_regVal;
  masked_regVal = unmasked_regVal & mask;
  bit_shifts = ((long) masked_regVal) >> 8;
  if (bit_shifts < 0){
    bit_shifts = (~bit_shifts) + 1;
  }
  cap_val = ((float) bit_shifts / (float) 524288);
  out.print("MEAS2 CAPACITANCE (pF): ");
  out.print(cap_val);
  out.println();
  
  // measurement 3
  msb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS3_MSB);
  lsb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS3_LSB);
  unmasked_regVal = ((uint32_t) msb_regVal << 16) | lsb_regVal;
  masked_regVal = unmasked_regVal & mask;
  bit_shifts = ((long) masked_regVal) >> 8;
  if (bit_shifts < 0){
    bit_shifts = (~bit_shifts) + 1;
  }
  cap_val = ((float) bit_shifts / (float) 524288);
  out.print("MEAS3 CAPACITANCE (pF): ");
  out.print(cap_val);
  out.println();
  
  // measurement 4
  msb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS4_MSB);
  lsb_regVal = mySensor.read_register16bit(slight_FDC1004::REG_MEAS4_LSB);
  unmasked_regVal = ((uint32_t) msb_regVal << 16) | lsb_regVal;
  masked_regVal = unmasked_regVal & mask;
  bit_shifts = ((long) masked_regVal) >> 8;
  if (bit_shifts < 0){
    bit_shifts = (~bit_shifts) + 1;
  }
  cap_val = ((float) bit_shifts / (float) 524288);
  out.print("MEAS4 CAPACITANCE (pF): ");
  out.print(cap_val);
  out.println();
}

void setup(void) {
  Serial.begin(9600);
  init_sensor(Serial);
}

void loop() {
  mySensor.update();
  calc_capacitance(Serial);
  // waiting 10 seconds before taking next measurement
  delay(10000);
}
