#pragma once
#include <stdint.h>
#include <Windows.h>

extern HWND WINDOW_HWND;
extern uint8_t APPLICATION_ACTIVE;
extern uint8_t NEED_EXIT;
extern int32_t WINDOW_POS_X;
extern int32_t WINDOW_POS_Y;
extern int32_t WINDOW_WIDTH;
extern int32_t WINDOW_HEIGHT;

int init_window(HINSTANCE hwnd, HINSTANCE prev_hwnd, LPSTR lp_cmd_line);
LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WPARAM message_dispatcher();
void destroy();
LRESULT send_destroy_message();
