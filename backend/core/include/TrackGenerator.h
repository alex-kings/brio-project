#pragma once
#include "Piece.h"
#include <json/json.h>

/**
 * Generates tracks for a given set of pieces.
*/


/**
 * Returns a track 
*/
std::vector<Piece> generateTrack(const Json::Value& selection);