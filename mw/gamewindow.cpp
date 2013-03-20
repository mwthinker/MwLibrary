#include "gamewindow.h"

#include <string>

namespace mw {

    GameWindow::GameWindow(int width, int height, std::string title, std::string icon) : Window(width, height, title, icon) {
        timeStep_ = 40;
        accumulator_ = 0;
    }

    void GameWindow::setTimeStep(Uint32 timeStep) {
        timeStep_ = timeStep;
    }

    Uint32 GameWindow::getTimeStep() const {
        return timeStep_;
    }

    void GameWindow::physicUpdate(Uint32 deltaTime) {

	}

	void GameWindow::renderPhysics(Uint32 deltaTime) {

	}

	void GameWindow::graphicUpdate(Uint32 deltaTime) {
    }

    void GameWindow::update(Uint32 deltaTime) {
        graphicUpdate(deltaTime);
        fixTimeStepPhysicUpdate(deltaTime);
    }

    void GameWindow::fixTimeStepPhysicUpdate(Uint32 deltaTime) {
		// To avoid to big accumulations. Can be very processor demanding.
		if (deltaTime > 300) {
			deltaTime = 300;
		}

		accumulator_ += deltaTime;
		while (accumulator_ >= timeStep_) {
			accumulator_ -= timeStep_;
			physicUpdate(timeStep_);
		}

		renderPhysics(accumulator_);
	}

} // namespace mw