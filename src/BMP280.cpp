#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <BMP280.h>
#include "GyverFilters.h"

Adafruit_BMP280 bmp;
float bmpData;

GKalman BMP280_Filter(30, 30, 0.3);

float BMP280_Filtered(){
    bmpData = bmp.readPressure() / 10.0F;
    bmpData = BMP280_Filter.filtered(bmpData);
    return bmpData;
}



void Init_BMP280(){
    unsigned status;
    status = bmp.begin(0x76);
    //status = bmp.begin();
    if (!status) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                        "try a different address!"));
        Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X16,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_1); /* Standby time. */
    //bmp_temp->printSensorDetails();
}