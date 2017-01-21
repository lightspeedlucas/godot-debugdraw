#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <object.h>
#include <list.h>

class DebugDraw : public Object
{
    OBJ_TYPE(DebugDraw, Object);
public:
    /** Lifecycle */
    DebugDraw();
	~DebugDraw();

    /** Singleton */
    static DebugDraw *get_singleton();

    /** Methods */
    void circle(const Vector2 &position, float radius, const Color &color, float duration = .0f);
    void line(const Vector2 &a, const Vector2 &b, const Color &color, float width = 1, float duration = .0f);
    void rect(const Rect2 &rect, const Color &color, float duration = .0f);

    void clear();
    void _idle_frame();

protected:
    /** Godot bindings */
    static void _bind_methods();

    /** Singleton */
    static DebugDraw *singleton;

    /** Current drawings */
    struct Drawing
    {
        RID canvas_item;
        float time_left;
    };

    List<Drawing> drawings;
    RID canvas;

    /** State */
    bool init();
    bool ready;
};

#endif
