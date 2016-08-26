#include "Xgui.hpp"
#include "Widget.hpp"
#include "Types.hpp"
#include "FontXft.hpp"
#include "Painter.hpp"

#include <X11/Xlib.h>
#include <iostream>


namespace Xgui
{
    Widget::Widget(Widget *parent, int x, int y, size_t w, size_t h, Flags flags)
        : _parent(parent)
        , _rect(x, y, w, h)
        , _font("Monospace-10")
    {
        _init(flags);
    }

    Widget::Widget(Widget *parent, const Point &pos, const Size &size, Flags flags)
        : _parent(parent)
        , _rect(pos, size)
        , _font("Monospace-10")
    {
        _init(flags);
    }

    Widget::Widget(Widget *parent, const Rect &rect, Flags flags)
        : _parent(parent)
        , _rect(rect)
        , _font("Monospace-10")
    {
        _init(flags);
    }

    void Widget::_init(Flags flags)
    {
        _color = Color::fromRGB(229, 229, 229);
        _doubleBuffered = false;

        ::Window parentWindow;
        if (_parent) {
            _parent->addChild(this);
            parentWindow = _parent->X11Window();
        } else {
            parentWindow = DefaultRootWindow(Xgui::display);
        }

        if (flags & Flags::DoubleBuffered) {
            /* window is double buffered */
            _doubleBuffered = true;
        }

        _X11Window = XCreateSimpleWindow(Xgui::display, parentWindow, _rect.x(), _rect.y(), _rect.w(), _rect.h(),
            0, Xgui::screen->XScreen()->black_pixel, Xgui::screen->XScreen()->white_pixel);
        Xgui::addWidget(this);

        XSelectInput(Xgui::display, _X11Window, ExposureMask|ButtonPressMask|ButtonReleaseMask|
            StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);

        Atom WM_DELETE_WINDOW = XInternAtom(Xgui::display, "WM_DELETE_WINDOW", False); 
        XSetWMProtocols(Xgui::display, _X11Window, &WM_DELETE_WINDOW, 1);

        _X11GC = XCreateGC(Xgui::display, _X11Window, 0, NULL);
        XSetForeground(Xgui::display, _X11GC, Xgui::screen->XScreen()->black_pixel);
    }

    void Widget::addChild(Widget *child)
    {
        _children.push_back(child);
    }

    void Widget::redraw(void)
    {
        /* Force an expose event on the widget */
        PaintEventData d = {0};
        paintEvent(d);
        for (auto const &child: _children) {
            child->redraw();
        }
    }

    void Widget::show(void)
    {
        XMapWindow(Xgui::display, _X11Window);
        for (auto const &child: _children) {
            child->show();
        }
    }

    void Widget::hide(void)
    {
        XUnmapWindow(Xgui::display, _X11Window);
        for (auto const &child: _children) {
            child->hide();
        }
    }

    void Widget::event(Event *event)
    {
        switch (event->type) {
            case EventType::MousePress:
                mousePressEvent(event->mousedata);
                break;
            case EventType::MouseMove:
                mouseMoveEvent(event->mousedata);
                break;
            case EventType::MouseRelease:
                mouseReleaseEvent(event->mousedata);
                break;
            case EventType::Scroll:
                scrollEvent(event->scrolldata);
                break;
            case EventType::KbdPress:
                keyPressEvent(event->keyboarddata);
                break;
            case EventType::KbdRelease:
                keyReleaseEvent(event->keyboarddata);
                break;
            case EventType::Paint:
                paintEvent(event->paintdata);
                break;
            default:
                break;
        }
    }

    void Widget::paintEvent(PaintEventData &)
    {
        Painter painter(this);
        painter.fillRect(Rect(Point(), _rect.size()));
    }

    void Widget::resizeEvent(void *)
    {
    }

    void Widget::mousePressEvent(MouseEventData &)
    {
    }

    void Widget::mouseMoveEvent(MouseEventData &)
    {
    }

    void Widget::mouseReleaseEvent(MouseEventData &)
    {
    }

    void Widget::keyPressEvent(KeyboardEventData &)
    {
    }

    void Widget::keyReleaseEvent(KeyboardEventData &)
    {
    }

    void Widget::scrollEvent(ScrollEventData &)
    {
    }
}
