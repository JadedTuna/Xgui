#ifndef XGUI_WIDGET_HPP
#define XGUI_WIDGET_HPP
#include "Types.hpp"
#include "Event.hpp"
#include "Xgui_defs.hpp"
#include "Painter.hpp"

#include <X11/Xlib.h>
#include <vector>
#include <cstddef>


namespace Xgui
{
    class Widget
    {
    public:
        Widget(Widget *parent, int x, int y, size_t w, size_t h, Flags flags = Flags::NoFlags);
        Widget(Widget *parent, const Point &pos, const Size &size, Flags flags = Flags::NoFlags);
        Widget(Widget *parent, const Rect &rect, Flags flags = Flags::NoFlags);
        virtual void        event(Event *);
        inline Rect         rect(void) const { return _rect; }
        inline Widget       *parent(void) { return _parent; }
        inline ::Window     X11Window(void) { return _X11Window; }
        inline ::GC         X11GC(void) { return _X11GC; }
        inline Font         font(void) const { return _font; }
        inline void         setFont(Font font) { _font = font; }
        inline Color        color(void) const { return _color; }
        inline void         setColor(const Color &color) { _color = color; }
        inline Color        textColor(void) const { return _textColor; }
        inline void         setTextColor(const Color &color) { _textColor = color; }
        inline bool         doubleBuffered(void) const { return _doubleBuffered; }
        inline void         setDoubleBuffered(bool doubleBuffered) { _doubleBuffered = doubleBuffered; }
        virtual void        redraw(void);
        virtual void        show(void);
        virtual void        hide(void);

        Callback clicked;

    protected:
        ::Window            _X11Window;
        ::GC                _X11GC;
        Widget *_parent;
        Rect _rect;
        Font _font;
        Color _color;
        Color _textColor;
        bool _doubleBuffered;
        std::vector<Widget *> _children;

        void                addChild(Widget *);
        /*inline void         connect(Callback *callback, Callback *function) { ;*/
        virtual void        paintEvent(PaintEventData &);
        virtual void        resizeEvent(void *);
        virtual void        mousePressEvent(MouseEventData &);
        virtual void        mouseMoveEvent(MouseEventData &);
        virtual void        mouseReleaseEvent(MouseEventData &);
        virtual void        scrollEvent(ScrollEventData &);
        virtual void        keyPressEvent(KeyboardEventData &);
        virtual void        keyReleaseEvent(KeyboardEventData &);

    private:

        virtual void _init(Flags flags);
    };
}

#endif