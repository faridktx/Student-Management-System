#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;


// I put everything into one file just for the sake of it being easier to zip it all up and submit, I can put everything back into seperate files if needed.


// Person class that is going to have the Student and Teacher class branching off of
class Person {
protected:
    int id;
    string name;
    string email;

public:
    Person(int id, string name, string email) : id(id), name(name), email(email) {}
    int getId() const {
        return id;
    }
    string getName() const {
        return name;
    }
    string getEmail() const {
        return email;
    }
};


//Student class inheriting from the Person class
class Student : public Person {
private:
    map<string, string> attendance;
    map<string, double> assignments;

public:
    Student(int id, string name, string email) : Person(id, name, email) {}


    // All the functions needed for attendance checking and inputting grades
    void markAttendance(const string& date, const string& status) {
        attendance[date] = status;
    }

    void submitAssignment(const string& assignment, double grade) {
        assignments[assignment] = grade;
    }

    double calculateClassGrade(const string& className) {
        double sum = 0;
        int count = 0;
        for (const auto& [assignment, grade] : assignments) {
            sum += grade;
            count++;
        }

        return (count > 0) ? sum / count : 0;
    }
};


//Teacher class inheriting off the Person class
class Teacher : public Person {
private:
    vector<string> subjects;

public:

    // Allows the user to input information needed about a certain teacher and to update their subjects and such
    Teacher(int id, string name, string email, const vector<string>& subjects)
            : Person(id, name, email), subjects(subjects) {}

    void updateSubjects(const vector<string>& newSubjects) {
        subjects = newSubjects;
    }
};


//Separate Class class
class Class {
private:
    int id;
    string name;
    Teacher* teacher;
    vector<Student*> students;
    map<int, map<string, string>> studentAttendances;

public:
    Class(int id, string name, Teacher* teacher) : id(id), name(name), teacher(teacher) {}

    int getId() const {
        return id;
    }
    string getName() const {
        return name;
    }
    int getTeacherId() const {
        return teacher->getId();
    }

    void addStudent(Student* student) {
        students.push_back(student);
    }

    void enterExamGrades(const string& exam, const vector<double>& grades) {
        for (size_t i = 0; i < students.size() && i < grades.size(); i++) {
            students[i]->submitAssignment(exam, grades[i]);
        }
    }

    void recordStudentAttendance(int studentId, const string& date, const string& status) {
        studentAttendances[studentId][date] = status;
    }

    map<string, string> generateAttendanceReport(const string& startDate, const string& endDate) {
        map<string, string> report;

        for (Student* student : students) {
            string studentInfo = "ID: " + to_string(student->getId()) + ", Name: " + student->getName();
            report[studentInfo] = ""; // Initialize with an empty string

            for (const auto& date_status : studentAttendances[student->getId()]) {
                if (date_status.first >= startDate && date_status.first <= endDate) {
                    report[studentInfo] += date_status.first + ": " + date_status.second + "; ";
                }
            }
        }

        return report;
    }
};


//The method that's going to be called repeatedly to get the main menu into the console
void displayMenu() {
    cout << "---------------------------------------------\n";
    cout << "| 1. Add teacher                            |\n";
    cout << "| 2. Add student                            |\n";
    cout << "| 3. Add class                              |\n";
    cout << "| 4. Assign student to class                |\n";
    cout << "| 5. Record attendance                      |\n";
    cout << "| 6. Record grades                          |\n";
    cout << "| 7. Generate attendance report             |\n";
    cout << "| 8. Calculate student's class grade        |\n";
    cout << "| 9. Display all teachers                   |\n";
    cout << "| 10. Display all students                  |\n";
    cout << "| 11. Display all classes                   |\n";
    cout << "| 12. Save data                             |\n";
    cout << "| 13. Load data                             |\n";
    cout << "| 14. Exit                                  |\n";
    cout << "---------------------------------------------\n";
}



// Saves all the data for teachers, students, and classes into seperate files
void saveData(const vector<Teacher>& teachers, const vector<Student>& students, const vector<Class>& classes) {
    ofstream teacherFile("teachers.txt");


    // All of these simply loop through every teacher, student, and class and saves them into separate files so they can be loaded later

    for (const auto& teacher : teachers) {
        teacherFile << teacher.getId() << "," << teacher.getName() << "," << teacher.getEmail() << "\n";
    }
    teacherFile.close();

    ofstream studentFile("students.txt");
    for (const auto& student : students) {
        studentFile << student.getId() << "," << student.getName() << "," << student.getEmail() << "\n";
    }
    studentFile.close();

    ofstream classFile("classes.txt");
    for (const auto& classObj : classes) {
        classFile << classObj.getId() << "," << classObj.getName() << "," << classObj.getTeacherId() << "\n";
    }
    classFile.close();
}


//loads all the data from those files that you save with the save data function
void loadData(vector<Teacher>& teachers, vector<Student>& students, vector<Class>& classes) {
    ifstream teacherFile("teachers.txt");string line;
    while (getline(teacherFile, line)) {
        stringstream ss(line);
        string idStr, name, email;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');

        int id = stoi(idStr);
        teachers.emplace_back(id, name, email, vector<string>());
    }
    teacherFile.close();

    ifstream studentFile("students.txt");
    while (getline(studentFile, line)) {
        stringstream ss(line);
        string idStr, name, email;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');

        int id = stoi(idStr);
        students.emplace_back(id, name, email);
    }
    studentFile.close();

    ifstream classFile("classes.txt");
    while (getline(classFile, line)) {
        stringstream ss(line);
        string idStr, name, teacherIdStr;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, teacherIdStr, ',');

        int id = stoi(idStr);
        int teacherId = stoi(teacherIdStr);

        Teacher* teacher = nullptr;
        for (Teacher& t : teachers) {
            if (t.getId() == teacherId) {
                teacher = &t;
                break;
            }
        }

        if (teacher != nullptr) {
            classes.emplace_back(id, name, teacher);
        }
    }
    classFile.close();
}


//the main class where everything goes down
int main() {

    vector<Teacher> teachers;
    vector<Student> students;
    vector<Class> classes;

    loadData(teachers, students, classes);

    int choice;

    //do while loop that keeps putting up the menu and acting correspondingly as long as the user doesn't choose to exit the program
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id;
                string name, email;
                vector<string> subjects;
                string subject;

                cout << "Enter teacher ID: ";
                cin >> id;
                cout << "Enter teacher name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter teacher email: ";
                getline(cin, email);
                cout << "Enter subjects taught (one per line, empty line to finish):\n";
                while (getline(cin, subject) && !subject.empty()) {
                    subjects.push_back(subject);
                }

                teachers.emplace_back(id, name, email, subjects);
                break;
            }
            case 2: {
                int id;
                string name, email;

                cout << "Enter student ID: ";
                cin >> id;
                cout << "Enter student name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter student email: ";
                getline(cin, email);

                students.emplace_back(id, name, email);
                break;
            }
            case 3: {
                int id, teacherId;
                string name;

                cout << "Enter class ID: ";
                cin >> id;
                cout << "Enter class name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter teacher ID: ";
                cin >> teacherId;

                Teacher *teacher = nullptr;
                for (Teacher &t: teachers) {
                    if (t.getId() == teacherId) {
                        teacher = &t;
                        break;
                    }
                }

                if (teacher == nullptr) {
                    cout << "Error: Teacher not found.\n";
                } else {
                    classes.emplace_back(id, name, teacher);
                }
                break;
            }
            case 4: {
                int classId, studentId;

                cout << "Enter class ID: ";
                cin >> classId;
                cout << "Enter student ID: ";
                cin >> studentId;

                Class *classObj = nullptr;
                for (Class &c: classes) {
                    if (c.getId() == classId) {
                        classObj = &c;
                        break;
                    }
                }

                Student *student = nullptr;
                for (Student &s: students) {
                    if (s.getId() == studentId) {
                        student = &s;
                        break;
                    }
                }

                if (classObj == nullptr) {
                    cout << "Error: Class not found.\n";
                } else if (student == nullptr) {
                    cout << "Error: Student not found.\n";
                } else {
                    classObj->addStudent(student);
                }
                break;
            }
            case 5: {
                int studentId;
                string date, status;

                cout << "Enter student ID: ";
                cin >> studentId;
                cout << "Enter date (YYYY-MM-DD): ";
                cin.ignore();
                getline(cin, date);
                cout << "Enter attendance status (P/A): ";
                getline(cin, status);

                Student *student = nullptr;
                for (Student &s: students) {
                    if (s.getId() == studentId) {
                        student = &s;
                        break;
                    }
                }

                if (student == nullptr) {
                    cout << "Error: Student not found.\n";
                } else {
                    student->markAttendance(date, status);
                    cout << "Attendance recorded successfully.\n";
                }
                break;
            }
            case 6: {
                int classId;
                string examName;
                vector<double> grades;
                double grade;
                cout << "Enter class ID: ";
                cin >> classId;
                cout << "Enter exam name: ";
                cin.ignore();
                getline(cin, examName);
                cout << "Enter grades (one per line, -1 to finish):\n";
                while (cin >> grade && grade != -1) {
                    grades.push_back(grade);
                }

                Class *classObj = nullptr;
                for (Class &c: classes) {
                    if (c.getId() == classId) {
                        classObj = &c;
                        break;
                    }
                }

                if (classObj == nullptr) {
                    cout << "Error: Class not found.\n";
                } else {
                    classObj->enterExamGrades(examName, grades);
                }
                break;
            }
            case 7: {
                int classId;
                string startDate, endDate;

                cout << "Enter class ID: ";
                cin >> classId;
                cout << "Enter start date (YYYY-MM-DD): ";
                cin.ignore();
                getline(cin, startDate);
                cout << "Enter end date (YYYY-MM-DD): ";
                getline(cin, endDate);

                Class *classObj = nullptr;
                for (Class &c: classes) {
                    if (c.getId() == classId) {
                        classObj = &c;
                        break;
                    }
                }

                if (classObj == nullptr) {
                    cout << "Error: Class not found.\n";
                } else {
                    map<string, string> report = classObj->generateAttendanceReport(startDate, endDate);
                    cout << "Attendance report:\n";
                    for (const auto &entry : report) {
                        cout << entry.first << ": " << entry.second << "\n";
                    }
                }
                break;
            }
            case 8: {
                int studentId;
                string className;

                cout << "Enter student ID: ";
                cin >> studentId;
                cout << "Enter class name: ";
                cin.ignore();
                getline(cin, className);

                Student *student = nullptr;
                for (Student &s: students) {
                    if (s.getId() == studentId) {
                        student = &s;
                        break;
                    }
                }

                if (student == nullptr) {
                    cout << "Error: Student not found.\n";
                } else {
                    double grade = student->calculateClassGrade(className);
                    cout << "Student's class grade: " << grade << "\n";
                }
                break;
            }
            case 9: {
                cout << "Teachers list:\n";
                for (const Teacher &t : teachers) {
                    cout << "ID: " << t.getId() << ", Name: " << t.getName() << ", Email: " << t.getEmail() << "\n";
                }
                break;
            }
            case 10: {
                cout << "Students list:\n";
                for (const Student &s : students) {
                    cout << "ID: " << s.getId() << ", Name: " << s.getName() << ", Email: " << s.getEmail() << "\n";
                }
                break;
            }
            case 11: {
                cout << "Classes list:\n";
                for (const Class &c : classes) {
                    cout << "ID: " << c.getId() << ", Name: " << c.getName() << ", Teacher ID: " << c.getTeacherId() << "\n";
                }
                break;
            }
            case 12: {
                saveData(teachers, students, classes);
                cout << "Data saved successfully.\n";
                break;
            }
            case 13: {
                loadData(teachers, students, classes);
                cout << "Data loaded successfully.\n";
                break;
            }
            case 14:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option, try again.\n";
                break;
        }
    } while (choice != 14);

    saveData(teachers, students, classes);

    return 0;
}