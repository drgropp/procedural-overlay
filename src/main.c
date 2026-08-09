#include "raylib.h"
#include "effects.h"
#include <stddef.h>

#include <stdbool.h>

#if defined(_WIN32)
typedef struct WindowsPoint {
    long x;
    long y;
} WindowsPoint;

__declspec(dllimport) int __stdcall GetCursorPos(WindowsPoint *point);
#endif

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 150
#define MIN_WINDOW_WIDTH 180
#define MIN_WINDOW_HEIGHT 100
#define RESIZE_MARGIN 6.0f
#define EFFECT_COUNT 5

enum ResizeEdge {
    RESIZE_NONE = 0,
    RESIZE_LEFT = 1,
    RESIZE_RIGHT = 2,
    RESIZE_TOP = 4,
    RESIZE_BOTTOM = 8
};

typedef void (*EffectDrawFunction)(const EffectCanvas *canvas, float time);

static const char *const effect_names[EFFECT_COUNT] = {
    "Sphere", "Vortex", "Waves", "Ambience", "Spectrum"
};

static const EffectDrawFunction effect_drawers[EFFECT_COUNT] = {
    EffectDrawSphere, EffectDrawVortex, EffectDrawWaves,
    EffectDrawAmbience, EffectDrawSpectrum
};

static Color RayColor(EffectColor color)
{
    Color result = { color.r, color.g, color.b, color.a };
    return result;
}

static void RayLine(void *user, float x1, float y1, float x2, float y2,
                    float thickness, EffectColor color)
{
    (void)user;
    DrawLineEx((Vector2){ x1, y1 }, (Vector2){ x2, y2 }, thickness,
               RayColor(color));
}

static void RayCircle(void *user, float x, float y, float radius,
                      EffectColor color)
{
    (void)user;
    DrawCircleV((Vector2){ x, y }, radius, RayColor(color));
}

static void RayRectangle(void *user, float x, float y, float width, float height,
                         EffectColor color)
{
    (void)user;
    DrawRectangleRec((Rectangle){ x, y, width, height }, RayColor(color));
}

static bool PointInRectangle(Vector2 point, Rectangle rectangle)
{
    return CheckCollisionPointRec(point, rectangle);
}

static void ChangeEffect(int *effect, int direction)
{
    *effect = (*effect + direction + EFFECT_COUNT) % EFFECT_COUNT;
}

static int GetResizeEdges(Vector2 mouse, float width, float height)
{
    int edges = RESIZE_NONE;
    if (mouse.x < RESIZE_MARGIN) edges |= RESIZE_LEFT;
    if (mouse.x >= width - RESIZE_MARGIN) edges |= RESIZE_RIGHT;
    if (mouse.y < RESIZE_MARGIN) edges |= RESIZE_TOP;
    if (mouse.y >= height - RESIZE_MARGIN) edges |= RESIZE_BOTTOM;
    return edges;
}

static void SetResizeCursor(int edges)
{
    int cursor = MOUSE_CURSOR_DEFAULT;
    if ((edges & (RESIZE_LEFT | RESIZE_TOP)) ==
            (RESIZE_LEFT | RESIZE_TOP) ||
        (edges & (RESIZE_RIGHT | RESIZE_BOTTOM)) ==
            (RESIZE_RIGHT | RESIZE_BOTTOM)) {
        cursor = MOUSE_CURSOR_RESIZE_NWSE;
    } else if ((edges & (RESIZE_RIGHT | RESIZE_TOP)) ==
                   (RESIZE_RIGHT | RESIZE_TOP) ||
               (edges & (RESIZE_LEFT | RESIZE_BOTTOM)) ==
                   (RESIZE_LEFT | RESIZE_BOTTOM)) {
        cursor = MOUSE_CURSOR_RESIZE_NESW;
    } else if (edges & (RESIZE_LEFT | RESIZE_RIGHT)) {
        cursor = MOUSE_CURSOR_RESIZE_EW;
    } else if (edges & (RESIZE_TOP | RESIZE_BOTTOM)) {
        cursor = MOUSE_CURSOR_RESIZE_NS;
    }
    SetMouseCursor(cursor);
}

static Vector2 GetCursorScreenPosition(Vector2 mouse, Vector2 window_position)
{
#if defined(_WIN32)
    WindowsPoint point;
    if (GetCursorPos(&point)) {
        return (Vector2){ (float)point.x, (float)point.y };
    }
#endif
    return (Vector2){
        window_position.x + mouse.x, window_position.y + mouse.y
    };
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST |
                   FLAG_WINDOW_RESIZABLE |
                   FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Procedural Overlay");
    SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    SetTargetFPS(60);

    int effect = 0;
    bool show_ui = true;
    bool dragging = false;
    int resize_edges = RESIZE_NONE;
    Vector2 action_cursor = { 0 };
    Vector2 action_position = { 0 };
    int action_width = 0;
    int action_height = 0;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        float width = (float)GetScreenWidth();
        float height = (float)GetScreenHeight();
        Rectangle left_button = { width - 48.0f, 5.0f, 20.0f, 18.0f };
        Rectangle right_button = { width - 24.0f, 5.0f, 20.0f, 18.0f };
        bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        Vector2 window_position = GetWindowPosition();
        Vector2 cursor_screen = GetCursorScreenPosition(mouse, window_position);
        int hover_edges = GetResizeEdges(mouse, width, height);

        if (!dragging && resize_edges == RESIZE_NONE) {
            SetResizeCursor(hover_edges);
        }

        if (IsKeyPressed(KEY_LEFT)) ChangeEffect(&effect, -1);
        if (IsKeyPressed(KEY_RIGHT)) ChangeEffect(&effect, 1);
        if (IsKeyPressed(KEY_F11) || IsKeyPressed(KEY_H)) show_ui = !show_ui;

        if (clicked && hover_edges != RESIZE_NONE) {
            resize_edges = hover_edges;
            action_cursor = cursor_screen;
            action_position = window_position;
            action_width = (int)width;
            action_height = (int)height;
        } else if (show_ui && clicked && PointInRectangle(mouse, left_button)) {
            ChangeEffect(&effect, -1);
        } else if (show_ui && clicked && PointInRectangle(mouse, right_button)) {
            ChangeEffect(&effect, 1);
        } else if (clicked && mouse.y <= 26.0f) {
            dragging = true;
            action_cursor = cursor_screen;
            action_position = window_position;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
            resize_edges = RESIZE_NONE;
        }
        if (dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            SetWindowPosition((int)(action_position.x + cursor_screen.x - action_cursor.x),
                              (int)(action_position.y + cursor_screen.y - action_cursor.y));
        } else if (resize_edges != RESIZE_NONE &&
                   IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int dx = (int)(cursor_screen.x - action_cursor.x);
            int dy = (int)(cursor_screen.y - action_cursor.y);
            int x = (int)action_position.x;
            int y = (int)action_position.y;
            int new_width = action_width;
            int new_height = action_height;

            if (resize_edges & RESIZE_LEFT) {
                x += dx;
                new_width -= dx;
            }
            if (resize_edges & RESIZE_RIGHT) new_width += dx;
            if (resize_edges & RESIZE_TOP) {
                y += dy;
                new_height -= dy;
            }
            if (resize_edges & RESIZE_BOTTOM) new_height += dy;

            if (new_width < MIN_WINDOW_WIDTH) {
                if (resize_edges & RESIZE_LEFT) {
                    x = (int)action_position.x + action_width - MIN_WINDOW_WIDTH;
                }
                new_width = MIN_WINDOW_WIDTH;
            }
            if (new_height < MIN_WINDOW_HEIGHT) {
                if (resize_edges & RESIZE_TOP) {
                    y = (int)action_position.y + action_height - MIN_WINDOW_HEIGHT;
                }
                new_height = MIN_WINDOW_HEIGHT;
            }

            SetWindowSize(new_width, new_height);
            SetWindowPosition(x, y);
        }

        EffectCanvas canvas = {
            width, height, NULL, RayLine, RayCircle, RayRectangle
        };

        BeginDrawing();
        ClearBackground((Color){ 5, 7, 13, 255 });
        BeginBlendMode(BLEND_ADDITIVE);
        effect_drawers[effect](&canvas, (float)GetTime());
        EndBlendMode();
        DrawRectangleLinesEx((Rectangle){ 0, 0, width, height }, 1.0f,
                             (Color){ 105, 125, 160, 95 });

        if (show_ui) {
            DrawRectangle(0, 0, (int)width, 28, (Color){ 3, 5, 10, 178 });
            DrawText(effect_names[effect], 8, 8, 12, (Color){ 225, 232, 245, 235 });
            DrawRectangleRounded(left_button, 0.25f, 4,
                                 PointInRectangle(mouse, left_button)
                                     ? (Color){ 70, 85, 110, 210 }
                                     : (Color){ 32, 40, 56, 200 });
            DrawRectangleRounded(right_button, 0.25f, 4,
                                 PointInRectangle(mouse, right_button)
                                     ? (Color){ 70, 85, 110, 210 }
                                     : (Color){ 32, 40, 56, 200 });
            DrawText("<", (int)left_button.x + 7, 7, 12, RAYWHITE);
            DrawText(">", (int)right_button.x + 7, 7, 12, RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
