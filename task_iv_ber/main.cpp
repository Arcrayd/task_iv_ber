#include "pch.h"
using namespace std;
struct berResults
{
    double tot;
    double err;
    float ber;
    clock_t t1;
    clock_t t2;
};

uint8_t hammingDistance(uint8_t n1, uint8_t n2);
void createFile1(const string name, const int count, const char value);
berResults calculateBer(string fpath1, string fpath2);
void printResult(berResults results);

int main(int argc, char* argv[])
{
    string fpath1;
    string fpath2;
    berResults results;

    openLog("log.log");
    if (argc != 3)
    {
        saveLog("Uzytkownik nie przedstawil sciezki do plikow.");
        saveLog("Tworzenie plikow testowych przez uzytkownika...");



        printf("\n\n Prosze wybrac test \n\n 1.Test 1: Przygotuj dwa identyczne pliki zawierajace binarnie zapisana wartosc 0x55. Kazdy plik po 100 Bajtow. \n\n 2.Test 2: Przygotuj dwa pliki kazdy po 100 bajtow. Pierwszy zawiera binarnie zapisana wartosc 0x55. Drugi plik rozni sie od pierwszego o dowolne 10 bitow. \n\n 3.Test 3: Przygotuj dwa pliki kazdy po 400 MB. Pierwszy zawiera binarnie zapisana wartosc 0x55. Drugi plik zawiera binarnie zapisana wartosc 0x50. \n\n ");
        int wybor;
        cin >> wybor;
        switch (wybor) {

        case 1:
        {
            //test 1 tworzymy takie same pliki z 0x55 

            createFile1("test1_file1.bin", 100, 0x55);
            createFile1("test1_file2.bin", 100, 0x55);
            results = calculateBer("test1_file1.bin", "test1_file2.bin");
            printResult(results);
            break;
        }
        //test 2 pliki, ktore roznia sie 10 bajtami
        case 2:
        {
            createFile1("test2_file1.bin", 100, 0x55);
            createFile1("test2_file2.bin", 100, 0x5F);
            results = calculateBer("test2_file1.bin", "test2_file2.bin");
            printResult(results);
            break;
        }

        case 3:
        {
            // test 3, 2 pliki po 400mb
            createFile1("test3_file1.bin", 400000000, 0x55);
            createFile1("test3_file2.bin", 400000000, 0x50);
            results = calculateBer("test3_file1.bin", "test3_file2.bin");
            printResult(results);
            break;
        }
        }


        saveLog("Pliki testowe gotowe.");
        saveLog("Ponowne uruchamianie z odpowiednimi argumentami: ./task_iv_ber.exe test1_file1.bin test1_file2.bin");
    }
    else
    {
        fpath1 = argv[1];
        fpath2 = argv[2];

        saveLog("Trwa przetwarzanie plików testowych.");
        results = calculateBer(fpath1, fpath2);
        printResult(results);
    }

    closeLog();
    return 0;
}

uint8_t hammingDistance(uint8_t n1, uint8_t n2)
{
    uint8_t x = n1 ^ n2;
    uint8_t setBits = 0;
    while (x > 0)
    {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

void createFile1(const string name, const int count, const char value)
{
    fstream f;
    f.open(name.c_str(), ios::binary | ios::out);
    for (int i = 0; i < count; i++)
    {
        f.write((char*)&value, 1);
    }
    f.close();
}

berResults calculateBer(string fpath1, string fpath2)
{
    fstream f1, f2;
    berResults results;
    results.t1 = 0;
    results.t2 = 0;
    results.ber = 0;
    results.err = 0;
    results.tot = 0;

    saveLog("Obliczenia Bit Error Rate...");
    f1.open(fpath1.c_str(), ios::binary | ios::in);
    f2.open(fpath2.c_str(), ios::binary | ios::in);
    char a = 0x00;
    char b = 0x00;
    results.t1 = clock();

    while (!f1.eof())
    {
        f1 >> a;
        f2 >> b;
        if (!f1.eof())
        {
            results.err += hammingDistance(a, b);
            results.tot += 8;
        }
    }

    results.ber = (float)results.err / results.tot;
    results.t2 = clock();
    saveLog("Obliczenia Bit Error Rate zakończone.");
    return results;
}

void printResult(berResults results)
{
    stringstream message;
    message << "Results are: " << endl;
    message << "BER: " << results.ber << endl;
    message << "Tot: " << results.tot << endl;
    message << "Err: " << results.err << endl;
    message << "Calc time: " << ((float)results.t2 - results.t1) / CLOCKS_PER_SEC << " sec " << endl;
    saveLog(message.str());
}