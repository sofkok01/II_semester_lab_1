#include <iostream>
#include <fstream>
#include <ctime>
#include <limits>
#include <cctype>
using namespace std;

int const gradeCount = 8;
int const maxStudent = 10;
int const maxTeam = 10;
int const sportCount = 5;
int const currentYear = 2026;
const string sports[sportCount] = {"волейбол", "баскетбол", "футбол", "скалолазание", "плавание"};
string const path = "database.txt";
string const teamPath = "competition.txt";


void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

struct profileOfStudent {
private:
    unsigned int id;
    string fullName;
    char sex;
    int group;
    int grades[gradeCount];
    bool hasGTO;
    int birthYear;
    string date;

public:
    //гарантия что значения не случайны
    profileOfStudent() {
        id = 0;
        group = 0;
        sex = 'U';
        hasGTO = false;
        birthYear = 2000;
    }
    //вход
    void setId(unsigned int newId) {id = newId;}
    bool setFullName(string newName) {
        if (newName.empty()) {
            cout << "Ошибка: ввод пустой" << endl;
            return false;
        }
        fullName = newName;
        return true;
    }
    bool setSex(char newSex) {
        newSex = toupper(newSex);
        if (newSex != 'M' && newSex != 'F') {
            cout << "Ошибка: пол только eng M/F!" << endl;
            return false;
        }
        sex = newSex;
        return true;
    }
    bool setGroup(int newGroup) {
        if (newGroup <= 0) {
            cout << "Ошибка: группа должна быть положительным числом!" << endl;
            return false;
        }
        group = newGroup;
        return true;
    }
    bool setGrade(int index, int newGrade) {
        if (newGrade < 2 || newGrade > 5) {
            cout << "Ошибка: диапозон оценок 2..5!" << endl;
            return false;
        }
        grades[index] = newGrade;
        return true;
    }
    void setHasGTO(bool gto) { hasGTO = gto; }
    void setBirthYear(int year) { birthYear = year; }
    void setDate(string newDate) {date = newDate;}

    //выход
    unsigned int getId() const {return id;}
    string getFullName() const {return fullName;}
    int getGroup() const { return group; }
    char getSex() const { return sex; }
    int getGrade(int index) const { return grades[index]; }
    bool getHasGTO() const { return hasGTO; }
    int getBirthYear() const { return birthYear; }
    string getDate() const { return date; }

    //вспомогательные методы
    float getAverage() const {
        float sum = 0;
        for (int i = 0; i < gradeCount; i++) {
            sum+=grades[i];
        }
        return sum/gradeCount;
    }
    bool hasDebt() const {
        for (int i = 0; i < gradeCount; i++) {
            if (grades[i] == 2) return true;
        }
        return false;
    }
    bool isExcellent() const {
        for (int i = 0; i < gradeCount; i++) {
            if (grades[i] != 5) return false;
        }
        return true;
    }
    bool isGood() const {
        for (int i = 0; i < gradeCount; i++) {
            if (grades[i] < 4) return false;
        }
        return true;
    }
    void updateDate() {
        time_t tstamp = time(nullptr);
        date = ctime(&tstamp);
        date.pop_back();
    }

    void inputFromUser() {
        cout << "Введите ФИО: ";
        cin.ignore();
        do {
            getline(cin, fullName);
            if (fullName.empty()) {
                cout << "ФИО не может быть пустым! Повторите ввод: ";
            }
        } while (fullName.empty());

        cout << "Введите номер группы: ";
        string groupStr;
        bool validGroup = false;
        do {
            getline(cin, groupStr);
            if (!isNumber(groupStr)) {
                cout << "Ошибка! Введите число: ";
            } else {
                group = stoi(groupStr);
                if (group <= 0) {
                    cout << "Группа должна быть положительным числом! Повторите: ";
                } else {
                    validGroup = true;
                }
            }
        } while (!validGroup);

        cout << "Введите пол (M/F): ";
        string sexStr;
        do {
            getline(cin, sexStr);
            if (sexStr.length() != 1) {
                cout << "Ошибка! Введите один символ (M или F): ";
                continue;
            }
            sex = toupper(sexStr[0]);
            if (sex != 'M' && sex != 'F') {
                cout << "Ошибка! Пол должен быть M или F. Повторите: ";
            }
        } while (sex != 'M' && sex != 'F');

        cout << "Введите год рождения: ";
        string yearStr;
        do {
            getline(cin, yearStr);
            if (!isNumber(yearStr)) {
                cout << "Ошибка! Введите число: ";
            } else {
                birthYear = stoi(yearStr);
                if (birthYear < 1950 || birthYear > currentYear) {
                    cout << "Ошибка! Год должен быть между 1900 и " << currentYear << ": ";
                } else {
                    break;
                }
            }
        } while (true);

        cout << "Есть золотой значок ГТО? (y/n): ";
        string gtoStr;
        getline(cin, gtoStr);
        hasGTO = (gtoStr == "y" || gtoStr == "Y");

        cout << "Введите " << gradeCount << " оценок (2-5):" << endl;
        for (int i = 0; i < gradeCount; i++) {
            string gradeStr;
            bool validGrade = false;
            do {
                cout << "Оценка " << i+1 << ": ";
                getline(cin, gradeStr);
                if (!isNumber(gradeStr)) {
                    cout << "Ошибка! Введите число от 2 до 5." << endl;
                    continue;
                }
                int g = stoi(gradeStr);
                if (g < 2 || g > 5) {
                    cout << "Ошибка! Оценка должна быть от 2 до 5." << endl;
                } else {
                    grades[i] = g;
                    validGrade = true;
                }
            } while (!validGrade);
        }
        cin.clear();
        updateDate();
    }
    void writeToFile(ofstream& file) const{
        file << id << endl;
        file << fullName << endl;
        file << group << endl;
        file << sex << endl;
        for (int i = 0; i < gradeCount; i++) {
            file << grades[i] << " ";
        }
        file << endl;
        file << hasGTO << endl;
        file << birthYear << endl;
        file << date << endl;
        file << endl;
    }
    void readFromFile(ifstream& file) {
        file >> id;
        file.ignore();

        getline(file, fullName);
        if (fullName.empty()) return;

        file >> group;
        file >> sex;

        for (int i = 0; i < gradeCount; i++) {
            file >> grades[i];
        }
        file.ignore();

        file >> hasGTO;
        file >> birthYear;
        file.ignore();
        getline(file, date);

        string temp;
        getline(file, temp); // пустая строка
    }
    void display() const {
        cout << "\nID: " << id << endl;
        cout << "ФИО: " << fullName << endl;
        cout << "Группа: " << group << endl;
        cout << "Пол: " << sex << endl;
        cout << "Оценки: ";
        for (int i = 0; i < gradeCount; i++) {
            cout << grades[i] << " ";
        }
        cout << "\nСредний балл: " << getAverage() << endl;
        cout << "Дата: " << date << endl;
    }

};
struct Team {
    string sportName;
    string teamName;
    unsigned int captainId;
};

void createRecord();                // 1. Создание записи
void modifyRecord();                // 2. Изменение записи
void displayAll();                  // 3. Вывод всех
void displayByGroup();              // 4. Вывод по группе
void displayTopStudents();          // 5. Топ успешных
void countGender();                 // 6. Статистика по полу
void displayScholarship();          // 7. Информация о стипендии
void displayById();                 // 8. Поиск по номеру в списке
void createTeam();                  // 9. ИДЗ создание команд
void displayYoungAndGTOCaptains();  //10. ИДЗ вывод моложе 18, гто, капитан

unsigned int getLastId() {
    ifstream file(path);
    if (!file.is_open()) return 0;

    profileOfStudent temp;
    unsigned int maxId = 0;

    while (!file.eof()) {
        temp.readFromFile(file);
        if (temp.getId() > maxId) maxId = temp.getId();
    }

    file.close();
    return maxId;
}
int loadAllStudents(profileOfStudent students[]) {
    ifstream file(path);
    if (!file.is_open()) return 0;

    int count = 0;
    while (!file.eof() && count < maxStudent) {
        students[count].readFromFile(file);
        if (students[count].getFullName().empty()) break;
        count++;
    }

    file.close();
    return count;
}
void saveAllStudents(profileOfStudent students[], int count) {
    ofstream file(path, ios::trunc);
    if (!file.is_open()) {
        cout << "Ошибка сохранения!" << endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        students[i].writeToFile(file);
    }

    file.close();
}
Team teams[maxTeam];
int teamCount = 0;
int loadTeamsFromFile() {
    ifstream file(teamPath);
    if (!file.is_open()) return 0;

    int count = 0;
    while (!file.eof() && count < maxTeam) {
        getline(file, teams[count].sportName);
        if (teams[count].sportName.empty()) break;

        getline(file, teams[count].teamName);
        file >> teams[count].captainId;
        file.ignore();

        string temp;
        getline(file, temp); // пустая строка

        count++;
    }

    file.close();
    return count;
}
void saveTeamsToFile() {
    ofstream file(teamPath, ios::trunc);
    if (!file.is_open()) {
        cout << "Ошибка сохранения команд!" << endl;
        return;
    }

    for (int i = 0; i < teamCount; i++) {
        file << teams[i].sportName << endl;
        file << teams[i].teamName << endl;
        file << teams[i].captainId << endl;
        file << endl;
    }

    file.close();
}

void menu() {
    cout << endl;
    cout << " 1. Создать новую запись о студенте" << endl;
    cout << " 2. Внести изменения в существующую запись" << endl;
    cout << " 3. Вывести все данные о студентах" << endl;
    cout << " 4. Вывести студентов указанной группы" << endl;
    cout << " 5. Вывести топ самых успешных студентов" << endl;
    cout << " 6. Вывести статистику по полу" << endl;
    cout << " 7. Вывести информацию о стипендии" << endl;
    cout << " 8. Найти студента по номеру в списке" << endl;
    cout << " 9. Работа с командами" << endl;
    cout << "10. Выход из программы" << endl;
    cout << "═══════════════════════════════════════════" << endl;
    cout << "Выберите действие (1-10): ";
}

int main() {
    teamCount = loadTeamsFromFile();  //загружаем один раз сюда, тк не изменяем их

    int choice;
    do {
        menu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: createRecord(); break;
            case 2: modifyRecord(); break;
            case 3: displayAll(); break;
            case 4: displayByGroup(); break;
            case 5: displayTopStudents(); break;
            case 6: countGender(); break;
            case 7: displayScholarship(); break;
            case 8: displayById(); break;
            case 9: {
                int teamChoice;
                do {
                    cout << "\n=== РАБОТА С КОМАНДАМИ ===" << endl;
                    cout << "1. Создать новую команду" << endl;
                    cout << "2. Вывести студентов моложе 18 лет и капитанов с ГТО" << endl;
                    cout << "3. Вернуться в главное меню" << endl;
                    cout << "Выберите действие: ";
                    cin >> teamChoice;
                    cin.ignore();

                    switch (teamChoice) {
                        case 1: createTeam(); break;
                        case 2: displayYoungAndGTOCaptains(); break;
                        case 3: cout << "Возврат в главное меню" << endl; break;
                        default: cout << "Неверный выбор!" << endl;
                    }
                } while (teamChoice != 3);
                break;
            }
            case 10:
                cout << "\nПрограмма завершена" << endl;
                break;
            default:
                cout << "\nНеверный выбор! Введите число от 1 до 10." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
        }
    } while (choice != 10);
    return 0;
}

void createRecord() {
    profileOfStudent newStudent;
    newStudent.inputFromUser();

    if (newStudent.hasDebt()) {
        cout << "Студент имеет задолженности! Запись не сохранена." << endl;
        return;
    }
    else {
        unsigned int lastId = getLastId();
        newStudent.setId(lastId + 1);
        cout << "Студент числится" << endl;
    }

    ofstream file(path, ios::app);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }
    else {
        newStudent.writeToFile(file);
        cout << "Запись сохранена в файл, id студента: " << newStudent.getId() << endl;
        file.close();
    }
}
void modifyRecord() {
    unsigned int searchId;
    unsigned int maxId = getLastId();

    if (maxId == 0) {
        cout << "Файл пуст"<< endl;
        return;
    }
    do {
        cout << "Допустимое id: 1.." << maxId << endl;
        cout << "Введите id студента для изменения: ";
        cin >> searchId;

        if (cin.fail()) {
            clearInput();
            searchId = 0;
            cout << "Ошибка! Введите ЧИСЛО в диапозоне от 1.." << maxId << endl;
        }
    } while (searchId < 1 || searchId > maxId);

    //считываем всех стужентов
    profileOfStudent students[maxStudent];
    int count = loadAllStudents(students);

    int foundIndex = 0;
    for (int i =0; i< count; i++) {
        if (students[i].getId() == searchId) {
            foundIndex = i;
            break;
        }
    }

    // Вводим новые данные
    cout << "\nВведите новые данные (Enter - оставить без изменений):" << endl;

    cout << "Новое ФИО: ";
    clearInput();
    string newName;
    getline(cin, newName);
    if (!newName.empty()) {
        students[foundIndex].setFullName(newName);
    }

    cout << "Новая группа: ";
    string newGroup;
    getline(cin, newGroup);
    if (!newGroup.empty()) {
        if (isNumber(newGroup)) {
            students[foundIndex].setGroup(stoi(newGroup));
        } else {
            cout << "Ошибка! Группа должна быть числом. Изменение пропущено." << endl;
        }
    }

    cout << "Новый пол (M/F): ";
    string newSex;
    getline(cin, newSex);
    if (!newSex.empty()) {
        if (newSex.length() == 1) {
            students[foundIndex].setSex(newSex[0]);
        } else {
            cout << "Ошибка! Введите один символ. Изменение пропущено." << endl;
        }
    }

    cout << "Хотите изменить оценки? (y/n): ";
    string changeGrades;
    getline(cin, changeGrades);
    if (changeGrades == "y" || changeGrades == "Y") {
        cout << "Введите " << gradeCount << " оценок (2-5):" << endl;
        for (int i = 0; i < gradeCount; i++) {
            string gradeStr;
            bool validGrade = false;
            do {
                cout << "Оценка " << i+1 << ": ";
                getline(cin, gradeStr);
                if (!isNumber(gradeStr)) {
                    cout << "Ошибка! Введите число от 2 до 5." << endl;
                    continue;
                }
                int g = stoi(gradeStr);
                if (g < 2 || g > 5) {
                    cout << "Ошибка! Оценка должна быть от 2 до 5." << endl;
                } else {
                    students[foundIndex].setGrade(i, g);
                    validGrade = true;
                }
            } while (!validGrade);
        }
    }

    // Обновляем дату
    students[foundIndex].updateDate();

    // Сохраняем
    saveAllStudents(students, count);
    cout << "Данные обновлены" << endl;
}
void displayAll() {
    profileOfStudent students[maxStudent];
    int count = loadAllStudents(students);

    if (count == 0) {
        cout << "Нет записей." << endl;
        return;
    }

    cout << "ВСЕ СТУДЕНТЫ" << endl;
    for (int i = 0; i<count; i++) {
        students[i].display();
    }

}
void displayByGroup() {
    int targetGroup;
    cout << "Введите номер группы: ";

    string groupStr;
    cin >> groupStr;
    if (!isNumber(groupStr)) {
        cout << "Ошибка! Некорректный номер группы." << endl;
        return;
    }
    targetGroup = stoi(groupStr);

    // Загружаем всех студентов
    profileOfStudent students[maxStudent];
    int count = loadAllStudents(students);

    if (count == 0) {
        cout << "Нет записей в базе данных." << endl;
        return;
    }

    int found = 0;
    cout << "СТУДЕНТЫ ГРУППЫ " << targetGroup << endl;
    for (int i = 0; i < count; i++) {
        if (students[i].getGroup() == targetGroup) {
            found++;
            students[i].display();
            cout << "------------------------" << endl;
        }
    }

    if (found == 0) {
        cout << "Студенты группы " << targetGroup << " не найдены." << endl;
    } else {
        cout << "Всего студентов в группе " << targetGroup << ": " << found << endl;
    }
}
void displayTopStudents() {
    profileOfStudent students[maxStudent];
    int count = loadAllStudents(students);

    if (count ==0) {
        cout << "Файл пуст" << endl;
        return;
    }
    //по среднему баллу сортирую и после вывожу
    struct Rating {
        int index;
        float avarege;
    } ratings[maxStudent];
    //иницилизируем
    for (int i = 0; i< count; i++) {
        ratings[i].index = i;
        ratings[i].avarege = students[i].getAverage();
    }

    //сортируем
    for (int i = 0; i<count; i++) {
        for (int j = 0; j<count -i-1; j++) {
            if (ratings[j].avarege < ratings[j + 1].avarege) {
                swap(ratings[j], ratings[j + 1]);
            }
        }
    }
    int topCount = min(3, count);
    cout << "-----ТОП " << topCount << "СТУДЕНТОВ------" << endl;
    for (int i=0; i<topCount; i++) {
        int idx = ratings[i].index; //получаем исхожный индекс
        cout << students[idx].getFullName() << " " << ratings[i].avarege << endl;
        cout << endl;
    }
}
void countGender() {
    profileOfStudent students[maxStudent];
    int count = loadAllStudents(students);
    int male = 0, female =0;

    for (int i = 0; i < count; i++) {
        if (students[i].getSex() == 'M') male++;
        else if (students[i].getSex() == 'F') female++;
    }

    cout << "\nСтатистика по полу:" << endl;
    cout << "Мужчины: " << male << endl;
    cout << "Женщины: " << female << endl;
}
void displayScholarship() {
    profileOfStudent students[maxStudent];
    int count = loadAllStudents(students);

    if (count == 0) {
        cout << "Файл пуст" << endl;
        return;
    }

    cout << "--НЕ ПОЛУЧАЮТ СТЕПУХУ--" << endl;
    for (int i = 0; i < count; i++) {
        if (!students[i].isExcellent() && !students[i].isGood()) {
            cout << "ID:" << students[i].getId() << " " << students[i].getFullName() << endl;
        }
    }
    cout << "--ХОР И ОТЛ--" << endl;
    for (int i = 0; i < count; i++) {
        if (!students[i].isExcellent() && students[i].isGood()) {
            cout << "ID:" << students[i].getId() << " " << students[i].getFullName() << endl;
        }
    }
    cout << "--ОТЛИЧНИКИ--" << endl;
    for (int i = 0; i < count; i++) {
        if (students[i].isExcellent() && !students[i].isGood()) {
            cout << "ID:" << students[i].getId() << " " << students[i].getFullName() << endl;
        }
    }
}
void displayById() {
    unsigned int searchId;
    cout << "Введите ID студента: ";
    string idStr;
    cin >> idStr;

    if (!isNumber(idStr)) {
        cout << "Ошибка! Некорректный ID." << endl;
        return;
    }
    searchId = stoi(idStr);

    profileOfStudent students[maxStudent];
    int count = loadAllStudents(students);
    bool found = false;

    for (int i = 0; i < count; i++) {
        if (students[i].getId() == searchId) {
            found = true;
            cout << "СТУДЕНТ НАЙДЕН:" << endl;
            students[i].display();
            break;
        }
    }
    if (!found) {
        cout << "Студент с ID " << searchId << " не найден!" << endl;
    }
}
void createTeam() {
    // Загружаем студентов
    profileOfStudent students[maxStudent];
    int studentCount = loadAllStudents(students);
    if (studentCount == 0) {
        cout << "Нет студентов в базе! Сначала добавьте студентов." << endl;
        return;
    }
    if (teamCount >= maxTeam) {
        cout << "Достигнуто максимальное количество команд!" << endl;
        return;
    }

    Team newTeam;

    cout << "=== СОЗДАНИЕ КОМАНДЫ ===" << endl;
    cout << "Выберите вид спорта:" << endl;
    for (int i = 0; i < sportCount; i++) {
        cout << " " << i+1 << ". " << sports[i] << endl;
    }

    int sportChoice;
    do {
        cout << "Ваш выбор (1-5): ";
        cin >> sportChoice;
        if (sportChoice < 1 || sportChoice > 5) {
            cout << "Ошибка! Введите число от 1 до 5." << endl;
        }
    } while (sportChoice < 1 || sportChoice > 5);

    newTeam.sportName = sports[sportChoice - 1];
    cin.ignore();

    cout << "Введите название команды: ";
    getline(cin, newTeam.teamName);


    cout << "\nСписок доступных студентов:" << endl;
    for (int i = 0; i < studentCount; i++) {
        cout << " ID: " << students[i].getId()
             << " | " << students[i].getFullName() << endl;
    }

    unsigned int captainId;
    bool foundCaptain = false;

    do {
        cout << "Введите ID студента-капитана: ";
        cin >> captainId;

        foundCaptain = false;
        for (int i = 0; i < studentCount; i++) {
            if (students[i].getId() == captainId) {
                foundCaptain = true;
                break;
            }
        }

        if (!foundCaptain) {
            cout << "Ошибка! Студент с ID " << captainId << " не найден." << endl;
        }
    } while (!foundCaptain);

    newTeam.captainId = captainId;
    teams[teamCount] = newTeam;
    teamCount++;

    saveTeamsToFile();

    cout << "Команда успешно создана!" << endl;
    cout << "Вид спорта: " << newTeam.sportName << endl;
    cout << "Название: " << newTeam.teamName << endl;
    cout << "ID капитана: " << captainId << endl;
}
void displayYoungAndGTOCaptains() {
    profileOfStudent students[maxStudent];
    int studentCount = loadAllStudents(students);

    if (studentCount == 0) {
        cout << "Нет студентов в базе." << endl;
        return;
    }

    cout << "--СТУДЕНТЫ МОЛОЖЕ 18 ЛЕТ--" << endl;
    bool foundYoung = false;
    for (int i = 0; i < studentCount; i++) {
        int age = currentYear - students[i].getBirthYear();
        if (age < 18) {
            foundYoung = true;
            cout << students[i].getFullName()
                 << " (возраст: " << age << " лет, год рожд.: "
                 << students[i].getBirthYear() << ")" << endl;
        }
    }
    if (!foundYoung) cout << "Нет студентов моложе 18 лет" << endl;

    cout << "--КАПИТАНЫ С ЗОЛОТЫМ ЗНАЧКОМ ГТО--" << endl;
    bool foundCaptain = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].getHasGTO()) {
            for (int j = 0; j < teamCount; j++) {
                if (teams[j].captainId == students[i].getId()) {
                    foundCaptain = true;
                    cout << students[i].getFullName();
                    cout << "   Команда: " << teams[j].teamName << " (" << teams[j].sportName << ")" << endl;
                    cout << "   ID капитана: " << students[i].getId() << endl;
                    break;
                }
            }
        }
    }
    if (!foundCaptain) {
        cout << "Нет капитанов с золотым значком ГТО" << endl;
    }


}