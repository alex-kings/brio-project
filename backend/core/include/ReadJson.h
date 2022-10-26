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
 * Returns a Json Value representation of the pieces stored as ressources.
 * May throw domain error exception.
*/
Json::Value getPieceRessources(Json::Value piecesJson);

