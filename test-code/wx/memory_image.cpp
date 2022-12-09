//
// Created by fmorciano on 12/9/22.
//

#include <wx/mstream.h>
#include "wx/wx.h"

unsigned char bytes[4];

void fill(std::vector<u_int8_t>& buffer,
          unsigned long n,
          int channel = 3) {
/*
    unsigned char bytes[4];
    unsigned long n = 175;
*/

    buffer.push_back((n >> 24) & 0xFF);
    buffer.push_back((n >> 16) & 0xFF);
    buffer.push_back((n >> 8) & 0xFF);
    //bytes[0] = (n >> 24) & 0xFF;
    //bytes[1] = (n >> 16) & 0xFF;
    //bytes[2] = (n >> 8) & 0xFF;
    if(channel>3) {
        buffer.push_back(n & 0xFF);
      //  bytes[3] = n & 0xFF;
    }
}

int main() {
    wxInitAllImageHandlers ();
    std::vector<unsigned int> buffer(2*2);
    size_t counter = 0;
    unsigned int black = 0x0000;
    unsigned int white = 0x0FFF;
    for(size_t x=0;x<2;x++) {
        for(size_t y=0;y<2;y++) {
                buffer[counter++] = x % 2 ? black : white ;
        }
    }
    assert(counter == buffer.size());

    std::vector<u_int8_t> buffer8;
    for(size_t i=0;i<buffer.size();i++) {
        fill(buffer8, buffer[i]);
    }

    assert(buffer8.size() == buffer.size() * 3);

    wxImage img(2,2, &buffer8[0], true);
    assert(img.IsOk());
    wxBitmap bitmap(img);
    assert(img.SaveFile("Hello.png"));
    //this->pixmaps[i] = new wxBitmap(img);
}

void test() {
    //Create a DC for the whole screen area
    wxScreenDC dcScreen;

    //Get the size of the screen/DC
    wxCoord screenWidth, screenHeight;
    dcScreen.GetSize(&screenWidth, &screenHeight);

    //Create a Bitmap that will later on hold the screenshot image
    //Note that the Bitmap must have a size big enough to hold the screenshot
    //-1 means using the current default colour depth
    wxBitmap screenshot(screenWidth, screenHeight,-1);

    //Create a memory DC that will be used for actually taking the screenshot
    wxMemoryDC memDC;
    //Tell the memory DC to use our Bitmap
    //all drawing action on the memory DC will go to the Bitmap now
    memDC.SelectObject(screenshot);
    //Blit (in this case copy) the actual screen on the memory DC
    //and thus the Bitmap
    memDC.Blit( 0, //Copy to this X coordinate
                0, //Copy to this Y coordinate
                screenWidth, //Copy this width
                screenHeight, //Copy this height
                &dcScreen, //From where do we copy?
                0, //What's the X offset in the original DC?
                0  //What's the Y offset in the original DC?
    );
    //Select the Bitmap out of the memory DC by selecting a new
    //uninitialized Bitmap
    memDC.SelectObject(wxNullBitmap);

    //Our Bitmap now has the screenshot, so let's save it :-)
    screenshot.SaveFile("screenshot.jpg",wxBITMAP_TYPE_JPEG);
}


void test2() {
    //Create a DC for the whole screen area
    wxMemoryDC dcScreen;

    //Get the size of the screen/DC
    wxCoord screenWidth, screenHeight;
    dcScreen.GetSize(&screenWidth, &screenHeight);

    //Create a Bitmap that will later on hold the screenshot image
    //Note that the Bitmap must have a size big enough to hold the screenshot
    //-1 means using the current default colour depth
    wxBitmap screenshot(screenWidth, screenHeight,-1);

    //Create a memory DC that will be used for actually taking the screenshot
    wxMemoryDC memDC;
    //Tell the memory DC to use our Bitmap
    //all drawing action on the memory DC will go to the Bitmap now
    memDC.SelectObject(screenshot);
    //Blit (in this case copy) the actual screen on the memory DC
    //and thus the Bitmap
    memDC.Blit( 0, //Copy to this X coordinate
                0, //Copy to this Y coordinate
                screenWidth, //Copy this width
                screenHeight, //Copy this height
                &dcScreen, //From where do we copy?
                0, //What's the X offset in the original DC?
                0  //What's the Y offset in the original DC?
    );
    //Select the Bitmap out of the memory DC by selecting a new
    //uninitialized Bitmap
    memDC.SelectObject(wxNullBitmap);

    //Our Bitmap now has the screenshot, so let's save it :-)
    screenshot.SaveFile("screenshot.jpg",wxBITMAP_TYPE_JPEG);
}