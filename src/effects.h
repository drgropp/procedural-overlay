#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum EffectPalette {
    EFFECT_PALETTE_RAINBOW = 0,
    EFFECT_PALETTE_RED,
    EFFECT_PALETTE_ORANGE,
    EFFECT_PALETTE_YELLOW,
    EFFECT_PALETTE_GREEN,
    EFFECT_PALETTE_BLUE,
    EFFECT_PALETTE_INDIGO,
    EFFECT_PALETTE_VIOLET,
    EFFECT_PALETTE_CYAN,
    EFFECT_PALETTE_MAGENTA,
    EFFECT_PALETTE_WHITE,
    EFFECT_PALETTE_AMBER,
    EFFECT_PALETTE_PINK,
    EFFECT_PALETTE_TERMINAL,
    EFFECT_PALETTE_ICE,
    EFFECT_PALETTE_FIRE,
    EFFECT_PALETTE_COUNT
} EffectPalette;

typedef struct EffectColor {
    uint8_t r, g, b, a;
} EffectColor;

typedef struct EffectCanvas {
    float width;
    float height;
    EffectPalette palette;
    void *user;
    void (*line)(void *user, float x1, float y1, float x2, float y2,
                 float thickness, EffectColor color);
    void (*circle)(void *user, float x, float y, float radius,
                   EffectColor color);
    void (*rectangle)(void *user, float x, float y, float width, float height,
                      EffectColor color);
} EffectCanvas;

const char *EffectPaletteName(EffectPalette palette);
const char *EffectPaletteCommand(EffectPalette palette);
bool EffectPaletteFromCommand(const char *command, EffectPalette *palette);
EffectColor EffectPaletteColor(EffectPalette palette, float hue,
                               float saturation, float value, int alpha);

void EffectDrawSphere(const EffectCanvas *canvas, float time);
void EffectDrawVortex(const EffectCanvas *canvas, float time);
void EffectDrawWaves(const EffectCanvas *canvas, float time);
void EffectDrawAmbience(const EffectCanvas *canvas, float time);
void EffectDrawSpectrum(const EffectCanvas *canvas, float time);

#endif
