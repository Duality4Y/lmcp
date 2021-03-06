#include <string.h>
#include <lmcp.h>

#include <stdlib.h>
#include <time.h>

uint8_t data[2000] = {0};
uint8_t testdata[2000] = {0};

// enum for keep track where the bytes go!
// in the packet

int main(void)
{
    // process empty data.
    Lmcp lmcp = Lmcp(10, 10, 0xff);
    printf("[main]: processing empty data.\n");
    lmcp.processPacket(data, 2000);

    // process data with magic at 996.
    uint8_t testmagick[4] = {'L', 'M', 'C', 'P'};
    memcpy(data+996, testmagick, 4);
    printf("[main]: processing data with header at [996]\n");
    lmcp.processPacket(data, 2000);

    // clear data. and test with a header with invalid c-sum
    memset(data, 0, 2000);
    //                    [magic header     ]-[ver]-[c-sum    ]-[command  ]-[length   ]-[data    ]
    uint8_t test[2000] = {'L', 'M', 'C', 'P', 0x01, 0x00, 0x19, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04};
    // c-sum should be 25
    memcpy(data, test, 200);
    printf("[main]: processing data with correct header but incorrect csum.\n");
    lmcp.processPacket(data, 2000);

    // lets construct a packet with random data. and see if checksum
    // still checks out.
    printf("[main]: processing data with correct header and random data.\n");
    memset(testdata, 0, 2000);
    srand(time(NULL));

    // start is packet size - start of data
    uint16_t length = 1024 - DATA;
    for(int i = DATA; i < length; i++)
    {
        testdata[i] = rand() % 0xff;
    }

    // set version to 0x01
    testdata[VERSION] = 0x01;
    printf("[main]: packet version: %d\n", testdata[VERSION]);

    // add a command say write rgb image.
    uint16_t command = 0x32;
    testdata[COMMAND_H] = ((command >> 8) & 0xff);
    testdata[COMMAND_L] = command & 0xff;
    printf("[main]: packet command: %d\n", command);

    // add magick
    const char magick[] = "LMCP";
    memcpy(testdata, magick, 4);
    printf("[main]: magick: %s\n", magick);
    
    // add length
    testdata[LENGTH_H] = ((length >> 8) & 0xff);
    testdata[LENGTH_L] = length & 0xff;

    // create a checksum
    uint16_t csum = 0;
    // sum all bytes starting from COMMAND_H
    // including command bytes and length bytes (4)
    for(int i = COMMAND_H; i < length+4; i++)
    {
        csum += testdata[i];
    }
    printf("[main]: generated checksum: %d\n", csum);
    // add checksum to the packet.
    testdata[CSUM_H] = ((csum >> 8) & 0xff);
    testdata[CSUM_L] = csum & 0xff;
    // try and process this packet.
    lmcp.processPacket(testdata, 2000);

    return 0;
}
