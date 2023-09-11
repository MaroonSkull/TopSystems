#pragma once

#include <glad/gl.h>
#include <Shader.hpp>
#include <set>

class GLProgram {
private:
	// множество шейдеров, из которых будем собирать программу
	std::set<const Shader*, Shader::cmp> shadersList_;
	GLuint programId_{};
public:
	GLProgram();
	~GLProgram();

	void attachShader(const Shader*);
	void linkProgram() const;
	void useProgram() const;
};