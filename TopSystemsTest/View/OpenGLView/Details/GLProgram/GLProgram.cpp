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
	int32_t success;
	char infoLog[512];
	glGetProgramiv(programId_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId_, 512, nullptr, infoLog);
		std::stringstream err;
		err << programId_
			<< " linking failed: " << std::endl
			<< infoLog;
		throw std::runtime_error{ err.str() };
	}

	// todo тут пора бы glDeleteShader() по идее
	// https://stackoverflow.com/questions/9113154/proper-way-to-delete-glsl-shader
}

void GLProgram::useProgram() const {
	glUseProgram(programId_);
}