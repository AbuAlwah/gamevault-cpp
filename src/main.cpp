#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <windows.h>
#include <conio.h>
#include "User.h"


using namespace std;

void ClearScreen()
{
    system("cls");
}

void ensureUsersFile()
{
    ifstream fin("users.txt");
    if (!fin)
    {
        ofstream fout("users.txt");
        if (fout)
        {
            fout << "admin|123|admin\n";
            fout << "user|456|user\n";
            fout.close();
        }
    }
    else
        fin.close();
}

void PauseScreen()
{
    cout << "\n Press Enter to continue...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string inputPasswordMasked()
{
    const int max_length = 15;
    string password = "";
    char ch;
    while (true)
    {
        ch = getch();
        if (ch == 13)
        {
            cout << endl;
            break;
        }
        else if (ch == 8)
        {
            if (!password.empty())
            {
                password.erase(password.size() - 1);
                cout << "\b \b";
            }
        }
        else if (ch == 0 || ch == -32)
        {
            getch();
            continue;
        }
        else
        {
            if (password.size() < max_length)

            {
                password.push_back(ch);
                cout << '*';
            }

            else
            {
                MessageBeep(MB_ICONEXCLAMATION);
            }
        }
    }
    return password;
}

bool parseLineUser(const string &line, User &u)
{
    int pos1 = line.find('|');
    if (pos1 == string::npos)
        return false;
    int pos2 = line.find('|', pos1 + 1);
    if (pos2 == string::npos)
        return false;

    u.username = line.substr(0, pos1);
    u.password = line.substr(pos1 + 1, pos2 - pos1 - 1);
    u.role = line.substr(pos2 + 1);
    return true;
}

bool loginuserstxt(const string &inputUser, const string &inputPass, User &loggedUser)
{
    ensureUsersFile();

    ifstream fin("users.txt");
    if (!fin)
    {
        cout << "File users.txt not found!\n";
        return false;
    }

    string line;
    while (getline(fin, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        User temp;
        if (!parseLineUser(line, temp))
            continue;
        if (inputUser == temp.username && inputPass == temp.password)
        {
            loggedUser = temp;
            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}
int main()
{

    ClearScreen();

    return 0;
}
