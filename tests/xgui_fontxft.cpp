#include <Xgui>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>


int main(void)
{
    assert(Xgui::init());
    Xgui::Font font("Monospace-14");
    debug(font.textSize("  d").w());

    return 0;
}