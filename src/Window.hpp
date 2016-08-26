#ifndef XGUI_WINDOW_HPP
#define XGUI_WINDOW_HPP
#include "Xgui.hpp"
#include "Widget.hpp"
#include "Types.hpp"

#include <string>


namespace Xgui
{
    class Window : public Widget
    {
    public:
        Window(Widget *parent, const String &title, int x, int y, size_t w, size_t h, Flags flags = Flags::NoFlags);
        Window(Widget *parent, const String &title, const Point &pos, const Size &size, Flags flags = Flags::NoFlags);
        Window(Widget *parent, const String &title, const Rect &rect, Flags flags = Flags::NoFlags);

    private:
        void _init(Flags flags, const String &title);
    };


    extern Window *topLevelWindow;
    inline void setTopLevelWindow(Window *window) { topLevelWindow = window; }
}

#endif