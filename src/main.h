#pragma once
#include <stdint.h>
#include <Windows.h>



extern DWORD GAME_THREAD_ID;
extern HANDLE HANDLE_GAME_THREAD;


int16_t game_thread_create();
void game_thread_start_func();
void game_thread_exit();