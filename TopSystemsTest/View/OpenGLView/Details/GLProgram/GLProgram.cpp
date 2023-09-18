#include <GLProgram.hpp>
#include <sstream>


GLProgram::GLProgram() {
	programId_ = glCreateProgram();
}

GLProgram::~GLProgram() {
	glDeleteProgram(programId_);
}

void GLProgram::attachShader(const Shader* ShaderObject) {
	if (!shadersList_.insert(ShaderObject).second) {
		std::stringstream err;
		err << static_cast<int>(ShaderObject->getShaderType())
			<< " type shader is already attached!";
		throw std::runtime_error{ err.str() };
	}
}

void GLProgram::linkProgram() const {
	for (const auto& ShaderObject : shadersList_)
		glAttachShader(programId_, ShaderObject->getShaderId());

	glLinkProgram(programId_);

	// Check shader program linking errors
	GLint success;
	GLint InfoLogLength;
	glGetProgramiv(programId_, GL_LINK_STATUS, &success);
	glGetProgramiv(programId_, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::string infoLog{};
	infoLog.resize(static_cast<size_t>(InfoLogLength+1)); // для \0
	if (!success) {
		glGetProgramInfoLog(programId_, InfoLogLength, nullptr, &infoLog[0]);
		std::stringstream err;
		err << programId_
			<< " linking failed: " << std::endl
			<< infoLog;
		throw std::runtime_error{ err.str() };
	}

	// todo тут пора бы glDeleteShader() по идее
	// https://stackoverflow.com/questions/9113154/proper-way-to-delete-glsl-shader
	// как потом работать с UBO?
}

void GLProgram::useProgram() const {
	glUseProgram(programId_);
}

std::optional<const Shader *>GLProgram::getShader(Shader::type desiredShaderType) const {
	for(auto &ConcreteShader : shadersList_)
		if(ConcreteShader->getShaderType() == desiredShaderType)
			return ConcreteShader;
	return std::nullopt;
}
