#include "interpreter.hpp"

#include <beingEngine/systems/input.hpp>
#include <beingEngine/systems/time.hpp>

StoryInterpreter::StoryInterpreter() :
	m_characters(), m_flags(),
	m_currentLine(0),
	m_lines(), m_activeLines(),
	m_drawLine(0), m_drawCharacter(0), m_delay(0),
	m_buttonShader("shaders/button"), m_buttonUniforms(),
	m_buttonMesh(), m_buttonInstance() {
	readFile("story");

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

	const float buttonBaseSize = 0.5f;
	std::vector<Vector3> buttonVertices = {
		Vector3(-buttonBaseSize, -buttonBaseSize, 0),
		Vector3(buttonBaseSize, -buttonBaseSize, 0),
		Vector3(-buttonBaseSize, buttonBaseSize, 0),
		Vector3(buttonBaseSize, buttonBaseSize, 0),
	};
	std::vector<Vector2> buttonUVs = {
		Vector2(0, 1),
		Vector2(1, 1),
		Vector2(0, 0),
		Vector2(1, 0),
	};
	std::vector<uint16_t> buttonIndices = {
		0,
		1,
		2,
		2,
		1,
		3,
	};
	m_buttonMesh.setData("v_pos", buttonVertices, GL_FLOAT_VEC3);
	m_buttonMesh.setData("v_uv", buttonUVs, GL_FLOAT_VEC2);
	m_buttonMesh.setIndices(buttonIndices);
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
	if (m_delay > 0.0f) {
		m_delay -= Time::s_time->getDelta();
		return;
	}

	processInput();

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

		DrawInformation drawInformation;
		int lineSelectableOffset = 0;
		for (int j = 0; j < line.size() && j <= m_drawCharacter; j++) {
			String newLine = line.substr(j);
			const size_t newLineLength = newLine.size();

			if (!resolveSpecial(newLine, drawInformation)) {
				continue;
			}

			lineSelectableOffset += ((int)newLineLength - (int)newLine.size());
			line = line.substr(0, j) + newLine;
			j--;
		}

		if (m_drawCharacter < line.size() && m_drawCharacter + lineSelectableOffset < m_activeLines[m_drawLine].size()) {
			if (m_activeLines[m_drawLine][m_drawCharacter + lineSelectableOffset] == '.') {
				for (int i = m_drawCharacter + lineSelectableOffset; i >= 0; i--) {
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
static const float scale = 20.0f;
static const Vector2 offset(50.0f, 50.0f);

void StoryInterpreter::draw(Renderer& renderer, TextRenderer& textRenderer) {
	struct Selectable {
		int line;
		int lineStart;

		int drawStart;
		int width;

		DrawInformation drawInformation;
	};

	std::vector<Selectable> selectables;
	for (int i = 0; i < m_activeLines.size() && i <= m_drawLine; i++) {

		String line = m_activeLines[i];
		int lineSelectableOffset = 0;

		Vector3 defaultColor = Vector3(1.0f, 1.0f, 1.0f);

		for (int j = 0; j < line.size(); j++) {
			String newLine = line.substr(j);
			const size_t newLineLength = newLine.size();

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

			if (i == m_drawLine)
				drawInformation.specialCount = __min(drawInformation.specialCount + j, m_drawCharacter) - j;

			///////////////DRAW$FENCE//////////////////
			//   0   1   2   3   4   5   6   7
			// |---|---|---|---|---|---|---|---|

			if (drawInformation.specialCount > 0) {
				selectables.push_back((Selectable){
					.line = i,
					.lineStart = j + lineSelectableOffset,
					.drawStart = j,
					.width = drawInformation.specialCount,
					.drawInformation = drawInformation,
				});
			}

			if (!special)
				continue;

			lineSelectableOffset += ((int)newLineLength - (int)newLine.size());
			line = line.substr(0, j) + newLine;
			j--;
		}

		if (i == m_drawLine)
			line = line.substr(0, m_drawCharacter);
		// textRenderer.drawText(renderer, getDrawPosition(0, i), scale, line);

		for (int i = 0; i < selectables.size(); i++) {
			for (int j = i + 1; j < selectables.size(); j++) {
				const int iStart = selectables[i].drawStart, iEnd = iStart + selectables[i].width,
						  jStart = selectables[j].drawStart, jEnd = iStart + selectables[j].width;
				if (jStart >= iEnd)
					continue;

				if (jEnd > iEnd) {
					// Split.
					selectables.insert(
						selectables.begin() + j,
						(Selectable){
							.line = selectables[i].line,
							.lineStart = selectables[i].lineStart,
							.drawStart = selectables[i].drawStart,
							.width = iEnd - jEnd,
							.drawInformation = selectables[i].drawInformation,
						});
				}

				// Always shorten.
				selectables[i].width = jStart - iStart;
			}
		}

		bool alreadyInteracted = false;
		int lastDrawIndex = 0;
		for (auto it : selectables) {
			const int i = it.line;
			const int j = it.drawStart;
			const int specialCount = it.width;

			const Vector2 min(getDrawPosition((float)j - 0.5f, (float)i - 0.5f)), max(getDrawPosition((float)(j + specialCount) - 0.5f, (float)i + 0.5f));
			const Vector2 mpos = Input::s_input->getMousePosition();
			const bool mouseWithin = min.x() <= mpos.x() && mpos.x() <= max.x() && min.y() <= mpos.y() && mpos.y() <= max.y();

			String modifiableLine = m_activeLines[i].substr(it.lineStart);
			const bool interactable = it.drawInformation.enabled && !it.drawInformation.errorOccurred;
			const bool isInteracting = mouseWithin && interactable && !alreadyInteracted;
			const bool pressed = isInteracting && Input::s_input->isKeyPressed(GLFW_MOUSE_BUTTON_1);
			const bool used = isInteracting && Input::s_input->isKeyUp(GLFW_MOUSE_BUTTON_1);
			if (isInteracting)
				alreadyInteracted = true;

			Vector3 drawColor = it.drawInformation.color;
			if (it.drawInformation.errorOccurred) {
				drawColor = Vector3(1.0f, 0.0f, 1.0f);
			} else if (!interactable) {
			} else if (used) {
				drawColor = Vector3(1.0f, 1.0f, 1.0f);
			} else if (pressed) {
				drawColor = drawColor * Vector3(0.6f, 0.5f, 0.8);
			} else if (isInteracting) {
				drawColor = drawColor * Vector3(1.2f, 1.1f, 1.4);
			}

			const int drawIndex = (lastDrawIndex > j) ? j : lastDrawIndex;
			textRenderer.drawText(renderer, getDrawPosition(drawIndex, i), scale, line.substr(drawIndex, j - drawIndex), defaultColor);
			textRenderer.drawText(renderer, getDrawPosition(j, i), scale, line.substr(j, specialCount), drawColor);
			lastDrawIndex = j + specialCount;

			if (used) {
				if (handleInteraction(modifiableLine))
					return;
				m_activeLines[i] = m_activeLines[i].substr(0, it.lineStart) + modifiableLine;
			}
		}
		textRenderer.drawText(renderer, getDrawPosition(lastDrawIndex, i), scale, line.substr(lastDrawIndex, line.size() - lastDrawIndex), defaultColor);

		selectables.clear();
	}
}

void StoryInterpreter::readFile(const String& path) {
	const String story = File::readAll(path + ".txt");

	for (size_t i = 0; i < story.size(); i++) {
		String line = "";
		while (i < story.size() && story[i] != '\n')
			line += story[i++];
		m_lines.emplace_back(line);
	}
}

void StoryInterpreter::processInput() {
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

	if (input.substr(0, 2) == "\\<") {
		input = input.substr(2);

		std::vector<String> commands;

		int depth = 0;
		int i = 0, j = 0;
		bool spaceFound = false, closeFound = false;
		for (; i < input.size() && (input[i] != '>' || !(spaceFound || closeFound) || depth > 0); i++) {
			if (input[i] == ' ') {
				spaceFound = true;
			} else if (spaceFound && input[i] == '<') {
				depth++;
			} else if (spaceFound && input[i] == '>') {
				depth--;
			} else if (input[i] == '|') {
				commands.push_back(input.substr(j, i - j));
				j = i + 1;
				closeFound = true;
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

		int depth = 0;
		int i = 2, j = 2;
		bool spaceFound = false, closeFound = false;
		for (; i < input.size() && (input[i] != '>' || !(spaceFound || closeFound) || depth > 0); i++) {
			if (input[i] == ' ') {
				spaceFound = true;
			} else if (spaceFound && input[i] == '<') {
				depth++;
			} else if (spaceFound && input[i] == '>') {
				depth--;
			} else if (input[i] == '|') {
				commands.push_back(input.substr(j, i - j));
				j = i + 1;
				closeFound = true;
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

const Vector2 StoryInterpreter::getDrawPosition(const float xIndex, const float yIndex) const {
	return Vector2(xIndex * spacing * scale, yIndex * spacing * scale) + offset;
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
				if (('a' <= character && character <= 'z') || ('A' <= character && character <= 'Z') || ('0' <= character && character <= '9')) {
					name += character;
				} else {
					target = 1;
					i--;
				}
			} break;

			case 1: {
				if (character == '<' || character == '>' || character == '=') {
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
		   expression.size() > 0 && value.size() > 0;
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
		return m_flags.find(name) != m_flags.end();
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
				if (('a' <= character && character <= 'z') || ('A' <= character && character <= 'Z') || ('0' <= character && character <= '9')) {
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