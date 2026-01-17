//
// Created by launa on 15/01/2026.
//

#ifndef UNTITLED_LEDSERVICE_H
#define UNTITLED_LEDSERVICE_H


class LedService {
protected:
    LedService();

    int ledPosition;
    int sensor_threshold_nb;
    float threshold;
    int led_status;
    int threshold_mode;

public:
    LedService(LedService &other) = delete;

    void operator=(const LedService &) = delete;

    int getLedPos() const;

    void setSensorTreshold(int sensorNumber, float tresholdVal, int mode);

    void deleteThreshold();

    void changeStatus();

    int getStatus() const;

    void ledOn();

    void ledOff();

    static LedService *getInstance();

    int getThresholdMode() const;

    int getThresholdValue() const;

    int getThresholdSensor() const;

    void updateLedThreshold(float value, float oldValue);
};


#endif //UNTITLED_LEDSERVICE_H
