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
const Json::Value& readJson(const std::string& rawJson);


/**
 * Returns a Json Value representation of the pieces stored as ressources.
 * May throw domain error exception.
*/
const Json::Value& getPieceRessources();

