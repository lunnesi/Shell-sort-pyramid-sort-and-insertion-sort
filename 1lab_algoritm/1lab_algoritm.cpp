#include <iostream>
#include <vector>
#include <clocale>

template <typename T>
class Sort {
private:
    // Вспомогательный метод для сортировки Шелла
    static int shellInsertion(T* arr[], int b, int s, int n) {
        int comparisons = 0;
        int j = b + s; //нач со втор
        while (j < n) { //если размер массива ви
            T* x = arr[j]; //сохран элемент
            int k = j - s; //индекс прев. элемента
            // Проверяем элементы, пока не выйдем за границы
            while (k >= 0) {
                comparisons++; // Считаем сравнение
                if (*arr[k] > *x) { //срав значен обьектов
                    arr[k + s] = arr[k]; //если лев больше, то сдвигаем на шаг s
                    k = k - s; //перех след слева
                }
                else {
                    break; // Нашли правильное место
                }
            }
            arr[k + s] = x; //вставляем наш сох элемент х на его место
            j = j + s; //перех к след элементу в подгруппе
        }
        return comparisons;
    }

    // Вспомогательный метод просеивания для пирамидальной сортировки
    static int sift2(T* a[], int n, int i) {
        int comparisons = 0;
        int j = 2 * i + 1; // левый потомок
        T* x = a[i];       // запоминаем указатель
        int f = 1;

        while (j < n && f) {
            //выб наиб из двух потомков
            if ((j + 1) < n) {
                comparisons++; // Считаем сравнение потомков
                if (*a[j + 1] > *a[j]) {
                    j = j + 1; // правый потомок больше
                }
            }

            comparisons++; // Считаем сравнение с корнем/родителем
            if (*a[j] > *x) {
                a[i] = a[j]; // если потомок больще х,поднимает потомка на место родителя
                i = j; //переходим ниже
                j = 2 * i + 1; //
            }
            else { //если х больше или равен самому крупному потомку, место найдено
                f = 0; // свойство кучи выполняется
            }
        }
        a[i] = x; // ставим исходный указатель на нужное место
        return comparisons;
    }

public:
    //Простая сортировка вставками (косвенная)
    static int insertionSort(T* arr[], int n) {
        int comparisons = 0;
        for (int i = 1; i < n; i++) {
            T* x = arr[i];
            int j = i;
            while (j > 0) {
                comparisons++; // Учитываем операцию сравнения
                if (*arr[j - 1] > *x) { // Косвенное сравнение
                    arr[j] = arr[j - 1]; // Перемещаем указатели, а не сами данные
                    j = j - 1;
                }
                else {
                    break; // Прерываем цикл, так как условие нарушено
                }
            }
            arr[j] = x;
        }
        return comparisons;
    }

    //Сортировка Шелла (косвенная)
    static int shellSort(T* arr[], int n) {
        int comparisons = 0;
        int r = 1, t = 0; //колл шагов
        int h[100] = { 0 };
        h[0] = n / 2; первый шаг, половина длины массива

            // Вычисляем шаги (расстояния) для сортировки
            while ((h[r - 1] / 2) >= 1) { //каждый в два раза меньше
                h[r] = h[r - 1] / 2; //след шаг
                r++;
                t++;
            }

        int i = 0;
        while (i <= t) { //проходим по выч шагу
            int s = h[i];//расстояние между элемен
            int b = 0; //начинаем с первой группы
            // Применяем сортировку вставками для каждого шага
            while (b < s) {
                comparisons += shellInsertion(arr, b, s, n);
                b++;
            }
            i++;
        }
        return comparisons;
    }


    // Пирамидальная сортировка / HeapSort (косвенная)
    static int heapSort(T* a[], int n) {
        int comparisons = 0;

        // Построение кучи (max-heap)
        for (int i = n / 2 - 1; i >= 0; i--) {
            comparisons += sift2(a, n, i);
        }


        // Извлечение элементов из кучи
        for (int i = n - 1; i > 0; i--) {
            // Меняем местами указатели: корень (максимум) и последний элемент
            T* t = a[0];
            a[0] = a[i];
            a[i] = t;

            // Восстанавливаем свойства кучи для оставшейся части
            comparisons += sift2(a, i, 0);
        }
        return comparisons;
    }

    /**
     * Метод проверки косвенной упорядоченности массива
     * @return true, если массив отсортирован по возрастанию
     */
    static bool isSorted(T* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            // Если предыдущий элемент больше следующего, массив не отсортирован
            if (*arr[i] > *arr[i + 1]) {
                return false;
            }
        }
        return true;
    }
};


int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> data = { 54, 12, 89, 33, 21, 65, 1, 99, 45 };
    int n = data.size();

    //Создаем массив указателей 
    std::vector<int*> ptrArray(n);
    for (int i = 0; i < n; i++) {
        ptrArray[i] = &data[i];
    }

    // Вывод до сортировки
    std::cout << "Исходные значения: ";
    for (int i = 0; i < n; i++) {
        std::cout << *ptrArray[i] << " ";
    }
    std::cout << "\nОтсортировано ли? " << (Sort<int>::isSorted(ptrArray.data(), n) ? "Да" : "Нет") << "\n\n";


    //int comparisons = Sort<int>::insertionSort(ptrArray.data(), n);
    int comparisons = Sort<int>::shellSort(ptrArray.data(), n);
    //int comparisons = Sort<int>::heapSort(ptrArray.data(), n);

    // Вывод после сортировки
    std::cout << "После косвенной сортировки: ";
    for (int i = 0; i < n; i++) {
        std::cout << *ptrArray[i] << " ";
    }

    std::cout << "\nСравнений выполнено: " << comparisons;
    std::cout << "\nОтсортировано ли? " << (Sort<int>::isSorted(ptrArray.data(), n) ? "Да" : "Нет") << "\n\n";

    return 0;
}
