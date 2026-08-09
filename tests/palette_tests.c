#include "effects.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

static void TestCommands(void)
{
    static const char *const commands[EFFECT_PALETTE_COUNT] = {
        ":rainbow", ":red", ":orange", ":yellow",
        ":green", ":blue", ":indigo", ":violet",
        ":cyan", ":magenta", ":white", ":amber",
        ":pink", ":terminal", ":ice", ":fire"
    };

    for (int i = 0; i < EFFECT_PALETTE_COUNT; ++i) {
        EffectPalette parsed = EFFECT_PALETTE_COUNT;
        assert(strcmp(EffectPaletteCommand((EffectPalette)i), commands[i]) == 0);
        assert(EffectPaletteFromCommand(commands[i], &parsed));
        assert(parsed == (EffectPalette)i);
        assert(strcmp(EffectPaletteName(parsed), "Unknown") != 0);
    }

    EffectPalette unchanged = EFFECT_PALETTE_BLUE;
    assert(!EffectPaletteFromCommand(":purplee", &unchanged));
    assert(unchanged == EFFECT_PALETTE_BLUE);
    assert(!EffectPaletteFromCommand(NULL, &unchanged));
    assert(!EffectPaletteFromCommand(":red", NULL));
}

static void TestColorMapping(void)
{
    EffectColor rainbow = EffectPaletteColor(
        EFFECT_PALETTE_RAINBOW, 0.0f, 1.0f, 1.0f, 123);
    assert(rainbow.r == 255);
    assert(rainbow.g == 0);
    assert(rainbow.b == 0);
    assert(rainbow.a == 123);

    for (int i = EFFECT_PALETTE_RED; i < EFFECT_PALETTE_COUNT; ++i) {
        EffectColor darker = EffectPaletteColor(
            (EffectPalette)i, 0.0f, 0.1f, 1.0f, 37);
        EffectColor lighter = EffectPaletteColor(
            (EffectPalette)i, 0.25f, 1.0f, 1.0f, 37);
        assert(darker.r != lighter.r ||
               darker.g != lighter.g ||
               darker.b != lighter.b);
        assert(darker.a == 37);
        assert(lighter.a == 37);
    }

    EffectColor cyan = EffectPaletteColor(
        EFFECT_PALETTE_CYAN, 0.25f, 1.0f, 1.0f, 255);
    assert(cyan.g > cyan.r && cyan.b > cyan.r);

    EffectColor magenta = EffectPaletteColor(
        EFFECT_PALETTE_MAGENTA, 0.25f, 1.0f, 1.0f, 255);
    assert(magenta.r > magenta.g && magenta.b > magenta.g);

    EffectColor white = EffectPaletteColor(
        EFFECT_PALETTE_WHITE, 0.25f, 1.0f, 1.0f, 255);
    int white_min = white.r < white.g ? white.r : white.g;
    int white_max = white.r > white.g ? white.r : white.g;
    if (white.b < white_min) white_min = white.b;
    if (white.b > white_max) white_max = white.b;
    assert(white_max - white_min <= 16);

    EffectColor terminal = EffectPaletteColor(
        EFFECT_PALETTE_TERMINAL, 0.25f, 1.0f, 1.0f, 255);
    assert(terminal.g > terminal.r && terminal.g > terminal.b);

    EffectColor ice = EffectPaletteColor(
        EFFECT_PALETTE_ICE, 0.25f, 1.0f, 1.0f, 255);
    assert(ice.g > ice.r && ice.b > ice.r);

    EffectColor fire_red = EffectPaletteColor(
        EFFECT_PALETTE_FIRE, 0.0f, 1.0f, 1.0f, 255);
    EffectColor fire_amber = EffectPaletteColor(
        EFFECT_PALETTE_FIRE, 0.75f, 1.0f, 1.0f, 255);
    assert(fire_red.r > fire_red.g && fire_red.g == fire_red.b);
    assert(fire_amber.r > fire_amber.g && fire_amber.g > fire_amber.b);
    assert(fire_amber.g > fire_red.g);
}

int main(void)
{
    TestCommands();
    TestColorMapping();
    return 0;
}
