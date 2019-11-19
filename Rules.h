#pragma once

// Individual layer rules

bool originalRules(bool cell, uint liveNeighbours)
{
	return cell ? liveNeighbours == 2 || liveNeighbours == 3 : liveNeighbours == 3;
}

bool highLifeRules(bool cell, uint liveNeighbours)
{
	return cell ? liveNeighbours == 2 || liveNeighbours == 3 : liveNeighbours==3 || liveNeighbours==6;
}

bool seedsRules(bool cell, uint liveNeighbours)
{
	return !cell && liveNeighbours == 2;
}

bool diamoebaRules(bool cell, uint liveNeighbours)
{
	return cell? liveNeighbours == 3 || liveNeighbours > 4: liveNeighbours >4;
}
