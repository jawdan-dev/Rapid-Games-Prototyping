#pragma once

#include "goblinEngine.rendering.sprite.hpp"
#include "goblinEngine.rendering.text.hpp"
#include "goblinEngine.rendering.panel.hpp"

#include "6x6.h"
#include "test.h"
#include "panel.h"
#include "background.h"
#include "map.h"
#include "goblins.h"
#include "cards.h"
#include "attack.h"

static Sprite spr_test(testBitmap, 32, 32);
static TextSprite spr_6x6(_x6Bitmap, _x6Pal, 16, 60, 60, 6, 6);
static PanelSprite spr_panel(panelBitmap, panelPal, 16, 30, 120);
static Sprite spr_map(mapBitmap, mapPal, 16, 256, 192, false);
static Sprite spr_background(backgroundBitmap, backgroundPal, 16, 256, 192 * 2, 256, 192, false);
static Sprite spr_goblins(goblinsBitmap, goblinsPal, 16, 64, 64, 16, 16);
static Sprite spr_cards(cardsBitmap, cardsPal, 16, 64, 64, 16, 16);
static Sprite spr_attack(attackBitmap, attackPal, 16, 32, 32, 16, 16);