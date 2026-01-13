#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <vector>
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
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
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

void ShowReport(const vector<Game> &Games)
{
    cout << "\n\t\t\t\t\t\t Report of current inventory\n\n";

    cout << endl
         << left << setw(9) << "ID" << setw(22) << "Name" << setw(13) << "Price" << setw(12) << "Quantity" << endl;
    cout << "------------------------------------------------------\n";

    double totalValue = 0.0;
    for (auto &g : Games)
    {
        cout << setw(6) << g.ID << setw(25) << g.Name << setw(16) << g.Price << setw(10) << g.Quantity << endl;
        totalValue += parsePrice(g.Price) * g.Quantity;
    }
    cout << "------------------------------------------------------\n";

    cout << "\nTotal inventory value: " << fixed << setprecision(2) << totalValue << "$\n";

    ofstream fout("report.txt");
    if (!fout)
    {
        cout << "Unable to open (report.txt) for writing.\n";
        return;
    }

    fout << "Report of current inventory\n\n";

    fout << "------------------------------------------------------\n";

    fout << left << setw(9) << "ID" << setw(22) << "Name" << setw(13) << "Price" << setw(12) << "Quantity" << "\n";

    for (auto &g : Games)
    {
        fout << setw(6) << g.ID << setw(25) << g.Name << setw(16) << g.Price << setw(10) << g.Quantity << "\n";
    }
    fout << "------------------------------------------------------\n";

    fout << "\nTotal inventory value: " << totalValue << "$\n";
    fout.close();
    cout << "\nReport saved to report.txt\n";
}

void BuyGame(vector<Game> &Games, const string &username)
{
    string name;
    cout << "Enter game name to buy : ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin >> ws, name);

    bool Found = false;
    for (auto &g : Games)
    {
        if (g.Name == name)
        {
            Found = true;

            cout << "\n\t\tThe game has been found .\n\n";
            cout << "Name : " << g.Name << endl;
            cout << "Price : " << g.Price << endl;
            cout << "Available Quantity : " << g.Quantity << endl
                 << endl;

            int amount;
            cout << "\t\tEnter quantity to buy : ";
            cin >> amount;

            if (amount <= 0)
            {
                cout << "\n\t\tInvalid quantity." << endl;
                return;
            }

            if (amount > g.Quantity)
            {
                cout << "\n\t\tSorry , Only " << g.Quantity << " copies available." << endl;
                return;
            }

            string confirm;
            cout << "\n\t\t\t\t\t\t\tConfirm purchase ? \n\n\t\t\t\t\t\t\t   yes \t  no \n\n\t\t\t\t\t\t\t Your choice : ";
            getline(cin >> ws, confirm);

            if (confirm != "yes" && confirm != "no")
            {
                cout << "\n\t\t\t\t\t\t\t Wrong choice" << endl;
                return;
            }

            if (confirm == "no")
            {
                cout << "\n\t\t\t\t\t\t\t Purchase canceled." << endl;
                return;
            }

            ClearScreen();

            g.Quantity -= amount;

            double total = parsePrice(g.Price) * amount;

            cout << "\nPurchase successful! " << g.Name << endl;
            cout << "Total cost: " << fixed << setprecision(2) << total << "$" << endl;
            cout << "Remaining quantity: " << g.Quantity << endl;

            saveGamesToFile(Games);

            ofstream fout("purchases.txt", ios::app);
            if (!fout)
            {
                cout << " Error opening purchase.txt for append ! " << endl;
                return;
            }

            fout << username << "|" << g.Name << "|" << amount << "|" << fixed << setprecision(2) << total << "\n";

            fout.close();
        }
    }

    if (!Found)
    {
        cout << "\n\t\t\tGame with Name (" << name << ") not found!\n\n";
    }
}

void ShowBuyingBill(const string &username)
{
    ifstream fin("purchases.txt");
    if (!fin)
    {
        cout << "\nNo purchase history found.\n";
        return;
    }

    cout << "\n\t\tPurchase History for " << username << "\n\n\n";
    cout << left << setw(25) << "Game Name" << setw(18) << "Quantity" << setw(12) << "Total($)" << endl;

    cout << "------------------------------------------------------\n";

    string line;
    double totalSpent = 0.0;
    bool found = false;

    while (getline(fin, line))
    {
        string user, name;
        int qty;
        double total;

        stringstream ss(line);
        getline(ss, user, '|');
        getline(ss, name, '|');
        ss >> qty;
        ss.ignore(1, '|');
        ss >> total;

        if (user == username)
        {
            found = true;
            cout << left << setw(28) << name << setw(16) << qty << setw(12) << fixed << setprecision(2) << total << endl;
            totalSpent += total;
        }
    }

    if (!found)
    {
        cout << "No purchases yet.\n";
    }
    else
    {
        cout << string(54, '-') << endl;
        cout << left << setw(15) << "\nTOTAL SPENT:" << fixed << setprecision(2) << totalSpent << "$\n";
    }

    fin.close();
}

void ShowAllPurchases()
{
    ifstream fin("purchases.txt");

    if (!fin)
    {
        cout << "\nNo purchase data found (purchases.txt missing).\n";
        return;
    }

    cout << "\n\t\t=== All Sales (Purchases) ===\n\n";

    cout << "------------------------------------------------------------\n";
    cout << left << setw(15) << "Username" << setw(25) << "Game Name" << setw(10) << "Qty" << setw(10) << "Total ($)" << endl;
    cout << "------------------------------------------------------------\n";

    string line;
    bool empty = true;

    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string username, gameName;
        int qtyStr;
        double totalStr;

        getline(ss, username, '|');
        getline(ss, gameName, '|');
        ss >> qtyStr;
        ss.ignore(1, '|');
        ss >> totalStr;

        cout << left << setw(15) << username << setw(25) << gameName << setw(10) << qtyStr << setw(10) << totalStr << endl;

        empty = false;
    }

    if (empty)
        cout << "\nNo purchases recorded yet.\n";

    fin.close();
}

void ShowScreen1()
{
    cout << "\n"
         << string(10, '\t') << "Hello to Game Library System\n\n"
         << string(11, '\t') << "[100] Logout\n";
    cout << string(5, '\t') << "[1] Show Games \n";
    cout << string(5, '\t') << "[2] Adding Game \n";
    cout << string(5, '\t') << "[3] Undo Last Added Game \n";
    cout << string(5, '\t') << "[4] Edit Game Data \n";
    cout << string(5, '\t') << "[5] Search By ID \n";
    cout << string(5, '\t') << "[6] Search By Name \n";
    cout << string(5, '\t') << "[7] Delet Game \n";
    cout << string(5, '\t') << "[8] Undo deletion \n";
    cout << string(1, '\t') << "[9] White Screen Theme";
    cout << string(5, '\t') << "[10] Black Screen Theme \n\n";
    cout << string(5, '\t') << "[11] Show Report \n";
    cout << string(5, '\t') << "[12] Show Sales \n\n";
    cout << string(5, '\t') << "[0] Exit \n\n";
}

void ShowScreen2()
{
    cout << "\n"
         << string(10, '\t') << "Hello to Game Library\n\n"
         << string(10, '\t') << "   [100] Logout\n";
    cout << string(5, '\t') << "[1] Show Games \n";
    cout << string(5, '\t') << "[2] Find a Game \n";
    cout << string(5, '\t') << "[3] Buy a Game \n";
    cout << string(1, '\t') << "[4] White Screen Theme";
    cout << string(5, '\t') << "[5] Black Screen Theme \n\n";
    cout << string(5, '\t') << "[6] Show Buying Bill \n\n";
    cout << string(5, '\t') << "[0] Exit \n\n";
}

void WhiteCycleTheme() { system("color F0"); }

void BlackCycleTheme() { system("color 0F"); }

void ExitProgram()
{
    cout << string(6, '\t') << "( Program exited )\n\n";
    cout << "\t\t\t  " << string(63, '-') << "\n\n";
    exit(0);
}

void AdminMenu(vector<Game> &Games, Stack<Game> &addedStack, Stack<Game> &deletedStack)
{
    bool running = true;
    while (running)
    {
        ShowScreen1();
        int choice;
        cout << "\tEnter Choice : ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            ClearScreen();
            ShowGames(Games);
            break;

        case 2:
            ClearScreen();
            AddGame(Games, addedStack);
            break;

        case 3:
            ClearScreen();
            UndoAdd(Games, addedStack);
            break;

        case 4:
            ClearScreen();
            EditGame(Games);
            break;

        case 5:
            ClearScreen();
            SearchGameByID(Games);
            break;

        case 6:
            ClearScreen();
            SearchGameName(Games);
            break;

        case 7:
            ClearScreen();
            DeleteGameByID(Games, deletedStack);
            break;

        case 8:
            ClearScreen();
            UndoDeletion(Games, deletedStack);
            break;

        case 9:
            WhiteCycleTheme();
            break;

        case 10:
            BlackCycleTheme();
            break;

        case 11:
            ClearScreen();
            ShowReport(Games);
            break;

        case 12:
            ClearScreen();
            ShowAllPurchases();
            break;

        case 100:
            cout << "\n Logging out...\n";
            running = false;
            break;

        case 0:
            ExitProgram();

        default:
            cout << "Invalid choice.\n";
        }
        PauseScreen();
        ClearScreen();
    }
}

void UserMenu(vector<Game> &Games, const string &username)
{
    bool running = true;
    while (running)
    {
        ShowScreen2();
        cout << "\tEnter Choice : ";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            ClearScreen();
            ShowGames(Games);
            break;

        case 2:
            ClearScreen();
            SearchGameName(Games);
            break;

        case 3:
            ClearScreen();
            BuyGame(Games, username);
            break;

        case 4:
            WhiteCycleTheme();
            break;

        case 5:
            BlackCycleTheme();
            break;

        case 6:
            ClearScreen();
            ShowBuyingBill(username);
            break;

        case 100:
            cout << "\n Logging out...\n";
            running = false;
            break;

        case 0:
            ExitProgram();

        default:
            cout << "Invalid choice.\n";
            break;
        }
        PauseScreen();
        ClearScreen();
    }
}

int main()
{
    vector<Game> Games;
    InitNextId(Games);
    loadGamesFromFile(Games);

    Stack<Game> addedStack;
    Stack<Game> deletedStack;

  while (true)
    {
        ClearScreen();
        User current = login();

        if (current.role == "none")
            continue;

        if (current.role == "admin")
            AdminMenu(Games, addedStack, deletedStack);

        else if (current.role == "user")
            UserMenu(Games, current.username);
    }

    return 0;
}
