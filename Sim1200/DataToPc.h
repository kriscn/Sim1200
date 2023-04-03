#pragma once
#include <cstdint>
struct DataToPc {
    bool Ready; //1
    bool Error; //2
    bool bool02; //3
    bool bool03; //4
    bool bool04; //5
    bool bool05; //6
    bool bool06; //7
    bool bool07; //8
    bool StartScan;  //9
    bool bit11; //10
    bool bit12; //11
    bool bit13; //12
    bool RebootPower; //13
    bool RebootCompleted; //14
    bool bit16; //15
    bool bit17; //16
    uint8_t Spare; //17
    float SkyLiftX; //18
    float SkyLiftY; //19
    float SkyLiftZ; //20
    float IronPlateLength;  //22-25  // Ìú°åºñ¶È(IronPlateLength)
    float IronPlateWidth;   //26-29  // Ìú°å¿í¶È(IronPlateWidth)
    float IronPlateHeight;  //30-33  // Ìú°å¸ß¶È(IronPlateHeight)
    float float34; //34-37
    float float38; //38-41
    float float42; //42-45
    float float46; //46-49
};