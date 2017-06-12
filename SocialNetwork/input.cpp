#include "stdafx.h"
#include <iostream>
#include <cstring>
using std::cin;

//тази ф-я пренасочва ptr към нов указател,
//сочещ към конкатенацията на низовете ptr и buffer
//паметта, към която сочи е динамична
static void update(char *&ptr, const char *buffer)
{
	int len1 = strlen(ptr);
	int len2 = strlen(buffer);

	char* newPtr = new char[len1 + len2 + 1];
	strncpy(newPtr, ptr, len1);
	strncpy(&newPtr[len1], buffer, len2);
	newPtr[len1 + len2] = '\0';
	//сега вече newPtr сочи към конкатенацията на ptr и buffer 

	delete[] ptr;
	ptr = newPtr;
}

//насочва ptr към текст, въведен от клавиатурата
//Текстът се съхранява в динамичнара памет и 
//приключва тогава, когато се въведе enter
void input(char *&ptr)
{
	//присвояваме на ptr динамична памет, сочеща към празния низ,
	//за да можем да използваме strlen и оператора delete [] 
	ptr = new char[1];
	ptr[0] = '\0';

	//buffer временно съхранява нововъведените символи, 
	//след което ги добавя към ptr
	const int bufSize = 5;
	char buffer[bufSize];
	
	do
	{
		cin.get(buffer, bufSize);
		update(ptr, buffer);
	} while (cin.gcount());

	//изчистваме cin, в случай, че е въведен само символът '\n'
	cin.clear();

	//махаме '\n' от потока
	cin.ignore();
}