#include <stdlib.h>

#include "glad/glad.h"
#include "stb_image.h"

#include "render/texture.h"
#include "log.h"

struct TexturePrivate {
	unsigned int render_ID;
};

static inline struct TexturePrivate *get_private(Texture *texture)
{
	return texture->priv;
}

Texture *texture_alloc(void)
{
	Texture *texture;
	struct TexturePrivate *priv;
	
	texture = calloc(1, sizeof(*texture));
	priv = get_private(texture);
	priv = calloc(1, sizeof(*priv));
	texture->priv = priv;
	
	return texture;
}

void texture_set_data(Texture *texture, const unsigned char *data, size_t length)
{
	struct TexturePrivate *priv = get_private(texture);
	int channels;
	unsigned char *buffer;

	buffer = stbi_load_from_memory(data,
				       length,
				       &texture->width,
				       &texture->height,
				       &channels,
				       4);

	if(!buffer) {
		WARN("Failed to load image data from file!");
		return;
	}

	glGenTextures(1, &priv->render_ID);

	texture_bind(texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, GL_RGBA,
						GL_UNSIGNED_BYTE, buffer);
	texture_unbind();

	free(buffer);
}

void texture_bind(Texture *texture)
{
	struct TexturePrivate *priv = get_private(texture);
	glBindTexture(GL_TEXTURE_2D, priv->render_ID);
}

void texture_unbind(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_free(Texture *texture)
{
	struct TexturePrivate *priv = get_private(texture);
	glDeleteTextures(1, &priv->render_ID);
	free(texture->priv);
	free(texture);
}
