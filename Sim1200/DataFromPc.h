#pragma once
#include <cstdint>
struct DataFromPc {
    bool Ready; //1
    bool Error; //2
    bool bool02; //3
    bool bool03; //4
    bool bool04; //5
    bool bool05; //6
    bool bool06; //7
    bool bool07; //8
    bool StartScan;  //9
    bool ScanCompleted; //10
    bool bit12; //11
    bool bit13; //12
    bool RebootPower; //13
    bool bit15; //14
    bool bit16; //15
    bool bit17; //16
    uint8_t Spare; //17
    float RadarX; //18
    float RadarY; //19
    float RadarZ; //20
    float float22; //22-25
    float float26; //26-29
    float float30; //30-33
    float float34; //34-37
    float float38; //38-41
    float float42; //42-45
    float float46; //46-49
};