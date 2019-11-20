#pragma once

#define GRIDX_DEF		100
#define GRIDY_DEF		60
#define SCALE_DEF		16
#define NUMLAYERS_DEF	3
#define RULEDEF		originalRules

typedef struct{
	uint x, y, xlen, ylen, scale;
	Color liveColor;
	Grid grid, next;
	Rule rule;
}Layer;

void freeGrid(Grid grid, uint xlen)
{
	if(grid == NULL)
		return;
	for(uint x = 0; x < xlen; x++) {
		free(grid[x]);
	}
	free(grid);
	grid = NULL;
}

Grid mallocGrid(uint xlen, uint ylen)
{
	Grid grid = malloc(sizeof(bool*)*xlen);
	for(uint x = 0; x < xlen; x++) {
		grid[x] = malloc(sizeof(bool)*ylen);
	}
	return grid;
}

void freeLayers(Layer** layerArr, uint numLayers)
{
	if(numLayers==0 || layerArr==NULL)
		return;
	for(uint i = 0; i < numLayers; i++){
		freeGrid(layerArr[i]->grid, layerArr[i]->xlen);
		free(layerArr[i]);
	}
	free(layerArr);
	layerArr = NULL;
}

Layer** mallocLayers(int numLayers, uint xlen, uint ylen, uint scale, ...)
{
	if(numLayers == 0)
		return NULL;

	Layer** layerArr = malloc(sizeof(Layer*)*numLayers);

	va_list colorList;
	va_start(colorList, scale);
	for(uint i = 0; i < numLayers; i++){
		layerArr[i] = malloc(sizeof(Layer));
		layerArr[i]->xlen = xlen;
		layerArr[i]->ylen = ylen;
		layerArr[i]->scale = scale;
		layerArr[i]->liveColor = va_arg(colorList, Color);
		layerArr[i]->grid = mallocGrid(layerArr[i]->xlen, layerArr[i]->ylen);
		layerArr[i]->next = mallocGrid(layerArr[i]->xlen, layerArr[i]->ylen);
	}
	va_end(colorList);
	return layerArr;
}

void randomizeLayers(Layer **layerArr, uint numLayers, uint chanceOfLife)
{
	for(uint i = 0; i < numLayers; i++){
		for(uint x = 0; x < layerArr[i]->xlen; x++){
			for(uint y = 0; y < layerArr[i]->ylen; y++){
				layerArr[i]->grid[x][y]=rand()%(100+1) <= chanceOfLife;
			}
		}
	}
}

void drawLayer(Layer *layer)
{
			SDL_SetRenderDrawBlendMode(gfx.renderer, SDL_BLENDMODE_ADD);
	for(uint y = 0; y < layer->ylen; y++) {
		for(uint x = 0; x < layer->xlen; x++) {
			if(layer->grid[x][y]){
				setColor(layer->liveColor);
				fillSquare(x*layer->scale, y*layer->scale, layer->scale);
			}
			// SDL_SetRenderDrawBlendMode(gfx.renderer, SDL_BLENDMODE_NONE);
			// setColor(BLACK);
			// fillBorder(x*layer->scale, y*layer->scale, layer->scale, layer->scale, -1);
			// SDL_SetRenderDrawBlendMode(gfx.renderer, SDL_BLENDMODE_ADD);
		}
	}
			SDL_SetRenderDrawBlendMode(gfx.renderer, SDL_BLENDMODE_NONE);
}

void drawLayers(Layer **layerArr, uint numLayers)
{
	clear();
	for(uint i = 0; i < numLayers; i++){
		drawLayer(layerArr[i]);
	}
}

uint wrapX(int x, uint xmax)
{
	uint ret = (uint)x;
	if(x >= xmax)
		ret = x - xmax;
	if (x < 0)
		ret = xmax + x;
	return ret;
}

uint wrapY(int y, uint ymax)
{
	uint ret = (uint)y;
	if(y >= ymax)
		ret = y - ymax;
	if(y < 0)
		ret = ymax + y;
	return ret;
}

uint layerGetNeighbours(Layer *layer, uint x, uint y)
{
	uint liveNeighbours = 0;
	for(int xoffset = -1; xoffset <= 1; xoffset++) {
		for(int yoffset = -1; yoffset <= 1; yoffset++) {
			if(layer->grid
			[wrapX(x + xoffset, layer->xlen)]
			[wrapY(y + yoffset, layer->ylen)])
				liveNeighbours++;
		}
	}
	liveNeighbours -= layer->grid[x][y];
	return liveNeighbours;
}

void layersApplyRules(Layer **layers, uint numLayers)
{
	for(uint i = 0; i < numLayers; i++){
		for(uint x = 0; x < layers[i]->xlen; x++){
			for(uint y = 0; y < layers[i]->ylen; y++){
				uint liveNeighbours = layerGetNeighbours(layers[i], x, y);
				layers[i]->next[x][y] = (*layers[i]->rule)(
					layers[i]->grid[x][y], liveNeighbours);
			}
		}
		for(uint x = 0; x < layers[i]->xlen; x++){
			for(uint y = 0; y < layers[i]->ylen; y++){
				layers[i]->grid[x][y] = layers[i]->next[x][y];
			}
		}
	}
}

// Inter-layer rules

void layersMergeDown1(Layer **layers, uint numLayers)
{
	for(uint i = 0; i < numLayers; i++){
		for(uint x = 0; x < layers[i]->xlen; x++){
			for(uint y = 0; y < layers[i]->ylen; y++){
				layers[i]->grid[x][y] |= layers[(i+1)%numLayers]->grid[x][y];
			}
		}
	}
}

void layersCopyToBottom(Layer **layers, uint numLayers)
{
	if(numLayers == 0)
		return;
	for(uint i = 1; i < numLayers; i++){
		for(uint x = 0; x < layers[i]->xlen; x++){
			for(uint y = 0; y < layers[i]->ylen; y++){
				layers[0]->grid[x][y] |= layers[i]->grid[x][y];
			}
		}
	}
}

void layersXorToBottom(Layer **layers, uint numLayers)
{
	if(numLayers != 3)
		return;
	for(uint x = 0; x < layers[0]->xlen; x++){
		for(uint y = 0; y < layers[0]->ylen; y++){				layers[0]->grid[x][y] |= layers[1]->grid[x][y] ^ layers[2]->grid[x][y];
		}
	}
}
