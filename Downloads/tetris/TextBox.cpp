#include "TextBox.h"

Textbox::Textbox(int size, sf::Color color, bool sel) {
	textbox.setCharacterSize(size);
	textbox.setFillColor(color);
	isSelected = sel;

	// Check if the textbox is selected upon creation and display it accordingly:
	if (isSelected)
		textbox.setString("_");
	else
		textbox.setString("");
}

// Make sure font is passed by reference:
void Textbox::setFont(sf::Font& fonts) {
	textbox.setFont(fonts);
}

void Textbox::setPosition(sf::Vector2f point) {
	textbox.setPosition(point);
}

// Set char limits:
void Textbox::setLimit(bool ToF) {
	hasLimit = ToF;
}

void Textbox::setLimit(bool ToF, int lim) {
	hasLimit = ToF;
	limit = lim - 1;
}

// Change selected state:
void Textbox::setSelected(bool sel) {
	isSelected = sel;

	// If not selected, remove the '_' at the end:
	if (!sel) {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length(); i++) {
			newT += t[i];
		}
		textbox.setString(newT);
	}
}

std::string Textbox::getText() {
	return text.str();
}

void Textbox::drawTo(sf::RenderWindow& window) {
	window.draw(textbox);
}

// Function for event loop:
void Textbox::typedOn(sf::Event input) {
	if (isSelected) {
		int charTyped = input.text.unicode;

		// Only allow normal inputs:
		if (charTyped < 128) {
			if (hasLimit) {
				// If there's a limit, don't go over it:
				if (text.str().length() <= limit) {
					inputLogic(charTyped);
				}
				// But allow for char deletions:
				else if (text.str().length() > limit && charTyped == DELETE_KEY) {
					deleteLastChar();
				}
			}
			// If no limit exists, just run the function:
			else {
				inputLogic(charTyped);
			}
		}
	}
}
void Textbox::deleteLastChar() {
	std::string t = text.str();
	std::string newT = "";
	for (int i = 0; i < t.length() - 1; i++) {
		newT += t[i];
	}
	text.str("");
	text << newT;
	textbox.setString(text.str() + "_");
}

	// Get user input:
void Textbox::inputLogic(int charTyped) {
	// If the key pressed isn't delete, or the two selection keys, then append the text with the char:
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
		text << static_cast<char>(charTyped);
	}
	// If the key is delete, then delete the char:
	else if (charTyped == DELETE_KEY) {
		if (text.str().length() > 0) {
			deleteLastChar();
		}
	}
	// Set the textbox text:
	textbox.setString(text.str() + "_");
}