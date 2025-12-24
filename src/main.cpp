#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <windows.h>
#include <conio.h>

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

int main()
{

    ClearScreen();

    return 0;
}
