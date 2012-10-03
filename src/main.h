#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <string.h>
#include <webkit/webkit.h>

#define LENGTH(x) (sizeof x / sizeof x[0])

#ifdef DEBUG
#define PRINT_DEBUG(...) do { \
    fprintf(stderr, "\n\033[31;1mDEBUG:\033[0m %s:%d:%s()\t", __FILE__, __LINE__, __func__); \
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n"); \
} while(0);
#define TIMER_START GTimer *__timer; do {__timer = g_timer_new(); g_timer_start(__timer);} while(0)
#define TIMER_END do {gulong __debug_micro = 0; gdouble __debug_elapsed = g_timer_elapsed(__timer, &__debug_micro);\
    PRINT_DEBUG("\033[33mtimer:\033[0m elapsed: %f, micro: %lu", __debug_elapsed, __debug_micro);\
    g_timer_destroy(__timer); \
} while(0)
#else
#define PRINT_DEBUG(message, ...)
#define TIMER_START
#define TIMER_END
#endif

/* enums */
typedef enum _vp_mode {
    VP_MODE_NORMAL       = (1 << 0),
    VP_MODE_PATH_THROUGH = (1 << 1),
    VP_MODE_INSERT       = (1 << 2),
} Mode;

enum {
    VP_NAVIG_BACK,
    VP_NAVIG_FORWARD,
    VP_NAVIG_RELOAD,
    VP_NAVIG_RELOAD_FORCE,
    VP_NAVIG_STOP_LOADING
};

/*
1 << 0:  0 = jump,              1 = scroll
1 << 1:  0 = vertical,          1 = horizontal
1 << 2:  0 = top/left,          1 = down/right
1 << 3:  0 = paging/halfpage,   1 = line
1 << 4:  0 = paging,            1 = halfpage
*/
enum {VP_SCROLL_TYPE_JUMP, VP_SCROLL_TYPE_SCROLL};
enum {
    VP_SCROLL_AXIS_V,
    VP_SCROLL_AXIS_H = (1 << 1)
};
enum {
    VP_SCROLL_DIRECTION_TOP,
    VP_SCROLL_DIRECTION_DOWN  = (1 << 2),
    VP_SCROLL_DIRECTION_LEFT  = VP_SCROLL_AXIS_H,
    VP_SCROLL_DIRECTION_RIGHT = VP_SCROLL_AXIS_H | (1 << 2)
};
enum {
    VP_SCROLL_UNIT_PAGE,
    VP_SCROLL_UNIT_LINE     = (1 << 3),
    VP_SCROLL_UNIT_HALFPAGE = (1 << 4)
};

/* structs */
typedef struct {
    gint  i;
    char* s;
} Arg;

/* statusbar */
typedef struct {
    GtkBox*     box;
    GtkWidget*  left;
    GtkWidget*  right;
} StatusBar;

/* gui */
typedef struct {
    GtkWidget*     window;
    WebKitWebView* webview;
    GtkWidget*     viewport;
    GtkBox*        box;
    GtkWidget*     eventbox;
    GtkWidget*     inputbox;
    StatusBar      statusbar;
    GtkScrollbar*  sb_h;
    GtkScrollbar*  sb_v;
    GtkAdjustment* adjust_h;
    GtkAdjustment* adjust_v;
} Gui;

/* state */
typedef struct {
    Mode          mode;
    gchar         modkey;
    int           count;
} State;

/* behaviour */
typedef struct {
    /* command list: (key)name -> (value)Command  */
    GHashTable* commands;
} Behaviour;

/* core struct */
typedef struct {
    Gui           gui;
    State         state;
    Behaviour     behave;
#if 0
    Network       net;
    Ssl           ssl;
    Communication comm;
    Info          info;
#endif
} VpCore;

/* main object */
extern VpCore vp;

/* functions */
void vp_update_statusbar(void);
void vp_update_urlbar(const gchar* uri);
gboolean vp_load_uri(const Arg* arg);
void vp_navigate(const Arg* arg);
void vp_scroll(const Arg* arg);
void vp_close_browser(const Arg* arg);
void vp_view_source(const Arg* arg);

#endif /* end of include guard: MAIN_H */