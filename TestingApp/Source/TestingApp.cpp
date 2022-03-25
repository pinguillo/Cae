#include "Cae.h"

class TestingApp : public Cae::App {
public:
	TestingApp() {

	}
	~TestingApp() {

	}
};

int main(int argc, char** argv) {
	TestingApp* tApp = new TestingApp();
	tApp->Run();
	delete tApp;
}