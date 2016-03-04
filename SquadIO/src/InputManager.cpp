// TODO:
// map other keys in inputmap.h
// read up on https://wiki.libsdl.org/CategoryGameController SDL's equivalent of xinput?

#pragma once
#include "include\InputManager.h"
// #include "LogManager.h"

using namespace std;
const int JOYSTICK_DEAD_ZONE = 6000;
InputManager* InputManager::inputInstance = nullptr;

// constructor
InputManager::InputManager() :keyboardState(nullptr), mouseState(0),
mouseX(0), mouseY(0), locked(false) {
	mouseDown.resize(MOUSE_SIZE);
	mouseUp.resize(MOUSE_SIZE);
	joystickButtonHeld.resize(JOYSTICK_MAX);
	joystickButtonPressed.resize(JOYSTICK_MAX);
	joystickButtonReleased.resize(JOYSTICK_MAX);
	joystickAnalogs.resize(4);
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	isJoystickAvailable();
};

InputManager* InputManager::getInstance() {
	// if InputManager is nullptr
	if (!inputInstance) {
		inputInstance = new InputManager();
	}
	return inputInstance;
}

// updates at every frame for new input
void InputManager::update() {
	// reset mouse to neutral state, continue polling for up/down events
	for (int i = 0; i < MOUSE_SIZE; i++) {
		this->mouseDown[i] = 0;
		this->mouseUp[i] = 0;
	}
	for (int i = 0; i < JOYSTICK_MAX; i++) {
		joystickButtonPressed[i]=false;
		joystickButtonReleased[i] = false;
	}
	// poll for mouse events
	// http://wiki.libsdl.org/SDL_Event for case types
	//	int index;
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			cout << "quit the game" << endl;
			break;
			// SDL_MouseButtonEvent
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.button == SDL_BUTTON_LEFT) {
				cout << "left" << endl;
				this->mouseDown[MOUSE_LEFT] = 1;
			}
			else if (ev.button.button == SDL_BUTTON_MIDDLE) {
				this->mouseDown[MOUSE_MIDDLE] = 1;
			}
			else if (ev.button.button == SDL_BUTTON_RIGHT) {
				this->mouseDown[MOUSE_RIGHT] = 1;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (ev.button.button == SDL_BUTTON_LEFT) {
				this->mouseUp[MOUSE_LEFT] = 1;
			}
			else if (ev.button.button == SDL_BUTTON_MIDDLE) {
				this->mouseUp[MOUSE_MIDDLE] = 1;
			}
			else if (ev.button.button == SDL_BUTTON_RIGHT) {
				this->mouseUp[MOUSE_RIGHT] = 1;
			}
			break;
			// SDL_MouseWheelEvent
		case SDL_MOUSEWHEEL:
			break;
			// SDL_MouseMotionEvent
		case SDL_MOUSEMOTION:
			// store mouse location
			this->mouseX = ev.motion.x;
			this->mouseY = ev.motion.y;
			break;
		case SDL_JOYBUTTONDOWN:
			joystickButtonPressed[ev.jbutton.button] = true;
			joystickButtonHeld[ev.jbutton.button] = true;
			cout << "joystick button pressed down" << endl;
			break;
		case SDL_JOYBUTTONUP:
			joystickButtonReleased[ev.jbutton.button] = true;
			this->joystickButtonHeld[ev.jbutton.button] = false;
			cout << "joystick button pressed up" << endl;
			break;
		case SDL_JOYAXISMOTION:
			//X axis motion
			//cout << "axis value: " << ev.jaxis.value << endl;
			if (ev.jaxis.axis == 0){
				//out of dead zone
				if (abs(ev.jaxis.value) > JOYSTICK_DEAD_ZONE){
					joystickAnalogs[0] = ev.jaxis.value / 32767.0;
				}
				else{
					joystickAnalogs[0] = 0;
				}
			}//Y axis motion
			else if (ev.jaxis.axis == 1){
				//Below of dead zone
				if (abs(ev.jaxis.value) > JOYSTICK_DEAD_ZONE){
					joystickAnalogs[1] = ev.jaxis.value / 32767.0;
				}
				else{
					joystickAnalogs[1] = 0;
				}
			}
		default:
			break;
		}
	}
	keyboardState = SDL_GetKeyboardState(nullptr);
}

// return true if keydown
bool InputManager::isKeyDown(int key) {
	if (this->locked) {
		return false;
	}
	if (key < 0 || key >= KEYBOARD_SIZE) {
		return false;
	}
	return this->keyboardState[key] ? true : false;
}

// return true if keyup
bool InputManager::isKeyUp(int key) {
	if (this->locked) {
		return false;
	}
	if (key < 0 || key >= KEYBOARD_SIZE) {
		return false;
	}
	return this->keyboardState[key] ? true : false;
}

// return true if mousedown
bool InputManager::isMouseDown(int b) {
	if (this->locked) {
		return false;
	}
	if (b < 0 || b >= MOUSE_SIZE) {
		return false;
	}
	return this->mouseDown[b] ? true : false;
}

// return true if mouseup
bool InputManager::isMouseUp(int b) {
	if (this->locked) {
		return false;
	}
	if (b < 0 || b >= MOUSE_SIZE) {
		return false;
	}
	return this->mouseUp[b] ? true : false;
}

int InputManager::getMouseX() {
	return this->mouseX;
}

int InputManager::getMouseY() {
	return this->mouseY;
}

void InputManager::lock() {
	this->locked = true;
}

void InputManager::unlock() {
	this->locked = false;
}


bool InputManager::isJoystickAvailable(){
	SDL_JoystickEventState(SDL_ENABLE);
	SDL_Joystick *joystick;
	joystick = SDL_JoystickOpen(0);
	if (SDL_NumJoysticks() < 1)return false;
	else return true;
}

bool InputManager::isJoystickPressed(int b) {
	if (this->locked) return false;
	if (b < 0 || b >= JOYSTICK_MAX) return false;
	return this->joystickButtonPressed[b];
}

bool InputManager::isJoystickReleased(int b) {
	if (this->locked) return false;
	if (b < 0 || b >= JOYSTICK_MAX) return false;
	return this->joystickButtonReleased[b];
}

bool InputManager::isJoystickUp(int b) {
	if (this->locked) return false;
	if (b < 0 || b >= JOYSTICK_MAX) return false;
	return !joystickButtonHeld[b];
}

bool InputManager::isJoystickDown(int b) {
	if (this->locked) return false;
	if (b < 0 || b >= JOYSTICK_MAX) return false;
	return this->joystickButtonHeld[b];
}

double InputManager::getLeftThumbX(){
	return joystickAnalogs[0];
}
double InputManager::getLeftThumbY(){
	return joystickAnalogs[1];
}

double InputManager::getRightThumbX(){
	return 0;
}
double InputManager::getRightThumbY(){
	return 0;
}