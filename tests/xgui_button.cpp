#include <Xgui>
#include <assert.h>


void *callback(Xgui::Button *button)
{
    button->setLabel("Damn!");
    button->redraw();
    debug("tet");
    return NULL;
}


int main(void)
{
    assert(Xgui::init());
    Xgui::Window *window = new Xgui::Window(NULL, "Xgui::Button", Xgui::Rect(10, 10, 320, 240));
    Xgui::Button *button = new Xgui::Button(window, "Click me!", Xgui::Rect(20, 20, 200, 35), Xgui::Flags::DoubleBuffered);
    button->clicked = std::bind(callback, button);

    window->show();

    return Xgui::run();
}