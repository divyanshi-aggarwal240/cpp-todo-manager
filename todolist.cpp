#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

// encryption and decryption
string encryption(string password, int key, int digit = -1)
{
    string encryptData = "";
    key = key % 26;
    if (digit == -1)
    {
        digit = key;
    }
    digit = digit % 10;
    for (char chr : password)
    {
        if ('A' <= chr && 'Z' >= chr)
        {
            encryptData += (65 + (chr - 65 + key) % 26);
        }
        else if ('a' <= chr && 'z' >= chr)
        {
            encryptData += (97 + (chr - 97 + key) % 26);
        }
        else if ('0' <= chr && '9' >= chr)
        {
            encryptData += ('0' + (chr - '0' + key) % 10);
        }
        else
        {
            encryptData += chr;
        }
    }
    return encryptData;
}
string decryption(string password, int key)
{
    key = key % 26;
    int digitKey = key % 10;

    string decryptData = "";

    for (char chr : password)
    {
        if (chr >= 'A' && chr <= 'Z')
        {
            decryptData += char('A' + (chr - 'A' - key + 26) % 26);
        }
        else if (chr >= 'a' && chr <= 'z')
        {
            decryptData += char('a' + (chr - 'a' - key + 26) % 26);
        }
        else if (chr >= '0' && chr <= '9')
        {
            decryptData += char('0' + (chr - '0' - digitKey + 10) % 10);
        }
        else
        {
            decryptData += chr;
        }
    }

    return decryptData;
}
// use color function

void Red() { cout << "\033[31m"; }
void Green() { cout << "\033[32m"; }
void Yellow() { cout << "\033[33m"; }
void Blue() { cout << "\033[34m"; }
void Magenta() { cout << "\033[35m"; }
void Cyan() { cout << "\033[36m"; }
void White() { cout << "\033[37m"; }
void Reset() { cout << "\033[0m"; }

struct Task
{
    int id;
    string title;
    string description;
    bool status = false; // false= task not done; true= task done
    int priority;
};

ostream &operator<<(ostream &os, const Task &task)
{
    Green();
    os << "-----------------------------------" << endl;
    os << "Task ID           :" << task.id << endl;
    os << "Task title        :" << task.title << endl;
    os << "Task description  :" << task.description << endl;
    os << "Task status       :" << (task.status ? "completed" : "pending") << endl;
    os << "Task priority no. :" << task.priority << endl;
    Reset();
    return os;
}

istream &operator>>(istream &is, Task &task)
{
    Magenta();
    cout << "---------------------------------" << endl;
    cout << "Enter task title       :";
    getline(is, task.title);
    cout << "Enter task description :";
    getline(is, task.description);
    cout << "Enter priority no.     :";
    is >> task.priority;
    is.ignore();
    Reset();
    return is;
}
class TaskCompare
{
public:
    bool operator()(const Task &a, const Task &b) const
    {
        return a.priority > b.priority;
    }
};

class ToDoManager
{
private:
    priority_queue<Task, vector<Task>, TaskCompare> ListOfTasks;

protected:
    int taskIdCounter = 0;
    string userName;
    string password;
    void loadTasks();
    void savetask();

public:
    void mainMenu();
    void addtasks();
    void deletetask();
    void searchtaskbyid();
    void printalltask();
    void printalltaskpending();
    void printalltaskcompleted();
    void changestatusoftask();
    void updatetask();
};

void ToDoManager ::loadTasks()
{
    ifstream file(userName + ".txt");
    if (!file.is_open())
    {
        return;
    }

    Task task;

    string encryptedPassword;

    file >> userName >> encryptedPassword >> taskIdCounter;

    password = decryption(
        encryptedPassword,
        encryptedPassword.length());

    file.ignore();

    while (getline(file, task.title))
    {
        getline(file, task.description);

        file >> task.id >> task.status >> task.priority;

        file.ignore();

        ListOfTasks.push(task);
    }

    file.close();
}

void ToDoManager ::savetask()
{
    ofstream file(userName + ".txt");

    if (!file.is_open())
    {
        Red();
        cout << "Error opening file!" << endl;
        Reset();
        return;
    }

    string encryptedPassword =
        encryption(password, password.length());

    file << userName << " "
         << encryptedPassword << " "
         << taskIdCounter << endl;

    // Copy queue instead of destroying original queue
    priority_queue<Task, vector<Task>, TaskCompare> tempQueue =
        ListOfTasks;

    while (!tempQueue.empty())
    {
        Task task = tempQueue.top();
        tempQueue.pop();

        file << task.title << endl;
        file << task.description << endl;

        file << task.id << " "
             << task.status << " "
             << task.priority << endl;
    }

    file.close();

    Green();
    cout << "Tasks saved successfully!" << endl;
    Reset();
}

void ToDoManager ::mainMenu()
{
    int choice;
    do
    {
        Blue();
        cout << "|-----------------------------------------------|" << endl;
        cout << "|                 MAIN MENU                     |" << endl;
        cout << "| 1.Add Task                                    |" << endl;
        cout << "| 2.Delete Task                                 |" << endl;
        cout << "| 3.Search task by ID                           |" << endl;
        cout << "| 4.Print all task                              |" << endl;
        cout << "| 5.Print Pending tasks                         |" << endl;
        cout << "| 6.Print completed tasks                       |" << endl;
        cout << "| 7.Change task status                          |" << endl;
        cout << "| 8.Update Task                                 |" << endl;
        cout << "| 9.Exit                                        |" << endl;
        cout << "|-----------------------------------------------|" << endl;
        cout << "Enter your choice:-";
        cin >> choice;
        cin.ignore();
        system("cls");
        Reset();

        switch (choice)
        {
        case 1:
            addtasks();
            break;
        case 2:
            deletetask();
            break;
        case 3:
            searchtaskbyid();
            break;
        case 4:
            printalltask();
            break;
        case 5:
            printalltaskpending();
            break;
        case 6:
            printalltaskcompleted();
            break;
        case 7:
            changestatusoftask();
            break;
        case 8:
            updatetask();
            break;
        case 9:
            savetask();
            break;
        default:
            Red();
            cout << "Invalid Choice ! Please Try again." << endl;
            Reset();
        }
    } while (choice != 9);
}
void ToDoManager ::addtasks()
{
    Task temptask;
    cin >> temptask;
    temptask.id = ++taskIdCounter;
    ListOfTasks.push(temptask);
    Green();

    cout << "task added successfully!" << endl;
    cout << "task ID :" << temptask.id << endl;
    Reset();
}
void ToDoManager ::deletetask()
{
    int id;
    cout << " enter task ID to delete: ";
    cin >> id;
    priority_queue<Task, vector<Task>, TaskCompare> tempQueue;
    bool found = false;

    while (!ListOfTasks.empty())
    {
        Task task = ListOfTasks.top();
        ListOfTasks.pop();
        if (task.id == id)
        {
            found = true;
        }
        else
        {
            tempQueue.push(task);
        }
    }
    ListOfTasks = tempQueue;
    if (found)
    {
        Green();
        cout << "tassk deleted successly!" << endl;
        Reset();
    }
    else
    {
        Red();
        cout << "task not found!" << endl;
        Reset();
    }
}
void ToDoManager ::searchtaskbyid()
{
    int id;
    cout << "enter task ID to search:";
    cin >> id;
    priority_queue<Task, vector<Task>, TaskCompare> tempQueue;
    bool found = false;
    Task task;

    while (!ListOfTasks.empty())
    {
        task = ListOfTasks.top();
        ListOfTasks.pop();
        if (task.id == id)
        {
            cout << task;
            found = true;
        }
        tempQueue.push(task);
    }
    ListOfTasks = tempQueue;
    if (!found)
    {
        Red();
        cout << "task not found!" << endl;
        Reset();
    }
}
void ToDoManager ::printalltask()
{
    if (ListOfTasks.empty())
    {
        Red();
        cout << "no tasks available!" << endl;
        Reset();
        return;
    }
    else
    {
        priority_queue<Task, vector<Task>, TaskCompare> tempQueue;
        cout << "all task:-" << endl;
        while (!ListOfTasks.empty())
        {
            Task task = ListOfTasks.top();
            ListOfTasks.pop();
            cout << task;
            tempQueue.push(task);
        }
        ListOfTasks = tempQueue;
    }
}

void ToDoManager ::printalltaskpending()
{
    if (ListOfTasks.empty())
    {
        Red();
        cout << "No tasks available!!" << endl;
        Reset();
        return;
    }
    else
    {
        priority_queue<Task, vector<Task>, TaskCompare> tempQueue = ListOfTasks;
        cout << " All pending tasks:- " << endl;
        bool found = false;

        while (!tempQueue.empty())
        {
            Task task = tempQueue.top();
            tempQueue.pop();
            if (!task.status)
            {
                cout << task;
                found = true;
            }
            tempQueue.push(task);
        }
        if (!found)
        {
            cout << "no pending tasks" << endl;
        }
    }
}
void ToDoManager ::printalltaskcompleted()
{
    if (ListOfTasks.empty())
    {
        Red();
        cout << "No tasks available!!" << endl;
        Reset();
    }
    else
    {
        priority_queue<Task, vector<Task>, TaskCompare> tempQueue;
        cout << " All completed tasks:- " << endl;
        while (!ListOfTasks.empty())
        {
            Task task = ListOfTasks.top();
            ListOfTasks.pop();
            if (task.status)
            {
                cout << task;
            }
            tempQueue.push(task);
        }
        ListOfTasks = tempQueue;
    }
}

void ToDoManager ::changestatusoftask()
{
    int id;
    cout << " Enter task id to change status:";
    cin >> id;
    priority_queue<Task, vector<Task>, TaskCompare> tempQueue;
    bool found = false;
    Task task;
    while (!ListOfTasks.empty())
    {
        task = ListOfTasks.top();
        ListOfTasks.pop();
        if (task.id == id)
        {
            task.status = !task.status;
            Green();
            cout << " Task changed successfully!" << endl;
            Reset();
            found = true;
        }
        tempQueue.push(task);
    }
    ListOfTasks = tempQueue;

    if (!found)
    {
        Red();
        cout << " task not found!" << endl;
        Reset();
    }
}
void ToDoManager ::updatetask()
{
    int id;
    cout << " Enter task id to update:";
    cin >> id;
    cin.ignore();
    priority_queue<Task, vector<Task>, TaskCompare> tempQueue;
    bool found = false;
    Task task;
    while (!ListOfTasks.empty())
    {
        task = ListOfTasks.top();
        ListOfTasks.pop();
        if (task.id == id)
        {

            cout << " Enter new details for the task" << endl;
            cin >> task;
            task.id = id;
            found = true;
        }
        tempQueue.push(task);
    }
    ListOfTasks = tempQueue;

    if (!found)
    {
        Red();
        cout << " task not found!" << endl;
        Reset();
    }
}

class Authentication : public ToDoManager
{
public:
    void AuthMainMenu();
    void login();
    void registeruser();
};

void Authentication ::AuthMainMenu()
{ // to see
    int choice;
    do
    {
        Magenta();
        cout << "|----------------------------------|" << endl;
        cout << "|          Authentication          |" << endl;
        cout << "|1.Register User                   |" << endl;
        cout << "|2.Login User                      |" << endl;
        cout << "|3.Exit                            |" << endl;
        cout << "|----------------------------------|" << endl;
        cout << endl;

        cout << " Enter your choice:-" << endl;
        cin >> choice;
        cin.ignore();
        system("cls");
        Reset();

        switch (choice)
        {
        case 1:
            registeruser();
            break;
        case 2:
            login();
            break;
        case 3:
            break;
        default:
            Red();
            cout << " Invalid Choice ! Please try again" << endl;
            Reset();
        }

    } while (choice != 3);
    cout << " Exiting... " << endl;
}

void Authentication::registeruser()
{
    string inputusername;
    string inputpassword;
    Magenta();

    cout << "-----------------------------------" << endl;
    cout << "        Register User              " << endl;
    cout << "Enter Your Username(write only first name or put" - " in btw): ";
    cin >> inputusername;
    cout << "Enter Your Password: ";
    cin >> inputpassword;
    Reset();
    ifstream file(inputusername + " .txt");
    if (file.is_open())
    {
        Red();
        cout << "user already exists!" << endl;
        Reset();
        file.close();
        return;
    }
    file.close();
    // Store user information
    userName = inputusername;
    password = inputpassword;
    taskIdCounter = 0;

    // Create user file
    ofstream newFile(userName + ".txt");

    if (newFile.is_open())
    {
        string encryptedPassword =
            encryption(password, password.length());

        newFile << userName << " "
                << encryptedPassword << " "
                << taskIdCounter << endl;

        newFile.close();

        Green();
        cout << "User registered successfully!" << endl;
        Reset();
    }
    else
    {
        Red();
        cout << "Error creating user file!" << endl;
        Reset();
    }
}

void Authentication ::login()
{
    string inputusername;
    string inputpassword;
    Magenta();
    cout << "-------------------------------------" << endl;
    cout << "             Login User              " << endl;
    cout << "Enter user name:-";
    cin >> inputusername;
    cout << "Enter user password:-" << endl;
    cin >> inputpassword;
    Reset();
    // Check whether user exists
    ifstream file(inputusername + ".txt");

    if (!file.is_open())
    {
        Red();
        cout << "User does not exist!" << endl;
        Reset();
        return;
    }

    file.close();

    // Load user's data
    userName = inputusername;

    loadTasks();

    if (password == inputpassword)
    {
        Green();
        cout << "Login successful!" << endl;
        Reset();

        mainMenu();
    }
    else
    {
        Red();
        cout << "Invalid username or password!" << endl;
        Reset();
    }
}

int main()
{
    Authentication auth;
    auth.AuthMainMenu();
    return 0;
}
