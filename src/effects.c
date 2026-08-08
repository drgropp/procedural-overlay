#include "effects.h"

#include <math.h>

#define TAU 6.28318530717958647692f

static EffectColor rgba(int r, int g, int b, int a)
{
    EffectColor color = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
    return color;
}

static EffectColor hsv(float hue, float saturation, float value, int alpha)
{
    float h = hue - floorf(hue);
    float sector = h * 6.0f;
    int index = (int)floorf(sector);
    float fraction = sector - (float)index;
    float p = value * (1.0f - saturation);
    float q = value * (1.0f - saturation * fraction);
    float t = value * (1.0f - saturation * (1.0f - fraction));
    float r = value, g = t, b = p;

    switch (index % 6) {
        case 1: r = q; g = value; b = p; break;
        case 2: r = p; g = value; b = t; break;
        case 3: r = p; g = q; b = value; break;
        case 4: r = t; g = p; b = value; break;
        case 5: r = value; g = p; b = q; break;
        default: break;
    }
    return rgba((int)(r * 255.0f), (int)(g * 255.0f),
                (int)(b * 255.0f), alpha);
}

void EffectDrawSphere(const EffectCanvas *c, float time)
{
    const int count = 96;
    const float golden_angle = 2.3999632297f;
    float cx = c->width * 0.5f;
    float cy = c->height * 0.54f;
    float radius = fminf(c->width * 0.23f, c->height * 0.38f);
    float ay = time * 0.58f;
    float ax = 0.32f + sinf(time * 0.31f) * 0.15f;

    for (int i = 0; i < count; ++i) {
        float y = 1.0f - 2.0f * ((float)i + 0.5f) / (float)count;
        float ring = sqrtf(fmaxf(0.0f, 1.0f - y * y));
        float angle = golden_angle * (float)i;
        float x = cosf(angle) * ring;
        float z = sinf(angle) * ring;
        float rx = x * cosf(ay) + z * sinf(ay);
        float rz = -x * sinf(ay) + z * cosf(ay);
        float ry = y * cosf(ax) - rz * sinf(ax);
        rz = y * sinf(ax) + rz * cosf(ax);
        float wobble = 1.0f + 0.055f * sinf(time * 1.7f + (float)i * 1.91f);
        float perspective = 1.0f + rz * 0.16f;
        float size = 1.1f + (rz + 1.0f) * 1.15f;
        EffectColor color = hsv((float)i / (float)count + time * 0.035f,
                                0.72f, 1.0f, 105 + (int)((rz + 1.0f) * 70.0f));
        c->circle(c->user, cx + rx * radius * wobble * perspective,
                  cy + ry * radius * wobble * perspective, size, color);
    }
}

void EffectDrawVortex(const EffectCanvas *c, float time)
{
    const int rings = 12;
    const int sides = 6;
    float cx = c->width * 0.5f;
    float cy = c->height * 0.54f;
    float max_radius = fminf(c->width * 0.32f, c->height * 0.42f);

    for (int ring = 1; ring <= rings; ++ring) {
        float u = (float)ring / (float)rings;
        float radius = max_radius * powf(u, 0.82f);
        float spin = time * (1.28f - u * 0.78f) + u * 4.7f;
        EffectColor color = hsv(u * 0.72f - time * 0.06f, 0.68f, 1.0f,
                                190 - ring * 5);

        for (int side = 0; side < sides; ++side) {
            float a = spin + TAU * (float)side / (float)sides;
            float b = spin + TAU * (float)(side + 1) / (float)sides;
            c->line(c->user, cx + cosf(a) * radius, cy + sinf(a) * radius,
                    cx + cosf(b) * radius, cy + sinf(b) * radius,
                    1.0f + (1.0f - u) * 1.2f, color);
        }
        if (ring < rings) {
            float next_u = (float)(ring + 1) / (float)rings;
            float next_radius = max_radius * powf(next_u, 0.82f);
            float next_spin = time * (1.28f - next_u * 0.78f) + next_u * 4.7f;
            c->line(c->user, cx + cosf(spin) * radius, cy + sinf(spin) * radius,
                    cx + cosf(next_spin) * next_radius,
                    cy + sinf(next_spin) * next_radius, 1.2f, color);
        }
    }
}

void EffectDrawWaves(const EffectCanvas *c, float time)
{
    const int wave_count = 5;
    const int segments = 72;
    float center = c->height * 0.55f;

    for (int wave = 0; wave < wave_count; ++wave) {
        float offset = ((float)wave - 2.0f) * 8.0f;
        float amplitude = 16.0f + (float)wave * 2.5f;
        float frequency = 1.45f + (float)wave * 0.17f;
        float phase = time * (1.15f + (float)wave * 0.11f) + (float)wave * 0.9f;
        EffectColor color = hsv((float)wave / (float)wave_count + 0.56f,
                                0.64f, 1.0f, 180);

        for (int segment = 0; segment < segments; ++segment) {
            float x1 = c->width * (float)segment / (float)segments;
            float x2 = c->width * (float)(segment + 1) / (float)segments;
            float a1 = TAU * frequency * x1 / c->width + phase;
            float a2 = TAU * frequency * x2 / c->width + phase;
            float y1 = center + offset + sinf(a1) * amplitude
                       + sinf(a1 * 0.53f - time) * 3.0f;
            float y2 = center + offset + sinf(a2) * amplitude
                       + sinf(a2 * 0.53f - time) * 3.0f;
            c->line(c->user, x1, y1, x2, y2, 1.7f, color);
        }
    }
}

void EffectDrawAmbience(const EffectCanvas *c, float time)
{
    const int lights = 9;

    for (int i = 0; i < lights; ++i) {
        float seed = (float)i * 2.173f;
        float x = c->width * (0.5f + 0.42f * sinf(seed + time * (0.055f + i * 0.003f)));
        float y = c->height * (0.5f + 0.38f * cosf(seed * 1.37f + time * 0.071f));
        float radius = 8.0f + 8.0f * (0.5f + 0.5f * sinf(seed * 2.1f));
        EffectColor glow = hsv((float)i / (float)lights + time * 0.012f,
                               0.58f, 1.0f, 12);
        EffectColor core = glow;
        core.a = 80;

        for (int layer = 5; layer >= 1; --layer) {
            EffectColor layer_color = glow;
            layer_color.a = (uint8_t)(7 + (5 - layer) * 3);
            c->circle(c->user, x, y, radius + (float)layer * 6.5f, layer_color);
        }
        c->circle(c->user, x, y, radius, core);
    }
}

void EffectDrawSpectrum(const EffectCanvas *c, float time)
{
    const int bars = 30;
    float gap = 1.5f;
    float bar_width = c->width / (float)bars;
    float baseline = c->height * 0.86f;

    for (int i = 0; i < bars; ++i) {
        float u = (float)i / (float)(bars - 1);
        float pulse = 0.50f + 0.22f * sinf(time * 2.4f + (float)i * 0.58f)
                      + 0.16f * sinf(time * 1.13f - (float)i * 0.31f);
        float height = 16.0f + fmaxf(0.0f, pulse) * c->height * 0.60f;
        EffectColor color = hsv(u + time * 0.045f, 0.78f, 1.0f, 215);
        EffectColor reflection = color;
        reflection.a = 32;
        c->rectangle(c->user, (float)i * bar_width + gap * 0.5f,
                     baseline - height, bar_width - gap, height, color);
        c->rectangle(c->user, (float)i * bar_width + gap * 0.5f,
                     baseline + 2.0f, bar_width - gap, height * 0.12f, reflection);
    }
}
