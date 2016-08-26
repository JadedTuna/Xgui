// Include libX11 and libXext during linking

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdbe.h>

#include <stdio.h>
#include <unistd.h>


int main(void)
{
    Display *d_dpy;
    Window d_win;
    Visual *d_vis = NULL;
    XdbeBackBuffer d_backBuf;
    GC d_gc;
    int d_drawColor;
    int d_clearColor;

    int width = 800;
    int height = 600;


    /*
     * Create the context like this:
     */
    const char *display = ":0"; // Use NULL for default (environment variable)
    if (!(d_dpy = XOpenDisplay(display))) {
        fprintf(stderr, "Couldn't open X11 display\n");
        return 100;
    }

    // Xdbe extension for double buffering
    int major, minor;
    if (XdbeQueryExtension(d_dpy, &major, &minor)) {
        printf("Xdbe (%d.%d) supported, using double buffering\n", major, minor);
        int numScreens = 1;
        Drawable screens[] = { DefaultRootWindow(d_dpy) };
        XdbeScreenVisualInfo *info = XdbeGetVisualInfo(d_dpy, screens, &numScreens);
        if (!info || numScreens < 1 || info->count < 1) {
            fprintf(stderr, "No visuals support Xdbe\n");
            return 110;
        }

        // Choosing the first one, seems that they have all perflevel of 0,
        // and the depth varies.
        XVisualInfo xvisinfo_templ;
        xvisinfo_templ.visualid = info->visinfo[0].visual; // We know there's at least one
        // As far as I know, screens are densely packed, so we can assume that if at least 1 exists, it's screen 0.
        xvisinfo_templ.screen = 0;
        xvisinfo_templ.depth = info->visinfo[0].depth;

        int matches;
        XVisualInfo *xvisinfo_match =
            XGetVisualInfo(d_dpy, VisualIDMask|VisualScreenMask|VisualDepthMask, &xvisinfo_templ, &matches);

        if (!xvisinfo_match || matches < 1) {
            fprintf(stderr, "Couldn't match a Visual with double buffering\n");
            return 111;
        }

        /*
        printf("%d supported visuals\n", info->count);
        for (int i = 0; i < info->count; ++i) {
            printf("visual %d/%d: id %d, depth %d, perf %d\n",
                    i, info->count,
                    info->visinfo[i].visual,
                    info->visinfo[i].depth,
                    info->visinfo[i].perflevel);
        }
        printf("We got xvisinfo: id: %d, screen %d, depth %d\n",
                xvisinfo_match->visualid, xvisinfo_match->screen, xvisinfo_match->depth);
        */

        // We can use Visual from the match
        d_vis = xvisinfo_match->visual;
    } else {
        fprintf(stderr, "No Xdbe support\n");
        return 101;
    }

    d_drawColor = BlackPixel(d_dpy, DefaultScreen(d_dpy));
    d_clearColor = WhitePixel(d_dpy, DefaultScreen(d_dpy));

    // The actual window, front buffer, using Xdbe enabled Visual (d_vis)
    unsigned long xAttrMask = CWBackPixel;
    XSetWindowAttributes xAttr;
    xAttr.background_pixel = d_clearColor;
    d_win = XCreateWindow(d_dpy, DefaultRootWindow(d_dpy),
            0, 0, width, height, 0, CopyFromParent, CopyFromParent, d_vis,
            xAttrMask, &xAttr);
    if (!d_win) {
        fprintf(stderr, "Failure in window creation?\n");
        return 101;
    }

    // Creating the back buffer, setting swap action hint to background (automatic clearing)
    d_backBuf = XdbeAllocateBackBufferName(d_dpy, d_win, XdbeBackground);

    XSelectInput(d_dpy, d_win, StructureNotifyMask);
    XMapWindow(d_dpy, d_win);
    d_gc = XCreateGC(d_dpy, d_backBuf, 0, NULL);
    XSetForeground(d_dpy, d_gc, d_drawColor);
    XFillRectangle(d_dpy, d_backBuf, d_gc, 10, 10, 20, 30);

    // Waiting for MapNotify to inform that the window is mapped onto the screen
    for (XEvent e; e.type != MapNotify; XNextEvent(d_dpy, &e));

    //return 0;


    /*
     *  And swap the buffers like this:
     */
    XdbeSwapInfo swapInfo;
    swapInfo.swap_window = d_win;
    swapInfo.swap_action = XdbeBackground;

    // XdbeSwapBuffers returns true on success, we return 0 on success.
    if (!XdbeSwapBuffers(d_dpy, &swapInfo, 1))
       return 1;    

    XFlush(d_dpy);
    sleep(3);
}