#ifndef MW_GAMEWINDOW_H
#define MW_GAMEWINDOW_H

#include "window.h"

#include <string>

namespace mw {

	class GameWindow : public Window {
	public:
		GameWindow(int width, int height, std::string title = "Game", std::string icon = "");
	protected:
		// Sets the fix time, msTimeStep in millisecond, for each call to physicUpdate. 
		// However, the real time between calls can be shorter or longer but the mean
		// value is msTimeStep.
		void setTimeStep(Uint32 msTimeStep);
		// Returns the current fix time.
		Uint32 getTimeStep() const;

		// Is called by the loop with fix time between each call. The paramter msDeltaTime
		// is the current fix time.
		// Is intenden to be inherited, and is in this implemention empty.
		virtual void physicUpdate(Uint32 msDeltaTime);

		virtual void renderPhysics(Uint32 deltaTime);

		// Is called by the loop. The frequency in which this function is called is fixed
		// by the vertical frequency of the monitor. The time between two calls can vary,
		// but is displayed in parameter msDeltaTime in milliseconds.
		// Is intenden to be inherited, and is in this implemention empty.
		virtual void graphicUpdate(Uint32 msDeltaTime);
	private:
		void update(Uint32 deltaTime) override;
		void fixTimeStepPhysicUpdate(Uint32 deltaTime);

		Uint32 timeStep_;
		Uint32 accumulator_;
	};

} // Namespace mw

#endif // MW_GAMEWINDOW_H
