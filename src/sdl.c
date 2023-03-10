#include "sdl.h"

#include "core.h"
#include "opengl.h"

#include <stdlib.h>


FH_API s8 fh_sdl_init(void)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		ALARM(ALARM_ERR, "Failed to initialize SDL");
		ALARM(ALARM_ERR, SDL_GetError());
		goto err_return;
	}

	if(TTF_Init() < 0) {
		ALARM(ALARM_ERR, "Failed to initialize TTF");
		goto err_quit_sdl;
	}
	
	return 0;

err_quit_sdl:
	SDL_Quit();

err_return:
	ALARM(ALARM_ERR, "Failed to initialize SDL module");
	return -1;
}


FH_API void fh_sdl_quit(void)
{
	if(TTF_WasInit())
		TTF_Quit();

	SDL_Quit();
}


FH_API void fh_sdl_print_info(void)
{
	SDL_version sdl_comp;
	SDL_version sdl_link;
	SDL_version img_comp;
	const SDL_version *img_link;
	SDL_version ttf_comp;

	/* Get the TTF version */
	const SDL_version *ttf_link = TTF_Linked_Version();
	SDL_TTF_VERSION(&ttf_comp);

	/* Get the Image version */
	img_link = IMG_Linked_Version();
	SDL_IMAGE_VERSION(&img_comp);

	/* Get the SDL version */
	SDL_VERSION(&sdl_comp);
	SDL_GetVersion(&sdl_link);

	printf("-------------------- SDL Info --------------------\n");
	printf("SDL comp. Version: %d.%d.%d\n", 
			sdl_comp.major, sdl_comp.minor, sdl_comp.patch);
	printf("SDL link. Version: %d.%d.%d\n", 
			sdl_link.major, sdl_link.minor, sdl_link.patch);

	printf("IMG comp. Version: %d.%d.%d\n",
			img_comp.major, img_comp.minor, img_comp.patch);
	printf("IMG link. Version: %d.%d.%d\n",
			img_link->major, img_link->minor, img_link->patch);

	printf("TTF comp. Version: %d.%d.%d\n", 
			ttf_comp.major, ttf_comp.minor, ttf_comp.patch);
	printf("TTF link. Version: %d.%d.%d\n", 
			ttf_link->major, ttf_link->minor, ttf_link->patch);
	printf("--------------------------------------------------\n");
}

