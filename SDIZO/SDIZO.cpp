#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

typedef struct Output {
    string name;
    float value;
} Output;

void printInterface(vector <string> options, int select) {
    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    unsigned int max_length = 0;
    int swt = 2;
    const char LG = (const char)201;
    const char PG = (const char)187;
    const char LD = (const char)200;
    const char PD = (const char)188;
    const char LK = (const char)204;
    const char PK = (const char)185;
    const char poz = (const char)205;
    const char pion = (const char)186;
    for (string s : options) {
        if (s.length() > max_length) max_length = s.length();
    }
    if (max_length < 4) {
        cout << "Nadluzsza z opcji musi miec przynajmniej 4 znaki" << endl;
        return;
    }
    if (max_length % 2 == 0) swt = 1;
    cout << LG;
    for (int i = 0; i < max_length + 2; i++) cout << poz;
    cout << PG << endl << pion;
    for (int i = 0; i < (max_length - 4) / 2 + 1; i++) cout << " ";
    cout << "MENU";
    for (int i = 0; i < (max_length - 4) / 2 + swt; i++) cout << " ";
    cout << pion << endl << LK;
    for (int i = 0; i < max_length + 2; i++) cout << poz;
    cout << PK << endl;
    int x = 0;
    for (string s : options) {
        cout << pion;
        for (int i = 0; i < (max_length - s.length()) / 2 + 1; i++) cout << " ";
        if (x == select) SetConsoleTextAttribute(hOut, FOREGROUND_RED);
        cout << s;
        SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        if (s.length() % 2 == 0) {
            for (int i = 0; i < (max_length - s.length()) / 2 + swt; i++) cout << " ";
        }
        else {
            for (int i = 0; i < (max_length - s.length()) / 2 + abs(swt - 3); i++) cout << " ";
        }
        cout << pion << endl;
        x++;
    }
    cout << LD;
    for (int i = 0; i < max_length + 2; i++) cout << poz;
    cout << PD;
}

int printShortInterFace(vector <string> opts, string additionalWarning = "") {
    int select = 0;
    char pick;
    int x = 0;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    while (true) {
        x = 0;
        system("cls");
        if (additionalWarning != "")
        {
            cout << additionalWarning << '\n';
        }
        for (string s : opts)
        {
            if (x == select) SetConsoleTextAttribute(hOut, FOREGROUND_RED);
            cout << s << '\n';
            SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            x++;
        }
        pick = _getch();
        if (pick == 'w' || pick == 72)
        {
            if (select > 0) select--;
            else select = opts.size() - 1;
        }
        else if (pick == 's' || pick == 80)
        {
            if (select < opts.size() - 1) select++;
            else select = 0;
        }
        else if (pick == 'e' || pick == 13)
        {
            return select;
        }
    }
}
vector <float> openExcel(string& mode, bool& loaded)
{
    fstream file;
    string line;
    vector <float> tab;
    int load_size = 0;
    loaded = false;
    file.open("dane.csv", ios::in);
    if (file.good())
    {
        getline(file, line);
        getline(file, line);
        getline(file, line);
        line = line.substr(13);
        line.erase(line.begin() + line.find(" "), line.end());
        load_size = stoi(line);

        getline(file, line);
        line = line.substr(8);
        line.erase(line.begin() + line.find(" "), line.end());
        mode = line;

        //Linia kontrolna
        getline(file, line);
        if (line == "[DATA]")
        {
            int line_nr = 0;
            while (getline(file, line))
            {
                if (line_nr == load_size) break;
                if (mode == "int") tab.push_back(stoi(line));
                if (mode == "int") tab.push_back(stoi(line));
                else if (mode == "float") tab.push_back(stof(line));
                else
                {
                    cout << "mode error!";
                    cin.get();
                    return tab;
                }
                line_nr++;
            }
            loaded = true;
        }
        else
        {
            cout << "Blad pliku konfiguracyjnego!\n";
        }
    }
    file.close();

    return tab;
}
float mean(vector <float> data)
{
    float suma = 0;
    for (float i : data)
    {
        suma += i;
    }
    return suma / data.size();
}
float maximum(vector <float> data)
{
    float max = 1.17549e-038;
    for (float i : data)
    {
        if (i > max) max = i;
    }
    return max;
}
float minimum(vector <float> data)
{
    float min = 3.40282e+038;
    for (float i : data)
    {
        if (i < min) min = i;
    }
    return min;
}
void saveFile(vector <float> data, string mode, string filename, vector <Output> additionalOutput)
{
    fstream file;
    file.open(filename, ios::out);
    file << "# Plik z danymi SDiZO wyjsciowymi\n";
    file << "[SDIZO]\n";
    file << "READ_RECORDS=" << data.size() << " # Liczba rekordow do wczytania\n";
    file << "READ_AS=" << mode << " # Typ danych do wczytania - float/int\n";
    file << "[DATA];;;;";
    for (int i = 0; i < additionalOutput.size(); i++)
    {
        file << ";" << additionalOutput[i].name;
    }
    file << '\n';
    for (int i = 0; i < data.size(); i++)
    {
        if (i == 0)
        {
            file << data[i];
            file << ";;;;";
            for (int i = 0; i < additionalOutput.size(); i++)
            {
                string output = ";" + to_string(additionalOutput[i].value);
                output.replace(output.find('.'), 1, ",");
                file << output;
            }
            file << '\n';
        }
        else
        {
            file << data[i] << '\n';
        }
    }
    file.close();
}
void bubbleSort(vector <float>& data, unsigned int maxRange = 0, bool ascending = true)
{
    if (maxRange == 0) maxRange = data.size();
    else if (maxRange == 1) return;
    int x = 0;
    if (ascending)
    {
        while (x < maxRange) 
        {
            for (int j = 0; j < maxRange - 1; j++)
            {
                if (data[j] > data[j + 1])
                {
                    swap(data[j], data[j + 1]);
                    x = 0;
                }
                x++;

            }
        }
    }
    else
    {
        while (x < maxRange)
        {
            for (int j = 0; j < maxRange - 1; j++)
            {
                if (data[j] < data[j + 1])
                {
                    swap(data[j], data[j + 1]);
                    x = 0;
                }
                x++;
            }
        }
    }
}
void quickSort(vector <float>& data, int left, int right, bool ascending = true)
{
    float i = left;
    float j = right;
    float x = data[(left + right) / 2];
    if (ascending)
    {
        do
        {
            while (data[i] < x)
                i++;

            while (data[j] > x)
                j--;

            if (i <= j)
            {
                swap(data[i], data[j]);

                i++;
                j--;
            }
        } while (i <= j);

        if (left < j) quickSort(data, left, j, ascending);

        if (right > i) quickSort(data, i, right, ascending);
    }
    else
    {
        do
        {
            while (data[i] > x)
                i++;

            while (data[j] < x)
                j--;

            if (i <= j)
            {
                swap(data[i], data[j]);

                i++;
                j--;
            }
        } while (i <= j);

        if (left < j) quickSort(data, left, j, ascending);

        if (right > i) quickSort(data, i, right, ascending);
    }
}
void constructHeap(vector <float>& data, int i, unsigned int maxRange, bool ascending = true)
{
    if (maxRange == 0) maxRange = data.size();
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (ascending)
    {
        if (l < maxRange && data[l] > data[largest])
            largest = l;

        if (r < maxRange && data[r] > data[largest])
            largest = r;
    }
    else
    {
        if (l < maxRange && data[l] < data[largest])
            largest = l;

        if (r < maxRange && data[r] < data[largest])
            largest = r;
    }
    if (largest != i) {
        swap(data[i], data[largest]);

        constructHeap(data, largest, maxRange, ascending);
    }
}
void heapSort(vector <float>& data, unsigned int maxRange = 0, bool ascending = true)
{
    if (maxRange == 0) maxRange = data.size();
    for (int i = maxRange / 2 - 1; i >= 0; i--)
        constructHeap(data, i, maxRange, ascending);

    for (int i = maxRange - 1; i >= 0; i--) {
        swap(data[0], data[i]);
        constructHeap(data, 0, i, ascending);
    }
}
void countingSort(vector <float>& data, unsigned int maxRange = 0, bool ascending = true)
{
    if (maxRange == 0) maxRange = data.size();
    int max = maximum(data);
    int min = minimum(data);
    vector <float> positiveNumbers;
    vector <float> negativeNumbers;
    vector <float> newData;
    positiveNumbers.reserve(max + 1);
    negativeNumbers.reserve(abs(min) + 1);

    for (int i = 0; i <= max; i++)
    {
        positiveNumbers.push_back(0);
    }
    for (int i = 0; i <= abs(min); i++)
    {
        negativeNumbers.push_back(0);
    }

    for (int x : data)
    {
        if (x >= 0) positiveNumbers[x]++;
        else negativeNumbers[abs(x)]++;
    }
    if (ascending)
    {
        for (int i = min; i < 0; i++)
        {
            for (int j = 0; j < negativeNumbers[abs(i)]; j++)
            {
                newData.push_back(i);
            }
        }
        for (int i = 0; i <= max; i++)
        {
            for (int j = 0; j < positiveNumbers[i]; j++)
            {
                newData.push_back(i);
            }
        }
    }
    else
    {
        for (int i = max; i >= 0; i--)
        {
            for (int j = 0; j < positiveNumbers[i]; j++)
            {
                newData.push_back(i);
            }
        }
        for (int i = 0; i >= min; i--)
        {

            for (int j = 0; j < negativeNumbers[abs(i)]; j++)
            {
                newData.push_back(i);
            }
        }
    }
    data = newData;
}
class MeasurePoint
{
    long double nanoSec;
    unsigned int elem;
public:
    MeasurePoint(long double nanoSecond, unsigned int element) : nanoSec(nanoSecond), elem(element)
    {    }
    unsigned int getX()
    {
        return elem;
    }
    long double getY()
    {
        return nanoSec;
    }
};
class TimeMeas
{
    high_resolution_clock::time_point start, stop;
    vector < MeasurePoint > measurePoints;
    unsigned int elements, iters;
    bool isLog;
public:

    TimeMeas(unsigned int elements_c,  unsigned int iterations_c = 15) :
        elements(elements_c),
        iters(iterations_c),
        isLog(false)
    {
    }
    void countingSort(vector <float> data, unsigned int maxRange = 0, bool ascending = true)
    {
        vector <float> dataRanged;
        dataRanged = data;
        if (maxRange == 0) maxRange = data.size();
        else if (maxRange < data.size())
        {
            dataRanged.resize(maxRange);
        }
        int max = maximum(dataRanged);
        int min = minimum(dataRanged);
        vector <int> positiveNumbers;
        vector <int> negativeNumbers;
        vector <float> newData;
        positiveNumbers.reserve(max + 1);
        negativeNumbers.reserve(abs(min) + 1);

        for (int i = 0; i <= max; i++)
        {
            positiveNumbers.push_back(0);
        }
        for (int i = 0; i <= abs(min); i++)
        {
            negativeNumbers.push_back(0);
        }

        for (int x : dataRanged)
        {
            if (x >= 0) positiveNumbers[x]++;
            else negativeNumbers[abs(x)]++;
        }
        if (ascending)
        {
            for (int i = min; i < 0; i++)
            {
                for (int j = 0; j < negativeNumbers[abs(i)]; j++)
                {
                    newData.push_back(i);
                }
            }
            for (int i = 0; i <= max; i++)
            {
                for (int j = 0; j < positiveNumbers[i]; j++)
                {
                    newData.push_back(i);
                }
            }
        }
        else
        {
            for (int i = max; i >= 0; i--)
            {
                for (int j = 0; j < positiveNumbers[i]; j++)
                {
                    newData.push_back(i);
                }
            }
            for (int i = 0; i >= min; i--)
            {

                for (int j = 0; j < negativeNumbers[abs(i)]; j++)
                {
                    newData.push_back(i);
                }
            }
        }
        data = newData;
    }
    void measureFunctionTime(vector <float> data, char s)
    {
        long double mean = 0;
        switch (isLog)
        {
        case true:
            break;

        case false:
                system("cls");
                mean = 0;
                for (int t = 1; t <= iters; t++) {
                    vector <float> sample = data;
                    start = high_resolution_clock::now();
                    switch (s)
                    {
                    case 1:
                        bubbleSort(sample, elements);
                        break;
                    case 2:
                        quickSort(sample, 0, elements - 1);
                        break;
                    case 3:
                        heapSort(sample, elements);
                        break;
                    case 4:
                        this->countingSort(sample, elements);
                        break;
                    }
                    stop = high_resolution_clock::now();


                    mean += duration_cast<duration<long double, milli>>(stop - start).count() / iters;
                }
                measurePoints.push_back(MeasurePoint(mean, elements));
        }
    }
    vector < MeasurePoint > getMeasurePoints()
    {
        return measurePoints;
    }
};
void timeMeasToCSV(vector < MeasurePoint > points, string filename, string algorithmname)
{
    fstream file;
    file.open(filename, ios::out);
    file << "# Plik z pomiarami czasow algortmu" << "[" << algorithmname << "]\n";
    file << "ILOSC REKORDOW;CZAS WYKONANIA [ms]\n";
    for (int i = 0; i < points.size(); i++)
    {
        string output = to_string(points[i].getX()) + ";" + to_string(points[i].getY()) + "\n";
        output.replace(output.find('.'), 1, ",");
        file << output;
    }
    file.close();
}

int main()
{
    vector < MeasurePoint > measurePoints;
    vector < Output > output_file;
    Output out;
    out.name = "none";
    out.value = 0;
    vector <string> options;
    vector <float> data;
    unsigned int selected = 0;
    vector <string> opts;
    bool data_loaded = false;
    int ilosc = 0;
    float srednia = 0;
    float min = 1.17549e-038;
    float max = 3.40282e+038;
    string mode = "int";
    options.push_back("1.Wczytaj plik wejsciowy");
    options.push_back("2.Wyswietl rekordy");
    options.push_back("3.Oblicz srednia arytmetyczna");
    options.push_back("4.Znajdz max zbioru");
    options.push_back("5.Znajdz min zbioru");
    options.push_back("6.Sortowanie");
    options.push_back("7.Pomiar czasu");
    options.push_back("8.Zapisz dane");
    options.push_back("9.Zamknij program");
    while (true)
    {
        system("cls");
        printInterface(options, selected);
        char pick = _getch();
        if (pick == 'w' || pick == 72)
        {
            if (selected > 0) selected--;
            else selected = options.size() - 1;
        }
        else if (pick == 's' || pick == 80)
        {
            if (selected < options.size() - 1) selected++;
            else selected = 0;
        }
        else if (pick == 'e' || pick == 13)
        {
            switch (selected)
            {
            case 0:
                system("cls");
                cout << "Wczytywanie...\n";
                data = openExcel(mode, data_loaded);

                if (data_loaded)
                {
                    system("cls");
                    cout << "Wczytano dane\nKliknij dowolny klaiwsz";
                }
                else cout << "Nie udalo sie wczytac danych!\nKliknij dowolny klaiwsz";
                cin.get();
                break;
            case 1:
                system("cls");
                cout << "Dostepne " << data.size() << " rekordow\n";
                if (data_loaded)
                {
                    cout << "Ile rekordow wyswietlic (liczba ujemna lub zero - wszystkie): ";
                    cin >> ilosc;
                    if (ilosc < data.size() && ilosc>0)
                    {
                        for (int i = 0; i < ilosc; i++)
                        {
                            cout << data[i] << '\n';
                        }
                    }
                    else
                    {
                        for (float i : data)
                        {
                            cout << i << '\n';
                        }
                    }
                    cin.get();
                    cout << "Kliknij dowolny klaiwsz\n";
                }
                else
                {
                    cout << "Prosze najpierw wczytac dane\nKliknij dowolny klaiwsz";
                }
                cin.get();
                break;
            case 2:
                system("cls");
                if (data_loaded)
                {
                    srednia = mean(data);
                    out.name = "srednia";
                    out.value = srednia;
                    output_file.push_back(out);
                    cout << "Srednia arytmetyczna zbioru: " << srednia << "\nKliknij dowolny klaiwsz";
                }
                else
                {
                    cout << "Prosze najpierw wczytac dane\nKliknij dowolny klaiwsz";
                }
                cin.get();
                break;
            case 3:
                system("cls");
                if (data_loaded)
                {
                    max = maximum(data);
                    out.name = "max zbioru";
                    out.value = max;
                    output_file.push_back(out);
                    cout << "Max zbioru: " << max << "\nKliknij dowolny klaiwsz";
                }
                else
                {
                    cout << "Prosze najpierw wczytac dane\nKliknij dowolny klaiwsz";
                }
                cin.get();
                break;
            case 4:
                system("cls");
                if (data_loaded)
                {
                    min = minimum(data);
                    out.name = "min zbioru";
                    out.value = min;
                    output_file.push_back(out);
                    cout << "Min zbioru: " << min << "\nKliknij dowolny klaiwsz";
                }
                else
                {
                    cout << "Prosze najpierw wczytac dane\nKliknij dowolny klaiwsz";
                }
                cin.get();
                break;
            case 5:
                if (data_loaded)
                {
                    int c;
                    bool ascending;
                    opts.push_back("1.Sortuj rosnaco");
                    opts.push_back("2.Sortuj malejaco");
                    opts.push_back("3.Wroc do MENU");
                    c = printShortInterFace(opts);
                    opts.clear();
                    switch (c)
                    {
                    case 0:
                        ascending = true;
                        break;
                    case 1:
                        ascending = false;
                        break;
                    default:
                        c = -1;
                        break;
                    }
                    if (c == -1) break;
                    opts.push_back("1.Sortowanie babelkowe");
                    opts.push_back("2.Sortowanie szybkie");
                    opts.push_back("3.Sortowanie przez kopcowanie");
                    opts.push_back("4.Sortowanie przez zliczanie");
                    c = printShortInterFace(opts);
                    opts.clear();
                    switch (c)
                    {
                    case 0:
                        bubbleSort(data, data.size(), ascending);
                        break;
                    case 1:
                        quickSort(data, 0, data.size() - 1, ascending);
                        system("cls");
                        break;
                    case 2:
                        heapSort(data, data.size(), ascending);
                        system("cls");
                        break;
                    case 3:
                        if (mode != "int")
                        {
                            system("cls");
                            string warning = "Obecny typ danych to: " + mode + '\n';
                            if (mode == "float")
                            {
                                opts.push_back("1.Kontynuuj");
                                opts.push_back("2.Wroc");
                                c = printShortInterFace(opts, warning);
                                opts.clear();
                                if (c == 0) countingSort(data, ascending);
                            }
                        }
                        else
                        {
                            countingSort(data, ascending);
                        }
                        system("cls");
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    system("cls");
                    cout << "Prosze najpierw wczytac dane\nKliknij dowolny klaiwsz";
                    cin.get();
                }
                break;
            case 6:
                if (data_loaded)
                {
                    unsigned int elements_f;
                    unsigned int step_f;
                    unsigned int iter_f;
                    system("cls");
                    do
                    {
                        cout << "Liczba iteracji: ";
                        cin >> iter_f;
                    } while (iter_f <= 0);
                    TimeMeas meas(data.size(), iter_f);
                    system("cls");
                    short c;
                    opts.push_back("1.Pomiar czasu sortowania babelkowego");
                    opts.push_back("2.Pomiar czasu sortowania szybkiego");
                    opts.push_back("3.Pomiar czasu sortowania przez kopcowanie");
                    opts.push_back("4.Pomiar czasu sortowania przez zliczanie");
                    opts.push_back("5.Wroc do MENU");
                    c = printShortInterFace(opts);
                    opts.clear();
                    switch (c)
                    {
                    case 0:
                        meas.measureFunctionTime(data, 1);
                        measurePoints = meas.getMeasurePoints();
                        for (int i = 0; i < measurePoints.size(); i++)
                        {
                            cout << "Czas sortowania : " << measurePoints[i].getY() << " ms.\n";
                        }
                        timeMeasToCSV(measurePoints, "pomiar.csv", "Sortowanie babelkowe");
                        cout << "Dane zapisano do pliku measurment.csv\nKliknij dowolny klaiwsz\n";
                        cin.get();
                        cin.get();
                        break;
                    case 1:
                        meas.measureFunctionTime(data, 2);
                        measurePoints = meas.getMeasurePoints();
                        for (int i = 0; i < measurePoints.size(); i++)
                        {
                            cout << "Czas sortowania : " << measurePoints[i].getY() << " ms.\n";
                        }
                        timeMeasToCSV(measurePoints, "pomiar.csv", "Sortowanie szybkie");
                        cout << "Dane zapisano do pliku pomiar.csv\nKliknij dowolny klaiwsz\n";
                        cin.get();
                        cin.get();
                        break;
                    case 2:
                        meas.measureFunctionTime(data, 3);
                        measurePoints = meas.getMeasurePoints();
                        for (int i = 0; i < measurePoints.size(); i++)
                        {
                            cout << "Czas sortowania : "<< measurePoints[i].getY() << " ms.\n";
                        }
                        timeMeasToCSV(measurePoints, "pomiar.csv", "Sortowanie przez kopcowanie");
                        cout << "Dane zapisano do pliku pomiar.csv\nKliknij dowolny klaiwsz\n";
                        cin.get();
                        cin.get();
                        break;
                    case 3:
                        meas.measureFunctionTime(data, 4);
                        measurePoints = meas.getMeasurePoints();
                        for (int i = 0; i < measurePoints.size(); i++)
                        {
                            cout << "Czas sortowania : " << measurePoints[i].getY() << " ms.\n";
                        }
                        timeMeasToCSV(measurePoints, "pomiar.csv", "Sortowanie przez zliczanie");
                        cout << "Dane zapisano do pliku pomiar.csv\nKliknij dowolny klaiwsz\n";
                        cin.get();
                        cin.get();
                        break;
                    default:
                        break;
                    }

                }
                else
                {
                    system("cls");
                    cout << "Prosze najpierw wczytac dane\nKliknij dowolny klaiwsz";
                    cin.get();
                }
                break;
            case 7:
                short c;
                opts.push_back("1.Nadpisz plik z danymi ");
                opts.push_back("2.Zapisz do osobnego pliku (SDZIO_OUTPUT_FILE.csv)");
                opts.push_back("3.Wroc do MENU");
                c = printShortInterFace(opts);
                opts.clear();
                system("cls");
                cout << "Zapisuje...\nProsze czekac...";
                if (c == 1)
                {
                    saveFile(data, mode, "SDZIO_OUTPUT_FILE.csv", output_file);
                    system("cls");
                    cout << "Zapisano!\nKliknij dowolny klaiwsz";
                    cin.get();
                }
                else if (c == 0)
                {
                    saveFile(data, mode, "dane.csv", output_file);
                    system("cls");
                    cout << "Zapisano!\nKliknij dowolny klaiwsz";
                    cin.get();
                }
                break;
            case 8:
                exit(0);
                break;
            default:
                break;
            }
        }
    }
}