#include <Xgui>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>


int main(void)
{
    assert(Xgui::init());

    Xgui::Window *window = new Xgui::Window(NULL, "Xgui test", Xgui::Rect(0, 0, 1024, 900));
    Xgui::Input *input = new Xgui::Input(window, Xgui::Rect(10, 10, 500, 25), Xgui::Flags::DoubleBuffered);

    window->show();
    input->show();
    return Xgui::run();
}