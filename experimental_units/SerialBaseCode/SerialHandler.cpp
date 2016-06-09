/**
 * ODVpi - Benchmark code extending OpenDaVinci 
 * (https://github.com/se-research/OpenDaVINCI)
 * Copyright (C) 2016 Philip Masek, Magnus Thulin
 *
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <csignal>
#include <sstream>
#include <iostream>
#include <cerrno>

// WRITE TO FILE
#include <fstream>
// WRITE TO FILE END

// SERIAL
#include <termios.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <cstring>
// SERIAL END

using namespace std;


int setUpSerialPort(const char* port_location) {

    int PORT = open( port_location, O_RDWR| O_NOCTTY );

    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if ( tcgetattr ( PORT, &tty ) != 0 ) {
       std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t)B115200);
    cfsetispeed (&tty, (speed_t)B115200);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines


    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( PORT, TCIFLUSH );
    if ( tcsetattr ( PORT, TCSANOW, &tty ) != 0) {
       std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    return PORT;
}

const char* readFromPort(int PORT) {
    int n = 0,
        spot = 0;
    char buf = '\0';

    /* Whole response*/
    char response[64];
    memset(response, '\0', sizeof response);
    int i = 0;

    do {
        n = read( PORT, &buf, 1 );
        sprintf( &response[spot], "%c", buf );
        spot += n;
    } while( buf != '\r' && n > 0 );


    if (n < 0) {
    } else if (n == 0) {
    } else {
        stringstream ss;
        ss << response;
        return ss.str().c_str();
    }

    return "";
}

int32_t main(int32_t argc, char **argv) {

    const char* device_location = NULL;
    const char* output_file_path = NULL;

    for (int args=0;args<argc;args++){
        if (string(argv[args])=="-d" || string(argv[args])=="--device") {
            device_location = argv[args+1];
        } else if (string(argv[args])=="-o" || string(argv[args])=="--output") {
            output_file_path = argv[args+1];
        }
    }

    int port = setUpSerialPort(device_location);
    ofstream output_file;

    int in = 0;
    timespec end, start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    if (output_file_path) {
        output_file.open(output_file_path);
        while (true) {
            const char* msg = readFromPort(port);
            output_file << msg;
            in++;
            if (std::strstr(msg, "end") != NULL) {
                output_file.close();
                break;
            }
        }  
    } else {
        while (true) {
            readFromPort(port);
            in++;
            if (in == 12000)
                break;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    cout << in << " Start: " << start.tv_sec << " End: " << end.tv_sec << " Duration: " << end.tv_sec-start.tv_sec << endl;

    return 0;

}