#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    MessageBoxA(0, "Hello Title", "Hello World!", MB_OK | MB_ICONINFORMATION);
}