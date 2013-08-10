#include "soundbuffer.h"

namespace mw {

	int SoundBuffer::nbrOfInstances_ = 0;

	std::map<int,int> SoundBuffer::channelList_;

	SoundBuffer::SoundBuffer(std::string filename) : valid_(true) {
		mixChunk_ = 0;
		if (nbrOfInstances_ < 1) {
			int success = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
			if (success == -1) {
				valid_ = false;
				error_ = "Sound failed to initiate!";
			}
		}
		++nbrOfInstances_;
		if (valid_) {
			mixChunk_ = Mix_LoadWAV(filename.c_str());
			if (mixChunk_ == 0) {
				error_ = filename + " failed to load!";
			}
		}
	}

	SoundBuffer::~SoundBuffer() {
		if (mixChunk_ != 0) {
			Mix_FreeChunk(mixChunk_);
		}

		--nbrOfInstances_;
		if (nbrOfInstances_ < 1) {
			Mix_CloseAudio();
		}
	}

	bool SoundBuffer::isValid() const {
		return valid_;
	}

	std::string SoundBuffer::getError() const {
		return error_;
	}

} // Namespace mw
