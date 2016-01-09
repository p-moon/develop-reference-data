#include <QCoreApplication>
#include <QString>
#include <QDate>
#include <QList>
#include <QDebug>

class Person
{
public:
    Person(const QString &name,
           const QDate &birthday,
           const QString &address)
        : m_name(name)
        , m_birthday(birthday)
        , m_address(address)
    {

    }

    Person(const Person &person)
        : m_name(person.m_name)
        , m_birthday(person.m_birthday)
        , m_address(person.m_address)
    {

    }

    void show()
    {
        qDebug() << "++Name: " << m_name;
        qDebug() << "  Birthday: "
                 << m_birthday.toString("yyyy-MM-dd");
        qDebug() << "  Address: " << m_address;
    }

    QString m_name;
    QDate m_birthday;
    QString m_address;
};

void loadPersons(QList<Person*> & persons)
{
    QDate date(1980, 3, 1);
    persons.append(new Person("Mike",
                              date, "xi\'an, China"));
    persons.append(new Person("Jack",
                              date.addDays(500),
                              "New Jersey, American"));
    persons.append(new Person("ZhangSan",
                              date.addYears(1),
                              "zheng zhou, China"));
    persons.append(new Person("Mary",
                              date.addMonths(20),
                              "London, England"));
}

void showPersonsByIndex(QList<Person*> & persons)
{
    int size = persons.size();
    qDebug() << "showPersonsByIndex, total - " << size;
    for(int i = 0; i < size; i++)
    {
        persons[i]->show();
    }
}

void showPersonsByIterator(QList<Person*> &persons)
{
    qDebug() << "showPersonsByIterator, total - "
             << persons.count();
    QList<Person*>::iterator it = persons.begin();
    while( it != persons.end() )
    {
        (*it)->show();
        ++it;
    }
}

void deletePerson(QList<Person*> &persons
                  ,const QString &name)
{
    qDebug() << "deletePerson, name - " << name;
    int size = persons.size();
    for(int i = 0; i < size; i++)
    {
        if( persons[i]->m_name == name )
        {
            delete persons.takeAt(i);
            qDebug() << name << " was deleted";
            break;
        }
    }
}

void createMapFromList(QList<Person*> &persons,
                       QMap<QString, Person*> &map)
{
    foreach (Person* p, persons) {
        // map[p->m_name] = new Person(*p);
        map.insert(p->m_name, new Person(*p));
    }
}

void showPersonsInMap(QMap<QString, Person*> &map)
{
    qDebug() << "showPersonsInMap, total - " << map.size();
    QMap<QString, Person*>::iterator it = map.begin();
    while(it != map.end())
    {
        qDebug() << "Key: " << it.key();
        it.value()->show();
        ++it;
    }
}

void deletePersonInMap(QMap<QString, Person*> &map,
                       const QString &name)
{
    QMap<QString, Person*>::iterator it = map.find(name);
    if(it != map.end())
    {
        delete *it;
        map.erase(it);
        qDebug() << name << " was deleted";
    }
    else
    {
        qDebug() << "could not find - " << name;
    }
}

void deleteMap(QMap<QString, Person*> &map)
{
    QMap<QString, Person*>::iterator it = map.begin();
    while(it != map.end())
    {
        delete *it;
        ++it;
    }
    map.clear();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QList<Person*> persons;
    loadPersons(persons);

    QMap<QString, Person*> map;
    createMapFromList(persons, map);

    showPersonsByIndex(persons);
    showPersonsInMap(map);
    qSort(persons.begin(), persons.end());
    showPersonsByIterator(persons);

    qDebug() << "Use qFind on QList<Person*>";
    Person *p = persons.first();
    QList<Person*>::iterator it = \
            qFind(persons.begin(), persons.end(), p);
    if(it != persons.end())
    {
        qDebug() << "  found!";
        (*it)->show();
    }

    qDebug() << endl;
    deletePerson(persons, "Mike");
    showPersonsByIterator(persons);
    qDeleteAll(persons.begin(), persons.end());
    persons.clear();

    qDebug() << endl;
    deletePersonInMap(map, "LiSi");
    deletePersonInMap(map, "ZhangSan");
    showPersonsInMap(map);
    deleteMap(map);

    return a.exec();
}
