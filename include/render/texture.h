#ifndef __RENDER_TEXTURE_H_
#define __RENDER_TEXTURE_H_

#include <stddef.h>

struct _Texture {
	int width;
	int height;
	int bpp;

	void *priv;
};

typedef struct _Texture Texture;

Texture *texture_alloc(void);
void texture_set_data(Texture *texture, const unsigned char *data, size_t length);
void texture_bind(Texture *texture);
void texture_unbind(void);
void texture_free(Texture *texture);

#endif
