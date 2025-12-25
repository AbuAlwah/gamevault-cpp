#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include "User.h"
#include "Game.h"

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

User login()
{
    const int MaxAttemps = 3;
    int Attempts = 0;
    User current;

    while (Attempts < MaxAttemps)
    {
        string username;
        cout << "Enter username: ";
        getline(cin >> ws, username);
        cout << "Enter password: ";
        string password = inputPasswordMasked();

        if (loginuserstxt(username, password, current))
        {
            cout << "Login successful! Role: " << current.role << endl;
            return current;
        }
        else
        {
            Attempts++;
            cout << "\n\t\t\t\t\tErrore in username or password !\n";
            cout << "\t\tAttempt No." << Attempts << " Of " << MaxAttemps << endl;

            if (Attempts < MaxAttemps)
            {
                cout << "\n Press Enter to try again...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                ClearScreen();
            }
            else
            {
                ClearScreen();
                cout << "\n\tThe password limit has been exceeded !\n\n";
                cout << "\t\t\t\t\t The program is closing...\n\n";

                exit(0);
            }
        }
    }
    current.role = "none";
    return current;
}

template <typename T>
class Stack
{
private:
    T *arr;
    int capacity;
    int topIndex;
    void resize(int newCap)
    {
        T *newArr = new T[newCap];
        for (int i = 0; i <= topIndex; i++)
            newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
        capacity = newCap;
    }

public:
    Stack()
    {
        capacity = 4;
        arr = new T[capacity];
        topIndex = -1;
    }
    ~Stack()
    {
        delete[] arr;
    }
    void push(const T &val)
    {
        if (topIndex + 1 >= capacity)
            resize(capacity * 2);
        arr[++topIndex] = val;
    }
    T pop()
    {
        if (isEmpty())
        {
            throw runtime_error("Stack is empty");
        }

        T val = arr[topIndex--];
        if (topIndex + 1 <= capacity / 4 && capacity > 4)
            resize(capacity / 2);
        return val;
    }
    T top() const
    {
        if (isEmpty())
            throw runtime_error("Stack is empty");

        return arr[topIndex];
    }
    bool isEmpty() const
    {
        return topIndex == -1;
    }
    int size() const
    {
        return topIndex + 1;
    }
};

int NextId = 0;

void InitGames(vector<Game> &Games)
{
    Game s1{1110, "Spider-Man 2", "49.99$", 13};
    Game s2{1111, "Frontiers Of Pandora", "35.99$", 10};
    Game s3{1112, "The Dark Ages", "85.99$", 7};
    Game s4{1113, "Cyberpunk 2077", "23.99$", 18};
    Games = {s1, s2, s3, s4};
}

void InitNextId(vector<Game> &Games)
{
    NextId = 0;
    for (auto &g : Games)
    {
        if (g.ID > NextId)
            NextId = g.ID;
    }
}

void ShowGames(const vector<Game> &Games)
{
    cout << endl
         << left << setw(9) << "ID" << setw(22) << "Name" << setw(13) << "Price" << setw(12) << "Quantity" << endl;

    cout << "------------------------------------------------------\n";

    for (auto &g : Games)
    {
        cout << setw(6) << g.ID << setw(25) << g.Name << setw(16) << g.Price << setw(10) << g.Quantity << endl;
    }
    cout << "------------------------------------------------------\n";
}

void saveGamesToFile(const vector<Game> &Games, const string &filename = "games.txt")
{
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Unable to open " << filename << " for writing.\n";
        return;
    }

    fout << endl
         << left << setw(9) << "ID" << setw(22) << "Name" << setw(13) << "Price" << setw(12) << "Quantity" << endl;

    fout << "----------------------------------------------------------------\n";

    for (auto &g : Games)
    {
        fout << setw(6) << g.ID << setw(25) << g.Name << setw(16) << g.Price << setw(10) << g.Quantity << endl;
    }
    fout << "----------------------------------------------------------------\n";

    fout.close();
}

void loadGamesFromFile(vector<Game> &Games, const string &filename = "games.txt")
{
    Games.clear();
    ifstream fin(filename);
    if (!fin)
    {
        InitGames(Games);
        InitNextId(Games);
        saveGamesToFile(Games, filename);
        return;
    }

    string line;
    while (getline(fin, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty())
            continue;
        if (line.find("ID") != string::npos)
            continue;
        if (line.find("----") != string::npos)
            continue;

        stringstream ss(line);
        Game g;
        ss >> g.ID >> ws;

        g.Name.clear();
        g.Price.clear();

        string token;
        while (ss >> token)
        {

            if (token.find('$') != string::npos)
            {
                g.Price = token;
                break;
            }

            if (!g.Name.empty())
                g.Name += " ";
            g.Name += token;
        }

        ss >> g.Quantity;

        if (!g.Name.empty() || !g.Price.empty())
            Games.push_back(g);
    }

    fin.close();
    InitNextId(Games);
}

double parsePrice(const string &priceStr)
{
    string num = "";
    for (auto c : priceStr)
    {
        if ((c >= '0' && c <= '9') || c == '.')
            num.push_back(c);
    }
    if (num.empty())
        return 0.0;
    try
    {
        return stod(num);
    }
    catch (...)
    {
        return 0.0;
    }
}

void AddGame(vector<Game> &Games, Stack<Game> &addedStack)
{
    Game NewGame;
    NewGame.ID = ++NextId;
    cout << "Added game name: ";
    getline(cin >> ws, NewGame.Name);

    cout << "Game price: ";
    getline(cin >> ws, NewGame.Price);

    cout << "Available quantity: ";
    cin >> NewGame.Quantity;

    Games.push_back(NewGame);

    addedStack.push(NewGame);

    saveGamesToFile(Games);

    cout << "\nGame added successfully.\n";
}

bool UndoAdd(vector<Game> &Games, Stack<Game> &addedStack)
{
    if (addedStack.isEmpty())
    {
        cout << "Nothing to undo .\n";
        return false;
    }
    Game g = addedStack.pop();
    for (int i = Games.size() - 1; i >= 0; i--)
    {
        if (Games[i].ID == g.ID)
        {
            Games.erase(Games.begin() + i);
            saveGamesToFile(Games);
            cout << "Undo successful. Added game removed: " << g.Name << " (ID" << g.ID << ")" << endl;
            return true;
        }
    }
    return false;
}

void EditGame(vector<Game> &Games)
{
    string EditName;
    cout << "Enter game name to edit : ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin >> ws, EditName);

    bool Found = false;
    for (auto &g : Games)
    {
        if (g.Name == EditName)
        {
            Found = true;
            cout << "\n\t\tThe game has been found .\n\n";

            cout << "New name : ";
            getline(cin, g.Name);

            cout << "New price : ";
            getline(cin, g.Price);

            cout << "New quantity : ";
            cin >> g.Quantity;

            cout << "\n\n\t\t\t Game data has been successfully modified\n\n";

            break;
        }
    }
    if (!Found)
        cout << "\n\t\t\tGame not found !\n\n";
}

void SearchGameByID(const vector<Game> &Games)
{
    int id;
    cout << "Enter Game ID : ";
    cin >> id;
    ClearScreen();

    bool Found = false;
    for (auto &g : Games)
    {
        if (g.ID == id)
        {
            Found = true;
            cout << "\n\t\tThe game has been found .\n\n";
            cout << "Name : " << g.Name << endl;
            cout << "Price : " << g.Price << endl;
            cout << "Quantity : " << g.Quantity << endl
                 << endl;
            return;
        }
    }
    if (!Found)
        cout << "\n\t\t\tGame not found !\n\n";
}

void SearchGameName(const vector<Game> &Games)
{
    string name;
    cout << "Enter Game's name to search : ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    bool Found = false;
    for (auto &g : Games)
    {
        if (g.Name == name)
        {
            Found = true;
            cout << "\n\t\tThe game has been found .\n\n";
            cout << "Name : " << g.Name << endl;
            cout << "Price : " << g.Price << endl;
            cout << "Quantity : " << g.Quantity << endl
                 << endl;
            return;
        }
    }
    if (!Found)
        cout << "\n\t\t\tGame not found !\n\n";
}

bool DeleteGameByID(vector<Game> &Games, Stack<Game> &deletedStack)
{
    int id;
    cout << "Enter ID to delete : ";
    cin >> id;
    cout << endl;
    int index = -1;
    for (size_t i = 0; i < Games.size(); i++)
    {
        if (Games[i].ID == id)
        {
            index = (int)i;

            cout << "Name : " << Games[i].Name << endl;
            cout << "Price : " << Games[i].Price << endl;
            cout << "Quantity : " << Games[i].Quantity << endl
                 << endl;
            break;
        }
    }
    if (index == -1)
    {
        cout << "\nGame with ID " << id << " not found.\n";
        return false;
    }

    deletedStack.push(Games[index]);

    Games.erase(Games.begin() + index);
    saveGamesToFile(Games);
    cout << "\nGame deleted and pushed to undo stack.\n";
    return true;
}

bool UndoDeletion(vector<Game> &Games, Stack<Game> &deletedStack)
{
    if (deletedStack.isEmpty())
    {
        cout << "Nothing to undo.\n";
        return false;
    }
    Game g = deletedStack.pop();
    Games.push_back(g);

    if (g.ID > NextId)
        NextId = g.ID;
    saveGamesToFile(Games);
    cout << "Undo successful. Game restored: " << g.Name << " (ID " << g.ID << ")\n";
    return true;
}

int main()
{
    vector<Game> Games;
    InitNextId(Games);
    loadGamesFromFile(Games);

    Stack<Game> addedStack;
    Stack<Game> deletedStack;

    ClearScreen();

    return 0;
}
