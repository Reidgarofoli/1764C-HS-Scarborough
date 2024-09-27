#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"
#define threeWireExpander_Port 15
auto led = sylib::Addrled(threeWireExpander_Port,3,60);

int counter = 0;
int numColors = 255;
char team = 'r';

long HSBtoRGB(float _hue, float _sat, float _brightness) {
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    
    if (_sat == 0.0) {
        red = _brightness;
        green = _brightness;
        blue = _brightness;
    } else {
        if (_hue == 360.0) {
            _hue = 0;
        }

        int slice = _hue / 60.0;
        float hue_frac = (_hue / 60.0) - slice;

        float aa = _brightness * (1.0 - _sat);
        float bb = _brightness * (1.0 - _sat * hue_frac);
        float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));
        
        switch(slice) {
            case 0:
                red = _brightness;
                green = cc;
                blue = aa;
                break;
            case 1:
                red = bb;
                green = _brightness;
                blue = aa;
                break;
            case 2:
                red = aa;
                green = _brightness;
                blue = cc;
                break;
            case 3:
                red = aa;
                green = bb;
                blue = _brightness;
                break;
            case 4:
                red = cc;
                green = aa;
                blue = _brightness;
                break;
            case 5:
                red = _brightness;
                green = aa;
                blue = bb;
                break;
            default:
                red = 0.0;
                green = 0.0;
                blue = 0.0;
                break;
        }
    }

    long ired = red * 255.0;
    long igreen = green * 255.0;
    long iblue = blue * 255.0;
    
    return long((ired << 16) | (igreen << 8) | (iblue));
}

void cycle() {
    for (int i = 0; i < 60; i++){
        float colorNumber = (counter + i) % (numColors * 2) > numColors ? (counter + i) % (numColors * 2) - numColors: (counter + i) % (numColors * 2);

        float saturation = 1;
        float brightness = 1;
        float hue = (colorNumber / float(numColors)) * 360;
        long color = HSBtoRGB(hue, saturation, brightness);
        int red = color >> 16 & 255;
        int green = color >> 8 & 255;
        int blue = color & 255;
        led.set_pixel((red*65536) + (green*256) + blue, i);

                //leds[i] = CRGB ( red, green, blue );
    }
    
    counter = (counter + 1) % (numColors * 2);
}

void lightsCheck() {
    if (team == 'r'){
		led.set_all(0xff0000);
	} else if (team == 'b'){
		led.set_all(0x0000ff);
	}
}


#include <iostream>
#include <fstream>
#include <stdio.h>
#include "stb_image.h"
void drawImage(const char *filename, int x, int y, const int w, const int h) {
    int width, height, n;
    uint32_t* buf = new uint32_t[w * h];

    FILE *f;
    char c;
    char r, g, b;
    int i = 0, index = 0;
    f = fopen(filename, "rb");
    if (f) {
        while (EOF != r || EOF != g || EOF != b) {
            c = fgetc(f);
            switch(i){
                case 0:
                    r = c;
                    break;
                case 1:
                    g = c;
                    break;
                case 2:
                    b = c;
                    buf[index] = ((r * 65536) + (g * 256) + (b));
                    index++;
                    break;
            }
            i++;
        }
        fclose(f);
    }


    screen::copy_area(0,0, 480, 240, buf, 480 + 1);
    delete[] buf;
}