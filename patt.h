#ifndef PATT_H_INCLUDED
#define PATT_H_INCLUDED


// Шаблонный класс, реализующий паттерн "Итератор"
// (класс абстрактный, так как содержит функции без реализации)

template<typename T> // T - подставляемый тип данных (элемент контейнера)
class Iterator
{
protected:
    Iterator() {}

public:
    virtual ~Iterator() {}
    virtual void First() = 0; // Спозиционироваться на начале контейнера
    virtual void Next() = 0;  // Перейти к следующему элементу
    virtual bool IsDone() const = 0;  // Проверить, достигнут ли конец
    virtual T GetCurrent() const = 0; // Получить текущий элемент
};

// Реализация паттерна "Итератор",
// предназначенная для обхода стека

template<typename T>
class StackIterator : public Iterator<T>
{
private:
    T *StackContainer; // Указатель на массив
    size_t Pos; // Текущее положение в массиве
    size_t Size; // Размер массива

public:
    StackIterator(T *container, size_t size)
    : StackContainer(container), Pos(0), Size(size) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= Size); }
    T GetCurrent() const override { return StackContainer[Pos]; }
};

// Шаблонный класс "Стек"

const size_t MaxSize = 100;

template <typename T> // T - подставляемый тип данных
class StackClass
{
private:

    // Собственно контейнер, содержащий данные типа T
    T Items[MaxSize];

    // Индекс элемента стека, следующего за верхним
    size_t Top;

public:

    // Проверка того, является ли стек пустым
    bool IsEmpty() const { return (Top == 0); }

    // Определение размера стека
    size_t Size() const { return Top; }

    // Поместить в стек новый элемент
    void Push(T newObject) { Items[Top++] = newObject; }

    // Извлечь из списка верхний элемент
    T Pop() { return Items[--Top]; }

    // Получение элемента по индексу
    T GetElementByIndex(size_t index) const { return Items[index]; }

    // Конструктор
    StackClass() : Top(0) {}

    // Получить итератор для обхода контейнера
    class Iterator<T> *GetIterator()
    {
        return new StackIterator<T>(Items, Top);
    };
};

// Реализация паттерна "Итератор",
// предназначенная для обхода массива

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:

    const T *ArrayContainer; // Указатель на массив
    size_t Pos; // Текущее положение в массиве
    size_t Size; // Размер массива

public:
    ArrayIterator(T *container, size_t size)
    : ArrayContainer(container), Pos(0), Size(size) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= Size); }
    T GetCurrent() const override { return ArrayContainer[Pos]; }
};

// Шаблонный класс "Массив"

template <typename T>
class ArrayClass
{
private:

    T Items[MaxSize];
    size_t ArraySize;

public:

    void Add(T newObject) { Items[ArraySize++] = newObject; }
    size_t Size() const { return ArraySize; }

    // Первый вариант получения элемента по индексу (перегрузка оператора [])
    T operator[](size_t index) const { return Items[index]; }

    // Второй вариант получения элемента по индексу
    T GetElement(size_t index) const { return Items[index]; }

    // Получить итератор для обхода контейнера
    class Iterator<T> *GetIterator()
    {
        return new ArrayIterator<T>(Items, ArraySize);
    };

    ArrayClass() : ArraySize(0) {}
};

// Декоратор для итератора (пустой)

template<typename T>
class IteratorDecorator : public Iterator<T>
{
protected:
    Iterator<T> *It;

public:
    IteratorDecorator(Iterator<T> *it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};

// Адаптер для контейнерных классов STL

template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType *Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType *container)
    : Container(container)
    {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}
    virtual void First() { It = Container->begin(); }
    virtual void Next() { It++; }
    virtual bool IsDone() const { return (It == Container->end()); }
    virtual ItemType GetCurrent() const { return *It; }
};


#endif // PATT_H_INCLUDED
