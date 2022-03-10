#ifndef Fehler_H
#define Fehler_H

#include <iostream>

using namespace std;


class Fehler {
public:
	virtual void show() {
		cerr << "Unbekannte Error." << endl;
	}
};

class Eingabe : public Fehler {
private:
	const char* info;
public:
	Eingabe() { info = ""; }
	Eingabe(const char* message): info(message){}
	void show() {
		cerr << "Ungueltige Eingabe." << endl;
		cerr << info << endl;
	}
};

class Position : public Fehler {
public:
	virtual void show() {
		cerr << "Ungueltige Position." << endl;
	}
};

class SmallPosition : public Position {
public:
	void show() {
		cerr << "Erforderliche Position ist zu klein." << endl;
	}
};

class BigPosition : public Position {
public:
	void show() {
		cerr << "Gesuchte Position ist zu gross." << endl;
	}
};

class NotSquare : public Fehler {
public:
	void show() {
		cerr << "Matrix ist nicht quadratisch." << endl;
	}
};

class DifferentDimensions : public Fehler {
public:
	void show() {
		cerr << "Matrizen haben unterschiedliche Dimensionen." << endl;
	}
};

class InappropriateDimensions : public Fehler {
public:
	void show() {
		cerr << "Matrizen haben unpassende Dimensionen." << endl;
	}
};
#endif