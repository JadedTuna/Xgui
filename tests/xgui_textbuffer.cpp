#include <Xgui>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>

#define debug(x) std::cout << x << std::endl;


int main(void)
{
    assert(Xgui::init());
    Xgui::TextBuffer *buff = new Xgui::TextBuffer();
    Xgui::Font font("Ubuntu Mono-14");
    
    {
        //std::ifstream t("/home/victor/Documents/test.txt");
        //std::ifstream t("/home/victor/Documents/lorem2.txt");
        std::ifstream t("/home/victor/Documents/lorem.txt");
        std::stringstream buf;
        buf << t.rdbuf();
        buff->setText(buf.str());
        t.close();
    }

    buff->wrap(20, font, Xgui::WrapMode::Word);

    buff->setSelectionStart(0, 0);
    buff->setSelectionEnd(0, 30);

    Xgui::Point selectionStart, selectionEnd;
    buff->getSelection(&selectionStart, &selectionEnd);
    Xgui::String text = buff->getSelectedText();

    debug(selectionStart.x() << "..." << selectionStart.y());
    debug(selectionEnd.x() << "..." << selectionEnd.y());
    debug(text);

    debug("-----------------------------------------------------");
    buff->setSelection(Xgui::Point(50, 3), Xgui::Point(300, 6));
    debug(buff->getSelectedText());

    return 0;
}