#include "ofMain.h"
#include "Spout.h"

class Receiver {

private:

	vector<string> inputs;

	SpoutReceiver	*spoutReceiver;
	bool			bInit;
	bool			bEnabled;
	bool			bMemShare;
	bool			bChanged;
	ofTexture		tex;
	unsigned int	width, height;
	char			senderName[256];

public:

	void setup() {
		bInit = false;
		bEnabled = false;
		width = 320;
		height = 240;
		senderName[0] = NULL;

		inputs.push_back(""); // always its size is 1

		spoutReceiver = new SpoutReceiver();

		tex.allocate(width, height, GL_RGBA);
	}

	void update() {

		char str[256];
		unsigned int w, h;
		char tempName[256];

		bEnabled = false;

		if (!bInit) {
			// specify true to attempt to connect to active sender
			if (spoutReceiver->CreateReceiver(senderName, w, h, true)) {
				// is the size of the detected sender defferent?
				if (w != width || h != height) {
					// fit size of texture
					width = w;
					height = h;
					tex.allocate(width, height, GL_RGBA);
				}
				bInit = true;
			}
			return;
		}

		// the receiver has initialized so OK to draw
		if (bInit) {
			w = width;
			h = height;

			if ( spoutReceiver->ReceiveTexture(senderName,w, h, tex.getTextureData().textureID, tex.getTextureData().textureTarget) ) {
			
				// width and height are changed. the local tex the has to be resized
				if (w != width || h != height) {
					width = w;
					height = h;
					tex.allocate(width, height, GL_RGBA);
					return;
				}

				// OK
			} else {
				// a tex read failure might happen if the sender is closed.
				// release the receiver and start again.
				spoutReceiver->ReleaseReceiver();
				bInit = false;
				return;
			}
		}

		bEnabled = true;
	}

	void exit() {
		spoutReceiver->ReleaseReceiver();
		delete spoutReceiver;
		spoutReceiver = NULL;
	}

	void next() {
		bChanged = false;
	}

	void bind() {
		tex.bind();
	}

	void unbind() {
		tex.unbind();
	}

	vector<string>& getInputs() {
		inputs[0] = senderName;
		return inputs;
	}

	bool isEnabled() {
		return bEnabled;
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void setInput(int index) {
	}

	string getActiveInput() {
		return "";
	}
};