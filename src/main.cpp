```cpp
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

const string DATA_FILE = "tasks.txt";

struct Task
{
    string title;
    bool completed;
};

void saveTasks(const vector<Task>& tasks)
{
    ofstream file(DATA_FILE);

    for (const Task& task : tasks)
    {
        file << (task.completed ? 1 : 0)
             << "|"
             << task.title
             << '\n';
    }
}

void loadTasks(vector<Task>& tasks)
{
    ifstream file(DATA_FILE);

    if (!file.is_open())
    {
        return;
    }

    string line;

    while (getline(file, line))
    {
        size_t separatorPos = line.find('|');

        if (separatorPos == string::npos)
        {
            continue;
        }

        string statusPart = line.substr(0, separatorPos);
        string titlePart = line.substr(separatorPos + 1);

        Task task;
        task.completed = (statusPart == "1");
        task.title = titlePart;

        tasks.push_back(task);
    }
}

void addTask(vector<Task>& tasks, const string& title)
{
    if (title.empty())
    {
        cout << "Task title cannot be empty\n";
        return;
    }

    Task task;
    task.title = title;
    task.completed = false;

    tasks.push_back(task);

    saveTasks(tasks);

    cout << "Task added\n";
}

void listTasks(const vector<Task>& tasks)
{
    if (tasks.empty())
    {
        cout << "No tasks found\n";
        return;
    }

    for (size_t i = 0; i < tasks.size(); i++)
    {
        cout << i + 1 << ". ";

        cout << (tasks[i].completed ? "[x] " : "[ ] ");

        cout << tasks[i].title << '\n';
    }
}

bool isValidTaskNumber(int number, const vector<Task>& tasks)
{
    return number >= 1 && number <= static_cast<int>(tasks.size());
}

void markDone(vector<Task>& tasks, int number)
{
    if (!isValidTaskNumber(number, tasks))
    {
        cout << "Invalid task number\n";
        return;
    }

    tasks[number - 1].completed = true;

    saveTasks(tasks);

    cout << "Task marked as completed\n";
}

void markUndone(vector<Task>& tasks, int number)
{
    if (!isValidTaskNumber(number, tasks))
    {
        cout << "Invalid task number\n";
        return;
    }

    tasks[number - 1].completed = false;

    saveTasks(tasks);

    cout << "Task marked as not completed\n";
}

void deleteTask(vector<Task>& tasks, int number)
{
    if (!isValidTaskNumber(number, tasks))
    {
        cout << "Invalid task number\n";
        return;
    }

    tasks.erase(tasks.begin() + (number - 1));

    saveTasks(tasks);

    cout << "Task deleted\n";
}

void clearCompletedTasks(vector<Task>& tasks)
{
    cout << "Delete all completed tasks? (y/n): ";

    string answer;
    getline(cin, answer);

    if (answer != "y" && answer != "Y")
    {
        cout << "Operation cancelled\n";
        return;
    }

    tasks.erase(
        remove_if(
            tasks.begin(),
            tasks.end(),
            [](const Task& task)
            {
                return task.completed;
            }),
        tasks.end());

    saveTasks(tasks);

    cout << "Completed tasks removed\n";
}

int main()
{
    vector<Task> tasks;

    loadTasks(tasks);

    cout << "Todo List started\n";
    cout << "Type 'exit' to quit\n";

    while (true)
    {
        cout << "> ";

        string command;
        getline(cin, command);

        if (command.empty())
        {
            cout << "Empty command\n";
            continue;
        }

        if (command == "exit")
        {
            break;
        }
        else if (command == "list")
        {
            listTasks(tasks);
        }
        else if (command == "clear")
        {
            clearCompletedTasks(tasks);
        }
        else if (command.rfind("add ", 0) == 0)
        {
            string title = command.substr(4);
            addTask(tasks, title);
        }
        else if (command.rfind("done ", 0) == 0)
        {
            int number = stoi(command.substr(5));
            markDone(tasks, number);
        }
        else if (command.rfind("undone ", 0) == 0)
        {
            int number = stoi(command.substr(7));
            markUndone(tasks, number);
        }
        else if (command.rfind("delete ", 0) == 0)
        {
            int number = stoi(command.substr(7));
            deleteTask(tasks, number);
        }
        else
        {
            cout << "Unknown command\n";
        }
    }

    return 0;
}
```

