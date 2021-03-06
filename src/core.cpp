
#include "briefing_room.h"
#include "core.h"
#include "ddraw_func.h"
#include "draw_img.h"
#include "game_level.h"
#include "levels.h"
#include "main_menu.h"
#include "map_room.h"
#include "mouse.h"
#include "palette.h"
#include "shop_room.h"

Rnd RND1;
Rnd RND2;

uint8_t BACKGROUND_BUF[307200u];

BIN_File GENERAL_BIN;
BIN_File LOAD_US_BIN;
BIN_File FULLFONT_BIN;

File LOADPALU_PAL;
File FULLPAL_PAL;

uint8_t DIFFICULTY;

std::string PLAYER_NAME("PLAYER");

int32_t GAME_MODE;

//0041C050
int32_t game_core() {

    int32_t entry = MAIN_MENU;

    // needs for cursor
    GENERAL_BIN.load("GAMEGFX/GENERAL.BIN");
    LOADPALU_PAL.load("GAMEGFX/LOADPALU.PAL");

    // draw waiting cursor
    set_palette(LOADPALU_PAL.get_ptr());
    set_cursor_icon(ICON_WAIT);
    draw_cursor_by_timer();

    // load files here

    LOAD_US_BIN.load("GAMEGFX/LOAD_US.BIN");
    FULLFONT_BIN.load("GAMEGFX/FULLFONT.BIN");

    FULLPAL_PAL.load("GAMEGFX/FULLPAL.PAL");

    init_zone_mision_arr();

    while (entry) {
        switch (entry)
        {
        case MAIN_MENU:
            entry = main_menu();
            break;
        case MAP_ROOM:
            entry = map_room();
            break;
        case SHOP_ROOM:
            entry = shop_room();
            break;
        case BRIEFING_ROOM:
            entry = briefing_room();
            break;
        case GAME_LEVEL:
            entry = game_level();
            break;
        default:
            entry = CLOSE_GAME;
            break;
        }
    }

    return 0;
}

