#include <Windows.h>

int main() {

	LPCSTR text = "Content";
	LPCSTR title = "Title";
	MessageBoxA(NULL, text, title, NULL);

	return 0;
}