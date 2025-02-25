#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>  // برای تنظیم خروجی
#include <cstdlib>

using namespace std;

// تعاریف ثابت
const int Lx = 40;      // طول دامنه
const int Ly = 20;      // عرض دامنه
const int maxiter = 1000; // تعداد تکرارها
const double T_1 = 110.0;  // دمای چیپ 1
const double T_2 = 120.0;  // دمای چیپ 2
const double T_3 = 110.0; // دمای چیپ 3
const double T_fin = 20.0; // دمای نهایی
const double T_initial = 60.0; // دمای اولیه

// تابع محاسبات دما به روش گوس سایدل
void computeTemperature(vector<vector<double>>& T) {
    int lenX = Lx;
    int lenY = Ly;
    for (int iteration = 0; iteration < maxiter; ++iteration) {
        for (int i = 1; i < lenY - 1; ++i) {
            for (int j = 1; j < lenX - 1; ++j) {
                T[i][j] = 0.25 * (T[i + 1][j] + T[i - 1][j] + T[i][j + 1] + T[i][j - 1]);
            }
        }
    }
}

int main() {
    // ایجاد مش
    int lenX = Lx;
    int lenY = Ly;
    vector<vector<double>> T(lenY, vector<double>(lenX, T_initial));

    // اعمال شرایط مرزی
    for (int j = 0; j < 16; ++j) {
        T[lenY - 1][j] = T_1;  // چیپ 1
    }

    for (int j = 0; j < int(0.2 * lenX); ++j) {
        T[0][j] = T_1 + (110 - 40) * j / (0.2 * lenX - 1); // چیپ 2
    }

    for (int j = int(0.4 * lenX); j < int(0.8 * lenX); ++j) {
        T[0][j] = T_3 + (120 - 80) * (j - int(0.4 * lenX)) / (0.4 * lenX - 1); // چیپ 3
    }

    for (int i = 4; i < 16; ++i) {
        T[i][lenX - 1] = T_fin; // دمای چیپ در سمت راست
    }

    // محاسبات دما
    computeTemperature(T);

    // ذخیره‌سازی خروجی در فایل قابل خواندن برای Tecplot
    ofstream outFile("temperature_data.dat");

    outFile << "TITLE = \"Temperature Distribution in Circuit\"\n";
    outFile << "VARIABLES = \"X\", \"Y\", \"Temperature\"\n";
    outFile << "ZONE T=\"Temperature Data\"\n";
    outFile << "I=" << lenX << ", J=" << lenY << ", F=POINT\n";

    // نوشتن داده‌ها به فایل
    for (int i = 0; i < lenY; ++i) {
        for (int j = 0; j < lenX; ++j) {
            outFile << fixed << setprecision(2) << j << " " << i << " " << T[i][j] << "\n";
        }
    }

    outFile.close();

    // نمایش پیغام موفقیت
    cout << "Data has been written to temperature_data.dat for Tecplot visualization.\n";

    return 0;
}
