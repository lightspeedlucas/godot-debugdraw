## Debug drawing module for Godot Engine

A simple C++ module that adds debug drawing functions into Godot Engine, like Unity's `Debug.DrawLine` and UE4's `DrawDebugLine`.

- Godot: https://godotengine.org

### How to use

Currently, the following functions are supported:

- `line(a, b, color, width = 1, duration = 0)` for a simple line
- `circle(position, radius, color, duration = 0)` for a filled circle
- `rect(rect, color, width = 1, duration = 0)` for a hollow rectangle
- `area(rect, color, duration = 0)` for a filled rectangle

All functions have an optional *duration* parameter that indicates how long they should stay on screen. The default value of 0 means a single frame.

#### GDScript

Call `DebugDraw.line()` from anywhere.

#### C++

Include `"modules/debugdraw/debugdraw.h"` and call `DebugDraw::get_singleton()->line(...)`.

### Compilation

- Clone the contents of this repository into `<Your Godot source folder>/modules/debugdraw`
  - Please note that this project is meant to be used with Godot's **2.1** branch.
- Build and run Godot normally

### License

Released under the [MIT License](LICENSE).
