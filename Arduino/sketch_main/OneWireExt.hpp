#pragma once

#ifdef WIN32

class OneWire
{
public:
    OneWire(short PIN){}
};

struct dsW
{};
typedef dsW DeviceAddress;

class DallasTemperature
{
public:
    DallasTemperature(OneWire *pOneWire){}
    void begin(){ }
    void setResolution(DeviceAddress add, short pin) {};
    float getTempC(DeviceAddress add) {return 0.0;}
    void requestTemperatures(){}
    void getAddress(DeviceAddress add, int n){}
};

#endif


class CTempSensor_DS18B20
{
public:
    CTempSensor_DS18B20(short PIN)
        : m_onewire(PIN),
          m_sensors(&m_onewire)
    {
        m_pin = PIN;
        m_sensors.begin();
    }

    void requestTemperatures() { m_sensors.requestTemperatures(); }
    
    void SetResolution(DeviceAddress adress)
  {
         m_sensors.setResolution(adress, 9);
    }

    float GetSensorValueCelse(DeviceAddress adress)
    {
        return m_sensors.getTempC(adress);
    }

 void getAddress(DeviceAddress& add, short num)
 {
     m_sensors.getAddress(add, num);
 }
 
private:
    OneWire m_onewire;
    DallasTemperature m_sensors;
    short m_pin;
};
