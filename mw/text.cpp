#include "text.h"

#include "initttf.h"
#include "font.h"

#include <SDL_ttf.h>
#include <string>

#include <memory>

namespace mw {	
	Text::Text() {
	}

    Text::Text(std::string text, FontPtr font) : font_(font) {
        text_ = text;
        loadText(text_);
        characterSize_ = font->getCharacterSize();
    }

	Text::Text(std::string text, FontPtr font, unsigned int characterSize) : font_(font) {
        text_ = text;
        loadText(text_);
        characterSize_ = characterSize;
    }

    Text::~Text() {
    }

    void Text::setText(std::string text) {
		// Text changed => load new textImage.
        if (text_ != text) {
            loadText(text);
            //std::cout << "\nText changed from '" << text_ << "' to '" << text<<"'";
            text_ = text;
        }
    }

    std::string Text::getText() const {
        return text_;
    }

    void Text::setCharacterSize(unsigned int size) {
        characterSize_ = size;
    }

    unsigned int Text::getCharacterSize() const {
        return characterSize_;
    }

	// Returns the width of the text to be drawn. (size in openGl
    double Text::getWidth() const {
		if (font_) {
			double scale = characterSize_*1.0/font_->getCharacterSize();
			return scale * texture_->getWidth();
		}
		return 0.0;
    }

	// Returns the height of the text to be drawn.
    double Text::getHeight() const {
		if (font_) {
			double scale = characterSize_*1.0/font_->getCharacterSize();
			return scale*texture_->getHeight();
		}
		return 0.0;
    }

    void Text::draw() {
		if (font_) {
			if (text_.size() > 0) {
				glPushMatrix();
				double scale = characterSize_*1.0/font_->getCharacterSize();
				glScaled(scale * texture_->getWidth(),scale * texture_->getHeight(),1.0);
				glTranslated(0.5,0.5,0.0);
				sprite_.draw();
				glPopMatrix();
			}
		}
    }
// private
    void Text::loadText(std::string str) {
		if (font_) {
			if (str.size() > 0) {
				SDL_Color color = {255,255,255};
				SDL_Surface* surface = TTF_RenderText_Blended(font_->font_, str.c_str(), color);
				// texture_ takes ownership of surface
				texture_ = TexturePtr(new Texture(surface));
				sprite_.setTexture(texture_);
			}
		}
    }

} // Namespace mw
