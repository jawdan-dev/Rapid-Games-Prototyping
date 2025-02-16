#pragma once
#include <beingEngine/core/core.hpp>

#include <beingEngine/rendering/renderer/renderer.hpp>
#include <virtualGame/story/character/character.hpp>
#include <virtualGame/story/textRenderer/textRenderer.hpp>

class StoryInterpreter {
public:
	StoryInterpreter();
	StoryInterpreter(const StoryInterpreter& other) = delete;
	~StoryInterpreter();

	const int getFlag(const String& flagName) const;
	const int getCharacterIndex(const String& characterName) const;
	const bool hasCharacter(const String& characterName) const;
	const Character& getCharacter(const String& characterName) const;

	void addCharacter(const Character& character);
	void setFlag(const String& flagName, const int flagValue);
	void setScale(const float scale) { m_relativeScale = scale; };

	void process();
	void draw(Renderer& renderer, TextRenderer& textRenderer);

private:
	void readFile();

	void processInput();

	const int getGotoLine(const String& name) const;
	const bool gotoLine(const String& name);

	struct DrawInformation {
		int specialCount;
		Vector3 color;
		bool enabled;
		bool errorOccurred;
		bool colorWholeLine;
	};
	const int shouldHideCharacter(const char* string);
	const bool resolveSpecial(String& input, DrawInformation& drawInformation) const;

	const bool handleInteraction(String& input);

	const Vector3 getDrawPosition(const float xIndex, const float yIndex) const;

	const bool getCharacterConditionArguments(const String& condition, String& name, String& expression, String& value) const;
	const bool isCharacterCondition(const String& condition) const;
	const bool characterConditionMet(const String& condition) const;

	const bool getStatementArguments(const String& condition, String& name, String& expression, String& value) const;
	const bool isStatement(const String& condition) const;
	const bool runStatement(const String& condition);

private:
	std::vector<Character> m_characters;
	std::map<String, int> m_flags;

	size_t m_currentLine;
	std::vector<String> m_lines;

	std::vector<String> m_activeLines;

	int m_drawLine, m_drawCharacter;
	float m_delay;

	String m_lastGoto;
	float m_inputCooldown;
	float m_relativeScale;
};