#include "Includes.h"

uint gridx = 100, gridy = 60, scale = 16;
uint winx, winy;

Grid grids[3];
Grid nextGrids[3];

Grid mallocGrid()
{
	Grid ret = malloc(sizeof(bool*)*gridx);
	for(uint x = 0; x < gridx; x++) {
		ret[x] = malloc(sizeof(bool)*gridy);
		memset(ret[x], 0, sizeof(bool)*gridy);
	}
	return ret;
}

Grid freeGrid(Grid grid)
{
	if(grid == NULL)
		return grid;
	for(uint x = 0; x < gridx; x++) {
		if(grid[x] == NULL)
			continue;
		free(grid[x]);
		grid[x] = NULL;
	}
	free(grid);
	grid == NULL;
	return grid;
}

Grid randomizeGrid(Grid grid, uint chanceOfLife/*0-100*/)
{
	for(uint y = 0; y < gridy; y++){
		for(uint x = 0; x < gridx; x++){
			grid[x][y] = rand() % (100 + 1) <= chanceOfLife;
		}
	}
	return grid;
}

uint getTotalLive(Grid grid)
{
	uint live = 0;
	for(uint y = 0; y < gridy; y++) {
		for(uint x = 0; x < gridx; x++) {
			live += grid[x][y];
		}
	}
	return live;
}

void drawGrid(Grid grid, Color liveColor)
{
	for(uint y = 0; y < gridy; y++) {
		for(uint x = 0; x < gridx; x++) {
			if(grid[x][y]){
				setColor(liveColor);
				fillSquare(x*scale, y*scale, scale);
			}
			setColor(BLACK);
			fillBorder(x*scale, y*scale, scale, scale, -1);
		}
	}
}

uint wrapX(int x)
{
	uint ret = (uint)x;
	if(x >= gridx)
		ret = x - gridx;
	if (x < 0)
		ret = gridx + x;
	return ret;
}

uint wrapY(int y)
{
	uint ret = (uint)y;
	if(y >= gridy)
		ret = y - gridy;
	if(y < 0)
		ret = gridy + y;
	return ret;
}

uint getLiveNeighbors(Grid grid, uint x, uint y)
{
	uint liveNeighbors = 0;
	uint posx, posy;
	for(int xoffset = -1; xoffset <= 1; xoffset++) {
		for(int yoffset = -1; yoffset <= 1; yoffset++) {
			posx = wrapX(x + xoffset);
			posy = wrapY(y + yoffset);
			if(grid[posx][posy])
				liveNeighbors++;
		}
	}
	if(grid[x][y])
		liveNeighbors--;
	return liveNeighbors;
}

void mergeDown(Grid grid1, Grid grid2)
{
	for(uint y = 0; y < gridy; y++) {
		for(uint x = 0; x < gridx; x++) {
			// grid1[x][y] or equals grid2[x][y]
			grid1[x][y] |= grid2[x][y];
		}
	}
}

void mergeAll(Grid *grids, uint numGrids)
{
	static Grid temp = NULL;
	if(temp == NULL)
		temp = mallocGrid();
	for(uint x = 0; x < gridx; x++) {
		memcpy(temp[x], (grids[0])[x], sizeof(bool)*gridy);
	}

	for(uint i = 0; i < numGrids; i++){
		mergeDown(grids[i], i<numGrids-1? grids[i+1]:temp);
	}
}

int main(int argc, char *argv[])
{
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
	winx = scale*gridx;
	winy = scale*gridy;
	gfx_init(winx, winy);
	printf("grid: (%u, %u)\n", gridx, gridy);
	const static Color colors[3] = {
		{255, 0, 0, 85},
		{0, 255, 0, 85},
		{0, 0, 255, 85}
	};
	for(uint i = 0; i < 3; i++) {
		grids[i] = mallocGrid();
		nextGrids[i] = mallocGrid();
		randomizeGrid(grids[i], 33);
		//memcpy(nextGrids[i], grids[i], sizeof(bool) * gridx * gridy);
		uint total = gridx * gridy;
		uint alive = getTotalLive(grids[i]);
		uint dead = total - alive;
		float percentage = ((float)alive / (float)total)*100.0f;
		printf("Grid %u-\n\talive: %u\n\tdead: %u\n\tpercentage: %.2f\n",
			i, alive, dead, percentage);
		printf("\tAlive r: %u\n", colors[i].r);
		printf("\tAlive g: %u\n", colors[i].g);
		printf("\tAlive b: %u\n", colors[i].b);
		printf("\tAlive a: %u\n", colors[i].a);
		drawGrid(grids[i], colors[i]);
	}

	/*
	state	|	neighbors	|	outcome
	------|-----------------|-------------
	live	|	< 2		|	dead
	live	|	> 3		|	dead
	dead	|	3		|	live
	*/

	while(1) {
		for(uint i = 0; i < 3; i++) {
			Grid grid = grids[i];
			Grid next = nextGrids[i];
			for(uint y = 0; y < gridy; y++) {
				for(uint x = 0; x < gridx; x++) {
					switch (events()) {
						case E_MERGE:
							mergeAll(grids, 3);
							break;
						default:
							break;
					}
					uint liveNeighbors = getLiveNeighbors(grid, x, y);
					next[x][y] = grid[x][y];
					if(grid[x][y] && (liveNeighbors < 2 || liveNeighbors > 3))
							next[x][y] = false;
					if(!grid[x][y] && liveNeighbors == 3)
						next[x][y] = true;
				}
			}
			Color c = rgbaToColor(255 * (i == 0), 255 * (i == 1), 255 * (i == 2), 85);
			grids[i] = next;
			nextGrids[i] = grid;
			drawGrid(grid, c);
		}
		draw();
		clear();
	}

	return 0;
}
