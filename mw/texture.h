#ifndef MW_TEXTURE_H
#define MW_TEXTURE_H

#include <string>

#include <SDL_opengl.h>
#include <SDL_image.h>

#include <memory>

namespace mw {

	class Texture {
	public:
		// Loads a image from a file. It stores the image in memory and no opengl
		// code are of use in the constructor (safe to call constructor in other threads).
		Texture(std::string filename);

		// Cleens the image from memory and the opengl texture from graphic memory.
		~Texture();

		// Binds the texture to the target GL_TEXTURE_2D. First call, copies 
		// the image data to graphic memory.
		void bind();		

		// Returns the width of the image in pixels.
		int getWidth() const;

		// Returns the height of the image in pixels. 
		int getHeight() const;
	private:
		friend class Text;

		// class Texture takes over ownership of surface and is responsable of deallocation.
		// Not safe to use surface outside this class after calling the constuctor.
		Texture(SDL_Surface* surface);

		// Is called when the opengl kontext need to be loaded.
		// I.e.
		void loadToVideo();

		GLuint texture_;
		int width_, height_;

		SDL_Surface* preLoadSurface_;
		int loadedToVideoId_;
	};

	typedef std::shared_ptr<Texture> TexturePtr;

} // Namespace mw.

#endif // MW_TEXTURE_H
