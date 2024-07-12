#ifndef __RENDER_RENDER_H_
#define __RENDER_RENDER_H_

#include "render/render-object.h"
#include "render/texture.h"
#include "render/shader.h"

int renderer_create(void);
void renderer_draw(RenderObject *obj, Shader *shader, Texture *texture);
void renderer_clear(void);
void renderer_destroy(void);

#endif
