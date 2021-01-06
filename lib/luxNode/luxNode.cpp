#include "luxNode.hpp"

luxNode::luxNode(const char* id, const char* name, const char* type) : HomieNode(id, name, type), _lux(0x4A), _luxAverage(0),_sensorStatus(0), _Navg(0), _lastMeasurement(0) {
    Wire.begin();
    _lux.setAutomaticMode();
}

void
luxNode::setup() {
    Homie.getLogger() << F("Calling Node On Setup...") << endl;
    Homie.getLogger() << F("  ◦ Current Lux: ") << _luxAverage << " lux" << endl;
    Homie.getLogger() << F("  ◦ Sensor Status: ") << _sensorStatus << endl;

    setRunLoopDisconnected(false);
    advertise("lux").setName("Luxometer")
                    .setDatatype("float")
                    .setFormat("0.045-188000")
                    .setUnit("lux");
}

void
luxNode::loop() {
    if (millis() - _lastMeasurement >=  1000UL) {
        sensorDataAverage();
        _lastMeasurement = millis();
    }
    if (millis() - _lastSended >=  60000UL) {
        setProperty("lux").send(String(_luxAverage, 3).c_str());
        Homie.getLogger() << F("  ◦ Average Lux: ") << _luxAverage << " lux" << endl;
        sensorConfiguration();
        _lastSended = millis();
    }        
}

void
luxNode::onReadyToOperate() {
    float lux = NAN;
    Homie.getLogger() << F("Calling Ready To Operate... ") << endl;
    Homie.getLogger() << F("  ◦ Node Name: ") << getName() << endl;
    lux = _lux.getLux();
    _sensorStatus = _lux.getError();        
    Homie.getLogger() << F("  ◦ Current Lux: ") << lux << " lux" << endl;
    Homie.getLogger() << F("  ◦ Sensor Status: ") << _sensorStatus << endl;
}

void
luxNode::sensorConfiguration() {
    int conf = _lux.getConfiguration();
    int CDR = (conf & 0x80) >> 3;
    int TIM = (conf & 0x07);
    int integrationTime = _lux.getIntegrationTime();

    Homie.getLogger() << F("⚙ Sensor Configuration... ") << endl; 
    Homie.getLogger() << F("  ◦ CDR: ") << CDR << endl;
    Homie.getLogger() << F("  ◦ TIM: ") << TIM << endl;
    Homie.getLogger() << F("  ◦ Integration Time: ") << integrationTime << " ms" << endl;
}

void
luxNode::sensorDataAverage() {
    float lux = _lux.getLux();
    _sensorStatus = _lux.getError();
    if (_sensorStatus == 0) {
        _luxAverage =  _luxAverage + (lux - _luxAverage) / (_Navg == 60 ? _Navg : ++_Navg);
        Homie.getLogger() << F("  ◦ Current Lux: ") << lux << " lux and Navg:" << _Navg << endl;
    } else {
        Homie.getLogger() << F("  ◦ Sensor Status: ") << _sensorStatus << endl;
    }
}