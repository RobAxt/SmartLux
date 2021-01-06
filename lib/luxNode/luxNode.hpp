#ifndef luxNode_hpp
#define luxNode_hpp

#include <Homie.hpp>
#include "Wire.h"
#include "max44009.h"

class luxNode : public HomieNode {
    public:
        luxNode(const char* id, const char* name, const char* type);

    protected:
        virtual void setup() override;
        virtual void loop() override;
        virtual void onReadyToOperate() override;

    private:
        Max44009 _lux;
        float _luxAverage;
        int _sensorStatus;
        int _Navg;
        unsigned long _lastMeasurement;
        unsigned long _lastSended;
        void sensorConfiguration();
        void sensorDataAverage();
};
#endif //luxNode_hpp