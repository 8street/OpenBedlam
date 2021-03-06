

#include "core.h"
#include "ddraw_func.h"
#include "game_level.h"
#include "helper.h"
#include "mouse.h"
#include "window.h"


int16_t CURSOR_X1;
int16_t CURSOR_Y1;

int32_t CURSOR_ICON;
int32_t CURSOR_ICON2;

int16_t CURSOR_SURFACE_IS_LOCKED;

uint16_t CURSOR_SURFACE_SIZE;

uint8_t UPDATE_CURSOR_BY_TIMER;
uint8_t CURSOR_BY_TIMER;

int32_t CURSOR_POS_X;
int32_t CURSOR_POS_Y;

int32_t CURSOR_POS_LCLICK_X;
int32_t CURSOR_POS_LCLICK_Y;
int32_t CURSOR_POS_RCLICK_X;
int32_t CURSOR_POS_RCLICK_Y;

uint32_t CURSOR_UNKNOWN;
uint8_t CURSOR_IS_BLITTING;

int32_t MOUSE_BUTTONS_STATE;
int32_t MOUSE_BUTTONS_STATE1;

uint8_t MOUSE_CLICK;

//00425AB9
void mouse_update()
{
    int icon; // eax
    LONG pos_x; // [esp+0h] [ebp-1Ch] BYREF
    LONG pos_y; // [esp+4h] [ebp-18h] OVERLAPPED BYREF
    uint32_t prev_state;
    uint32_t mouse_up = 0;

    //if ((MOUSE_BUTTONS_STATE1 & 2) != 0)
    //    mouse_r_click1 = 1;
    prev_state = MOUSE_CLICK;
    if (!MOUSE_CLICK)
    {
        if ((MOUSE_BUTTONS_STATE1 & 3) != 0)
        {
            if (mouse_up)
            {
                MOUSE_CLICK = 1;
                mouse_up = prev_state;
            }
        }
        else
        {
            mouse_up = 1;
        }
    }
    MOUSE_BUTTONS_STATE1 = MOUSE_BUTTONS_STATE;
    get_cursor_pos(&pos_x, &pos_y);
    pos_x += 9;
    pos_y += 9;
    if (pos_x < 9)
        pos_x = 9;
    if (pos_x > 631)
        pos_x = 631;
    if (pos_y < 9)
        pos_y = 9;
    if (pos_y > 463)
        pos_y = 463;
    CURSOR_POS_X = pos_x;
    CURSOR_POS_Y = pos_y;
    if ((MOUSE_BUTTONS_STATE1 & 1) != 0)
    {
        CURSOR_POS_LCLICK_X = CURSOR_POS_X;
        CURSOR_POS_LCLICK_Y = CURSOR_POS_Y;
    }
    if ((MOUSE_BUTTONS_STATE1 & 2) != 0)
    {
        CURSOR_POS_RCLICK_X = CURSOR_POS_X;
        CURSOR_POS_RCLICK_Y = CURSOR_POS_Y;
    }
    
    if (GAME_IS_PLAYING)
    {
        if (CURSOR_POS_X < 480) {
            icon = ICON_CROSSHAIR;
        }
        else {
            icon = ICON_CURSOR;
        }
        set_cursor_icon(icon);
    }
}

//0041D714
void set_cursor_icon(int32_t icon)
{
    uint8_t* cursor_surface; // ebp
    uint8_t* pos; // edi
    uint8_t* cursor_pos; // edx
    int i; // esi
    uint8_t* line; // eax
    int icon1; // [esp+0h] [ebp-20h]
    uint8_t* cursor_pos1; // [esp+4h] [ebp-1Ch]
    uint8_t* general;

    icon1 = icon;
    if (icon != CURSOR_ICON)
    {

        if (CURSOR_SURFACE_IS_LOCKED != 1)
        {
            CURSOR_SURFACE_IS_LOCKED = 1;
            set_cursor_surface_size(24);
            Set_Cursor_ColorKey(0);
            if (icon >= ICON_WAIT && icon < 152)
            {
                cursor_surface = lock_and_get_cursor_surface();
                if (!cursor_surface) {
                    return;
                }

            }
            else
            {
                cursor_surface = NULL;
                while (!cursor_surface) {
                    cursor_surface = lock_and_get_cursor_surface();
                }

            }
            clear_buffer(1024u, cursor_surface);
            pos = cursor_surface + 24;
            general = GENERAL_BIN.get_ptr();
            cursor_pos = &general[4 * icon1 + 8 + *(uint32_t*)&general[4 * icon1 + 2]];
            for (i = 0; i < 24; ++i)
            {
                cursor_pos1 = cursor_pos;
                line = &cursor_surface[32 * i];
                do {
                    *line++ = *cursor_pos++;
                } while (line != pos);
                pos += 32;
                cursor_pos = cursor_pos1 + 24;
            }
            unlock_cursor_surface();
            icon = icon1;
            CURSOR_ICON = icon1;
            CURSOR_ICON2 = icon1;
            CURSOR_SURFACE_IS_LOCKED = 0;
        }
    }
}

//0044BBAC
void set_cursor_surface_size(int16_t size)
{
    uint8_t by_timer;

    by_timer = UPDATE_CURSOR_BY_TIMER;
    if (UPDATE_CURSOR_BY_TIMER)
    {
        UPDATE_CURSOR_BY_TIMER = 0;
        blit_cursor_bg_to_screen();
    }
    CURSOR_SURFACE_SIZE = size;
    if (by_timer)
    {
        CURSOR_X1 = -1;
        UPDATE_CURSOR_BY_TIMER = 1;
    }
}


//0042391D
int draw_cursor_one_time()
{
    CURSOR_BY_TIMER = 0;
    return blit_cursor_one_time();
}

//0044B3F8
int blit_cursor_one_time()
{
    UPDATE_CURSOR_BY_TIMER = 0;
    return blit_cursor_bg_to_screen();
}

//0044B428
void get_cursor_pos(LONG* x, LONG* y)
{
    struct tagPOINT cursor_pos; // [esp+0h] [ebp-1Ch] BYREF

    GetCursorPos(&cursor_pos);
    cursor_pos.x -= WINDOW_POS_X;
    cursor_pos.y -= WINDOW_POS_Y;
    cursor_pos.x = cursor_pos.x * GAME_WIDTH / WINDOW_WIDTH;
    cursor_pos.y = cursor_pos.y * GAME_HEIGHT / WINDOW_HEIGHT;
    if (cursor_pos.x < 0)
        cursor_pos.x = 0;
    if (cursor_pos.y < 0)
        cursor_pos.y = 0;
    if (cursor_pos.x >= WINDOW_WIDTH)
        cursor_pos.x = WINDOW_WIDTH - 1;
    if (cursor_pos.y >= WINDOW_HEIGHT)
        cursor_pos.y = WINDOW_HEIGHT - 1;
    *x = cursor_pos.x;
    *y = cursor_pos.y;
}

//0042392D
void draw_cursor_by_timer()
{
    set_cursor_icon(CURSOR_ICON);
    CURSOR_BY_TIMER = 1;
    set_update_cursor_by_timer();
}

//0044B3D8
void set_update_cursor_by_timer()
{
    CURSOR_X1 = -1;
    UPDATE_CURSOR_BY_TIMER = 1;
}

//0041BF35
void mouse_buttons(uint16_t r_butt, uint16_t l_button)
{
    if (!r_butt && !l_button)
    {
        MOUSE_BUTTONS_STATE |= 1;
    }
    if (!r_butt && l_button == 1)
    {
        MOUSE_BUTTONS_STATE &= 0xFE;
    }
    if (r_butt == 1 && !l_button) 
    {
        MOUSE_BUTTONS_STATE |= 2u;
    }
    if (r_butt == 1 && l_button == 1) 
    {
        MOUSE_BUTTONS_STATE &= 0xFFFFFFFD;
    }
}
