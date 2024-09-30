#include <Windows.h>

int main() {

	DebugBreak();

	LPCSTR text = "Content Text";
	LPCSTR title = "Title";
	MessageBoxA(NULL, text, title, NULL);

	return 0;
}