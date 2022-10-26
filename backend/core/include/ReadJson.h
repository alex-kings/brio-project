#pragma once
#include <json/json.h>
#include <fstream>
#include "Piece.h"

/**
 * Read and parse JSON to useful data structures.
*/


/**
 * Return the Json object representation of a string input.
*/
Json::Value readJson(std::string rawJson);


/**
 * Get collection of pieces and their available quantities from the user selection.
 * This may throw a domain error exception.
*/
// std::vector<std::pair<Piece, int>> getPiecesAvailable(Json::Value piecesJson);
