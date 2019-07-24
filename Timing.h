#pragma once

/*
clock_t ClockPerMs = CLOCKS_PER_SEC / 1000;

clock_t refTime;

void resetTime()
{
	refTime = clock();
}

ull elapsedTime()
{
	return (ull)((clock() - refTime)/ClockPerMs);
}
*/

u32 refTime;

void resetTime(void)
{
	refTime = SDL_GetTicks();
}

u32 elapsedTime(void)
{
	return SDL_GetTicks() - refTime;
}
