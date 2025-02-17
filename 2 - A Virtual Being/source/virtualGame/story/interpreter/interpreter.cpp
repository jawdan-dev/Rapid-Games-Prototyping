#include "interpreter.hpp"

#include <beingEngine/systems/input.hpp>
#include <beingEngine/systems/time.hpp>

StoryInterpreter::StoryInterpreter() :
	m_characters(), m_flags(),
	m_currentLine(0),
	m_lines(), m_activeLines(),
	m_drawLine(0), m_drawCharacter(0), m_delay(0),
	m_lastGoto(""), m_relativeScale(1.0f),
	m_interactingGroup(-1), m_inputCooldown(0) {
	readFile();

	addCharacter(Character("Player", Vector3(1.0f, 1.0f, 1.0f)));
	addCharacter(Character("System", Vector3(102, 119, 153) / 255.0f));
	addCharacter(Character("AI", Vector3(255, 0, 68) / 255.0f));

	addCharacter(Character("Jeremy", Vector3(153, 34, 51) / 255.0f));
	addCharacter(Character("Markus", Vector3(255, 170, 119) / 255.0f));
	addCharacter(Character("Alex", Vector3(0, 187, 170) / 255.0f));
	addCharacter(Character("Ernest", Vector3(221, 68, 34) / 255.0f));
	addCharacter(Character("Sage", Vector3(204, 255, 153) / 255.0f));
	addCharacter(Character("Pamela", Vector3(153, 34, 136) / 255.0f));
	addCharacter(Character("Tori", Vector3(255, 187, 51) / 255.0f));
	addCharacter(Character("Abby", Vector3(255, 153, 153) / 255.0f));
	addCharacter(Character("Vincent", Vector3(136, 17, 255) / 255.0f));

	setFlag("AIFactor", 50);
	setFlag("HumanFactor", 50);
	setFlag("EnemyPower", 100);
}
StoryInterpreter::~StoryInterpreter() {}

const int StoryInterpreter::getFlag(const String& flagName) const {
	auto it = m_flags.find(flagName);
	if (it == m_flags.end())
		return 0;
	return it->second;
}
const int StoryInterpreter::getCharacterIndex(const String& characterName) const {
	for (int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i].getName() == characterName)
			return i;
	}
	return -1;
}
const bool StoryInterpreter::hasCharacter(const String& characterName) const {
	return getCharacterIndex(characterName) != -1;
}
const Character& StoryInterpreter::getCharacter(const String& characterName) const {
	return m_characters[getCharacterIndex(characterName)];
}

void StoryInterpreter::addCharacter(const Character& character) {
	m_characters.emplace_back(character);
}
void StoryInterpreter::setFlag(const String& flagName, const int flagValue) {
	auto it = m_flags.find(flagName);
	if (it != m_flags.end())
		m_flags.erase(it);
	m_flags.emplace(flagName, flagValue);
}

void StoryInterpreter::process() {
	processInput();

	if (m_delay > 0.0f) {
		m_delay -= Time::s_time->getDelta();
		return;
	}

	if (m_activeLines.size() <= 0) {
		while (m_currentLine < m_lines.size() && m_lines[m_currentLine] != "\\end") {
			printf("[%zu] %s\n", m_lines[m_currentLine].size(), m_lines[m_currentLine].c_str());
			m_activeLines.emplace_back(m_lines[m_currentLine]);
			m_currentLine++;
		}
		m_drawLine = 0;
		m_drawCharacter = 0;
	}

	if (m_drawLine < m_activeLines.size()) {
		String line = m_activeLines[m_drawLine];
		std::vector<int> lineEquivalent;
		lineEquivalent.reserve(line.size());

		DrawInformation drawInformation;
		int lineSelectableOffset = 0;
		for (int j = 0; j < line.size(); j++) {
			String newLine = line.substr(j);
			const size_t newLineLength = newLine.size();

			if (!resolveSpecial(newLine, drawInformation)) {
				lineEquivalent.push_back(j + lineSelectableOffset);
				continue;
			}

			lineSelectableOffset += ((int)newLineLength - (int)newLine.size());
			line = line.substr(0, j) + newLine;
			j--;
		}

		if (m_drawCharacter < line.size()) {
			if (line[m_drawCharacter] == '.') {
				for (int i = lineEquivalent[m_drawCharacter]; i >= 0; i--) {
					if (m_activeLines[m_drawLine][i] == '.')
						continue;
					if (m_activeLines[m_drawLine][i] != '\\')
						break;
					m_delay = 0.5f;
					break;
				}
			}
			m_drawCharacter++;
		} else {
			m_drawCharacter = 0;
			m_drawLine++;
		}
	}
	if (m_delay <= 0)
		m_delay = 0.01f;
}

static const float spacing = 1.2f;
static const float lineSpacing = 1.5f;
static const float scale = 17.5f;
static const Vector3 offset(50.0f, 50.0f, 0.0f);

static const int maxLineWidth =
	(1280.0f - (offset.x() * 2.0f) + ((spacing - 1.0f) * scale)) /
	(scale * spacing);

void StoryInterpreter::draw(Renderer& renderer, TextRenderer& textRenderer) {
	int drawHeight = 0;
	int activeLineGroup = 0;

	m_interactingGroup = -1;
	for (int lineIndex = 0; lineIndex < m_activeLines.size() && lineIndex <= m_drawLine; lineIndex++) {
		struct SectionProperties {
			int group, originalLine;
			Vector3 color;
			bool enabled, errorOccurred, canSplit;
		};

		Vector3 defaultColor = Vector3(1.0f, 1.0f, 1.0f);

		String line = m_activeLines[lineIndex];
		std::vector<SectionProperties> lineSections;
		for (int i = 0; i < line.size(); i++)
			lineSections.emplace_back((SectionProperties){
				.group = -1,
				.originalLine = i,
				.color = defaultColor,
				.enabled = false,
				.errorOccurred = false,
				.canSplit = false,
			});

		for (int j = 0; j < line.size(); j++) {
			String newLine = line.substr(j);
			const int newLineLength = newLine.size();

			DrawInformation drawInformation{
				.specialCount = 0,
				.color = defaultColor,
				.enabled = false,
				.errorOccurred = false,
				.colorWholeLine = false,
			};
			const bool special = resolveSpecial(newLine, drawInformation);
			if (drawInformation.colorWholeLine)
				defaultColor = drawInformation.color;

			if (drawInformation.specialCount > 0) {
				for (int i = 0; i < drawInformation.specialCount; i++) {
					const int index = j + i;
					lineSections[index].color = drawInformation.color;

					lineSections[index].originalLine = lineSections[j].originalLine;
					lineSections[index].enabled |= drawInformation.enabled;
					lineSections[index].errorOccurred |= drawInformation.errorOccurred;

					if (lineSections[index].group == -1)
						lineSections[index].group = activeLineGroup;
				}
				activeLineGroup++;
			} else {
				if (lineSections[j].group == -1)
					lineSections[j].color = drawInformation.color;
			}

			if (!special)
				continue;

			line = line.substr(0, j) + newLine;
			const int lineSelectableOffset = ((int)newLineLength - (int)newLine.size());
			if (lineSelectableOffset > 0) {
				// Smaller -> Remove elements.
				for (int i = 0; i < lineSelectableOffset; i++)
					lineSections.erase(lineSections.begin() + j + drawInformation.specialCount);
			} else if (lineSelectableOffset < 0) {
				// Bigger -> Add elements.
				for (int i = 0; i < -lineSelectableOffset; i++)
					lineSections.insert(lineSections.begin() + j + 1, lineSections[j]);
			}

			j--;
		}

		for (int i = 0; i < line.size() && i < lineSections.size(); i++) {
			if (lineSections[i].group != -1 || line[i] != ' ')
				continue;
			lineSections[i].canSplit = true;
		}

		// printf("%s\n", line.c_str());
		// for (int i = 0; i < line.size(); i++)
		// 	printf("%c", lineSections[i].enabled ? '^' : (lineSections[i].canSplit ? '|' : '_'));
		// printf("\n");
		// for (int i = 0; i < line.size(); i++)
		// 	printf("%c", lineSections[i].group != -1 ? '0' + lineSections[i].group : 'x');
		// printf("\n");
		// for (int i = 0; i < line.size(); i++)
		// 	printf("%i", (int)(lineSections[i].color.y() * 9));
		// printf("\n");
		// exit(0);

		struct DrawLine {
			String line;
			std::vector<SectionProperties> sections;
		};
		std::vector<DrawLine> drawLines;

		int textOffset = 0;
		if (m_activeLines[lineIndex].substr(0, 3) == "\\@[") {
			for (int i = 0; i + 1 < line.size(); i++) {
				if (m_activeLines[lineIndex].substr(i, 3) == "]: ") {
					textOffset = i + 2;
					break;
				}
			}
		}
		String lineOffsetSpacing = "";
		for (int i = 0; i < textOffset; i++)
			lineOffsetSpacing += ' ';

		int maxLineWidthLocal = maxLineWidth;
		while (line.size() > maxLineWidthLocal) {
			int splitIndex = maxLineWidthLocal;
			for (; splitIndex >= 0 && !lineSections[splitIndex].canSplit; splitIndex--)
				;
			if (splitIndex < 0) {
				splitIndex = maxLineWidthLocal;
				for (; splitIndex < line.size() && !lineSections[splitIndex].canSplit; splitIndex++)
					;
				if (splitIndex >= line.size())
					break;
			}

			std::vector<SectionProperties> drawSections;
			if (drawLines.size() > 0)
				for (int i = 0; i < textOffset; i++)
					drawSections.push_back(lineSections[0]);
			for (int i = 0; i < splitIndex; i++)
				drawSections.push_back(lineSections[i]);

			drawLines.emplace_back((DrawLine){
				.line = (drawLines.size() > 0) ? (lineOffsetSpacing + line.substr(0, splitIndex)) : line.substr(0, splitIndex),
				.sections = drawSections,
			});

			line = line.substr(splitIndex + 1);
			lineSections.erase(lineSections.begin(), lineSections.begin() + splitIndex + 1);

			maxLineWidthLocal = maxLineWidth - textOffset;
		}
		if (drawLines.size() > 0) {
			for (int i = 0; i < textOffset; i++)
				lineSections.push_back(lineSections[0]);
			drawLines.emplace_back((DrawLine){
				.line = lineOffsetSpacing + line,
				.sections = lineSections,
			});
		} else {
			drawLines.emplace_back((DrawLine){
				.line = line,
				.sections = lineSections,
			});
		}

		for (int i = 0; i < drawLines.size() && m_interactingGroup == -1; i++) {
			for (int j = 0; j < drawLines[i].line.size() && j < drawLines[i].sections.size(); j++) {
				if (!drawLines[i].sections[j].enabled || drawLines[i].sections[j].errorOccurred)
					continue;

				const Vector3
					min(getDrawPosition((float)j - 0.5f, (float)(drawHeight + i) - 0.5f)),
					max(getDrawPosition((float)j + 0.5f, (float)(drawHeight + i) + 0.5f));
				const Vector2 mpos = Input::s_input->getMousePosition();
				const bool mouseWithin = min.x() <= mpos.x() && mpos.x() <= max.x() && min.y() <= mpos.y() && mpos.y() <= max.y();

				const bool isInteracting = mouseWithin && m_inputCooldown <= 0.0f;
				if (!isInteracting)
					continue;

				m_interactingGroup = drawLines[i].sections[j].group;
				const bool used = Input::s_input->isKeyUp(GLFW_MOUSE_BUTTON_1);

				if (!used)
					continue;

				m_inputCooldown = 0.4f;

				const int lineStart = drawLines[i].sections[j].originalLine;
				String modifiableLine = m_activeLines[lineIndex].substr(lineStart);

				if (handleInteraction(modifiableLine))
					return;

				m_activeLines[lineIndex] = m_activeLines[lineIndex].substr(0, lineStart) + modifiableLine;
			}
		}
		for (int i = 0; i < drawLines.size(); i++) {
			for (int j = 0; j < drawLines[i].line.size() && j < drawLines[i].sections.size(); j++) {
				Vector3 drawColor = drawLines[i].sections[j].color;
				if (drawLines[i].sections[j].errorOccurred) {
					drawColor = Vector3(1.0f, 0.0f, 1.0f);
				} else if (m_interactingGroup != -1 && m_interactingGroup == drawLines[i].sections[j].group) {
					const bool pressed = Input::s_input->isKeyPressed(GLFW_MOUSE_BUTTON_1),
							   used = Input::s_input->isKeyUp(GLFW_MOUSE_BUTTON_1);

					if (used) {
						drawColor = Vector3(1.0f, 1.0f, 1.0f);
					} else if (pressed) {
						drawColor = drawColor * Vector3(0.6f, 0.5f, 0.8);
					} else {
						drawColor = drawColor * Vector3(1.2f, 1.1f, 1.4);
					}
				}

				textRenderer.drawCharacter(renderer, getDrawPosition(j, drawHeight), scale * m_relativeScale, drawLines[i].line[j], drawColor);
			}
			drawHeight++;
		}
	}
}

void StoryInterpreter::readFile() {
	const String path = "story";
	const String story = File::readAll(path + ".txt");

	m_lines.clear();
	for (size_t i = 0; i < story.size(); i++) {
		String line = "";
		while (i < story.size() && story[i] != '\n')
			line += story[i++];

		m_lines.emplace_back(line);
	}
}

void StoryInterpreter::processInput() {
	if (m_inputCooldown > 0.0f) {
		m_inputCooldown -= Time::s_time->getDelta();
		return;
	}

	if (Input::s_input->getMousePosition().lengthSquared() <= 0.0f || m_interactingGroup != -1)
		return;

	if (Input::s_input->isKeyUp(GLFW_MOUSE_BUTTON_1)) {
		if (m_drawLine < m_activeLines.size()) {
			m_drawLine = m_activeLines.size();
			m_inputCooldown = 0.6f;
		}
	}
	if (Input::s_input->isKeyUp(GLFW_KEY_F1)) {
		readFile();
		if (!gotoLine(m_lastGoto)) {
			m_activeLines.clear();
			m_currentLine = 0;
		}
		m_inputCooldown = 1.0f;
	}
	if (Input::s_input->isKeyUp(GLFW_KEY_F5)) {
		readFile();
		m_activeLines.clear();
		m_currentLine = 0;
		m_inputCooldown = 1.0f;
	}
}

const int StoryInterpreter::getGotoLine(const String& name) const {
	const String targetLine = "\\#" + name + ":";

	for (size_t i = 0; i < m_lines.size(); i++) {
		if (m_lines[i] != targetLine)
			continue;

		return i + 1;
	}
	return -1;
}
const bool StoryInterpreter::gotoLine(const String& name) {
	const int gotoLine = getGotoLine(name);
	if (gotoLine == -1)
		return false;

	m_lastGoto = name;
	m_currentLine = gotoLine;
	m_activeLines.clear();

	return true;
}

const int StoryInterpreter::shouldHideCharacter(const char* string) {
	if (*string != '\\')
		return 0;

	switch (*(string + 1)) {
		case '.':
			return 1;
	}

	if (strncmp(string, "\\high", 5) == 0)
		return 5;

	return 0;
}
const bool StoryInterpreter::resolveSpecial(String& input, DrawInformation& drawInformation) const {
	if (input.substr(0, 4) == "\\end") {
		input = input.substr(4);
		return true;
	}
	if (input.substr(0, 2) == "\\.") {
		input = input.substr(1);
		return true;
	}

	struct HighlightColor {
		String text;
		Vector3 color;
	};
	std::vector<HighlightColor> highlightColors = {
		(HighlightColor){
			.text = "\\high[",
			.color = Vector3(0.2f, 0.6f, 1.0f),
		},
		(HighlightColor){
			.text = "\\error[",
			.color = Vector3(255, 0, 68) / 255.0f,
		},
		(HighlightColor){
			.text = "\\warning[",
			.color = Vector3(255, 187, 51) / 255.0f,
		},
		(HighlightColor){
			.text = "\\success[",
			.color = Vector3(85, 221, 85) / 255.0f,
		},
	};

	for (auto highlight : highlightColors) {
		if (input.substr(0, highlight.text.size()) != highlight.text)
			continue;

		input = input.substr(highlight.text.size());

		int i = 0;
		int depth = 0;
		for (; i < input.size() && (input[i] != ']' || depth > 0); i++) {
			if (input[i] == '[')
				depth++;
			else if (input[i] == ']')
				depth--;
		}
		if (i >= input.size() || input[i] != ']')
			return false;

		input.erase(input.begin() + i);
		drawInformation.color = highlight.color;
		drawInformation.specialCount = i;
		return true;
	}

	if (input.substr(0, 5) == "\\line") {
		input = input.substr(5);
		for (int i = 0; i < maxLineWidth; i++)
			input = "-" + input;

		return true;
	}
	if (input.substr(0, 8) == "\\center[") {
		input = input.substr(8);

		int i = 0;
		int depth = 0;
		for (; i < input.size() && (input[i] != ']' || depth > 0); i++) {
			if (input[i] == '[')
				depth++;
			else if (input[i] == ']')
				depth--;
		}
		if (i >= input.size() || input[i] != ']')
			return false;

		const String centered = input.substr(0, i);
		input.erase(input.begin() + i);

		const int spaceCount = (maxLineWidth - centered.size()) / 2;
		for (int i = 0; i < spaceCount; i++)
			input = " " + input;

		return true;
	}
	if (input.substr(0, 14) == "\\relationship[") {
		input = input.substr(14);

		int i = 0;
		int depth = 0;
		for (; i < input.size() && (input[i] != ']' || depth > 0); i++) {
			if (input[i] == '[')
				depth++;
			else if (input[i] == ']')
				depth--;
		}
		if (i >= input.size() || input[i] != ']')
			return false;

		const String name = input.substr(0, i);

		String output = "";
		auto it = m_flags.find(name);
		int countOffset = 0;
		if (it != m_flags.end()) {
			output = "\\@[" + name + "] [";
			const int value = __min(__max(it->second, -2), 2);
			for (int i = -2; i <= 2; i++)
				output += i == value ? (i == 0 ? "+" : (i < 0 ? "\\error[+]" : "\\success[+]")) : "-";

			countOffset += 4;
			if (value < 0)
				countOffset += 8;
			if (value > 0)
				countOffset += 10;

			output += "]";
		} else {
			output = "{  ?  }";
		}

		while (output.size() < (maxLineWidth / 3) + countOffset)
			output = " " + output;

		input = output + input.substr(i + 1);
		drawInformation.specialCount = output.size() - 1;
		drawInformation.color = Vector3(1.0f, 1.0f, 1.0f);
		return true;
	}

	if (input.substr(0, 2) == "\\<") {
		input = input.substr(2);

		std::vector<String> commands;

		int i = 0, j = 0;
		for (; i < input.size() && (input[i] != '>' || (i + 1 < input.size() && input[i + 1] != ' ')); i++) {
			if (input[i] == '|') {
				commands.push_back(input.substr(j, i - j));
				j = i + 1;
			}
		}
		if (i < input.size() && input[i] == '>') {
			commands.push_back(input.substr(j, i - j));
		} else { // if (commands.size() <= 0)
			return false;
		}

		bool passesAllConditions = true;
		String finalCommand = commands[0];
		for (size_t k = 0; k < commands.size(); k++) {
			if (isCharacterCondition(commands[k])) {
				if (!characterConditionMet(commands[k])) {
					passesAllConditions = false;
				}
				continue;
			}

			finalCommand = commands[k];
			break;
		}
		input = finalCommand + input.substr(i + 1);

		drawInformation.enabled = passesAllConditions;
		drawInformation.color = drawInformation.enabled ? Vector3(1.0f, 0.2f, 0.4f) : Vector3(0.3f, 0.2f, 0.4f);
		drawInformation.specialCount = finalCommand.size();

		if (getGotoLine(commands[commands.size() - 1]) == -1)
			drawInformation.errorOccurred = true;

		return true;
	}
	if (input.substr(0, 3) == "\\~[") {
		input = input.substr(3);
		int i = 0;
		int depth = 0;
		for (; i < input.size() && input[i] != '|' && (input[i] != ']' || depth > 0); i++) {
			if (input[i] == '[')
				depth++;
			else if (input[i] == ']')
				depth--;
		}
		const bool hasPipe = input[i] == '|';
		int j = i;
		while (j < input.size() && (input[j] != ']' || depth > 0)) {
			if (input[j] == '[')
				depth++;
			else if (input[j] == ']')
				depth--;
			j++;
		}
		if (input[j] == ']')
			j++;
		input.erase(input.begin() + i, input.begin() + j);

		drawInformation.color = Vector3(1.0f, 0.2f, 0.4f);
		drawInformation.specialCount = hasPipe ? i : 0;
		drawInformation.enabled = true;
		return true;
	}

	if (input.substr(0, 3) == "\\@[") {
		input = input.substr(3);
		int i = 0;
		for (; i < input.size() && input[i] != ']'; i++)
			;
		if (i >= input.size() || input[i] != ']')
			return false;

		const String name = input.substr(0, i);
		String displayName = name;

		if (hasCharacter(name)) {
			const Character& character = getCharacter(name);
			drawInformation.color = character.getColor();
			displayName = character.getName();
		} else {
			drawInformation.color = Vector3(0.3f, 0.2f, 0.4f);
		}

		const int nextI = i + 1;
		if (nextI >= input.size() || input[nextI] != ':') {
			if (nextI < input.size() && input[nextI] == '(') {
				int j = nextI + 1;
				for (; j < input.size() && input[j] != ')'; j++)
					;
				if (j >= input.size() || input[j] != ')')
					return false;
				const String nickname = input.substr(nextI + 1, j - (nextI + 1));
				displayName = nickname;
				i = j;
			}
		} else if (nextI < input.size() && input[nextI] == ':') {
			i++;

			if (displayName == "System") {
				displayName = "> System";
			} else {
				displayName = "@<" + displayName + ">:";
			}

			drawInformation.colorWholeLine = true;
		}

		input = displayName + input.substr(i + 1);
		drawInformation.specialCount = displayName.size();
		// drawInformation.color = Vector3(0.0f, 0.5f, 1.0f);

		return true;
	}

	return false;
}

const bool StoryInterpreter::handleInteraction(String& input) {
	if (input.substr(0, 2) == "\\<") {
		std::vector<String> commands;

		int i = 0, j = 0;
		for (; i < input.size() && (input[i] != '>' || (i + 1 < input.size() && input[i + 1] != ' ')); i++) {
			if (input[i] == '|') {
				commands.push_back(input.substr(j, i - j));
				j = i + 1;
			}
		}
		if (i < input.size() && input[i] == '>')
			commands.push_back(input.substr(j, i - j));

		int k = 0;
		while (k < commands.size() && isCharacterCondition(commands[k]))
			k++;

		for (int l = k + 1; l < commands.size() - 1; l++)
			runStatement(commands[l]);

		return gotoLine(commands[commands.size() - 1]);
	}

	if (input.substr(0, 3) == "\\~[") {
		int depth = 0;
		int i = 0;
		for (; i < input.size() && input[i] != '|' && (input[i] != ']' || depth > 0); i++) {
			if (input[i] == '[')
				depth++;
			else if (input[i] == ']')
				depth--;
		}

		if (i < input.size() && input[i] == '|') {
			input = "\\~[" + input.substr(i + 1);
		}
	}

	return false;
}

const Vector3 StoryInterpreter::getDrawPosition(const float xIndex, const float yIndex) const {
	return Vector3(
			   xIndex * spacing * scale * m_relativeScale,
			   yIndex * lineSpacing * scale * m_relativeScale, 0) +
		   offset;
}

const bool StoryInterpreter::getCharacterConditionArguments(
	const String& condition,
	String& name, String& expression, String& value) const {
	name = "", expression = "", value = "";

	int target = 0;
	for (int i = 0; i < condition.size(); i++) {
		if (condition[i] == ' ')
			return false;

		const char character = condition[i];
		switch (target) {
			case 0: {
				if (('a' <= character && character <= 'z') ||
					('A' <= character && character <= 'Z') ||
					('0' <= character && character <= '9') ||
					character == '_') {
					name += character;
				} else {
					target = 1;
					i--;
				}
			} break;

			case 1: {
				if (character == '?' || character == '<' || character == '>' || character == '=') {
					expression += character;
				} else {
					target = 2;
					i--;
				}
			} break;

			case 2: {
				value += character;
			} break;
		}
	}

	return true;
}
const bool StoryInterpreter::isCharacterCondition(const String& condition) const {
	String name, expression, value;
	const bool valid = getCharacterConditionArguments(condition, name, expression, value);

	return valid && name.size() > 0 &&
		   expression.size() > 0 && (value.size() > 0 || expression == "?");
}

const bool StoryInterpreter::characterConditionMet(const String& condition) const {
	if (!isCharacterCondition(condition))
		return false;

	String name, expression, value;
	getCharacterConditionArguments(condition, name, expression, value);

	int nameValue = 0, valueValue = 0;

	{ // Evaluate name.
		nameValue = getFlag(name);
	}

	{ // Evaluate value.
		bool negative = false;
		const char* vstr = value.c_str();
		if (*vstr == '-') {
			negative = true;
			vstr++;
		}
		while ('0' <= *vstr && *vstr <= '9') {
			valueValue *= 10;
			valueValue += int(*vstr - '0');
			vstr++;
		}
		if (negative)
			valueValue *= -1;

		if (*vstr != '\0') {
			printf("Invalid value string '%s' entered in condition '%s'\n", value.c_str(), condition.c_str());
			return false;
		}
	}

	if (expression == "?") {
		// This is technically inverted, but its for Markus so its fine <3.
		return m_flags.find(name) == m_flags.end();
	} else if (expression == "==" || expression == "=") {
		return nameValue == valueValue;
	} else if (expression == ">=") {
		return nameValue >= valueValue;
	} else if (expression == ">") {
		return nameValue > valueValue;
	} else if (expression == "<=") {
		return nameValue <= valueValue;
	} else if (expression == "<") {
		return nameValue < valueValue;
	}

	printf("Invalid expression '%s' entered in condition '%s'\n", expression.c_str(), condition.c_str());
	return false;
}

const bool StoryInterpreter::getStatementArguments(
	const String& condition,
	String& name, String& expression, String& value) const {
	name = "", expression = "", value = "";

	int target = 0;
	for (int i = 0; i < condition.size(); i++) {
		if (condition[i] == ' ')
			return false;

		const char character = condition[i];
		switch (target) {
			case 0: {
				if (('a' <= character && character <= 'z') ||
					('A' <= character && character <= 'Z') ||
					('0' <= character && character <= '9') ||
					(character == '_')) {
					name += character;
				} else {
					target = 1;
					i--;
				}
			} break;

			case 1: {
				if (character == '=' || character == '+' || character == '-') {
					expression += character;
				} else {
					target = 2;
					i--;
				}
			} break;

			case 2: {
				value += character;
			} break;
		}
	}

	return true;
}
const bool StoryInterpreter::isStatement(const String& condition) const {
	String name, expression, value;
	const bool valid = getStatementArguments(condition, name, expression, value);

	return valid && name.size() > 0 &&
		   expression.size() > 0 && value.size() > 0;
}
const bool StoryInterpreter::runStatement(const String& condition) {
	if (!isStatement(condition))
		return false;

	String name, expression, value;
	const bool valid = getStatementArguments(condition, name, expression, value);

	int nameValue = 0, valueValue = 0;

	{ // Evaluate name.
		nameValue = getFlag(name);
	}

	{ // Evaluate value.
		bool negative = false;
		const char* vstr = value.c_str();
		if (*vstr == '-') {
			negative = true;
			vstr++;
		}
		while ('0' <= *vstr && *vstr <= '9') {
			valueValue *= 10;
			valueValue += int(*vstr - '0');
			vstr++;
		}
		if (negative)
			valueValue *= -1;

		if (*vstr != '\0') {
			printf("Invalid value string '%s' entered in condition '%s'\n", value.c_str(), condition.c_str());
			return false;
		}
	}

	if (expression == "?") {
		return m_flags.find(name) != m_flags.end();
	} else if (expression == "=") {
		setFlag(name, valueValue);
		return true;
	} else if (expression == "+") {
		setFlag(name, nameValue + valueValue);
		return true;
	} else if (expression == "-") {
		setFlag(name, nameValue - valueValue);
		return true;
	}

	printf("Invalid expression '%s' entered in condition '%s'\n", expression.c_str(), condition.c_str());
	return false;
}