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
// Реализация паттерна "Стратегия"

enum class TravelModeEnum : int
{
    Drive,
    Take,
    Ride,

    // Новая стратегия для варианта 2 (передвигаться пешком)
    Walk,

    None
};

class TravelStrategy
{
public:
    virtual ~TravelStrategy() {}
    virtual void Travel() = 0;
};

class DriveTravelStrategy : public TravelStrategy
{
    void Travel() { cout << "Driving the car..."; }
};

class TakeTravelStrategy : public TravelStrategy
{
    void Travel(){ cout << "Taking the trolleybus..."; }
};

class RideTravelStrategy : public TravelStrategy
{
    void Travel() { cout << "Riding the bicycle..."; }
};

class WalkTravelStrategy : public TravelStrategy
{
    void Travel() { cout << "Walking to the destination..."; }
};

// Фабричный метод для создания стратегий
TravelStrategy *CreateTravelStrategy(TravelModeEnum travelMode)
{
  switch(travelMode)
  {
    case TravelModeEnum::Drive: return new DriveTravelStrategy;
    case TravelModeEnum::Take: return new TakeTravelStrategy;
    case TravelModeEnum::Ride: return new RideTravelStrategy;

    // Новый способ (для варианта 2)
    case TravelModeEnum::Walk: return new WalkTravelStrategy;

    default: return nullptr;
  }
}

class Transport // Родительский (базовый) класс
{
private: // "Закрытые" компоненты
    TransportSpeed Speed;
    double Weight;

    TravelStrategy *TravelMode;

    void DoTravelUsingStrategy()
    {
      if(TravelMode == nullptr)
      {
        // Способ съедания не задан, ничего не делаем
        cout << "No travel strategy set!";
        return;
      }
      else
      {
        // Съесть заданным способом
        TravelMode->Travel();
      }
    }
    void DetectGoodOrNot()
    {
      if(IsElectric())
      {
          cout << "Electric";
      }
      else
      {
          cout << "Not electric";
      }
    }
protected: // "Защищенные" компоненты
    bool Electric;

public: // "Открытые" компоненты, определяющие интерфейс класса

    Transport(TransportSpeed speed) : Speed(speed), Weight(0.0), Electric(false)
    {
        // Значение инициализируется случайным числом 0 или 1
        Electric = static_cast<bool>(rand()%2);
    }

    virtual ~Transport() // Деструктор (объявлен виртуальным, чтобы обеспечить корректное уничтожение унаследованных классов)
    {
      if(TravelMode != nullptr) delete TravelMode;
    }

    // Функция с реализацией
    bool IsElectric() const { return Electric; }

    // Функция с реализацией
    TransportSpeed GetSpeed() const { return Speed; }

    // Функция с реализацией
    double GetWeight() const { return Weight; }

    virtual void PrintType() = 0;

    virtual void FuelUp() = 0;

    // Функция с реализацией
    void Travel()
    {
        // 1. Вывести название транспорта
        PrintType();
        cout << " : ";

        // 2. Определить, хороший транспорт или нет
        DetectGoodOrNot();
        cout << " : ";

        // 2.1 Заправить топливом
        FuelUp();
        cout << " : ";

        // 3. Если транспорт готов, начать движение с использованием выбранной стратегии
        DoTravelUsingStrategy();

        // 4. Конец алгоритма
        cout << endl;
    }
    void SetTravelStrategy(TravelStrategy *travelMode) { TravelMode = travelMode; }
};

class Car : public Transport // Класс-наследник "Яблоко"
{
public:
    Car();
    ~Car() {}

    void PrintType() { cout << "Car"; }
    void FuelUp() { cout << "Refueling with gasoline"; }
};


// Реализация конструктора
Car::Car() : Transport(TransportSpeed::Fast)
{
  // Определить стратегию съедания по умолчанию для яблока (вариант 1)
  SetTravelStrategy(CreateTravelStrategy(TravelModeEnum::Drive));
}
class Trolleybus : public Transport // Класс-наследник "Киви"
{
public:
    Trolleybus() : Transport(TransportSpeed::Medium) { SetTravelStrategy(CreateTravelStrategy(TravelModeEnum::Take)); }
    ~Trolleybus() {}

    void PrintType() { cout << "Trolleybus"; }
    void FuelUp() { cout << "Refueling with gasoline is not necessary"; }
};

class Bicycle : public Transport // Класс-наследник "Апельсин"
{
public:
    Bicycle() : Transport(TransportSpeed::Slow) { SetTravelStrategy(CreateTravelStrategy(TravelModeEnum::Ride)); }
    ~Bicycle() {}

    void PrintType() { cout << "Bicycle"; }
    void FuelUp() { cout << "Refueling with gasoline"; }
};

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

// Декоратор итератора для выделения транспорта по скорости

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
void TravelEmAll(Iterator<Transport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Transport *currentTransport = it->GetCurrent();
        currentTransport->Travel();
    }
}

// Функция, позволяющая выполнить только електрический транспорт
// (демонстрация решения проблемы "в лоб")
void TravelEmAllElectric(Iterator<Transport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Transport *currentTransport = it->GetCurrent();
        if(!currentTransport->IsElectric()) continue;

        currentTransport->Travel();
    }
}

// Функция, позволяющая выполнить только медленный транспорт
// (демонстрация решения проблемы "в лоб")

void TravelEmAllSlow(Iterator<Transport*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Transport *currentTransport = it->GetCurrent();
        if(currentTransport->GetSpeed() != TransportSpeed::Slow) continue;

        currentTransport->Travel();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    size_t N = 10;

    // Массив транспорта

    ArrayClass<Transport*> transportArray;
    for(size_t i=0; i<N; i++)
    {
        int transport_num = rand()%3+1; // Число от 1 до 3
        TransportType transport_type = static_cast<TransportType>(transport_num);
        Transport *newTransport = CreateTransport(transport_type);
        // Задать способ съедания на этапе создания (вариант 2)
        // newTransport->SetTravelStrategy(CreateTravelStrategy(TravelModeEnum::Walk));

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
        transportVector.push_back(newTransport); // Добавить transport спорт в конец списка
    }

    wcout << L"Размер списка транспорта: " << transportVector.size() << endl;

    // Обход в простом цикле
    cout << endl << "Travel all in a simple loop:" << endl;
    for(size_t i=0; i<transportArray.Size(); i++)
    {
        Transport *currentTransport = transportArray[i];
        currentTransport->Travel();
    }

    // Обход всех элементов при помощи итератора
    cout << endl << "Travel all using iterator:" << endl;
    Iterator<Transport*> *allIt = transportArray.GetIterator();
    TravelEmAll(allIt);
    delete allIt;

    // Обход всех електрических видов транспорта
    cout << endl << "Travel all electric using iterator:" << endl;
    Iterator<Transport*> *electricIt = new TransportElectricDecorator(transportArray.GetIterator(), true);
    TravelEmAll(electricIt);
    delete electricIt;

    // Обход всех быстрых видов транспорта
    cout << endl << "Travel all fast using iterator:" << endl;
    Iterator<Transport*> *fastIt = new TransportSpeedDecorator(transportArray.GetIterator(), TransportSpeed::Fast);
    TravelEmAll(fastIt);
    delete fastIt;

    // Обход всех електрических быстрых по скорости транспортов
    cout << endl << "Travel all electric medium using iterator:" << endl;
    Iterator<Transport*> *goodMediumIt =
        new TransportElectricDecorator(new TransportSpeedDecorator(transportArray.GetIterator(), TransportSpeed::Medium), true);
    TravelEmAll(goodMediumIt);
    delete goodMediumIt;

    // Демонстрация работы адаптера
    cout << endl << "Travel all not electric fast using adapted iterator (another container):" << endl;
    Iterator<Transport*> *adaptedIt = new ConstIteratorAdapter<std::list<Transport*>, Transport*>(&transportVector);
    Iterator<Transport*> *adaptedFastIt = new TransportElectricDecorator(new TransportSpeedDecorator(adaptedIt, TransportSpeed::Fast), false);
    TravelEmAll(adaptedFastIt);
    delete adaptedFastIt;
    // adaptedIt удалять не надо, так как он удаляется внутри декоратора adaptedDangerousIt

    return 0;
}
