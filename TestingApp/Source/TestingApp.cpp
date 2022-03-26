#include "Cae.h"
#include <iostream>

class TestingApp : public Cae::App {
public:
	TestingApp() {

	}
	~TestingApp() {

	}
};

using namespace std;

int main(int argc, char** argv) {
	TestingApp* tApp = new TestingApp();
	tApp->Run();
	delete tApp;

	return 0;
}
/*

*/