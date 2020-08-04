//
// Created by caryzhou on 17-11-15.
//

#ifndef TESTUWBLPS_UWBLPS_H
#define TESTUWBLPS_UWBLPS_H

#include<iostream>
#include<cassert>
#include<cmath>
#include<fstream>
#include<string>
#include<vector>
#include "m100Location/inf_typedef.h"
#include"m100Location/serial.h"
#include"m100Location/timeout.h"

using namespace std;

struct uwblps_package{
    S_FLOAT_XYZ position;
    S_FLOAT_XYZ velocity;
    float dis_buf[8];

    S_INT16_XYZ gyro;
    S_INT16_XYZ acc;
    S_INT16_XYZ mag;

    S_FLOAT_XYZ angle;
};

class uwb_zyl{
private:
    ofstream log;

    serial uwblps_serial;

    uwblps_package i_uwb_lps;

    //Point2D posizition;

    Point3D posizition;

    bool flag;

    int count;

public:

    uwb_zyl()
    {
        log.open("./log.txt",ios::app);
        this->count = 0;

    }

    uwb_zyl(string port)
    {
        int ret;
        ret = this->uwblps_serial.Open(port.c_str(), 460800, 8, NO, 1);
        if(ret == 0)
            cout<<"uwb open error!"<<endl;
        log.open("./log.txt",ios::app);
        this->count = 0;
    }

    uwb_zyl(const uwb_zyl& other);

    uwb_zyl operator=(const uwb_zyl& other);

    ~uwb_zyl()
    {
        log.close();

    }


    bool openUwblpsSerial(const char* port, int baud,char bits,parity parity,char stopbit);

    int readData(uint8 *buffer);

    int readData(uint8 *buffer,int size);

    //Point2D uwblps_Data_Convert(const uint8 recieve_buf[],int p);

    Point3D uwblps_Data_Convert(const uint8 recieve_buf[],int p);

    Point3D getPosizition();

    int getUwbSerialFd();

    void closeUwblpsSerial();

    void uwblps_Data_Convert(uint8 recieve_buf[]);

    void uwblps_Tag_Data_Updata(void);



};

#endif //TESTUWBLPS_UWBLPS_H
