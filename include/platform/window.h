#pragma once

struct Window;

Window* InitWindow(const char* title);
const char* GetWindowTitle(Window* window);