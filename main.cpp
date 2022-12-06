#include <iostream>
#include <string>
#include <vector>

#include "pugixml.hpp"
#include "pugixml.cpp"
#include "pugiconfig.hpp"

using namespace std;
using namespace pugi;

class Student {
    private:
        string Name;
        string SecondName;
        int Age;
    public:
        Student() = default;
        Student(string Name, string SecondName, int Age) {
            this->Name = Name;
            this->SecondName = SecondName;
            this->Age = Age;
        }
        string getName() { return this->Name; }
        string getSecondName() { return this->SecondName; }
        int getAge() { return this->Age; }

        void setName(string Name) { this->Name = Name; }
        void setSecondName(string SecondName) { this->SecondName = SecondName; }
        void setAge(int Age) { this->Age = Age; }
};

class Group {
    private:
        string GroupName;
        vector<Student> Students;
    public:
        Group() = default;
        Group(string GroupName, vector<Student> Students) {
            this->GroupName = GroupName;
            this->Students = Students;
        }
        string getGroupName() { return this->GroupName; }
        vector<Student> getStudents() { return this->Students; }
        void OutputStudents() {
            for (int i = 0; i < this->Students.size(); i++) {
                cout << '\t' << "Name: " << this->Students[i].getName() << "    " << "SecondName: "
                     << this->Students[i].getSecondName() << "    " << "Age: " << this->Students[i].getAge() << '\n';
            }
        }
        void Output() {
            cout << "Group Name: " << this->GroupName << '\n'
                 << "Students:" << '\n';
            OutputStudents();
            cout << '\n';
        }
};

class XmlWorker {
    private:
        Group group;

        void xmlInit(xml_node root) {}

    public:
        XmlWorker() = default;
        XmlWorker(xml_node root, Group group) : group(group) {
            this->xmlInit(root);
        }

        void xmlAddEvent(xml_node parent) {
            xml_node log = parent.append_child("LOG");
            xml_node addLog = log.append_child("ADD");
            addLog.append_child(node_cdata).set_value("Add new record!");
        }
        void xmlDeleteEvent(xml_node parent) {
            xml_node log = parent.append_child("LOG");
            xml_node addLog = log.append_child("DELETE");
            addLog.append_child(node_cdata).set_value("Record is Deleted!");
        }
        void xmlChangeEvent(xml_node parent) {
            xml_node log = parent.append_child("LOG");
            xml_node addLog = log.append_child("CHANGE");
            addLog.append_child(node_cdata).set_value("Record is changed!");
        }
};

int main() {
    xml_document document;
    xml_node root = document.append_child("LOGS");

    document.save_file("/home/iamvirgoo/CLionProjects/Observer/log.xml");

    auto AS = Group("AS-21-2",
                    {
                            Student("Vadim", "Breev", 21),
                            Student("Alexey", "Ryumkin", 19),
                            Student("Karen", "Khachatryan", 18)
                    });
    int operationNumber;
    vector<Student> students;
    for (auto el : AS.getStudents()) {
        students.push_back(el);
    }
    XmlWorker worker = XmlWorker(root, AS);
    do {
        cout << "1 - Append Record" << '\n'
             << "2 - Delete Record" << '\n'
             << "3 - Change the Record" << '\n'
             << "4 - Output the Record" << '\n'
             << "Enter the operation number: ";
        cin >> operationNumber;
        cout << '\n';
        switch (operationNumber) {
            case 1 : {
                string StudentName;
                string StudentSecondName;
                int StudentAge;

                cout << "Enter the name of new student: "; cin >> StudentName;
                cout << "Enter the second name of new student: "; cin >> StudentSecondName;
                cout << "Enter the age of new student: "; cin >> StudentAge;
                students.emplace_back(StudentName, StudentSecondName, StudentAge);
                AS = Group("AS-21-2", students);
                cout << "Added!" << '\n';
                worker.xmlAddEvent(root);
                break;
            }
            case 2 : {
                int IndexPosition;
                cout << "Enter the record position: "; cin >> IndexPosition;
                students.erase(students.begin() + IndexPosition);
                AS = Group("AS-21-2", students);
                cout << "Deleted!";
                worker.xmlDeleteEvent(root);
                break;
            }
            case 3 : {
                int IndexPosition;
                cout << "Enter the record position: "; cin >> IndexPosition;
                cout << AS.getStudents()[IndexPosition].getName() << " "<< AS.getStudents()[IndexPosition].getSecondName()
                << " " << AS.getStudents()[IndexPosition].getAge() << '\n';
                string Name;
                cout << "Enter new name: "; cin >> Name;
                string SecondName;
                cout << "Enter new secondName: "; cin >> SecondName;
                int Age;
                cout << "Enter new age: "; cin >> Age;
                students.erase(students.begin() + IndexPosition);
                students.insert(students.begin() + IndexPosition, Student(Name, SecondName, Age));
                AS = Group("AS-21-2", students);
                worker.xmlChangeEvent(root);
                break;
            }
            case 4 : {
                AS.Output();
                break;
            }
        }
    } while (operationNumber != 0);
    document.save_file("/home/iamvirgoo/CLionProjects/Observer/log.xml");
    return 0;
}