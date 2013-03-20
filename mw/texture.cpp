#include "texture.h"
#include "window.h"

#include <SDL_image.h>
#include <SDL_opengl.h>

#include <iostream>

namespace mw {

	namespace {
		// Quick utility function for texture creation.
		int powerOfTwo(int input) {
			int value = 1;

			while (value < input) {
				value <<= 1;
			}

			return value;
		}

		// Creates and returns a opengl texture from the surface provided.
		GLuint SdlGlLoadTexture(SDL_Surface* const surface) {
			GLuint textureId;

			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);

			int mode = GL_RGB;

			if (surface->format->BytesPerPixel == 4) {
				mode = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			return textureId;
		}
	}

	Texture::Texture(std::string filename) : preLoadSurface_(0) {
		preLoadSurface_ = IMG_Load(filename.c_str());
		if (preLoadSurface_ == 0) {
			std::cout << "\nImage " << filename << " failed to load: " << IMG_GetError();			
		} else {
			width_ = preLoadSurface_->w;
			height_ = preLoadSurface_->h;
		}

		// Is called when the opengl kontext need to be loaded.
		// I.e.
		loadedToVideoId_ = mw::Window::getVideoId()-1; // loadedToVideoId_ != mw::Window::getVideoId() means that the opengl texture is not loaded yet.
	}

	Texture::~Texture() {
		if (loadedToVideoId_ == mw::Window::getVideoId()) {
			// Is called if the opengl texture is valid and therefore need to be cleaned up.
			glDeleteTextures(1,&texture_);
		}

		// Safe to pass null.
		SDL_FreeSurface(preLoadSurface_);
	}

	void Texture::bind() {
		if (loadedToVideoId_ != mw::Window::getVideoId()) {
			// Loads to video memory if current texture not valid.
			loadToVideo();
		}
		glBindTexture(GL_TEXTURE_2D, texture_);        
	}

	int Texture::getWidth() const {
		return width_;
	}

	int Texture::getHeight() const {
		return height_;
	}

	// class Texture takes over ownership of surface and is responsable of deallocation.
	// Not safe to use surface outside this class after calling the constuctor.
	Texture::Texture(SDL_Surface* surface) : preLoadSurface_(surface) {
		width_ = preLoadSurface_->w;
		height_ = preLoadSurface_->h;

		// To tell that a opengl texture should be created.
		loadedToVideoId_ = mw::Window::getVideoId()-1;
	}

	// Is called when the opengl context need to be loaded.
	void Texture::loadToVideo() {
		texture_ = SdlGlLoadTexture(preLoadSurface_);
		loadedToVideoId_ = mw::Window::getVideoId();
	}


} // namespace mw