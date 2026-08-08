#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdint.h>

typedef struct EffectColor {
    uint8_t r, g, b, a;
} EffectColor;

typedef struct EffectCanvas {
    float width;
    float height;
    void *user;
    void (*line)(void *user, float x1, float y1, float x2, float y2,
                 float thickness, EffectColor color);
    void (*circle)(void *user, float x, float y, float radius,
                   EffectColor color);
    void (*rectangle)(void *user, float x, float y, float width, float height,
                      EffectColor color);
} EffectCanvas;

void EffectDrawSphere(const EffectCanvas *canvas, float time);
void EffectDrawVortex(const EffectCanvas *canvas, float time);
void EffectDrawWaves(const EffectCanvas *canvas, float time);
void EffectDrawAmbience(const EffectCanvas *canvas, float time);
void EffectDrawSpectrum(const EffectCanvas *canvas, float time);

#endif
