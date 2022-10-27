#pragma once
#include "Piece.h"
#include <json/json.h>

/**
 * Generates tracks for a given set of pieces.
*/

std::vector<Piece> generateTrack(Json::Value selection);