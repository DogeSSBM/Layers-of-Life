#pragma once



typedef enum{E_NONE, E_MERGE, E_EXIT}Event;
Event events()
{
	static SDL_Event e;
	while(SDL_PollEvent(&e)){
		switch(e.type){
		case SDL_QUIT:
			printf("\nQuitting now!\n");
			exit(0);
			break;
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym){
			case SDLK_ESCAPE:
				printf("Escape pressed, cleaning up before exit.\n");
				return E_EXIT;
				break;
			case SDLK_SPACE:
				printf("Merging all grids down\n");
				return E_MERGE;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	return E_NONE;
}
