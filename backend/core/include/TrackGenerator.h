#pragma once
#include "Piece.h"
#include <json/json.h>
#include "ReadJson.h"
#include "Part.h"
#include "Obb.h"
#include "Vec2D.h"
#include <vector>

/**
 * Generates tracks for a given set of pieces.
*/

/**
 * Returns a track generated from the given piece selection. Returns the starting piece of the track.
*/
Piece getTrack(const Json::Value& selection);


/**
 * Takes a starting piece, a next piece and a selection of pieces to pick from and generated a track looping back to the starting piece.
*/
bool generateTrack(const Piece& startPiece, const Piece& lastPiece, std::map<Piece,int> selection, std::vector<Piece> placedPieces);


/**
 * Returns a vector containing all available pieces from a Json::Value selection.
*/
std::vector<Piece> getAvailablePieces(const Json::Value& selection);
