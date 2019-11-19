#include "Includes.h"

void parseArgs(int argc, char const *argv[], uint* numLayers, uint* gridx,
	uint* gridy, uint* scale, Rule* preferredRules, float* randomThresh)
{
	for(uint i = 1; i < argc; i++){
		if(strncmp(argv[i], "numLayers=", 10)==0){
			if(sscanf(argv[i], "numLayers=%u", numLayers)!=1){
				printf("the numLayers arg is borked\n");
				exit(1);
			}else{
				printf("numLayers=%u\n", *numLayers);
			}
		}
		if(strncmp(argv[i], "gridx=", 6)==0){
			if(sscanf(argv[i], "gridx=%u", gridx)!=1){
				printf("the gridx arg is borked\n");
				exit(1);
			}else{
				printf("gridx=%u\n", *gridx);
			}
		}
		if(strncmp(argv[i], "gridy=", 6)==0){
			if(sscanf(argv[i], "gridy=%u", gridy)!=1){
				printf("the gridy arg is borked\n");
				exit(1);
			}else{
				printf("gridy=%u\n", *gridy);
			}
		}
		if(strncmp(argv[i], "scale=", 6)==0){
			if(sscanf(argv[i], "scale=%u", scale)!=1){
				printf("the scale arg is borked\n");
				exit(1);
			}else{
				printf("scale=%u\n", *scale);
			}
		}
		if(strncmp(argv[i], "init=", 5)==0){
			if(sscanf(argv[i], "init=%f", randomThresh)!=1){
				printf("the init arg is borked\n");
				exit(1);
			}else{
				printf("init=%u\n", *scale);
			}
		}
		if(strncmp(argv[i], "rules=", 6)==0){
			char ruleset[32];
			if(sscanf(argv[i], "rules=%s", ruleset)!=1){
				printf("the rules arg is borked\n");
				exit(1);
			}else{
				if (!strcmp(ruleset, "original")) {
					*preferredRules = originalRules;
				} else if (!strcmp(ruleset, "highLife")) {
					*preferredRules = highLifeRules;
				} else if (!strcmp(ruleset, "seeds")) {
					*preferredRules = seedsRules;
				} else if (!strcmp(ruleset, "diamoeba")) {
					*preferredRules = diamoebaRules;
				} else if (!strcmp(ruleset, "cellular")) {
					*preferredRules = cellularRules;
				} else {
					printf("unknown ruleset: %s\n", ruleset); exit(1);
				}
				printf("rules=%s\n", ruleset);
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	uint numLayers = NUMLAYERS_DEF,
		gridx = GRIDX_DEF,
		gridy = GRIDY_DEF,
		scale = SCALE_DEF;
	Rule preferredRules = RULEDEF;
	float randomThresh = 0.33;
	parseArgs(argc, argv, &numLayers,&gridx,&gridy,&scale,&preferredRules,&randomThresh);

	gfx_init(gridx*scale, gridy*scale);

	Layer **layerArr = mallocLayers(numLayers, gridx, gridy, scale,
		RED,BLUE,GREEN,WHITE);
	//layerArr[0]->rule = diamoebaRules;
	for (int i = 0; i < numLayers; i++)
		layerArr[i]->rule = preferredRules;

	randomizeLayers(layerArr, numLayers, (int)(100 * randomThresh));

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
					layersMergeDown1(layerArr, numLayers);
					mergeWait = 2;
				}
			default:
				if(elapsedTime()<66)
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
