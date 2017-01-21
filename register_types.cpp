/* register_types.cpp */

#include <object_type_db.h>
#include <globals.h>
#include <print_string.h>

#include "register_types.h"
#include "debugdraw.h"

void register_debugdraw_types()
{
    // initialize singleton
    memnew(DebugDraw);

    // register singleton
    Globals::get_singleton()->add_singleton(Globals::Singleton("DebugDraw", DebugDraw::get_singleton()));
}

void unregister_debugdraw_types()
{
    // free singleton
    memdelete(DebugDraw::get_singleton());
}
