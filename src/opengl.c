#include "opengl.h"

#include <stdlib.h>


FH_API s8 fh_gl_init(void)
{
	printf("Initialize OpenGL\n");

	/* 
	 * Request opengl 4.6 context.
	 * SDL doesn't have the ability to choose which profile at this time of writing,
	 * but it should default to the core profile.
	 */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	return 0;
}


FH_API struct fh_context *fh_gl_create(struct fh_window *win)
{
	struct fh_context *ctx;

	if(!win) {
		ALARM(ALARM_ERR, "Input parameters invalid");
		goto err_return;
	}

	/* Allocate memory for the new context */
	if(!(ctx = smalloc(sizeof(struct fh_context)))) {
		ALARM(ALARM_ERR, "Failed to allocate memory for new glcontext");
		goto err_return;
	}

	if(!(ctx->context = SDL_GL_CreateContext(win->handle))) {
		ALARM(ALARM_ERR, "Failed to create GL context");
		goto err_return;
	}


	glClearColor(1, 1, 1, 1.0);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	return ctx;

err_return:
	ALARM(ALARM_ERR, "Failed to create GL context");
	return NULL;
}


FH_API void fh_gl_destroy(struct fh_context *ctx)
{
	if(!ctx) {
		ALARM(ALARM_WARN, "Input parameters invalid");
		return;
	}


	SDL_GL_DeleteContext(ctx->context);

	sfree(ctx);
}

FH_API void fh_gl_print_info(void)
{
	const unsigned char *ver;
	const unsigned char *vendor;
	const unsigned char *gpu;
	const unsigned char *glsl_ver;
	
	ver = glGetString(GL_VERSION);
	vendor = glGetString(GL_VENDOR);
	gpu = glGetString(GL_RENDERER);
	glsl_ver = glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("------------------ OpenGL Info -------------------\n");
	printf("GL Version: %s\n", ver);
	printf("Vendor: %s\n", vendor);
	printf("GPU: %s\n", gpu);
	printf("GLSL Version: %s\n", glsl_ver);
	printf("--------------------------------------------------\n");
}

