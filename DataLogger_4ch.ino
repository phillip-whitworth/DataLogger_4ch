#include <SPI.h>
#include <SD.h>

// setup information

int frequency = 10; // in hertz
int start_pin = 0; // This is the trigger pin from the ardupilot telling the camera to start recording....need link to website


int loop_freq = 1000 / frequency; // this is the loop interrupt timer
long current = 0; //initializing the timer for the loop
long next_time = loop_freq; // initializing the place holder for the next time to record data based on frequency


// SD Card Specific variables required for
const int chipSelect = 4;


String g_curr_filename = "datalog.000";
bool is_running = false;
void update_curr_filename() {
  int curr_num = -1;

  do {
    curr_num++;
    String curr_pad_num = "";
    if ( curr_num < 100 ) {
      curr_pad_num = curr_pad_num + '0';
    }
    if ( curr_num < 10 ) {
      curr_pad_num = curr_pad_num + '0';
    }
    curr_pad_num = curr_pad_num + String(curr_num);

    g_curr_filename = "datalog." + curr_pad_num;

  } while ( SD.exists(g_curr_filename) );
}


void setup() {
  Serial.begin(250000);

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  delay(2000);
  Serial.println("waiting for start");
}

void loop() {
//  uncomment when ready for PWM signal control
//
//  int A15SensorValue = analogRead(A15);
//  float A15voltage = A15SensorValue * (5.0 / 1023.0);
//  //Serial.println(A15voltage, 4);
//  if (A15Voltage > 3.0) {
//    start_pin = 1;
//  }


  ///////////////////////////////////////////////////////////////
  // Test Switch Code
  ///////////////////////////////////////////////////////////////
  if (Serial.available() > 0) // this area is used to set the clock time.
  {
    int incomingByte = Serial.read();
    if (incomingByte == 'a')
    {
      start_pin = 1;
    }
  }


  ///////////////////////////////////////////////////////////////////////////////
  while (start_pin) {
    if (is_running == false) {
      update_curr_filename();
      Serial.println(g_curr_filename);
      is_running = true;
    }
    current = millis();
    if (current >= next_time) {
      String dataString = "";
      next_time = current + loop_freq;
      Serial.print(current);
      Serial.print(',');
      int A0SensorValue = analogRead(A0);
      float A0voltage = A0SensorValue * (5.0 / 1023.0);
      Serial.print(A0voltage, 4);
      Serial.print(',');
      int A1SensorValue = analogRead(A1);
      float A1voltage = A1SensorValue * (5.0 / 1023.0);
      Serial.print(A1voltage, 4);
      Serial.print(',');
      int A2SensorValue = analogRead(A2);
      float A2voltage = A2SensorValue * (5.0 / 1023.0);
      Serial.print(A2voltage, 4);
      Serial.print(',');
      int A3SensorValue = analogRead(A3);
      float A3voltage = A3SensorValue * (5.0 / 1023.0);
      Serial.println(A3voltage, 4);

      dataString += String(current);
      dataString += String(',');
      dataString += String(A0voltage, 4);
      dataString += String(',');
      dataString += String(A1voltage, 4);
      dataString += String(',');
      dataString += String(A2voltage, 4);
      dataString += String(',');
      dataString += String(A3voltage, 4);

      File dataFile = SD.open(g_curr_filename, FILE_WRITE);
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
      }
    }

//    uncomment when ready for PWM signal control
//
//    int A15SensorValue = analogRead(A15);
//    float A15voltage = A15SensorValue * (5.0 / 1023.0);
//    //Serial.println(A15voltage, 4);
//    if (A15Voltage < 3.0) {
//      start_pin = 0;
//    }



    ///////////////////////////////////////////////////////////////
    // Test Switch Code
    ///////////////////////////////////////////////////////////////
    if (Serial.available() > 0) // this area is used to set the clock time.
    {
      int incomingByte = Serial.read();
      if (incomingByte == 's')
      {
        start_pin = 0;
      }
    }



    ///////////////////////////////////////////////////////////////////////////////
    if ((start_pin == 0) && (is_running == true)) {
      is_running = false;
      Serial.println("not recording");
    }
  }



}
