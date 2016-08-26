#include <Xgui>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>


int main(int argc, char **)
{
    assert(Xgui::init());

    Xgui::Window *window = new Xgui::Window(NULL, "Xgui TextView test", Xgui::Rect(0, 0, 1024, 900));
    Xgui::Flags flags;
    if (argc == 1) {
        flags = Xgui::Flags::DoubleBuffered;
    } else {
        flags = Xgui::Flags::NoFlags;
    }
    const int margin = 10;
    Xgui::TextView *textview = new Xgui::TextView(window, Xgui::Rect(margin, margin, 1024 - margin * 2, 900 - margin * 2), flags);
    textview->setColor(Xgui::Color::fromRGB(245, 245, 245));
    textview->setFont(Xgui::Font("Inconsolata-14"));
    textview->setLineHeight(textview->lineHeight() * 2);

    //Xgui::TextEdit *textedit = new Xgui::TextEdit(window, Xgui::Rect(10, 440, 1004, 450), flags);
    //textedit->setColor(Xgui::Color::fromRGB(245, 245, 245));
    //textedit->setFont(Xgui::Font("Ubuntu Mono-14"));
    //textedit->setLineHeight(textview->lineHeight());

    Xgui::TextBuffer *buff = new Xgui::TextBuffer();
    
    if (1) {
        //std::ifstream t("/home/victor/Documents/test.txt");
        //std::ifstream t("/home/victor/Documents/lorem2.txt");
        std::ifstream t("/home/victor/Documents/lorem.txt");
        std::stringstream buf;
        buf << t.rdbuf();
        buff->setText(buf.str());
        t.close();
    }
    textview->setTextBuffer(buff);

    window->show();
    textview->show();
    //textedit->show();

    Xgui::run();
}