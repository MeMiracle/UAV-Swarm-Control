//
// Created by caryzhou on 17-11-15.
//

#include <zconf.h>
#include <m100Location/uwblps.h>

bool uwb_zyl::openUwblpsSerial(const char *port, int baud, char bits, parity parity, char stopbit)
{
    return uwblps_serial.Open(port,baud,bits,parity,stopbit);
}

void uwb_zyl::closeUwblpsSerial()
{
    uwblps_serial.Close();
}

void uwb_zyl::uwblps_Data_Convert(uint8 *recieve_buf)
//已经检查开头的0X55和0X01只需要读取后面的126位即可
{
    uint8 recieve_buf_sum = 0;
    recieve_buf_sum += 0x55;
    recieve_buf_sum += 0x01;
    for(int i = 0;i<125;i++)
        recieve_buf_sum += recieve_buf[i];

    if(recieve_buf_sum == recieve_buf[128-1])
    {
        this->count++;
        i_uwb_lps.position.x = (float)Byte32(s32 ,recieve_buf[6],recieve_buf[5],recieve_buf[4], 0) / 256000.0f;
        i_uwb_lps.position.y = (float)Byte32(s32 ,recieve_buf[9],recieve_buf[8],recieve_buf[7], 0) / 256000.0f;
        i_uwb_lps.position.z = (float)Byte32(s32 ,recieve_buf[12],recieve_buf[11],recieve_buf[10], 0) / 256000.0f;

        i_uwb_lps.velocity.x = (float)Byte32(s32 ,recieve_buf[15],recieve_buf[14],recieve_buf[13], 0) / 2560000.0f;
        i_uwb_lps.velocity.y = (float)Byte32(s32 ,recieve_buf[18],recieve_buf[17],recieve_buf[16], 0) / 2560000.0f;
        i_uwb_lps.velocity.z = (float)Byte32(s32 ,recieve_buf[21],recieve_buf[20],recieve_buf[19], 0) / 2560000.0f;

        i_uwb_lps.dis_buf[0] = (float)Byte32(s32 ,recieve_buf[24],recieve_buf[23],recieve_buf[22], 0) / 256000.0f;
        i_uwb_lps.dis_buf[1] = (float)Byte32(s32 ,recieve_buf[27],recieve_buf[26],recieve_buf[25], 0) / 256000.0f;
        i_uwb_lps.dis_buf[2] = (float)Byte32(s32 ,recieve_buf[30],recieve_buf[29],recieve_buf[28], 0) / 256000.0f;
        i_uwb_lps.dis_buf[3] = (float)Byte32(s32 ,recieve_buf[33],recieve_buf[32],recieve_buf[31], 0) / 256000.0f;
        i_uwb_lps.dis_buf[4] = (float)Byte32(s32 ,recieve_buf[36],recieve_buf[35],recieve_buf[34], 0) / 256000.0f;
        i_uwb_lps.dis_buf[5] = (float)Byte32(s32 ,recieve_buf[39],recieve_buf[38],recieve_buf[37], 0) / 256000.0f;
        i_uwb_lps.dis_buf[6] = (float)Byte32(s32 ,recieve_buf[42],recieve_buf[41],recieve_buf[40], 0) / 256000.0f;
        i_uwb_lps.dis_buf[7] = (float)Byte32(s32 ,recieve_buf[45],recieve_buf[44],recieve_buf[43], 0) / 256000.0f;

        i_uwb_lps.gyro.x = Byte16(sint16, recieve_buf[47],  recieve_buf[46]);
        i_uwb_lps.gyro.y = Byte16(sint16, recieve_buf[49],  recieve_buf[48]);
        i_uwb_lps.gyro.z = Byte16(sint16, recieve_buf[51],  recieve_buf[50]);
        i_uwb_lps.acc.x = Byte16(sint16, recieve_buf[53],  recieve_buf[52]);
        i_uwb_lps.acc.y = Byte16(sint16, recieve_buf[55],  recieve_buf[54]);
        i_uwb_lps.acc.z = Byte16(sint16, recieve_buf[57],  recieve_buf[56]);
        i_uwb_lps.mag.x = Byte16(sint16, recieve_buf[59],  recieve_buf[58]);
        i_uwb_lps.mag.y = Byte16(sint16, recieve_buf[61],  recieve_buf[60]);
        i_uwb_lps.mag.z = Byte16(sint16, recieve_buf[63],  recieve_buf[62]);

        i_uwb_lps.angle.x = (float) (Byte16(sint16, recieve_buf[65],  recieve_buf[64])) / 100.0f;
        i_uwb_lps.angle.y = (float) (Byte16(sint16, recieve_buf[67],  recieve_buf[66])) / 100.0f;
        i_uwb_lps.angle.z = (float) (Byte16(sint16, recieve_buf[69],  recieve_buf[68])) / 100.0f;

        cout<<"good: "<<this->count<<": ("<<i_uwb_lps.position.x<<","<<i_uwb_lps.position.y<<","<<
            i_uwb_lps.position.z<<")\n";

        log<<"good: "<<this->count<<": ("<<i_uwb_lps.position.x<<","<<i_uwb_lps.position.y<<","<<
           i_uwb_lps.position.z<<")\n";
        //log<<"原始距离： "<<this->count<<": ("<<<<","<<tempdis.dis2<<","<<tempdis.dis3<<","<<tempdis.dis4<<")\n";

    }
    else
    {
        this->count++;
        this->flag = false;
        cout<<"bad package: "<<this->count<<endl;
        log<<"校验出错，丢弃该数据包\n";

    }
}

/*
void uwb_zyl::uwblps_Data_Convert(uint8 recieve_buf[])
{
    uint8 recieve_buf_sum = 0;

  //  uint8 i = 0;

    if((recieve_buf[0] == 0x55)&&(recieve_buf[1] == 0x01))
    {
        for(int i = 0;i<(128 -1);i++)
            recieve_buf_sum += recieve_buf[i];

        if(recieve_buf_sum == recieve_buf[128-1])
        {
            this->count++;
            i_uwb_lps.position.x = (float)Byte32(s32 ,recieve_buf[6],recieve_buf[5],recieve_buf[4], 0) / 256000.0f;
            i_uwb_lps.position.y = (float)Byte32(s32 ,recieve_buf[9],recieve_buf[8],recieve_buf[7], 0) / 256000.0f;
            i_uwb_lps.position.z = (float)Byte32(s32 ,recieve_buf[12],recieve_buf[11],recieve_buf[10], 0) / 256000.0f;

            i_uwb_lps.velocity.x = (float)Byte32(s32 ,recieve_buf[15],recieve_buf[14],recieve_buf[13], 0) / 2560000.0f;
            i_uwb_lps.velocity.y = (float)Byte32(s32 ,recieve_buf[18],recieve_buf[17],recieve_buf[16], 0) / 2560000.0f;
            i_uwb_lps.velocity.z = (float)Byte32(s32 ,recieve_buf[21],recieve_buf[20],recieve_buf[19], 0) / 2560000.0f;

            i_uwb_lps.dis_buf[0] = (float)Byte32(s32 ,recieve_buf[24],recieve_buf[23],recieve_buf[22], 0) / 256000.0f;
            i_uwb_lps.dis_buf[1] = (float)Byte32(s32 ,recieve_buf[27],recieve_buf[26],recieve_buf[25], 0) / 256000.0f;
            i_uwb_lps.dis_buf[2] = (float)Byte32(s32 ,recieve_buf[30],recieve_buf[29],recieve_buf[28], 0) / 256000.0f;
            i_uwb_lps.dis_buf[3] = (float)Byte32(s32 ,recieve_buf[33],recieve_buf[32],recieve_buf[31], 0) / 256000.0f;
            i_uwb_lps.dis_buf[4] = (float)Byte32(s32 ,recieve_buf[36],recieve_buf[35],recieve_buf[34], 0) / 256000.0f;
            i_uwb_lps.dis_buf[5] = (float)Byte32(s32 ,recieve_buf[39],recieve_buf[38],recieve_buf[37], 0) / 256000.0f;
            i_uwb_lps.dis_buf[6] = (float)Byte32(s32 ,recieve_buf[42],recieve_buf[41],recieve_buf[40], 0) / 256000.0f;
            i_uwb_lps.dis_buf[7] = (float)Byte32(s32 ,recieve_buf[45],recieve_buf[44],recieve_buf[43], 0) / 256000.0f;

            i_uwb_lps.gyro.x = Byte16(sint16, recieve_buf[47],  recieve_buf[46]);
            i_uwb_lps.gyro.y = Byte16(sint16, recieve_buf[49],  recieve_buf[48]);
            i_uwb_lps.gyro.z = Byte16(sint16, recieve_buf[51],  recieve_buf[50]);
            i_uwb_lps.acc.x = Byte16(sint16, recieve_buf[53],  recieve_buf[52]);
            i_uwb_lps.acc.y = Byte16(sint16, recieve_buf[55],  recieve_buf[54]);
            i_uwb_lps.acc.z = Byte16(sint16, recieve_buf[57],  recieve_buf[56]);
            i_uwb_lps.mag.x = Byte16(sint16, recieve_buf[59],  recieve_buf[58]);
            i_uwb_lps.mag.y = Byte16(sint16, recieve_buf[61],  recieve_buf[60]);
            i_uwb_lps.mag.z = Byte16(sint16, recieve_buf[63],  recieve_buf[62]);

            i_uwb_lps.angle.x = (float) (Byte16(sint16, recieve_buf[65],  recieve_buf[64])) / 100.0f;
            i_uwb_lps.angle.y = (float) (Byte16(sint16, recieve_buf[67],  recieve_buf[66])) / 100.0f;
            i_uwb_lps.angle.z = (float) (Byte16(sint16, recieve_buf[69],  recieve_buf[68])) / 100.0f;

            log<<"good: "<<this->count<<": ("<<i_uwb_lps.position.x<<","<<i_uwb_lps.position.y<<","<<
               i_uwb_lps.position.z<<")\n";
            //log<<"原始距离： "<<this->count<<": ("<<<<","<<tempdis.dis2<<","<<tempdis.dis3<<","<<tempdis.dis4<<")\n";

        }
        else
        {
            this->flag = false;
            log<<"校验出错，丢弃该数据包\n";

        }
    }
}
 */

int uwb_zyl::readData(uint8 *buffer) {
    ssize_t r = 1;
    int length = 0;

    // Set timeout between 2 bytes (20ms)
    _timeout timeout(TIME_OUT);

    //memset(buffer, 0x00, BUFFER_SIZE);

    while((r > 0) || timeout.end())
    {
        // Protect buffer
        if(length >= (BUFFER_SIZE - 1)) return length;


        r = read(uwblps_serial.getserialfd(), (buffer + length), 1);

        if(r > 0)
        {
            length++;

            // Start timer
            timeout.start();
        }
    }

    return length;
}

/*
Point2D uwb_zyl::uwblps_Data_Convert(const uint8 *recieve_buf, int p)
{
    uint8 recieve_buf_sum = 0;
    Point2D tempPoint;
    //  uint8 i = 0;

    if((recieve_buf[0 + p] == 0x55)&&(recieve_buf[1 + p] == 0x01))
    {

        for(int i = 0;i<(128 -1);i++)
            recieve_buf_sum += recieve_buf[i+p];

        if(recieve_buf_sum == recieve_buf[127+p])
        {

            this->count++;
            i_uwb_lps.position.x = (float)Byte32(s32 ,recieve_buf[6 + p],recieve_buf[5 + p],recieve_buf[4 + p], 0) / 256000.0f;
            i_uwb_lps.position.y = (float)Byte32(s32 ,recieve_buf[9 + p],recieve_buf[8 + p],recieve_buf[7 + p], 0) / 256000.0f;
            i_uwb_lps.position.z = (float)Byte32(s32 ,recieve_buf[12 + p],recieve_buf[11 + p],recieve_buf[10 + p], 0) / 256000.0f;

            i_uwb_lps.velocity.x = (float)Byte32(s32 ,recieve_buf[15 + p],recieve_buf[14 + p],recieve_buf[13 + p], 0) / 2560000.0f;
            i_uwb_lps.velocity.y = (float)Byte32(s32 ,recieve_buf[18 + p],recieve_buf[17 + p],recieve_buf[16 + p], 0) / 2560000.0f;
            i_uwb_lps.velocity.z = (float)Byte32(s32 ,recieve_buf[21 + p],recieve_buf[20 + p],recieve_buf[19 + p], 0) / 2560000.0f;

            i_uwb_lps.dis_buf[0] = (float)Byte32(s32 ,recieve_buf[24 + p],recieve_buf[23 + p],recieve_buf[22 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[1] = (float)Byte32(s32 ,recieve_buf[27 + p],recieve_buf[26 + p],recieve_buf[25 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[2] = (float)Byte32(s32 ,recieve_buf[30 + p],recieve_buf[29 + p],recieve_buf[28 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[3] = (float)Byte32(s32 ,recieve_buf[33 + p],recieve_buf[32 + p],recieve_buf[31 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[4] = (float)Byte32(s32 ,recieve_buf[36 + p],recieve_buf[35 + p],recieve_buf[34 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[5] = (float)Byte32(s32 ,recieve_buf[39 + p],recieve_buf[38 + p],recieve_buf[37 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[6] = (float)Byte32(s32 ,recieve_buf[42 + p],recieve_buf[41 + p],recieve_buf[40 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[7] = (float)Byte32(s32 ,recieve_buf[45] + p,recieve_buf[44 + p],recieve_buf[43 + p], 0) / 256000.0f;

            i_uwb_lps.gyro.x = Byte16(sint16, recieve_buf[47 + p],  recieve_buf[46 + p]);
            i_uwb_lps.gyro.y = Byte16(sint16, recieve_buf[49 + p],  recieve_buf[48 + p]);
            i_uwb_lps.gyro.z = Byte16(sint16, recieve_buf[51 + p],  recieve_buf[50 + p]);
            i_uwb_lps.acc.x = Byte16(sint16, recieve_buf[53 + p],  recieve_buf[52 + p]);
            i_uwb_lps.acc.y = Byte16(sint16, recieve_buf[55 + p],  recieve_buf[54 + p]);
            i_uwb_lps.acc.z = Byte16(sint16, recieve_buf[57 + p],  recieve_buf[56 + p]);
            i_uwb_lps.mag.x = Byte16(sint16, recieve_buf[59 + p],  recieve_buf[58 + p]);
            i_uwb_lps.mag.y = Byte16(sint16, recieve_buf[61 + p],  recieve_buf[60 + p]);
            i_uwb_lps.mag.z = Byte16(sint16, recieve_buf[63 + p],  recieve_buf[62 + p]);

            i_uwb_lps.angle.x = (float) (Byte16(sint16, recieve_buf[65 + p],  recieve_buf[64 + p])) / 100.0f;
            i_uwb_lps.angle.y = (float) (Byte16(sint16, recieve_buf[67 + p],  recieve_buf[66 + p])) / 100.0f;
            i_uwb_lps.angle.z = (float) (Byte16(sint16, recieve_buf[69 + p],  recieve_buf[68 + p])) / 100.0f;

            this->posizition.x = i_uwb_lps.position.x;
            this->posizition.y = i_uwb_lps.position.y;

            //cout<<"good: "<<this->count<<": ("<<i_uwb_lps.position.x<<","<<i_uwb_lps.position.y<<","<<i_uwb_lps.position.z<<")\n";
            log<<"good: "<<this->count<<": ("<<i_uwb_lps.position.x<<","<<i_uwb_lps.position.y<<","<<
               i_uwb_lps.position.z<<")\n";
            //log<<"原始距离： "<<this->count<<": ("<<<<","<<tempdis.dis2<<","<<tempdis.dis3<<","<<tempdis.dis4<<")\n";
            tempPoint.x = i_uwb_lps.position.x;
            tempPoint.y = i_uwb_lps.position.y;
            return tempPoint;
        }
        else
        {
            this->flag = false;
            log<<"校验出错，丢弃该数据包\n";
        }
    }
}
*/

Point3D uwb_zyl::uwblps_Data_Convert(const uint8 *recieve_buf, int p)
{
    uint8 recieve_buf_sum = 0;
    Point3D tempPoint = {0,0,0};
    //  uint8 i = 0;

    if((recieve_buf[0 + p] == 0x55)&&(recieve_buf[1 + p] == 0x01))
    {

        for(int i = 0;i<(128 -1);i++)
            recieve_buf_sum += recieve_buf[i+p];

        if(recieve_buf_sum == recieve_buf[127+p])
        {

            this->count++;
            i_uwb_lps.position.x = (float)Byte32(s32 ,recieve_buf[6 + p],recieve_buf[5 + p],recieve_buf[4 + p], 0) / 256000.0f;
            i_uwb_lps.position.y = (float)Byte32(s32 ,recieve_buf[9 + p],recieve_buf[8 + p],recieve_buf[7 + p], 0) / 256000.0f;
            i_uwb_lps.position.z = (float)Byte32(s32 ,recieve_buf[12 + p],recieve_buf[11 + p],recieve_buf[10 + p], 0) / 256000.0f;

            i_uwb_lps.velocity.x = (float)Byte32(s32 ,recieve_buf[15 + p],recieve_buf[14 + p],recieve_buf[13 + p], 0) / 2560000.0f;
            i_uwb_lps.velocity.y = (float)Byte32(s32 ,recieve_buf[18 + p],recieve_buf[17 + p],recieve_buf[16 + p], 0) / 2560000.0f;
            i_uwb_lps.velocity.z = (float)Byte32(s32 ,recieve_buf[21 + p],recieve_buf[20 + p],recieve_buf[19 + p], 0) / 2560000.0f;

            i_uwb_lps.dis_buf[0] = (float)Byte32(s32 ,recieve_buf[24 + p],recieve_buf[23 + p],recieve_buf[22 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[1] = (float)Byte32(s32 ,recieve_buf[27 + p],recieve_buf[26 + p],recieve_buf[25 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[2] = (float)Byte32(s32 ,recieve_buf[30 + p],recieve_buf[29 + p],recieve_buf[28 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[3] = (float)Byte32(s32 ,recieve_buf[33 + p],recieve_buf[32 + p],recieve_buf[31 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[4] = (float)Byte32(s32 ,recieve_buf[36 + p],recieve_buf[35 + p],recieve_buf[34 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[5] = (float)Byte32(s32 ,recieve_buf[39 + p],recieve_buf[38 + p],recieve_buf[37 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[6] = (float)Byte32(s32 ,recieve_buf[42 + p],recieve_buf[41 + p],recieve_buf[40 + p], 0) / 256000.0f;
            i_uwb_lps.dis_buf[7] = (float)Byte32(s32 ,recieve_buf[45] + p,recieve_buf[44 + p],recieve_buf[43 + p], 0) / 256000.0f;

            i_uwb_lps.gyro.x = Byte16(sint16, recieve_buf[47 + p],  recieve_buf[46 + p]);
            i_uwb_lps.gyro.y = Byte16(sint16, recieve_buf[49 + p],  recieve_buf[48 + p]);
            i_uwb_lps.gyro.z = Byte16(sint16, recieve_buf[51 + p],  recieve_buf[50 + p]);
            i_uwb_lps.acc.x = Byte16(sint16, recieve_buf[53 + p],  recieve_buf[52 + p]);
            i_uwb_lps.acc.y = Byte16(sint16, recieve_buf[55 + p],  recieve_buf[54 + p]);
            i_uwb_lps.acc.z = Byte16(sint16, recieve_buf[57 + p],  recieve_buf[56 + p]);
            i_uwb_lps.mag.x = Byte16(sint16, recieve_buf[59 + p],  recieve_buf[58 + p]);
            i_uwb_lps.mag.y = Byte16(sint16, recieve_buf[61 + p],  recieve_buf[60 + p]);
            i_uwb_lps.mag.z = Byte16(sint16, recieve_buf[63 + p],  recieve_buf[62 + p]);

            i_uwb_lps.angle.x = (float) (Byte16(sint16, recieve_buf[65 + p],  recieve_buf[64 + p])) / 100.0f;
            i_uwb_lps.angle.y = (float) (Byte16(sint16, recieve_buf[67 + p],  recieve_buf[66 + p])) / 100.0f;
            i_uwb_lps.angle.z = (float) (Byte16(sint16, recieve_buf[69 + p],  recieve_buf[68 + p])) / 100.0f;

            this->posizition.x = i_uwb_lps.position.x;
            this->posizition.y = i_uwb_lps.position.y;
            this->posizition.z = i_uwb_lps.position.z;

            //cout<<"good: "<<this->count<<": ("<<i_uwb_lps.position.x<<","<<i_uwb_lps.position.y<<","<<i_uwb_lps.position.z<<")\n";
            log<<"good: "<<this->count<<": ("<<i_uwb_lps.position.x<<","<<i_uwb_lps.position.y<<","<<
               i_uwb_lps.position.z<<")\n";
            //log<<"原始距离： "<<this->count<<": ("<<<<","<<tempdis.dis2<<","<<tempdis.dis3<<","<<tempdis.dis4<<")\n";
            tempPoint.x = i_uwb_lps.position.x;
            tempPoint.y = i_uwb_lps.position.y;
            tempPoint.z = i_uwb_lps.position.z;
            
            return tempPoint;
        }
        else
        {
            this->flag = false;
            log<<"校验出错，丢弃该数据包\n";
        }
    }
}

int uwb_zyl::getUwbSerialFd() {
    return this->uwblps_serial.getserialfd();
}

int uwb_zyl::readData(uint8 *buffer, int size) {
    ssize_t r = 1;
    int length = 0;

    // Set timeout between 2 bytes (20ms)
    _timeout timeout(TIME_OUT);

    //memset(buffer, 0x00, BUFFER_SIZE);

    while((r > 0) || timeout.end())
    {
        // Protect buffer
        if(length >= (size - 1)) return length;


        r = read(uwblps_serial.getserialfd(), (buffer + length), 1);

        if(r > 0)
        {
            length++;

            // Start timer
            timeout.start();
        }
    }

    return length;
}

Point3D uwb_zyl::getPosizition()
{
    return this->posizition;
}

uwb_zyl::uwb_zyl(const uwb_zyl &other) {
    this->count = other.count;
    this->uwblps_serial = other.uwblps_serial;
    this->posizition = other.posizition;
    this->i_uwb_lps = other.i_uwb_lps;
    this->flag = other.flag;
}

uwb_zyl uwb_zyl::operator=(const uwb_zyl &other) {
    this->count = other.count;
    this->uwblps_serial = other.uwblps_serial;
    this->posizition = other.posizition;
    this->i_uwb_lps = other.i_uwb_lps;
    this->flag = other.flag;
    return *this;
}


/*
void uwb_zyl::uwblps_Tag_Data_Updata(void)
{
    if(usart1_rx_irq_updata_user_reset_status == 1)
    {
        usart1_rx_irq_updata_user_reset_status = 0;
        uwblps_Data_Convert(usart1_receive_buf);

        //while(DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);

        DMA_SetCurrDataCounter(DMA2_Stream5,USART1_RECEIVE_BUF_LENGTH);/
        DMA_Cmd(DMA2_Stream5,ENABLE);
    }
}
 */


