#include "ofMain.h"
//#include "Spout.h"

class Receiver {

private:

	vector<string> inputs;

public:

	void setup() {
	}

	void update() {
	}

	void next() {
	}

	bool isChanged() {
		return false;
	}

	void bind() {
	}

	void unbind() {
	}

	vector<string>& getInputs() {
		return inputs;
	}

	bool isEnabled() {
		return false;
	}

	int getWidth() {
		return 1;
	}

	int getHeight() {
		return 1;
	}

	void setInput(int index) {
	}

	string getActiveInput() {
		return "";
	}
};