#ifndef __RENDER_SHADER_H_
#define __RENDER_SHADER_H_

struct _Shader;

typedef struct _Shader Shader;

Shader *shader_alloc(void);
void shader_set_data(Shader *shader, const char *vert, const char *frag);
void shader_bind(Shader *shader);
void shader_unbind(void);
void shader_set_mat4(Shader *shader, const char *name, float *value);

#endif
