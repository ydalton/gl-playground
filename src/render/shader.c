#include <stdlib.h>
#include <assert.h>

#include "glad/glad.h"
#include "render/shader.h"
#include "log.h"

struct _Shader {
	unsigned int render_ID;
};

static unsigned int compile_shader(const char *src, int type)
{
	unsigned int shader;
	int compile_ok;

	shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	if(!compile_ok) {
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

static unsigned int create_program(const char *vert_src, const char *frag_src)
{
	unsigned int program, frag_shader, vert_shader;
	int link_ok;
	char *log;

	vert_shader = compile_shader(vert_src, GL_VERTEX_SHADER);
	if(!vert_shader) {
		WARN("Failed to compile vertex shader!");
		return 0;
	}

	frag_shader = compile_shader(frag_src, GL_FRAGMENT_SHADER);
	if(!frag_shader) {
		WARN("Failed to compile fragment shader!");
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, frag_shader);
	glAttachShader(program, vert_shader);
	glLinkProgram(program);

	log = (char *) calloc(512, sizeof(char));

	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(!link_ok) {
		glGetProgramInfoLog(program, 512, NULL, log);
		WARN("Failed to link shader: %s", log);
		glDeleteProgram(program);
		program = 0;
	}

	free(log);
	
	glDeleteShader(frag_shader);
	glDeleteShader(vert_shader);

	return program;
}

Shader *shader_alloc(void)
{
	Shader *shader;

	shader = calloc(1, sizeof(*shader));

	return shader;
}

void shader_set_data(Shader *shader, const char *vert, const char *frag)
{
	unsigned int ret;

	assert(!shader->render_ID);
	ret = create_program(vert, frag);
	shader->render_ID = ret;
}

static int find_location(int prog, const char *name)
{
	int location = glGetUniformLocation(prog, name);
	if(location == -1)
		WARN("Could not find location %s\n", name);
	return location;
}

void shader_bind(Shader *shader)
{
	glUseProgram(shader->render_ID);
}

void shader_unbind(void)
{
	glUseProgram(0);
}

void shader_set_mat4(Shader *shader, const char *name, float *value)
{
	glUniformMatrix4fv(find_location(shader->render_ID, name), 1, GL_FALSE, value);
}
