#include <Xgui>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>


int main(int argc, char **)
{
    assert(Xgui::init());

    /*Xgui::Window *window = new Xgui::Window(NULL, "Xgui test", Xgui::Rect(0, 0, 1024, 900));
    //Xgui::Input *input = new Xgui::Input(window, Xgui::Rect(20, 10, 130, 30));
    Xgui::Flags flags;
    if (argc == 1) {
        flags = Xgui::Flags::DoubleBuffered;
    } else {
        flags = Xgui::Flags::NoFlags;
    }
    Xgui::TextView *textview = new Xgui::TextView(window, Xgui::Rect(10, 10, 1004, 420), flags);
    textview->setColor(Xgui::Color::fromRGB(245, 245, 245));
    textview->setFont(Xgui::Font("Ubuntu Mono-14"));
    //textview->setLineHeight(textview->lineHeight() * 2);

    Xgui::TextEdit *textedit = new Xgui::TextEdit(window, Xgui::Rect(10, 440, 1004, 450), flags);
    textedit->setColor(Xgui::Color::fromRGB(245, 245, 245));
    textedit->setFont(Xgui::Font("Ubuntu Mono-14"));
    //textedit->setLineHeight(textview->lineHeight());

    //std::ifstream t("/home/victor/Documents/test.txt");
    //std::ifstream t("/home/victor/Documents/file.txt");
    std::ifstream t("/home/victor/Documents/lorem.txt");
    //std::ifstream t("/usr/include/X11/X.h");
    //std::ifstream t(__FILE__);
    //std::ifstream t("textview.txt");
    //std::ifstream t("/home/victor/Documents/programming/python/projects/QWriter/main.py");
    {
        std::stringstream buf;
        buf << t.rdbuf();
        textview->setText(buf.str());
        t.close();
    }
    //textview->setText("Hello world\nHow are you?\n...\nI am good, thank you!");

    window->show();
    //input->show();
    textview->show();
    textedit->show();*/

    Xgui::TextBuffer *buff = new Xgui::TextBuffer();
    Xgui::Font *font = new Xgui::Font("Ubuntu Mono-14");
    
    if (1) {
        //std::ifstream t("/home/victor/Documents/test.txt");
        std::ifstream t("/home/victor/Documents/lorem2.txt");
        //std::ifstream t("/home/victor/Documents/lorem.txt");
        std::stringstream buf;
        buf << t.rdbuf();
        buff->setText(buf.str());
        t.close();
    }
    //buff->setText("Hello world how are you today??? Wanna come and play??? noooooooooooooooooooooooooooooooooo \nman");
    //buff->setText("noooooooooooooooooooooooooooooooooo a man dude why");
    //buff->setText("      ");

    std::cout << buff->numLines() << std::endl;
    Xgui::TextLine *line = buff->lines()->at(3);
    //std::cout << font->textSize("Ipsum").w() << std::endl;
    line->wrap(200, font, Xgui::WrapMode::Word);
    std::cout << font->textSize("e  printing    and").w() << "!!!" << std::endl;

    
    std::cout << *(line->line()) << std::endl << "------------------" << std::endl;
    for (size_t i=0; i < line->numWrappedLines(); i++) {
        size_t start = line->wrappedLines()->at(i).first,
               length = line->wrappedLines()->at(i).second;
        std::cout //<< "-`"
                  << line->line()->substr(start, length)
                  //<< "'"
                  << std::endl;
    }

    size_t x = 0;
    size_t lineN = line->lineFromPos(&x);
    std::cout << std::endl << std::endl << x << std::endl;
    std::cout << lineN << std::endl;
    std::pair<size_t, size_t> p = line->wrappedLines()->at(lineN);
    Xgui::String s = line->line()->substr(p.first, p.second);
    s.insert(x, 1, 'I');
    std::cout << s << std::endl;

    //return 0;
    return 0;//Xgui::run();
}