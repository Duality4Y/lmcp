#ifndef _LMCP_H_
#define _LMCP_H_

#include <stdint.h>
#include <stdio.h>

const size_t TEXT_CHAR_WIDTH = 5;
const size_t TEXT_CHAR_HEIGHT = 7;

enum 
{
    MAGICK,
    VERSION = 4,
    CSUM_H,
    CSUM_L,
    COMMAND_H,
    COMMAND_L,
    LENGTH_H,
    LENGTH_L,
    DATA,
};

class Lmcp
{
public:
    Lmcp(size_t, size_t, uint16_t);

    bool processPacket(uint8_t *, uint16_t);
    // output the buffer
    virtual void clear();
    virtual void writeScreen();
    virtual void setPixel(uint8_t, uint8_t, uint8_t);
    virtual void setPixelRgb(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    
    size_t width;
    size_t height;
    uint8_t set_color[3];

private:

    // draw functions
    uint16_t drawStringNoLen(char *, uint8_t, uint8_t, uint8_t brightness=0xFF, bool absolute=false);
    uint16_t drawString(char *, uint16_t, size_t, size_t, uint8_t brightness=0xFF, bool absolute=false);
    uint16_t drawImage(uint8_t x, uint8_t y, uint16_t width, uint16_t height, uint8_t *data);
    uint16_t drawImageRgb(uint8_t x, uint8_t y, uint16_t width, uint16_t height, uint8_t *data);
    bool matchMagick(uint8_t *);
    int findMagick(uint8_t *, uint16_t);

    uint16_t bitdepth;
    const char *magick = "LMCP";
};

#endif