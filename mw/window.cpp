#include "window.h"

#include <iostream>

namespace mw {

int Window::videoId_ = 0;

Window::Window(int width, int height, std::string title, std::string icon) {
    SDL_WM_SetIcon(SDL_LoadBMP(icon.c_str()), 0); // Must be called before first use of SDL_SetVideoMode (source: sdl documentation).

	videoflags_ = SDL_OPENGL | SDL_RESIZABLE;
	isResizable_ = true; // By default resizeable. If not, a white border will
	// appear and the window will be a little bigger. Then the window will go back to
	// normal size and without a border when window is moved or resized.
	// Cause of this behavior is unknown by me (MW).

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // TURNING VSYNC ON
    SDL_SetVideoMode(width, height, 32, videoflags_);

	quit_ = false;
    time_ = 0;
    width_ = width;
    height_ = height;

    setWindowsTitle(title);
	unicode_ = false;
	fullScreen_ = false;

	screenResolution_ = ScreenResolution(width,height);
	fullScreenResolution_ = getValidScreenResolutions().at(0);//front(); // Assumes vector is not empty, dangerous!
}

void Window::startLoop() {
	while (!quit_) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event eventSDL;
		while (SDL_PollEvent(&eventSDL)) {
			if (eventSDL.type == SDL_VIDEORESIZE) {
				setWindowsSize(eventSDL.resize.w,eventSDL.resize.h);

				std::cout << "\nSDL_VIDEORESIZE " << width_ << " " << height_;
			} else {
				eventUpdate(eventSDL);
			}
		}

		Uint32 currentTime = SDL_GetTicks();
		Uint32 deltaTime = currentTime - time_;
		time_ = currentTime;

		update(deltaTime);

		SDL_GL_SwapBuffers();
	}
}

// Returns an integer. If the openGl context is lost, the video id will be changed (i.e. old+1).
// Change indicates that all openGl context need to be reloaded.
int Window::getVideoId() {
	return videoId_;
}

// protected
void Window::setWindowsSize(int width, int height) {
	if (!isFullScreen()) {
		SDL_SetVideoMode(width, height, 32, videoflags_);
		width_ = width;
		height_ = height;
		screenResolution_ = ScreenResolution(width,height);
		resize(width_,height_);
		std::cout << "\nSetWindowsSize " << width_ << " " << height_;
	}
}

void Window::setResizable(bool resize) {
	if (isResizable_ != resize) {
		if (resize) {
			videoflags_ = SDL_OPENGL | SDL_RESIZABLE;
			isResizable_ = true;
		} else {
			videoflags_ = SDL_OPENGL;
			isResizable_ = false;
		}
		// Will only update video mode if something is changed.
		SDL_SetVideoMode(width_, height_, 32, videoflags_);
	}
}

bool Window::isResizable() const {
	return isResizable_;
}

void Window::setWindowsTitle(std::string title) {
	SDL_WM_SetCaption(title.c_str(), title.c_str());
	windowsTitle_ = title;
}

void Window::setUnicodeInputEnable(bool unicode) {
	unicode_ = unicode;
	if (unicode_) {
		SDL_EnableUNICODE(1);
	} else {
		SDL_EnableUNICODE(0);
	}
}

bool Window::isUnicodeInputEnable() const {
	return unicode_;
}

std::vector<ScreenResolution> Window::getValidScreenResolutions() {
	// Get available fullscreen/hardware modes
	SDL_Rect** modes;
	modes = SDL_ListModes(NULL, SDL_FULLSCREEN|videoflags_);

	std::vector<ScreenResolution> resolutions_;

	// Check if there are any modes available
	if (modes == (SDL_Rect**)0) {
		printf("No modes available!\n");
		return resolutions_;
	}

	// Check if our resolution is restricted
	if (modes == (SDL_Rect**)-1 ) {
		printf("All resolutions available.\n");
	} else {
		// Print valid modes
		printf("Available Modes\n");
		for (int i = 0; modes[i]; ++i) {
			//printf("  %d x %d\n", modes[i]->w, modes[i]->h);
			resolutions_.push_back(ScreenResolution(modes[i]->w,modes[i]->h));
		}
	}
	// TEST ________________________________________________________
	return resolutions_;
}

ScreenResolution Window::getFullScreenResolution() const {
	return fullScreenResolution_;
}

void Window::setFullScreenResolution(ScreenResolution resolution) {
	fullScreenResolution_ = resolution;
}

void Window::setFullScreen(bool fullScreen) {
	if (fullScreen_ != fullScreen) {
		if (fullScreen) {
			videoflags_ = SDL_OPENGL | SDL_FULLSCREEN;
			width_ = fullScreenResolution_.getWidth();
			height_ = fullScreenResolution_.getHeight();
			SDL_Surface* surface = SDL_SetVideoMode(fullScreenResolution_.getWidth(), fullScreenResolution_.getHeight(), 32, videoflags_);
			if (surface == 0) {
				// Reset to old the window settings.
				fullScreen = false;
			}
			++videoId_;
			fullScreen_ = true;
		}

		if (!fullScreen) {
			if (isResizable_) {
				videoflags_ = SDL_OPENGL | SDL_RESIZABLE;
			} else {
				videoflags_ = SDL_OPENGL;
			}
			width_ = screenResolution_.getWidth();
			height_ = screenResolution_.getHeight();
			SDL_SetVideoMode(width_, height_, 32, videoflags_);
			++videoId_;
			fullScreen_ = false;
		}

		resize(width_,height_);
	}
}

bool Window::isFullScreen() const {
	return fullScreen_;
}

std::string Window::getWindowsTitle() const {
	return windowsTitle_;
}

int Window::getWidth() const {
	return width_;
}

int Window::getHeight() const {
	return height_;
}

Uint32 Window::timeTick() {
	return SDL_GetTicks();
}

void Window::setQuiting(bool quit) {
	quit_ = quit;
}

bool Window::isQuiting() const {
	return quit_;
}

// Private
void Window::update(Uint32 deltaTime) {
}

void Window::eventUpdate(const SDL_Event& windowEvent) {
	switch (windowEvent.type) {
		// exit if the window is closed
	case SDL_QUIT:
		mw::Window::setQuiting(true);
		break;
		// check for keypresses
	default:
		break;
	}
}

void Window::resize(int width, int height) {
}

} // namespace mw
