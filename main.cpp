#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "patt.h"

using namespace std;

// уровень скорости
enum class TransportSpeed : int
{
    Fast, // быстрый
    Medium, // средний
    Slow, // медленный
    Unknown
};

class Transport // Родительский (базовый) класс
{
private: // "Закрытые" компоненты
    TransportSpeed Speed;
    double Weight;

protected: // "Защищенные" компоненты
    bool Electric;

public: // "Открытые" компоненты, определяющие интерфейс класса

    Transport(TransportSpeed speed) : Speed(speed), Weight(0.0), Electric(false)
    {
        // Значение инициализируется случайным числом 0 или 1
        Electric = static_cast<bool>(rand()%2);
    }

    virtual ~Transport() {}

    bool IsElectric() const { return Electric; }

    TransportSpeed GetSpeed() const { return Speed; }

    double GetWeight() const { return Weight; }

    virtual void Service()
    {
        if(IsElectric())
        {
            cout << "Servicing electric transport... ";
        }
        else
        {
            cout << "Servicing not electric transport... ";
        }
    }
};

class Car : public Transport // Класс-наследник
{
public:
    Car();
    ~Car() {}

    void Service() override;
};

// Реализация конструктора
Car::Car() : Transport(TransportSpeed::Fast)
{

}

void Car::Service()
{
    Transport::Service();
    cout << "Car is servicing..." << endl;
}

class Trolleybus : public Transport // Класс-наследник
{
public:
    Trolleybus() : Transport(TransportSpeed::Medium) { }
    ~Trolleybus() {}

    void Service() override;
};

void Trolleybus::Service()
{
    Transport::Service();
    cout << "Trolleybus is servicing..." << endl;
}

class Bicycle : public Transport // Класс-наследник
{
public:
    Bicycle() : Transport(TransportSpeed::Slow) { }
    ~Bicycle() {}

    void Service();
};

void Bicycle::Service()
{
    Transport::Service();
    cout << "Bicycle is servicing..." << endl;
}

// Реализация паттерна "Фабричный метод" для создания фруктов

enum class TransportType : int
{
    Car = 1,
    Trolleybus = 2,
    Bicycle = 3,

    Undefined = 0 // На всякий случай
};
Transport *CreateTransport(TransportType type)
{
    Transport *newTransport = nullptr;

    if(type == TransportType::Car)
    {
        newTransport = new Car;
    }
    else if(type == TransportType::Trolleybus)
    {
        newTransport = new Trolleybus;
    }
    else if(type == TransportType::Bicycle)
    {
        newTransport = new Bicycle;
    }

    return newTransport;
}

// Декоратор итератора для выделения спорта по опасности

class TransportSpeedDecorator : public IteratorDecorator<class Transport*>
{
private:
    TransportSpeed TargetSpeed;

public:
    TransportSpeedDecorator(Iterator<Transport*> *it, TransportSpeed speed)
    : IteratorDecorator<Transport*>(it), TargetSpeed(speed) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetSpeed() != TargetSpeed)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetSpeed() != TargetSpeed);
    }
};

// Декоратор итератора для выделения электрического и нет транспорта

class TransportElectricDecorator : public IteratorDecorator<class Transport*>
{
private:
    bool TargetElectric;

public:
    TransportElectricDecorator(Iterator<Transport*> *it, bool isElectric)
    : IteratorDecorator<Transport*>(it), TargetElectric(isElectric) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsElectric() != TargetElectric)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        }
        while(!It->IsDone() && It->GetCurrent()->IsElectric() != TargetElectric);
    }
};
// Функция, позволяющая выполнить любой транспорт из любого контейнера
// вне зависимости от его внутреннего устройства
void ServiceEmAll(Iterator<Transport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Transport *currentTransport = it->GetCurrent();
        currentTransport->Service();
    }
}

// Функция, позволяющая выполнить только електрический транспорт
// (демонстрация решения проблемы "в лоб")
void ServiceEmAllElectric(Iterator<Transport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Transport *currentTransport = it->GetCurrent();
        if(!currentTransport->IsElectric()) continue;

        currentTransport->Service();
    }
}

// Функция, позволяющая выполнить только медленный транспорт
// (демонстрация решения проблемы "в лоб")

void ServiceEmAllSlow(Iterator<Transport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Transport *currentTransport = it->GetCurrent();
        if(currentTransport->GetSpeed() != TransportSpeed::Slow) continue;

        currentTransport->Service();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    size_t N = 10;

    // Массив фруктов

    ArrayClass<Transport*> transportArray;
    for(size_t i=0; i<N; i++)
    {
        int transport_num = rand()%3+1; // Число от 1 до 3
        TransportType transport_type = static_cast<TransportType>(transport_num);
        Transport *newTransport = CreateTransport(transport_type);
        transportArray.Add(newTransport);
    }

    wcout << L"Размер массива транспорта: " << transportArray.Size() << endl;

    // Связанный список transport (для демонстрации адаптера)

    list<Transport*> transportVector;
    for(size_t i=0; i<N; i++)
    {
        int transport_num = rand()%3+1; // Число от 1 до 3
        TransportType transport_type = static_cast<TransportType>(transport_num);
        Transport *newTransport = CreateTransport(transport_type);
        transportVector.push_back(newTransport); // Добавить новый спорт в конец списка
    }

    wcout << L"Размер списка транспорта: " << transportVector.size() << endl;

    // Обход в простом цикле
    cout << endl << "Service all in a simple loop:" << endl;
    for(size_t i=0; i<transportArray.Size(); i++)
    {
        Transport *currentTransport = transportArray[i];
        currentTransport->Service();
    }

    // Обход всех элементов при помощи итератора
    cout << endl << "Service all using iterator:" << endl;
    Iterator<Transport*> *allIt = transportArray.GetIterator();
    ServiceEmAll(allIt);
    delete allIt;

    // Обход всех електрических видов транспорта
    cout << endl << "Service all electric using iterator:" << endl;
    Iterator<Transport*> *electricIt = new TransportElectricDecorator(transportArray.GetIterator(), true);
    ServiceEmAll(electricIt);
    delete electricIt;

    // Обход всех быстрых видов транспорта
    cout << endl << "Service all fast using iterator:" << endl;
    Iterator<Transport*> *fastIt = new TransportSpeedDecorator(transportArray.GetIterator(), TransportSpeed::Fast);
    ServiceEmAll(fastIt);
    delete fastIt;

    // Обход всех електрических быстрых по скорости транспортов
    cout << endl << "Service all electric medium using iterator:" << endl;
    Iterator<Transport*> *goodMediumIt =
        new TransportElectricDecorator(new TransportSpeedDecorator(transportArray.GetIterator(), TransportSpeed::Medium), true);
    ServiceEmAll(goodMediumIt);
    delete goodMediumIt;

    // Демонстрация работы адаптера
    cout << endl << "Service all electric fast using adapted iterator (another container):" << endl;
    Iterator<Transport*> *adaptedIt = new ConstIteratorAdapter<std::list<Transport*>, Transport*>(&transportVector);
    Iterator<Transport*> *adaptedFastIt = new TransportElectricDecorator(new TransportSpeedDecorator(adaptedIt, TransportSpeed::Fast), false);
    ServiceEmAll(adaptedFastIt);
    delete adaptedFastIt;
    // adaptedIt удалять не надо, так как он удаляется внутри декоратора adaptedDangerousIt

    return 0;
}
