// SocialNetwork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<cstring>
#include"MyNetwork.h"
#include"input.h"
using namespace std;
void d(istream& in)
{
	cout << "good= " << in.good()
		<< "\neof= " << in.eof()
		<< "\nfail= " << in.fail()
		<< "\nbad= " << in.bad() << endl;
}
void d(ostream& out)
{
	cout << "good= " << out.good()
		<< "\neof= " << out.eof()
		<< "\nfail= " << out.fail()
		<< "\nbad= " << out.bad() << endl;
}

extern const char* namesFileString;
bool areWeFriends(const char* username1, const char* username2);
bool do_I_Have_A_Friend_Request(const char* username1, const char* username2);
int _tmain(int argc, _TCHAR* argv[])
{
	
	//ofstream(namesFileString, ios::trunc);
	myNetwork();
	return 0;
}