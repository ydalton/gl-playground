#ifndef __RENDER_RENDER_H_
#define __RENDER_RENDER_H_

#include "sephine/render/render-object.h"
#include "sephine/render/texture.h"
#include "sephine/render/shader.h"

int renderer_create(void);
void renderer_draw(RenderObject *obj, Shader *shader, Texture *texture);
void renderer_set_clear_color(float r, float g, float b, float a);
void renderer_clear(void);
void renderer_destroy(void);

#endif
