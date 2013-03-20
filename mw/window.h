#ifndef MW_WINDOW_H
#define MW_WINDOW_H

#include "initsdl.h"
#include "joystick.h"

#include <string>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

namespace mw {

	class ScreenResolution {
	public:
		inline ScreenResolution() {
			width_ = 0;
			height_ = 0;
		}

		inline ScreenResolution(int width, int height) {
			width_ = width;
			height_ = height;
		}

		inline int getWidth() const {
			return width_;
		}

		inline int getHeight() const {
			return height_;
		}

	private:
		int width_, height_;
	};

	class Window : public InitSdl {
	public:
		// Creates a window with size determened by width and height in pixels. The windows name is defined by title. And
		// icon image is defined by filepath icon. Only one instance should be actived at each time due to SDL limitations.
		Window(int width, int height, std::string title = "Window", std::string icon = "");

		virtual ~Window() {};

		// Starts a loop which handles all events and graphics in the window. It will not return until the loop is ended.
		// Is closed when the windows is closed, i.e. a call to the protected function setQuiting(true).
		void startLoop();

		// Returns an integer. If the openGl context is lost, the video id will change.
		// Therefore, change indicates that all openGl context need to be reloaded.
		static int getVideoId();

	protected:
		// Forces the window to the width and height in pixels. Calls to resize(width,height) is made.
		void setWindowsSize(int width, int height);

		// Sets the window to be resized manually by a user. Is true by default.
		void setResizable(bool resize);

		// Returns if the windows can be resized by a user. Is true by default.
		bool isResizable() const;

		// Sets the title of the windows.
		void setWindowsTitle(std::string title);
		// Gets the current title of the windows.
		std::string getWindowsTitle() const;

		// Enables/Disable unicode keys to be interpreted from keyboard input. By default is it disabled.
		void setUnicodeInputEnable(bool unicode);
		// Returns, if unicode keys is to be interpreted from keyboard input, is enbled or disabled.
		bool isUnicodeInputEnable() const;

		// Returns a vector of all valid fullscreen resolutions.
		std::vector<ScreenResolution> getValidScreenResolutions();

		// Returns the current valid fullscreen resolution, i.e. the resolution which will be active if 
		// setFullScreen(true) is called.
		ScreenResolution getFullScreenResolution() const;

		// Sets the fullscreen resolution which will be used whenever the program is to go to fullscreen mode.
		// The parameter resolution may only be set to a valid resoltuion. Valid, is a resoltuion that exists in the vector 
		// returned by getValidScreenResolutions().
		void setFullScreenResolution(ScreenResolution resolution);

		// Sets the program in fullscreeen mode if true else in windows mode.
		// If mode is not changed from current mode, isFullScreen(), nothing happens.
		void setFullScreen(bool fullScreen);
		// Returns true if the program is in fullscreen mode.
		bool isFullScreen() const;

		// Returns the current windows width in pixels.
		int getWidth() const;
		// Returns the current windows height in pixels.
		int getHeight() const;

		// The returned value represents the number of milliseconds since SDL library initialization. 
		Uint32 timeTick();

		// Sets the program to quit by setting quit to true, i.e. the loop in startLoop will be made to stop and startloop will return.	
		void setQuiting(bool quit);

		// Returns if the program is quiting. Is false by default, until a call to setQuiting(true) is made.
		bool isQuiting() const;
	private:
		// Is called by the loop. The frequency in which this function is called is fixed
		// by the vertical frequency of the monitor. The time between two calls can vary,
		// but is displayed in parameter msDeltaTime in milliseconds.
		// Is intenden to be inherited, and is in this implemention empty.
		virtual void update(Uint32 deltaTime);

		// Is called by the loop. Is called when ever a SDL_EVENT occure, and the event is in windowEvent.
		// Is intenden to be inherited, but has a simple implemention which handles
		// the closing of the window.
		// The resize event will not be showed in here, it is handled by resize fucntion.
		virtual void eventUpdate(const SDL_Event& windowEvent);

		// Is called whenever the window is resized.
		virtual void resize(int width, int height);

		bool quit_;
		std::string windowsTitle_;
		bool unicode_;	

		int width_, height_;
		unsigned int time_;
		Uint32 videoflags_;
		bool isResizable_;
		bool fullScreen_;

		ScreenResolution fullScreenResolution_, screenResolution_;
		static int videoId_;
	};

} // Namespace mw

#endif // MW_WINDOW_H
