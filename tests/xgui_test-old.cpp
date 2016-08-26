#include <Xgui>

#include <cmath>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>


void *callback(Xgui::Widget *, Xgui::Input *input, Xgui::TextView *textview)
{
    std::ifstream t(input->value());
    if (t.is_open()) {
        std::stringstream buf;
        buf << t.rdbuf();
        textview->setText(buf.str());
        textview->redraw();
    }
    return 0;
}

int main(void)
{
    // lazy testing
    assert(Xgui::init());

    Xgui::Size winsize(1024, 900);

    Xgui::Window *window = new Xgui::Window(NULL, "Xgui::Window", 50, 50, winsize.w(), winsize.h());
    Xgui::Input *input = new Xgui::Input(window, 50, 50, 200, 21);
    Xgui::Button *button = new Xgui::Button(window, "Load", 50, 86, 60, 21);
    Xgui::TextView *textview = new Xgui::TextEdit(window, 0, 120, winsize.w() - 2 - 15 - 1, winsize.h() - 120 - 2);
    button->clicked = std::bind(callback, button, input, textview);
    input->enter = std::bind(callback, button, input, textview);
    //Xgui::ScrollBar *scrollbar = new Xgui::ScrollBar(window, textview, winsize.w() - 15 - 2, 120, 15, winsize.h() - 120 - 2);
    //textview->setVerticalScrollBar(scrollbar);
    //textview->setLineHeight(std::round(textview->lineHeight() * 1.2));

    std::ifstream t("/home/victor/Documents/test.txt");
    //std::ifstream t("/home/victor/Documents/file.txt");
    //std::ifstream t("/home/victor/Documents/lorem.txt");
    //std::ifstream t("/usr/include/X11/X.h");
    //std::ifstream t(__FILE__);
    //std::ifstream t("/home/victor/Documents/programming/python/projects/QWriter/main.py");
    std::stringstream buf;
    buf << t.rdbuf();
    //textview->setText(buf.str());

    Xgui_UNUSED(input);
    Xgui_UNUSED(button);
    Xgui_UNUSED(textview);
    Xgui_UNUSED(scrollbar);

    window->show();
    input->show();
    button->show();
    textview->show();
    //scrollbar->show();

    Xgui::run();
}