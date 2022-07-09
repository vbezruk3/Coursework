#include <iostream>
#include <cmath>
#include <fstream>
#include <cassert>
#include <graphics.h>

/*вхідний та вихідний файли*/
#define IN_FILE			"files/input.txt"
#define OUT_FILE		"files/output.txt"

/*висота та ширина графічного вікна*/
#define HEIGHT          720
#define WIDTH           1280

/*довжина рядка*/
#define STR_LEN         99

/*можливі помилки*/
#define DIV_ERROR		"Cannot be divided by zero!"
#define SQRT_ERROR		"Cannot take the root of a negative number!"
#define OPEN_ERROR		"The file cannot be opened!"
#define CREATE_ERROR	"The file cannot be created!"
#define DATA_ERROR		"Invalid data!"

/*межі параметрів*/
#define p0A 3
#define p0B 5
#define TA  400
#define TB  600

using namespace std;

/*клас об’єкт обчислення*/
class CompObj {
protected:
	/*обчислюваний параметр*/
	double calcPar;

public:
	/*віртуальна функція введення даних*/
	virtual void set() = 0;

	/*віртуальна функція виведення даних*/
	virtual void get() = 0;

	/*віртуальна функція обчислення*/
	virtual void calc() = 0;

	/*віртуальна функція друкування результату обчислення*/
	virtual void getResult() = 0;
};

/*клас ідеальний газ похідний від абстрактного класу об’єкт обчислення*/
class PerfectGas : virtual public CompObj {
private:
	/*параметри для обчислення*/
	double R, T, Kg, M, p, p0, K;

	/*функція для перевірки параметрів*/
	void checkParams() {
		assert(R > 0 && T >= TA && TB >= T && M > 0
			&& p0 > p && p > 0 && p0 >= p0A
			&& p0B >= p0 && K > 0 && DATA_ERROR);
	}

public:
	/*конструктор за замовчуванням*/
	PerfectGas() { R = T = Kg = M = p = p0 = K = 0; }

	/*констуктор з ініціалізацією списком*/
	PerfectGas(double _R, double _T, double _K, double _M, double _p, double _p0) : R(_R),
		T(_T), K(_K), M(_M), p(_p), p0(_p0) {
		checkParams();
	}

	/*констуктор копіювання*/
	PerfectGas(const PerfectGas& x) {
		R = x.R;
		T = x.T;
		Kg = x.Kg;
		M = x.M;
		p = x.p;
		p0 = x.p0;
		K = x.K;
		checkParams();
	}

	/*перевантажена фукнція введення даних*/
	void set() {
		cout << "Enter info about perfect gas:" << endl;
		cout << " R = "; 	cin >> R;
		cout << " T = "; 	cin >> T;
		cout << " M = "; 	cin >> M;
		cout << " p = "; 	cin >> p;
		cout << " p0 = "; 	cin >> p0;
		cout << " K = "; 	cin >> K;
		cout << endl;
		checkParams();
	}

	/*перевантажена функція виведення даних*/
	void get() {
		cout << "Info about perfect gas:" << endl;
		cout << " R = " << R << endl;
		cout << " T = " << T << endl;
		cout << " Kg = " << Kg << endl;
		cout << " M = " << M << endl;
		cout << " p = " << p << endl;
		cout << " p0 = " << p0 << endl;
		cout << " K = " << K << endl;
		cout << endl;
	}

	/*перевантажена фукнція для обчислення*/
	void calc() {
		assert(Kg * M && p0 && DIV_ERROR);

		calcPar = (2 * R * T / (Kg * M)) * (1 - pow(p / p0, Kg));

		assert(calcPar >= 0 && SQRT_ERROR);

		calcPar = sqrt(calcPar);
	}

	/*функція для обчислення параметра Kg*/
	void calcKg() {
		assert(K && DIV_ERROR);

		Kg = (K - 1) / K;
	}

	/*функція для виведення обчислювального параметру*/
	void getResult() {
		cout << "Calculated parameter = " << calcPar << endl;
		cout << endl;
	}

	/*оператор присвоювання*/
	PerfectGas& operator = (const PerfectGas& x) {
		R = x.R;
		T = x.T;
		Kg = x.Kg;
		M = x.M;
		p = x.p;
		p0 = x.p0;
		K = x.K;
		checkParams();
		return *this;
	}

	/*оператор ==*/
	bool operator == (const PerfectGas& x) { return calcPar == x.calcPar; }

	/*оператор !=*/
	bool operator != (const PerfectGas& x) { return calcPar != x.calcPar; }

	/*оператор <*/
	bool operator < (const PerfectGas& x) { return calcPar < x.calcPar; }

	/*оператор <=*/
	bool operator <= (const PerfectGas& x) { return calcPar <= x.calcPar; }

	/*оператор >*/
	bool operator > (const PerfectGas& x) { return calcPar > x.calcPar; }

	/*оператор >=*/
	bool operator >= (const PerfectGas& x) { return calcPar >= x.calcPar; }

	/*оператор для введення даних із файлу*/
	friend ifstream& operator >> (ifstream& ifs, PerfectGas& x);

	/*оператор для виведення даних у файл*/
	friend ofstream& operator << (ofstream& ofs, const PerfectGas& x);

	/*оператор для введення даних із консолі*/
	friend istream& operator >> (istream& is, PerfectGas& x);

	/*оператор для виведення даних у консоль*/
	friend ostream& operator << (ostream& os, const PerfectGas& x);

	/*дружня функція для табулювання*/
	friend void tabulate(PerfectGas data, double dp, double p0, double p1,
		string xLabel, string yLabel, string xSystem, string ySystem, ofstream& ofs);
};

/*оператор для введення даних із файлу*/
ifstream& operator >> (ifstream& ifs, PerfectGas& x) {
	assert(ifs && OPEN_ERROR);

	ifs >> x.R >> x.T >> x.K >> x.M >> x.p >> x.p0;
	x.checkParams();
	return ifs;
}

/*оператор для виведення даних у файл*/
ofstream& operator << (ofstream& ofs, const PerfectGas& x) {
	assert(ofs && CREATE_ERROR);

	ofs << "Info about perfect gas:" << endl;
	ofs << " R  = " << x.R << endl;
	ofs << " T  = " << x.T << endl;
	ofs << " Kg = " << x.Kg << endl;
	ofs << " M  = " << x.M << endl;
	ofs << " p  = " << x.p << endl;
	ofs << " p0 = " << x.p0 << endl;
	ofs << " K  = " << x.K << endl;
	ofs << endl;

	return ofs;
}

/*оператор для введення даних із консолі*/
istream& operator >> (istream& is, PerfectGas& x) {
	cout << "Enter info about perfect gas:" << endl;
	cout << " R  = "; 	is >> x.R;
	cout << " T  = "; 	is >> x.T;
	cout << " K  = "; 	is >> x.K;
	cout << " M  = "; 	is >> x.M;
	cout << " p  = "; 	is >> x.p;
	cout << " p0 = "; 	is >> x.p0;
	cout << endl;
	x.checkParams();
	return is;
}

/*оператор для виведення даних у консоль*/
ostream& operator << (ostream& os, const PerfectGas& x) {
	os << "Info about perfect gas:" << endl;
	os << " R  = " << x.R << endl;
	os << " T  = " << x.T << endl;
	os << " Kg = " << x.Kg << endl;
	os << " M  = " << x.M << endl;
	os << " p  = " << x.p << endl;
	os << " p0 = " << x.p0 << endl;
	os << " K  = " << x.K << endl;
	os << endl;

	return os;
}

/*допоміжна функція для побудови графіка (знаходження мінімуму у масиві)*/
double getMin(double arr[], int n) {
	int i, iMin = 0;

	for (i = 1; i < n; i++) {
		if (arr[iMin] > arr[i]) {
			iMin = i;
		}
	}

	return arr[iMin];
}

/*допоміжна функція для побудови графіка (знаходження максимуму у масиві)*/
double getMax(double arr[], int n) {
	int i, iMax = 0;

	for (i = 1; i < n; i++) {
		if (arr[iMax] < arr[i]) {
			iMax = i;
		}
	}

	return arr[iMax];
}

/*допоміжна функція для побудови графіка*/
int convert(double c, double dc, double cMin) {return round((c - cMin) / dc);}

/*функція для побудови графіка*/
void draw(double* x, double* y, int n, string xLabel, string yLabel, string xSystem, string ySystem) {
	string xLabelG;
	string yLabelG;

	double xMin = getMin(x, n);
	double xMax = getMax(x, n);
	double yMin = getMin(y, n);
	double yMax = getMax(y, n);

	int w = getmaxx();
	int h = getmaxy();

	double dx = (xMax - xMin) / w;
	double dy = (yMax - yMin) / (h - 10);

	string label = "Result";
	char labelBkp[STR_LEN];

	int i;

	setbkcolor(WHITE);
	setlinestyle(SOLID_LINE, 0, 5);
	cleardevice();
	setcolor(RED);

	for (i = 0; i < n - 1; i++) {
		line(convert(x[i], dx, xMin),
			h - convert(y[i], dy, yMin),
			convert(x[i + 1], dx, xMin),
			h - convert(y[i + 1], dy, yMin));
	}

	setcolor(BLUE);

	outtextxy(w / 2 - 10 * label.length(), 0, &label[0]);

	xLabelG = xLabel + ", " + xSystem;
	yLabelG = yLabel + ", " + ySystem;

	outtextxy(0, 0, &yLabelG[0]);
	outtextxy(w - 10 * xLabelG.length(), h - 20, &xLabelG[0]);

	setcolor(GREEN);

	snprintf(labelBkp, STR_LEN, "%lg", y[0]);

	outtextxy(0, h - 20, labelBkp);

	snprintf(labelBkp, STR_LEN, "%lg", y[n - 1]);

	outtextxy(0, 20, labelBkp);

	snprintf(labelBkp, STR_LEN, "%lg", (y[0] + y[n - 1]) / 2);

	outtextxy(0, h / 2, labelBkp);

	snprintf(labelBkp, STR_LEN, "%lg", x[0]);

	outtextxy(20, h - 30, labelBkp);

	snprintf(labelBkp, STR_LEN, "%lg", x[n - 1]);

	outtextxy(w - 30, h - 30, labelBkp);

	snprintf(labelBkp, STR_LEN, "%lg", (x[0] + x[n - 1]) / 2);

	outtextxy(w / 2, h - 30, labelBkp);
}

/*функція для табулювання*/
void tabulate(PerfectGas data, double dp, double p0, double p1,
	string xLabel, string yLabel, string xSystem, string ySystem, ofstream& ofs) {
	double p = p0;
	int n = ((p1 - p0) / dp) + 1, i = 0;
	double x[n], y[n];

	cout.precision(3);
	cout << scientific;

	ofs.precision(3);
	ofs << scientific;

	do {
		x[i] = p;

		data.p0 = p;
		data.calc();

		y[i] = data.calcPar;

		p += dp;

		cout << xLabel << " = " << x[i] << "\t" << yLabel << " = " << y[i] << endl;
		ofs << xLabel << " = " << x[i] << "\t" << yLabel << " = " << y[i] << endl;

		i++;
	} while (p <= (p1 + 1e-7));

	draw(x, y, n, xLabel, yLabel, xSystem, ySystem);
	getch();
}

int main() {
	initwindow(WIDTH, HEIGHT);

	cout << "###Constructors test###" << endl << endl;

	PerfectGas x;

	cout << "Default constructor:" << endl << x;

	PerfectGas y(8.31696, 400, 1.402, 28.96, 1, 3);

	y.calcKg();

	cout << "Init constructor:" << endl << y;

	PerfectGas z = y;

	cout << "Copy constructor:" << endl << z;

	cout << "###Functions test###" << endl << endl;

	cout << "Set function:" << endl;
	x.set();

	cout << "Get function:" << endl;
	x.get();

	x.calcKg(); x.calc();

	cout << "Result function:" << endl;
	x.getResult();

	cout << "###Operators test###" << endl << endl;

	cout << "x object:" << endl;
	x.get();

	cout << "y object:" << endl;
	y.get();

	cout << "Operator == between obj x & y" << endl;
	cout << "result = " << boolalpha << (x == y) << endl << endl;

	cout << "Operator != between obj x & y" << endl;
	cout << "result = " << boolalpha << (x != y) << endl << endl;

	cout << "Operator < between obj x & y" << endl;
	cout << "result = " << boolalpha << (x < y) << endl << endl;

	cout << "Operator <= between obj x & y" << endl;
	cout << "result = " << boolalpha << (x <= y) << endl << endl;

	cout << "Operator > between obj x & y" << endl;
	cout << "result = " << boolalpha << (x > y) << endl << endl;

	cout << "Operator >= between obj x & y" << endl;
	cout << "result = " << boolalpha << (x >= y) << endl << endl;

	cout << "Operator = between obj x & y" << endl;
	x = y;

	cout << "x object:" << endl;
	x.get();

	cout << "y object:" << endl;
	y.get();

	cout << "###Reading from file and calculating value###" << endl << endl;

	PerfectGas a; ifstream ifs(IN_FILE); ofstream ofs(OUT_FILE);

	ifs >> a;

	a.calcKg(); a.calc();

	cout << a;

	a.getResult();

	ofs << a;

	cout << "###Tabulate func###" << endl << endl;

	double dp, p0, p1;

	cout << "Enter dp, p0 & p1: ";
	cin >> dp >> p0 >> p1;

	cout << endl << "Result:" << endl;
	ofs << "Result:" << endl;

	tabulate(a, dp, p0, p1, "p0", "w", "ata", "m/s", ofs);

	return 0;
}