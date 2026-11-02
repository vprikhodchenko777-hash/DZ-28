
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class FileName
{
private:
    string name;

public:
    FileName(const string& name) : name(name)
    {
    }

    string getName() const
    {
        return name;
    }

    string getExtension() const
    {
        size_t pos = name.rfind('.');

        if (pos == string::npos)
            return "";

        return name.substr(pos + 1);
    }

    string getBaseName() const
    {
        size_t pos = name.rfind('.');

        if (pos == string::npos)
            return name;

        return name.substr(0, pos);
    }

    void changeExtension(const string& newExtension)
    {
        name = getBaseName() + "." + newExtension;
    }

    bool isImage() const
    {
        string ext = getExtension();

        return ext == "png" || ext == "jpg" || ext == "gif";
    }

    void print() const
    {
        cout << name
            << " | имя: " << getBaseName()
            << " | расширение: [" << getExtension() << "]";

        if (isImage())
            cout << " | изображение";

        cout << endl;
    }
};

class MessageTemplate
{
private:
    string text;

    static void replaceAll(string& text, const string& from,
        const string& to)
    {
        if (from.empty())
            return;

        size_t position = text.find(from);

        while (position != string::npos)
        {
            text.replace(position, from.length(), to);
            position = text.find(from, position + to.length());
        }
    }

public:
    MessageTemplate(const string& text) : text(text)
    {
    }

    int countPlaceholders() const
    {
        int count = 0;

        for (int i = 0; i < (int)text.length(); i++)
        {
            if (text[i] == '{')
                count++;
        }

        return count;
    }

    string fill(const string& name, int score) const
    {
        string result = text;

        replaceAll(result, "{name}", name);
        replaceAll(result, "{score}", to_string(score));

        return result;
    }
};

void parseStudent(const string& line)
{
    size_t pos1 = line.find(';');
    size_t pos2 = line.find(';', pos1 + 1);

    if (pos1 == string::npos || pos2 == string::npos)
    {
        cout << "Ошибка формата данных" << endl;
        return;
    }

    string name = line.substr(0, pos1);
    string ageText = line.substr(pos1 + 1, pos2 - pos1 - 1);
    string email = line.substr(pos2 + 1);

    int age = stoi(ageText);

    cout << "Имя: " << name << endl;
    cout << "Возраст: " << age
        << ", в следующем году: " << age + 1 << endl;
    cout << "Почта: " << email << endl;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "1. Работа с файлами" << endl;

    FileName report("report.final.docx");
    FileName photo("photo.png");
    FileName readme("README");

    report.print();
    photo.print();
    readme.print();

    cout << endl << "2. Изменение расширения" << endl;

    report.changeExtension("pdf");
    readme.changeExtension("md");

    report.print();
    readme.print();

    cout << endl << "3. Копирование" << endl;

    FileName copy = report;
    copy.changeExtension("txt");

    cout << "Оригинал: " << report.getName() << endl;
    cout << "Копия: " << copy.getName() << endl;

    cout << endl << "4. Сравнение файлов" << endl;

    if (photo.getName() < report.getName())
        cout << photo.getName() << " стоит раньше" << endl;
    else
        cout << report.getName() << " стоит раньше" << endl;

    cout << endl << "5. Рассылка сообщений" << endl;

    MessageTemplate message(
        "Привет, {name}! Твой балл за модуль: {score}. "
        "{name}, так держать!"
    );

    string names[3] = { "Олена", "Марко", "Тарас" };
    int scores[3] = { 11, 9, 12 };

    cout << "Количество мест для подстановки: "
        << message.countPlaceholders() << endl;

    for (int i = 0; i < 3; i++)
    {
        cout << message.fill(names[i], scores[i]) << endl;
    }

    cout << endl << "6. Данные студента" << endl;

    parseStudent("Олена Коваль;20;olena.koval@itstep.org");

    return 0;
}

/*

    1. 
       найти последнюю точку, определить длину расширения,
       выделить память через new[], скопировать символы
       и потом освободить память через delete[].
       string делает это автоматически через rfind и substr.

    2. Для "README" метод rfind('.') вернёт string::npos,
       потому что точки нет.
       

    3. FileName содержит поле string, которое само управляет
       памятью. При копировании создаётся независимая копия
       строки. 

    4. "Привет, " + "Олена" не компилируется, потому что
       оба значения являются C-строками.
       

    5. После замены продолжаем поиск с позиции
       position + to.length(), чтобы не искать повторно
       внутри уже вставленного текста.
       

    6.
       Строка "20" содержит текст, а не число,
       поэтому обычное арифметическое сложение
       с единицей для неё невозможно.
*/
