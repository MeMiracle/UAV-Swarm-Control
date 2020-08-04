//
// Created by caryzhou on 17-11-15.
//

#ifndef TESTUWBLPS_SERIAL_H
#define TESTUWBLPS_SERIAL_H

#include <stdint.h>

#if defined(__WIN32__) || defined(__CYGWIN__)
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#endif

#define BUFFER_SIZE 256
#define TIME_OUT 20 // 20 ms

enum parity
{
    NO,
    EVEN,
    ODD
};

class serial
{
private:

#if defined(__WIN32__) || defined(__CYGWIN__)
    HANDLE m_fd;
		DCB m_conf;
#else
    int	m_fd;
    struct termios m_oldtio;
    struct termios m_newtio;
#endif


public:

    bool Open(const char* port, int baud, char bits, parity parity, char stopbit);
    void Close(void);

    template <typename T>
    char Write(T* buffer,int length);

    char Write(char* buffer, int length);
    char Write(signed char* buffer,int length);
    char Write(uint8_t* buffer,int length);
    ssize_t  Write1(const char* buffer,int length);
    int Read(char* buffer);
    int getserialfd();

};


#endif //TESTUWBLPS_SERIAL_H
