#include "Includes.h"

int main(int argc, char const *argv[])
{
	uint gridx = GRIDX_DEF, gridy = GRIDY_DEF, scale = SCALE_DEF;
	switch (argc)
	{
	case 4:
		scale = strToInt(argv[3]);
	case 3:
		gridy = strToInt(argv[2]);
		gridx = strToInt(argv[1]);
		break;
	case 2:
		scale = strToInt(argv[1]);
		break;
	default:
		break;
	}

	gfx_init(gridx*scale, gridy*scale);
	uint numLayers = 3;
	Layer **layerArr = mallocLayers(numLayers, gridx, gridy, scale,
		RED,BLUE,GREEN);
	layerArr[0]->rule = diamoebaRules;
	layerArr[1]->rule = highLifeRules;
	layerArr[2]->rule = highLifeRules;

	randomizeLayers(layerArr, numLayers, 100/numLayers);

	drawLayers(layerArr, numLayers);
	resetTime();
	uint mergeWait = 0;
	while(1){
		switch (events()) {
			case E_EXIT:
				printf("Freeing layers.\n");
				freeLayers(layerArr, numLayers);
				printf("Exiting!\n");
				exit(0);
				break;
			case E_MERGE:
				if(mergeWait == 0){
					layersXorToBottom(layerArr, numLayers);
					mergeWait = 2;
				}
			default:
				if(elapsedTime()<100)
					break;
				layersApplyRules(layerArr, numLayers);
				mergeWait -= mergeWait>0;
				drawLayers(layerArr, numLayers);
				draw();
				resetTime();
				break;
		}
		delay(3);
	}
	return 0;
}
