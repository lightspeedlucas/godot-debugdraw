
#include "debugdraw.h"

#include <servers/visual_server.h>
#include <scene/main/scene_main_loop.h>
#include <scene/main/viewport.h>
#include <print_string.h>

DebugDraw *DebugDraw::singleton = NULL;

DebugDraw::DebugDraw()
    : ready(false)
{
    ERR_FAIL_COND(singleton);
    singleton = this;
}

DebugDraw::~DebugDraw()
{
    if (ready)
    {
        auto *vs = VS::get_singleton();

        for (auto *e = drawings.front(); e; e = e->next())
            vs->free(e->get().canvas_item);

        auto *st = SceneTree::get_singleton();
        st->disconnect("idle_frame", this, "_idle_frame");

        vs->free(canvas);
    }

    singleton = NULL;
}

bool DebugDraw::init()
{
    auto *st = SceneTree::get_singleton();
    ERR_FAIL_NULL_V(st, false);

    auto *vs = VS::get_singleton();
    auto viewport = st->get_root()->get_viewport();

    canvas = vs->canvas_create();
    vs->viewport_attach_canvas(viewport, canvas);
    vs->viewport_set_canvas_layer(viewport, canvas, (~0U) >> 1);

    st->connect("idle_frame", this, "_idle_frame");
    return ready = true;
}

void DebugDraw::circle(const Vector2 &position, float radius, const Color &color, float duration)
{
    if (ready || init())
    {
        auto *vs = VS::get_singleton();
        Drawing d = { vs->canvas_item_create(), duration };
        vs->canvas_item_set_parent(d.canvas_item, canvas);
        vs->canvas_item_add_circle(d.canvas_item, position, radius, color);
        drawings.push_back(d);
    }
}

void DebugDraw::line(const Vector2 &a, const Vector2 &b, const Color &color, float width, float duration)
{
    if (ready || init())
    {
        auto *vs = VS::get_singleton();
        Drawing d = { vs->canvas_item_create(), duration };
        vs->canvas_item_set_parent(d.canvas_item, canvas);
        vs->canvas_item_add_line(d.canvas_item, a, b, color, width);
        drawings.push_back(d);
    }
}

void DebugDraw::rect(const Rect2 &rect, const Color &color, float width, float duration)
{
    if (ready || init())
    {
        auto tl = rect.pos;
        auto tr = rect.pos + Vector2(rect.size.x, 0);
        auto bl = rect.pos + Vector2(0, rect.size.y);
        auto br = rect.pos + rect.size;

        auto *vs = VS::get_singleton();
        Drawing d = { vs->canvas_item_create(), duration };
        vs->canvas_item_set_parent(d.canvas_item, canvas);
        vs->canvas_item_add_line(d.canvas_item, tl, tr, color, width);
        vs->canvas_item_add_line(d.canvas_item, tr, br, color, width);
        vs->canvas_item_add_line(d.canvas_item, br, bl, color, width);
        vs->canvas_item_add_line(d.canvas_item, bl, tl, color, width);
        drawings.push_back(d);
    }
}

void DebugDraw::area(const Rect2 &rect, const Color &color, float duration)
{
    if (ready || init())
    {
        auto *vs = VS::get_singleton();
        Drawing d = { vs->canvas_item_create(), duration };
        vs->canvas_item_set_parent(d.canvas_item, canvas);
        vs->canvas_item_add_rect(d.canvas_item, rect, color);
        drawings.push_back(d);
    }
}

void DebugDraw::clear()
{
    auto *vs = VS::get_singleton();

    while (auto *e = drawings.front())
    {
        vs->free(e->get().canvas_item);
        e->erase();
    }
}

void DebugDraw::_idle_frame()
{
    auto *vs = VS::get_singleton();
    auto *st = SceneTree::get_singleton();
    const float delta = st->get_idle_process_time();

    for (auto *e = drawings.front(); e;)
    {
        auto &d = e->get();

        if (d.time_left < .0f)
        {
            vs->free(d.canvas_item);
            auto old = e;
            e = e->next();
            old->erase();
        }
        else
        {
            d.time_left -= delta;
            e = e->next();
        }
    }
}

DebugDraw *DebugDraw::get_singleton()
{
    return singleton;
}

void DebugDraw::_bind_methods()
{
    ObjectTypeDB::bind_method(_MD("circle", "position:Vector2", "radius:real", "color:Color", "duration:real"), &DebugDraw::circle, DEFVAL(.0f));
    ObjectTypeDB::bind_method(_MD("line", "a:Vector2", "b:Vector2", "color:Color", "width:real", "duration:real"), &DebugDraw::line, DEFVAL(1.f), DEFVAL(.0f));
    ObjectTypeDB::bind_method(_MD("rect", "rect:Rect2", "color:Color", "width:real", "duration:real"), &DebugDraw::rect, DEFVAL(1.f), DEFVAL(.0f));
    ObjectTypeDB::bind_method(_MD("area", "rect:Rect2", "color:Color", "duration:real"), &DebugDraw::area, DEFVAL(.0f));

    ObjectTypeDB::bind_method(_MD("clear"), &DebugDraw::clear);

    ObjectTypeDB::bind_method(_MD("_idle_frame"), &DebugDraw::_idle_frame);
}
