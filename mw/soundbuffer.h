#ifndef MW_SOUNDBUFFER_H
#define MW_SOUNDBUFFER_H

#include "initsdl.h"

#include <SDL_mixer.h>

#include <string>
#include <unordered_map>
#include <memory>

namespace mw {

	class SoundBuffer;
	typedef std::shared_ptr<SoundBuffer> SoundBufferPtr;

	class SoundBuffer : public InitSdl {
	public:
		friend class Sound;

		SoundBuffer(std::string filename);
		~SoundBuffer();

		// Returns true if everything is correct. Else false.
		bool isValid() const;

		// Returns error message, i.e. when the isValid function returns false.
		// If no error string will be empty.
		std::string getError() const;

	private:
		SoundBuffer(const SoundBuffer&) {
			// Not to be used. Is not copyable.
		}

		SoundBuffer& operator=(const SoundBuffer&) {
			// Not to be used. Is not copyable.
			return *this;
		}

		Mix_Chunk* mixChunk_;

		static int nbrOfInstances_;

		static std::unordered_map<int, int> channelList_;// <channel, sound id>
		bool valid_;
		std::string error_;
	};

} // namespace mw

#endif // MW_SOUNDBUFFER_H
