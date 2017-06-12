#include "stdafx.h"
#include<iostream>
#include<iomanip>
#include<fstream>
#include"input.h"
#include<cstring>
#include<cstdlib>
using namespace std;

//Възможни резултати, които връща ф-ята
//validateUsernameAndPassword, след като е извършила 
//търсене на username и password във файл
enum SearchResult
{
	SOMETHING_ELSE,
	USERNAME_OK_PASSWORD_OK,
	USERNAME_OK_PASSWORD_NO,
	USERNAME_NO,
	SEARCH_RESULT_COUNT
};

enum FunctionResult
{
	ERROR,
	EXIT,
	NORMAL
};

//команди след влизане в мрежата
const char* registrateString = "registrate";
const char* logInString = "log in";
const char *exitString = "exit";

//команди след влизане в профила 
const char* listFriendsString = "list friends";
const char* writeToFriendString = "write to friend";
const char* deleteFriendString = "remove friend";
const char* listFriendrequestsString = "list friendrequests";
const char* acceptFrendrequestString = "accept friendrequest";
const char* deleteFriendrequestString = "delete friendrequest";
const char* sendFriendrequestString = "send friendrequest";

//име на документа, съдържащ всички имена и регистрации
const char* namesFileString = "B:\\c++\\SocialNetwork\\SocialNetwork\\data\\names.bin"; 

//име на мястото, където ще се съхраняват списъците с приятели
const char* friendsListLocationString = "B:\\c++\\SocialNetwork\\SocialNetwork\\data\\friends";

//име на мястото, където ще се съхраняват списъците с молби за приятелство
const char* friendsrequestsListLocationString = "B:\\c++\\SocialNetwork\\SocialNetwork\\data\\friend_requests";


//Декларации на функции
////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//Връща позицията, от която започва информацията за
//username2, в списъка с приятели на username1
//Връща -1, ако не са приятели
int am_I_Friend_With_You(const char* username1, const char* username2);

//Връща позицията, от която започва информацията за 
//username2 в спъсъка с покани за приятелство на username1
//Връща -1, ако username2, не е изпращал
//покана на username1
int do_I_Have_A_Friend_Request_From(const char* username1, const char* username2);

bool listFriendrequests(const char* username);

//Извежда всички приятели на username         //checked                 Все още не е тествана!!!
bool listFriends(const char* username);

//Добавя името на username2 в края на списъка
//с молби за приятелство на username1 
bool sendFriendRequestToFrom(const char* username1, const char* username2);

//username ще се опита да изпрати покана
//за приятелство на въведен потребител potentialFriendName
bool tryToSendFriendRequest(const char* username);

//Връща резултат, зависещ от това,
//дали потребителят е съгласен с условието question
static bool chooseFlag(const char* question);

//Извежда началното съобщение при влизане 
//в мрежата на екрана на потребителя
static void printStart();

//Извежда начално съобщение при влизане
//в профила на потребителя с име username
static void printLogIn();

//Създава пълно име на файл в директория fileLocation
//с име fileName
//и с разширение .bin
//Името сочи към динамична памет!!!
char* makeBinaryFileName(const char* fileName, const char* fileLocation);

//Създава файл с пълно име fullFileName
bool makeBinaryFile(const char* fullFileName);

//От клавиатурата се въвеждат ст-ти на username и за password
//Паметта, към която сочат те е динамична!!!
static void enterUsernameAndPassword(char *&username, char *&password);

//Проверява дали в списъка с имена и пароли се срещат дадено име и парола
static SearchResult validateUsernameAndPassword(const char *username, const char *password);

void deleteUsernameAndPassword(const char* username, const char* password);

static FunctionResult validateProfileCommand(const char* username, const char* profileCommand);

//Тази ф-я стартира, след като потребителят е въвел
//правилни име и парола и влезе в профила си
FunctionResult logIn(const char *username);

//Записва username и password в списъка с имена и пароли
bool registrate(const char* username, const char* password);

//Взависимост от това, дали въведеното име пръсъства в спъсъка с имена,
//функцията или регистрира потребителя, или му предлага да опита пак
bool tryToRegistrate();

//Взависимост от това, дали въведените име и парола присъстват
//в списъка с пароли и имена, ф-ята или вкарва потребителя в 
//профила му, или го изхвърля от MyNetwork
FunctionResult tryToLogIn();

//В зависимост от ст-тта на startCommmand има 4 изхода
//1) регистрация
//2) влизане в профила
//3) изход, понеже е въведена команда за изход
//4) изход, понеже е въведена грешна команда
FunctionResult validateStartCommand(const char* startCommand);

FunctionResult myNetwork();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Край на декларации на ф-ции


//Извежда началното съобщение при влизане 
//в мрежата на екрана на потребителя
static void printStart()	//checked
{
	cout << "\t\tWelcome to MyNetwork\n\n"
		<< "Please, select one of the following three options\n"
		<< setw(15) << registrateString
		<< setw(15) << logInString
		<< setw(15) << exitString << "\n\n";
}

//Извежда начално съобщение при влизане
//в профила на потребителя с име username
static void printLogIn()	//checked
{

	cout << "You can choose one of the following commands:\n\n"
		<< listFriendsString << "\n\n"
		<< writeToFriendString << "\n\n"
		<< deleteFriendString << "\n\n"
		<< listFriendrequestsString << "\n\n"
		<< acceptFrendrequestString << "\n\n"
		<< deleteFriendrequestString << "\n\n"
		<< sendFriendrequestString << "\n\n"
		<< exitString << "\n\n";
}


//Връща резултат, зависещ от това,
//дали потребителят е съгласен с условието question
static bool chooseFlag(const char* question)	//checked
{
	cout << question << "  :  (y/n)\n";
	char answer;
	answer = cin.get();
	cin.ignore(10000, '\n');
	return (answer == 'Y' || answer == 'y') ? 1 : 0;
}

//От клавиатурата се въвеждат ст-ти на username и за password
//Паметта, към която сочат те е динамична!!!
static void enterUsernameAndPassword(char *&username, char *&password)	//checked
{
	cout << "Enter username: ";
	input(username);
	cout << "Enter password: ";
	input(password);
}

//От клавиатурата се въвежда ст-т на username
static void enterName(char *&username)
{
	cout << "Enter a name: ";
	input(username);
}


void deleteUsernameAndPassword(const char* username, const char* password)
{
	delete[] username;
	delete[] password;
}

//Проверява дали в списъка с имена и пароли се срещат дадено име и парола
static SearchResult validateUsernameAndPassword(const char *username, const char *password)	//checked
{
	size_t usernameLen;
	size_t passwordLen;
	//присвояваме им динамична памет, за да можем да ги изтрием
	char* usernameBuffer = new char[1];
	char* passwordBuffer = new char[1];

	ifstream inputFile(namesFileString, ios::binary);
	if (!inputFile)
	{
		deleteUsernameAndPassword(usernameBuffer, passwordBuffer);
		cerr << "Could not open " << namesFileString << " for reading\n";
		return SOMETHING_ELSE;
	}

	while (inputFile.good())
	{
		deleteUsernameAndPassword(usernameBuffer, passwordBuffer);

		inputFile.read((char*)&usernameLen, sizeof(size_t));
		if (inputFile.eof())
		{
			inputFile.close();
			return USERNAME_NO;
		}
		else
		{
			usernameBuffer = new char[usernameLen + 1];
			inputFile.read(usernameBuffer, usernameLen);
			usernameBuffer[usernameLen] = '\0';

			inputFile.read((char*)&passwordLen, sizeof(size_t));

			passwordBuffer = new char[passwordLen + 1];
			inputFile.read(passwordBuffer, passwordLen);
			passwordBuffer[passwordLen] = '\0';

			if (strcmp(username, usernameBuffer) == 0)
			{
				if (strcmp(password, passwordBuffer) == 0)
				{
					deleteUsernameAndPassword(usernameBuffer, passwordBuffer);
					inputFile.close();
					return USERNAME_OK_PASSWORD_OK;
				}
				else
				{
					deleteUsernameAndPassword(usernameBuffer, passwordBuffer);
					inputFile.close();
					return USERNAME_OK_PASSWORD_NO;
				}
			}

		}
	}
}


//Редактира, файла с име fullFileName,
//като изтрива len символа, започващи 
//от позиция pos
bool deletePartOfFile(const char* fullFilename, int pos, int len)	//ок
{
	ifstream inputFile(fullFilename, ios::binary);
	if (!inputFile)
	{
		cerr << "Could not open " << fullFilename << " for reading\n";
		return false;
	}
	int lengthOfFile;
	inputFile.seekg(0, ios::end);
	lengthOfFile = inputFile.tellg();
	inputFile.seekg(0, ios::beg);

	char* fileContentBuffer = new char[lengthOfFile];
	inputFile.read(fileContentBuffer, lengthOfFile);

	inputFile.close();

	//Файлът не се отваря в режим, ios::app
	//за, да може старото съдържание да се изтрие
	//и новото редактирано съдържание да се запише
	//в/у файла
	ofstream outputFile(fullFilename, ios::binary);
	if (!outputFile)
	{
		cerr << "Could not open " << fullFilename << " for writing\n";
		return false;
	}
	//Записва символите с индекси от
	//[0, pos) във файла
	outputFile.write(fileContentBuffer, pos);

	//Записва символите с индекси от
	//[pos+len, lengthOfFile) във файла
	outputFile.write(&fileContentBuffer[pos + len], lengthOfFile - (pos + len));

	return outputFile.good();
}

//Връща позицията, от която започва информацията за
//username2, в списъка с приятели на username1
//Връща -1, ако не са приятели
int am_I_Friend_With_You(const char* username1, const char* username2)		//checked
{
	const char* username1FullAddress = makeBinaryFileName(username1, friendsListLocationString);

	ifstream inputFile(username1FullAddress, ios::binary);
	if (!inputFile)
	{
		cerr << "Could not open " << username1FullAddress << " for reading\n";
		delete[] username1FullAddress;
		return -1;
	}
	delete[] username1FullAddress;

	//Това е форматът на файловете с приятелски съобщения
	size_t usernameLen;
	char *usernameBuffer = new char [1];
	size_t messageNumber;
	size_t messageLen;

	// 1 ако съобщението е на потребителя, който притежава файла
	// else 0
	//дължината на messageAutor не влиза в messageLen !!!
	bool messageAutor;		
	char *messageBuffer = new char[1];

	//Това е позицията, от която ще започва информацията за приятеля, който се търси
	int pos = 0;

	while (inputFile.good())
	{
		inputFile.read((char*)&usernameLen, sizeof(size_t));
		

		if (inputFile.eof())
		{
			deleteUsernameAndPassword(usernameBuffer, messageBuffer);
			inputFile.close();
			return -1;
		}

		else
		{
			delete[] usernameBuffer;
			usernameBuffer = new char[usernameLen + 1];
			inputFile.read(usernameBuffer, usernameLen);
			usernameBuffer[usernameLen] = '\0';

			//ако това е името, което търсим
			if (strcmp(usernameBuffer, username2) == 0)
			{
				deleteUsernameAndPassword(usernameBuffer, messageBuffer);
				return pos;
			}
			else
			{
				pos += sizeof(usernameLen)+usernameLen;

				inputFile.read((char*)&messageNumber, sizeof(size_t));

				pos += sizeof(messageNumber);

				for (int i = 0; i < messageNumber; ++i)
				{
					inputFile.read((char*)&messageLen, sizeof(size_t));

					pos += sizeof(messageLen);

					inputFile.read((char*)&messageAutor, sizeof(bool));

					pos += sizeof(messageAutor);

					delete[] messageBuffer;
					messageBuffer = new char[messageLen + 1];
					inputFile.read(messageBuffer, messageLen);

					pos += messageLen;

					messageBuffer[messageLen] = '\0';
				}
			}
		}
	}
}

//Връща позицията, от която започва информацията за 
//username2 в спъсъка с покани за приятелство на username1
//Връща -1, ако username2, не е изпращал
//покана на username1
int do_I_Have_A_Friend_Request_From(const char* username1, const char* username2)	//checked
{
	const char* username1FullAddress = makeBinaryFileName(username1, friendsrequestsListLocationString);

	ifstream inputFile(username1FullAddress, ios::binary);
	if (!inputFile)
	{
		cerr << "Could not open " << username1FullAddress << " for reading\n";
		delete[] username1FullAddress;
		return -1;
	}
	delete[] username1FullAddress;

	//Позиция, от която започва информацията за преиятеля username2
	int pos = 0;

	size_t usernameLen;
	char* usernameBuffer = new char[1];

	while (inputFile.good())
	{
		inputFile.read((char*)&usernameLen, sizeof(size_t));
		
		delete[] usernameBuffer;

		if (inputFile.eof())
		{
			inputFile.close();
			return -1;
		}
		else
		{
			usernameBuffer = new char[usernameLen+1];
			inputFile.read(usernameBuffer, usernameLen);
			usernameBuffer[usernameLen] = '\0';

			//ако това е името, което търсим
			if (strcmp(usernameBuffer, username2)==0)
			{
				inputFile.close();
				delete[] usernameBuffer;
				return pos;
			}
			//ако ли не
			else
				pos += sizeof(usernameLen)+usernameLen;
		}
	}
}


//Създава пълно име на файл в директория fileLocation
//с име fileName
//и с разширение .bin
//Името сочи към динамична памет!!!
char* makeBinaryFileName(const char* fileName, const char* fileLocation)	//checked
{
	const char* extention = ".bin";
	const char* connection = "\\";

	int len1 = strlen(fileLocation);
	int len2 = strlen(connection);
	int len3 = strlen(fileName);
	int len4 = strlen(extention);

	char* fullFileName = new char[len1 + len2 + len3 + len4 + 1];

	strncpy(&fullFileName[0], fileLocation, len1);
	strncpy(&fullFileName[len1], connection, len2);
	strncpy(&fullFileName[len1 + len2], fileName, len3);
	strncpy(&fullFileName[len1 + len2 + len3], extention, len4);
	fullFileName[len1 + len2 + len3 + len4] = '\0';

	return fullFileName;
}

//Създава файл с пълно име fullFileName
bool makeBinaryFile(const char* fullFileName)	//checked
{
	ofstream outputFile(fullFileName, ios::out);
	if (!outputFile)
	{
		cerr << "Could not open " << fullFileName << " for writing\n";
		return 0;
	}
	else
	{
		cout << fullFileName << " has been successfully created\n";
		outputFile.close();
		return 1;
	}
}


//Връща дължината на информацията за поканата за приятество
//във файл fullFileName, започваща от позиция pos
//връща -1 при грешка
int findLengthOfFriendRequest(const char* fullFilename, int pos)	//ok
{
	int len = -1;
	ifstream inputFile(fullFilename, ios::binary);

	if (!inputFile)
	{
		cerr << "Could not open " << fullFilename << " for reading\n";
		return -1;
	}

	inputFile.seekg(pos, ios::beg);

	size_t friendLen;
	inputFile.read((char*)&friendLen, sizeof(size_t));

	len = sizeof(friendLen)+friendLen;

	return len;
}

//Връща дължината на информацията за приятеля,
//във файл fullFileName, започваща от позиция pos.
//Връща -1 при грешка
int findLengthOfFriendInfo(const char* fullFilename, const int pos)
{
	//len е дължината, която
	//ще връща ф-ята
	int len = -1;
	ifstream inputFile(fullFilename, ios::binary);

	if (!inputFile)
	{
		cerr << "Could not open " << fullFilename << " for reading\n";
		return -1;
	}

	size_t friendLen;
	//char* friendNameBuffer;
	size_t messageNumber;
	size_t mesageLen;
	//bool autor;
	//char* messageBuffer;

	inputFile.seekg(pos, ios::beg);

	inputFile.read((char*)&friendLen, sizeof(size_t));

	//Вместо да прочитаме името на приятела,
	//само ще го прескочим. То не ни интересува
	inputFile.seekg(friendLen, ios::cur);

	inputFile.read((char*)&messageNumber, sizeof(size_t));

	len = sizeof(friendLen)+friendLen + sizeof(messageNumber);

	for (int i = 0; i < messageNumber; ++i)
	{
		inputFile.read((char*)&mesageLen, sizeof(size_t));

		//Вместо да прочитаме автора на съобщението,
		//само ще го прескочим. Той не ни интересува
		inputFile.seekg(sizeof(bool), ios::cur);

		//Вместо да прочитаме съобщението,
		//само ще го прескочим. То не ни интересува
		inputFile.seekg(mesageLen, ios::cur);

		len += sizeof(mesageLen)+sizeof(bool)+mesageLen;
	}

	if (inputFile.good() == true)
	{
		return len;
	}
	else
	{
		cerr << "An error occured while reading from " << fullFilename << '\n';
		return -1;
	}
}



bool listFriendrequests(const char* username)	//checked
{
	const char* usernameFullAddress = makeBinaryFileName(username, friendsrequestsListLocationString);

	ifstream inputFile(usernameFullAddress, ios::binary);
	if (!inputFile)
	{
		cerr << "Could not open " << usernameFullAddress << " for reading\n";
		delete[] usernameFullAddress;
		//logIn(username);
		return 0;
	}
	delete[] usernameFullAddress;

	size_t usernameLen;
	char* usernameBuffer = new char[1];

	while (inputFile.good())
	{
		inputFile.read((char*)&usernameLen, sizeof(size_t));

		delete[] usernameBuffer;

		if (inputFile.eof())
		{
			inputFile.close();
			system("pause");
			return 1;
		}
		else
		{
			usernameBuffer = new char[usernameLen + 1];
			inputFile.read(usernameBuffer, usernameLen);
			usernameBuffer[usernameLen] = '\0';

			cout << usernameBuffer << endl;
		}
	}
}

//Извежда всички приятели на username
bool listFriends(const char* username)
{
	const char* usernameFullAddress = makeBinaryFileName(username, friendsListLocationString);

	ifstream inputFile(usernameFullAddress, ios::binary);
	if (!inputFile)
	{
		cerr << "Could not open " << usernameFullAddress << " for reading\n";
		delete[] usernameFullAddress;
		return 0;
	}
	delete[] usernameFullAddress;

	//Това е форматът на файловете с приятелски съобщения
	size_t usernameLen;
	char *usernameBuffer = new char [1];
	size_t messageNumber;
	size_t messageLen;

	// 1 ако съобщението е на потребителя, който притежава файла
	// else 0
	//дължината на messageAutor не влиза в messageLen !!!
	bool messageAutor;		
	char *messageBuffer = new char[1];

	while (inputFile.good())
	{
		inputFile.read((char*)&usernameLen, sizeof(size_t));
		

		if (inputFile.eof())
		{
			deleteUsernameAndPassword(usernameBuffer, messageBuffer);
			inputFile.close();
			system("pause");
			return 1;
		}

		else
		{
			delete[] usernameBuffer;
			usernameBuffer = new char[usernameLen + 1];
			inputFile.read(usernameBuffer, usernameLen);
			usernameBuffer[usernameLen] = '\0';

			//Извежда името на приятеля
			cout << usernameBuffer << endl;

			inputFile.read((char*)&messageNumber, sizeof(size_t));
			for (int i = 0; i < messageNumber; ++i)
			{
				inputFile.read((char*)&messageLen, sizeof(size_t));

				inputFile.read((char*)&messageAutor, sizeof(bool));

				delete[] messageBuffer;
				messageBuffer = new char[messageLen + 1];
				inputFile.read(messageBuffer, messageLen);
				messageBuffer[messageLen] = '\0';
			}
		}
	}
}

//if autor=1, then username пише съобщение
//if autor=0, then другият пише съобщение
bool writeToFriend(const char* username,const char* message,const int usernameFriendListPosition, bool autor)
{
	const char* usernameFriendListFullName = makeBinaryFileName(username, friendsListLocationString);

	int lengthOfFriendInfo = findLengthOfFriendInfo(usernameFriendListFullName, usernameFriendListPosition);

	ifstream inputFile(usernameFriendListFullName, ios::binary);
	if (!inputFile)
	{
		cerr << "Could not open " << usernameFriendListFullName << " for reading\n";
		delete[]usernameFriendListFullName;
		return false;
	}

	inputFile.seekg(0, ios::end);
	int fileLength = inputFile.tellg();
	inputFile.seekg(0, ios::beg);

	//Низ, съдържащ цялото съдържание на файла,
	//преди да бъде редактиран
	char *fileContentBuffer = new char[fileLength];
	inputFile.read(fileContentBuffer, fileLength);
	inputFile.close();
	
	fstream ioFile(usernameFriendListFullName, ios::in | ios::out | ios::binary);
	if (!ioFile)
	{
		delete[] usernameFriendListFullName;
		delete[] fileContentBuffer;
		cerr << "Could not open " << usernameFriendListFullName << " for reading and writing\n";
		return false;
	}
	else
	{
		int readPosition;
		int writePosition;

		size_t nameLen;
		//char* nameBuffer;
		size_t messageNumber;
		size_t messageLen;
		//bool messageAutor;
		//char* messageBuffer;

		readPosition = usernameFriendListPosition;

		ioFile.seekg(readPosition, ios::beg);

		ioFile.read((char*)&nameLen, sizeof(size_t));

		//Прескачаме името на приятеля, вместо да го четем
		ioFile.seekg(nameLen, ios::cur);

		ioFile.read((char*)&messageNumber, sizeof(size_t));

		//Със съобщението, което ще запишем,
		//общият брой на съобщенията нараства с 1
		++messageNumber;
		
		//Променяме позицията за четене, т.ч.
		//да е в началото на messageNumber
		const int sizeofSize_t = sizeof(size_t);
		ioFile.seekg(-sizeofSize_t, ios::cur);

		//Променяме позицията за писане, т.ч.
		//да е в началото на messageNumber
		writePosition = ioFile.tellg();
		ioFile.seekp(writePosition, ios::beg);


		ioFile.write((char*)&messageNumber, sizeof(size_t));
		//Дотук сме променили само и единствено ст-тта на messageNumber във файла


		ioFile.seekp(usernameFriendListPosition + lengthOfFriendInfo, ios::beg);

		messageLen = strlen(message);

		ioFile.write((char*)&messageLen, sizeof(size_t));
		ioFile.write((char*)&autor, sizeof(bool));
		ioFile.write(message, messageLen);
		ioFile.write(&fileContentBuffer[usernameFriendListPosition + lengthOfFriendInfo],
			fileLength - (usernameFriendListPosition + lengthOfFriendInfo));

		delete[] usernameFriendListFullName;
		delete[] fileContentBuffer;

		return ioFile.good();
	}

}

//Извежда диалога м/у username и friendName
bool listDialog(const char* username, const char* friendName,const int usernameFriendListPosition)
{
	const char* usernameFriendListFullName = makeBinaryFileName(username, friendsListLocationString);

	ifstream inputFile(usernameFriendListFullName, ios::binary);

	if (!inputFile)
	{
		cerr << "Could not open " << usernameFriendListFullName << " for reading\n";
		delete[] usernameFriendListFullName;
		return false;
	}
	delete[] usernameFriendListFullName;

	//Това е форматът на файла с приятелите
	size_t friendLen;
	//По принцип friendBuffer съдържа дублираща се информация с friendName.
	//Ние обаче ще използваме това, за да проверим дали фактическите
	//параметри на ф-ята са коректни.
	char* friendBuffer;
	size_t messageNumber;
	size_t messageLen;
	bool autor;
	char* messageBuffer;

	//Стигаме до мастото във файла, откъдето започва информацията
	//за friendName
	inputFile.seekg(usernameFriendListPosition, ios::beg);
	
	inputFile.read((char*)&friendLen, sizeof(size_t));

	friendBuffer = new char[friendLen + 1];
	inputFile.read(friendBuffer, friendLen);
	friendBuffer[friendLen] = '\0';

	if (strcmp(friendBuffer, friendName) != 0)
	{
		delete[] friendBuffer;
		cerr << "The parameters, passed to the function listDialog are incorrect\n";
		return false;
	}
	else
	{
		delete[] friendBuffer;

		inputFile.read((char*)&messageNumber, sizeof(size_t));

		for (int i = 0; i < messageNumber; ++i)
		{
			inputFile.read((char*)&messageLen, sizeof(size_t));
			inputFile.read((char*)&autor, sizeof(bool));
			
			cout << ((autor == 1) ? username : friendName) << ": ";

			messageBuffer = new char[messageLen+1];

			inputFile.read(messageBuffer, messageLen);
			messageBuffer[messageLen] = '\0';

			cout << messageBuffer << endl;

			delete[] messageBuffer;
		}

		return inputFile.good();
	}
}

bool updateDialog(const char* username,const char* friendName,const int usernameFriendListPosition)
{
	system("cls");
	cout << "This is the dialog between you and " << friendName <<":"<< endl;
	listDialog(username, friendName, usernameFriendListPosition);

	char *message;
	cout << "\nEnter a message: ";
	cout.flush();
	input(message);

	if (am_I_Friend_With_You(username, friendName) == -1)
	{
		delete[]message;
		system("cls");
		cout << friendName << " is not your friend anymore"<<endl;
		system("pause");
		return true;
	}

	//Ще използваме празния низ, за да обновяваме диалога
	if (strcmp(message, "") == 0)
	{
		delete[] message;
		return updateDialog(username, friendName, usernameFriendListPosition);
	}
	else
	{
		if (strcmp(message, exitString) == 0)
		{
			delete[] message;
			system("cls");
			cout << "You have chosen to leave the conversation with " << friendName << endl;
			system("pause");
			return true;
		}
		else
		{
			bool result = true;
			int friendNameFriendListPosition = am_I_Friend_With_You(friendName, username);
			if (writeToFriend(username, message, usernameFriendListPosition, 1) == false)
				result = false;
			if (writeToFriend(friendName, message, friendNameFriendListPosition, 0) == false)
				result = false;

			delete[] message;
			
			if (result == false)
			{
				cerr << "Some error has occured\n";
				return false;
			}
			else
				return updateDialog(username, friendName, usernameFriendListPosition);
		}
	}

}

//Добавя името на username2 в края на списъка
//с молби за приятелство на username1 
bool sendFriendRequestToFrom(const char* username1, const char* username2)		//checked
{
	const char* fullFileName = makeBinaryFileName( username1, friendsrequestsListLocationString);
	ofstream outputFile(fullFileName, ios::binary | ios::app);
	
	//максимален брой опити да се отвори файла,
	//който, ако се достигне, ще се изведе съобщение за грешка
	int maxNumberOfAttempts = 100;
	int i = 0;
	while (i < maxNumberOfAttempts)
	{
		if (outputFile.is_open())
			break;
		else
		{
			outputFile.open(fullFileName, ios::binary | ios::app);
			++i;
		}
	}

	if (i == maxNumberOfAttempts)
	{
		cerr << "Could not open " << fullFileName << " for writing\n";
		delete[] fullFileName;
		return 0;
	}
	delete[] fullFileName;

	size_t usernameLen=strlen(username2);

	outputFile.write((char*)&usernameLen, sizeof(size_t));
	
	outputFile.write(username2, usernameLen);

	return outputFile.good();
}

//username ще се опита да изпрати покана
//за приятелство на въведен потребител potentialFriendName
bool tryToSendFriendRequest(const char* username)	//checked
{
	char* potentialFriendName;
	enterName(potentialFriendName);

	if (strcmp(username, potentialFriendName)==0)
	{
		cout << "You can't send a friend request to youtself\n";
	}
	else
	{
		//Няма значение каква парола ще
		//въведем. Интересува ни само дали името се
		//среща в списъка.
		SearchResult searchResult = validateUsernameAndPassword(potentialFriendName, "");
	
		if (searchResult == USERNAME_NO)
		{
			cout << "There is no such username\n";
		}
		else
		{
			if (searchResult == USERNAME_OK_PASSWORD_NO || searchResult == USERNAME_OK_PASSWORD_OK)
			{
				if (am_I_Friend_With_You(username, potentialFriendName) >= 0)
				{
					cout << "You are already friends with " << potentialFriendName << endl;
				}
				else
				{
					if (do_I_Have_A_Friend_Request_From(username, potentialFriendName) >= 0)
					{
						cout << potentialFriendName << " has already sent you a friend request" << endl;

					}
					else
					{
						if (do_I_Have_A_Friend_Request_From(potentialFriendName, username) >= 0)
						{
							cout << "You have already sent " << potentialFriendName << " a friendrequest\n";
						}

						//potentialFriendName съществува, не е приятел на username
						//не му е изпращал покана за приятелство и username
						//също не му е изпращал покана за приятелство
						else
						{
							if (sendFriendRequestToFrom(potentialFriendName, username))
								cout << "You have successfully sent " << potentialFriendName
								<< " a friendrequest\n";
							else
							{
								cerr << "Some error has occured\n";
								delete[] potentialFriendName;
								return 0;
							}
						}
					}
				}
			}
			else //searchResult = SOMETHING_ELSE
			{
				cerr << "Some error has occured\n";
				delete[] potentialFriendName;
				return 0;
			}
		}
	}
	system("pause");
	delete[] potentialFriendName;
	return 1;
}

//Добавя futureFriend в списъка с приятели на username
bool addToFriendsList(const char* username, const char* futureFriend)
{
	const char* fullFileName = makeBinaryFileName(username, friendsListLocationString);

	ofstream outputFile(fullFileName, ios::binary | ios::app);
	if (!outputFile)
	{
		cerr << "Could not open " << fullFileName << " for writing\n";
		delete[] fullFileName;
		return false;
	}
	delete[] fullFileName;

	size_t futureFriendsNameLen = strlen(futureFriend);

	//Пържоначално броят съобщения м/у двамата приятели е 0
	const size_t messageNumber = 0;

	outputFile.write((char*)&futureFriendsNameLen, sizeof(size_t));
	outputFile.write(futureFriend, futureFriendsNameLen);
	outputFile.write((char*)&messageNumber, sizeof(size_t));

	return outputFile.good();
}


bool tryToAcceptFriendRequest(const char* username)
{
	char* potentialFriendsName;
	
	enterName(potentialFriendsName);

	//system("cls");

	//позицията, от която започва информацията
	int pos = do_I_Have_A_Friend_Request_From(username, potentialFriendsName);

	if (pos == -1)
	{
		cout << potentialFriendsName << " hasn't sent you a friendsrequest\n";
		system("pause");
		delete[] potentialFriendsName;
		return true;
	}
	else
	{
		const char* fullFriendsListFilename = makeBinaryFileName(username, friendsrequestsListLocationString);

		//дължината на информацията
		int len = findLengthOfFriendRequest(fullFriendsListFilename, pos);

		if (len == -1)
		{
			cerr << "Some error has occured";
			delete[] potentialFriendsName;
			delete[] fullFriendsListFilename;
			return false;
		}
		else
		{
			if (deletePartOfFile(fullFriendsListFilename, pos, len) == false)
			{
				cerr << "Some error has occured\n";
				delete[] potentialFriendsName;
				delete[] fullFriendsListFilename;
				return false;
			}
			else
			{
				addToFriendsList(username, potentialFriendsName);
				addToFriendsList(potentialFriendsName, username);

				cout << potentialFriendsName << " is now your friend" << endl;
				system("pause");
				delete[] potentialFriendsName;
				delete[] fullFriendsListFilename;
				return true;
			}
		}
	}
}

bool tryToDeleteFriendRequest(const char* username)
{
	char* potentialFriendsName;
	enterName(potentialFriendsName);

	//system("cls");

	//позицията, от която започва информацията
	int pos = do_I_Have_A_Friend_Request_From(username, potentialFriendsName);

	if (pos == -1)
	{
		cout << potentialFriendsName << " hasn't sent you a friendsrequest\n";
		system("pause");
		delete[] potentialFriendsName;
		return true;
	}
	else
	{
		const char* fullFriendsListFilename = makeBinaryFileName(username, friendsrequestsListLocationString);

		//дължината на информацията
		int len = findLengthOfFriendRequest(fullFriendsListFilename, pos);

		if (len == -1)
		{
			cerr << "Some error has occured";
			delete[] potentialFriendsName;
			delete[] fullFriendsListFilename;
			return false;
		}
		else
		{
			if (deletePartOfFile(fullFriendsListFilename, pos, len) == false)
			{
				cerr << "Some error has occured\n";
				delete[] potentialFriendsName;
				delete[] fullFriendsListFilename;
				return false;
			}
			else
			{
				cout << "The friendrequest from " << potentialFriendsName << " has been successfully deleted" << endl;
				system("pause");
				delete[] potentialFriendsName;
				delete[] fullFriendsListFilename;
				return true;
			}
		}
	}
}


bool tryToWriteToFriend(const char* username)
{
	char* friendName;
	
	enterName(friendName);

	//Позиция, от която започва информацията за friendName
	//в списъка с приятели на username
	int usernameFriendListPosition = am_I_Friend_With_You(username, friendName);

	if (usernameFriendListPosition == -1)
	{
		cout << friendName << " isn't your friend" << endl;
		delete[] friendName;
		system("pause");
		return true;
	}
	else
	{
		bool result;
		result = updateDialog(username, friendName, usernameFriendListPosition);
		delete[] friendName;
		return result;
	}
}

//0 за грешка
//1 else
bool tryToDeleteFriend(const char* username)
{
	char* friendName;
	enterName(friendName);

	//позиция, от която започва информацията за friendName
	//в списъка с приятели на username
	const int usernameListPosition = am_I_Friend_With_You(username, friendName);

	if (usernameListPosition == -1)
	{
		cout << friendName << " isn't your friend" << endl;
		system("pause");
		delete[] friendName;
		return true;
	}
	else
	{
		const char* usernameFullFriendsListName = makeBinaryFileName(username, friendsListLocationString);

		//дължина на информацията за friendName в списъка с приятели на username
		const int usernameListLength = findLengthOfFriendInfo(usernameFullFriendsListName, usernameListPosition);

		if (usernameListLength == -1)
		{
			delete[] friendName;
			delete[] usernameFullFriendsListName;
			cerr << "Some error occured\n";
			return false;
		}
		else
		{
			//позиция, от която започва информацията за username
			//в списъка с приятели на friendName
			const int friendNameListPosition = am_I_Friend_With_You(friendName, username);

			if (friendNameListPosition == -1)
			{
				delete[] friendName;
				delete[] usernameFullFriendsListName;
				cerr << "A FATAL ERROR OCCURED\n"
					<< username << " has a friend " << friendName
					<< " but " << friendName << " hasn't got a friend " << username << '\n';
				return false;
			}
			else
			{
				const char* friendNameFullFriendsListName = makeBinaryFileName(friendName, friendsListLocationString);

				//дължина на информацията за username в списъка с приятели на friendName
				const int friendNameListLength = findLengthOfFriendInfo(friendNameFullFriendsListName, friendNameListPosition);

				deletePartOfFile(usernameFullFriendsListName, usernameListPosition, usernameListLength);
				deletePartOfFile(friendNameFullFriendsListName, friendNameListPosition, friendNameListLength);

				cout << "You have successfully removed " << friendName << " from your list of friends" << endl;
				system("pause");
				delete[] friendName;
				delete[]usernameFullFriendsListName;
				delete[] friendNameFullFriendsListName;
				return true;
			}
		}
	}

}


static FunctionResult validateProfileCommand(const char* username, const char* profileCommand)
{
	FunctionResult result;
	if (strcmp(profileCommand, listFriendsString) == 0)
	{
		result = (listFriends(username) == true) ? NORMAL : ERROR;
	}
	else if (strcmp(profileCommand, listFriendrequestsString) == 0)
	{
		result = (listFriendrequests(username) == true) ? NORMAL : ERROR;
	}
	else if (strcmp(profileCommand, writeToFriendString) == 0)
	{
		result = (tryToWriteToFriend(username) == true) ? NORMAL : ERROR;
	}
	else if (strcmp(profileCommand, acceptFrendrequestString) == 0)
	{
		result = (tryToAcceptFriendRequest(username) == true) ? NORMAL : ERROR;
	}
	else if (strcmp(profileCommand, sendFriendrequestString) == 0)
	{
		result = (tryToSendFriendRequest(username) == true) ? NORMAL : ERROR;
	}
	else if (strcmp(profileCommand, deleteFriendString) == 0)
	{
		result = (tryToDeleteFriend(username) == true) ? NORMAL : ERROR;
	}
	else if (strcmp(profileCommand, deleteFriendrequestString) == 0)
	{
		result = (tryToDeleteFriendRequest(username) == true) ? NORMAL : ERROR;
	}
	else if (strcmp(profileCommand, exitString) == 0)
	{
		cout << "You have chosen to exit your profile\n";
		cout << "Good bye!\n";
		system("pause");

		result = EXIT;
	}
	else
	{
		cout << "You have typed in a wrong command\n";
		system("pause");

		result = NORMAL;
	}

	return result;
}


//Тази ф-я стартира, след като потребителят е въвел
//правилни име и парола и влезе в профила си
FunctionResult logIn(const char *username)
{
	system("cls");
	printLogIn();
	char* profileCommand;
	input(profileCommand);
	system("cls");

	FunctionResult result = validateProfileCommand(username, profileCommand);
	delete[] profileCommand;

	if (result == NORMAL)
		return logIn(username);
	else
	{
		if (result == EXIT)
			return NORMAL;
		//тук вече result = ERROR
		else
			return ERROR;

	}
}



//Записва username и password в списъка с имена и пароли
bool registrate(const char* username, const char* password)
{
	ofstream outputFile(namesFileString, ios::binary | ios::app);
	if (!outputFile)
	{
		cerr << "Could not open " << namesFileString << " for writing\n";
		return false;
	}
	size_t usernameLen = strlen(username);
	size_t passwordLen = strlen(password);

	outputFile.write((char*)&usernameLen, sizeof(size_t));
	outputFile.write(username, usernameLen);

	outputFile.write((char*)&passwordLen, sizeof(size_t));
	outputFile.write(password, passwordLen);

	if (!outputFile)
	{
		cerr << "An error occured, while writing to " << namesFileString << "\n";
		return false;
	}
	else
		return true;
}

//Взависимост от това, дали въведеното име пръсъства в спъсъка с имена,
//функцията или регистрира потребителя, или му предлага да опита пак
bool tryToRegistrate()	//checked
{
	char *username;
	char *password;
	enterUsernameAndPassword(username, password);
	SearchResult searchResult = validateUsernameAndPassword(username, password);

	if (searchResult == USERNAME_OK_PASSWORD_NO || searchResult == USERNAME_OK_PASSWORD_OK)
	{
		deleteUsernameAndPassword(username, password);
		cout << "A user with that name already exists\n";
		if (chooseFlag("Do you want to try a registration again?"))
		{
			system("cls");
			tryToRegistrate();
		}
		else
			return true;
	}
	else
	{
		if (searchResult == USERNAME_NO)
		{
			if (registrate(username, password) == false)
			{
				deleteUsernameAndPassword(username, password);
				cerr << "An error occured while trying to registrate\n";
				return false;
			}
			else
			{
				//Създаване на файловете с приятели и молби за приятелство
				{
					char *fullFriendsListFileName = makeBinaryFileName(username, friendsListLocationString);
					makeBinaryFile(fullFriendsListFileName);

					char *fullFriendsrequestsListFileName = makeBinaryFileName(username, friendsrequestsListLocationString);
					makeBinaryFile(fullFriendsrequestsListFileName);

					delete[] fullFriendsListFileName;
					delete[] fullFriendsrequestsListFileName;
				}
				deleteUsernameAndPassword(username, password);
				cout << "Congratulations, You have successfully registered in MyNetwork!\n";
				system("pause");
				return true;
			}
		}
		else  //searchResult = SOMETHING_ELSE 
		{
			deleteUsernameAndPassword(username, password);
			cerr << "Some error has occured \n";
			return false;
		}
	}
}

//Взависимост от това, дали въведените име и парола присъстват
//в списъка с пароли и имена, ф-ята или вкарва потребителя в 
//профила му, или го изхвърля от MyNetwork
FunctionResult tryToLogIn()			//FunctionResult
{
	char *username;
	char *password;
	enterUsernameAndPassword(username, password);
	system("cls");
	SearchResult searchResult = validateUsernameAndPassword(username, password);

	if (searchResult==USERNAME_OK_PASSWORD_OK)
	{
		cout << "\tHello, " << username << "!\n";
		cout << "You are logged in\n";
		system("pause");
		
		FunctionResult result = logIn(username);

		deleteUsernameAndPassword(username, password);

		return result;
	}
	else
	{
		deleteUsernameAndPassword(username, password);

		if (searchResult == USERNAME_NO || searchResult == USERNAME_OK_PASSWORD_NO)
		{
			cout << "You have typed in a wrong username or a wrong password\n";
			system("pause");
			return NORMAL;
		}
		else  //SOMETHING_ELSE
		{
			cerr << "Some error has occured\n";
			return ERROR;
		}
	}
}

//В зависимост от ст-тта на startCommmand има 4 изхода
//1) регистрация
//2) влизане в профила
//3) изход, понеже е въведена команда за изход
//4) изход, понеже е въведена грешна команда
FunctionResult validateStartCommand(const char* startCommand)
{
	FunctionResult result;
	if (!strcmp(startCommand, registrateString))
	{
		result = (tryToRegistrate() == true) ? NORMAL : ERROR;
	}
	else
	{
		if (!strcmp(startCommand, logInString))
			
			result = tryToLogIn();
		
		else
		{
			if (!strcmp(startCommand, exitString))
			{
				cout << "You have chosen to exit MyNetwork\n"
					<< "Good bye!\n";
				result = EXIT;
			}
			else
			{
				cout << "You have typed in a wrong command\n";
				system("pause");
				result = NORMAL;
			}
		}
	}
	return result;
}


FunctionResult myNetwork()
{
	system("cls");
	printStart();

	char *startCommand;
	input(startCommand);

	system("cls");

	//Ф-ята ще прекрати действието си
	//само ако потребителят въведе exit
	//или ако някой файл не се отвори
	FunctionResult result = validateStartCommand(startCommand);

	delete[] startCommand;

	if (result == NORMAL)
		return myNetwork();
	else
		return result;
}


/*

6 дни

1500 реда код

33 функции

стил, на писане, който помага лесно да се открият грешките и да се поправят

програмата е написана така, че на потребителя да му е удобно да я използва


АЗ СЪМ БОГ!!!

*/