#include "shader.hpp"

Shader::Shader(const String& shaderPath) :
	Shader(File::readAll(shaderPath + ".vs"), File::readAll(shaderPath + ".fs")) {}

Shader::Shader(const String& vertexShaderSource, const String& fragmentShaderSource) :
	m_program(0),
	m_attributes(), m_uniforms(),
	m_instanceAttributesTotalSize(0), m_staticAttributesTotalSize(0) {
	m_program = compileProgram(vertexShaderSource, fragmentShaderSource);
	loadAttributes();
	loadUniforms();
}
Shader::~Shader() {
	if (m_program) {
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

const GLuint Shader::compileShader(const String& source, const GLenum shaderType) {
	if (source.size() == 0)
		BEING_ERROR("No shader source.");

	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
		BEING_ERROR("Failed to create shader.");

	String correctedSource = source;
#ifdef __EMSCRIPTEN__

	std::map<String, String> replacementTree;
	replacementTree.emplace("#version 460 core", "#version 300 es");
	if (shaderType == GL_FRAGMENT_SHADER) {
		replacementTree.emplace("out float", "out mediump float");
		replacementTree.emplace("out vec2", "out mediump vec2");
		replacementTree.emplace("out vec3", "out mediump vec3");
		replacementTree.emplace("out vec4", "out mediump vec4");
		replacementTree.emplace("in float", "in mediump float");
		replacementTree.emplace("in vec2", "in mediump vec2");
		replacementTree.emplace("in vec3", "in mediump vec3");
		replacementTree.emplace("in vec4", "in mediump vec4");
		replacementTree.emplace("\tfloat", "\tmediump float");
		replacementTree.emplace("\tvec2", "\tmediump vec2");
		replacementTree.emplace("\tvec3", "\tmediump vec3");
		replacementTree.emplace("\tvec4", "\tmediump vec4");
		replacementTree.emplace("texture2D", "texture");
	}

	for (int i = correctedSource.size() - 1; i >= 0; i--) {
		for (const auto& it : replacementTree) {
			if (i + it.first.size() > correctedSource.size() || correctedSource.substr(i, it.first.size()) != it.first)
				continue;
			correctedSource.replace(i, it.first.size(), it.second);
		}
	}
#endif

const char* cSource = correctedSource.c_str();
GLint len = correctedSource.size();
glShaderSource(shader, 1, &cSource, &len);
glCompileShader(shader);

int success;
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) {
	char infoLog[512];
	glGetShaderInfoLog(shader, 512, NULL, infoLog);
	printf("%s\n", infoLog);

	printf("%s\n", correctedSource.c_str());

	switch (shaderType) {
		case GL_VERTEX_SHADER: {
			BEING_ERROR("Vertex shader compilation failed.");
		} break;
		case GL_GEOMETRY_SHADER: {
			BEING_ERROR("Geometry shader compilation failed.");
		} break;
		case GL_FRAGMENT_SHADER: {
			BEING_ERROR("Fragment shader compilation failed.");
		} break;
	}
};

return shader;
}
const GLuint Shader::compileProgram(const String& vertexSource, const String& fragmentSource) {
	const GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
	if (vertexShader == 0)
		BEING_ERROR("Failed to compile vertex shader.");

	const GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		BEING_ERROR("Failed to compile fragment shader.");
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("%s\n", infoLog);

		BEING_ERROR("Program link failed.");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Shader::loadAttributes() {
	m_attributes.clear();
	m_instanceAttributesTotalSize = 0;
	m_staticAttributesTotalSize = 0;

	GLint attributeCount, maxNameSize;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &attributeCount);
	if (attributeCount <= 0)
		return;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameSize);
	maxNameSize += 1;

	char* nameBuffer = new char[maxNameSize];
	GLint nameLength, attributeSize, attributeLocation;
	GLenum attributeType;

	for (size_t i = 0; i < attributeCount; i++) {
		glGetActiveAttrib(
			m_program, i,
			maxNameSize, &nameLength,
			&attributeSize, &attributeType, nameBuffer);
		attributeLocation = glGetAttribLocation(m_program, nameBuffer);

		bool staticAttribute =
			nameLength >= 2 &&
			nameBuffer[0] == 'v' &&
			nameBuffer[1] == '_';

		m_attributes.emplace(
			String(nameBuffer),
			(ShaderAttribute){
				.m_location = attributeLocation,
				.m_glType = attributeType,
				.m_dataSize = glGetTypeByteSize(attributeType),
				.m_dataOffset = staticAttribute ? m_staticAttributesTotalSize : m_instanceAttributesTotalSize,
				.m_isStatic = staticAttribute,
			});

		const size_t dataSize = glGetTypeByteSize(attributeType);
		if (staticAttribute) {
			m_staticAttributesTotalSize += dataSize;
		} else {
			m_instanceAttributesTotalSize += dataSize;
		}
	}

	delete[] nameBuffer;
}
void Shader::loadUniforms() {
	m_uniforms.clear();

	GLint uniformCount, maxNameSize;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);
	if (uniformCount <= 0)
		return;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
	maxNameSize += 1;

	char* nameBuffer = new char[maxNameSize];
	GLint nameLength, uniformSize, uniformLocation;
	GLenum uniformType;

	for (size_t i = 0; i < uniformCount; i++) {
		glGetActiveUniform(
			m_program, i,
			maxNameSize, &nameLength,
			&uniformSize, &uniformType, nameBuffer);
		uniformLocation = glGetUniformLocation(m_program, nameBuffer);

		m_uniforms.emplace(
			String(nameBuffer),
			(ShaderUniform){
				.m_location = uniformLocation,
				.m_glType = uniformType,
			});
	}

	delete[] nameBuffer;
}