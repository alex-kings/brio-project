**Progress**

---

_20/09/2022_ **Work on piece characteristics determination helper**  
Add GUI buttons to help move rectangles up and down.  
Questions raised on best way to represent pieces. "Manual representation" with multiple rectangles, width, height, position of each, and separately position of connectors?  
Or succession of Bezier curves for the shape of piece, with points at the end of each bezier curve being a connector?
Latter seems much better. Would allow for precise connection of piece. However difficult to determine pieces with more than 2 connectors (or single connector pieces). A superposition of multiple bezier curves could do the trick.  
How to calculate collisions with just Bezier curve? This is an issue.  
Made Bezier curve plotter, need to add width to curves.
This is done by adding OBBs around curves

![Image of OBBs around Bezier curves](./progression_images/OBBS_Beziers.png "OBBs around Bezier curves")


_21/09/2022_ **Work on piece characteristics determination helper**  
Add inputs for users to interact with the Bezier plotter.  
![Image of Bezier curve with user inputs](./progression_images/bezier_with_user_inputs.png "Bezier with user inputs")

_22/09/2022_ **Work on Bezier curve piece helper: connectors**  
Add ability to determine the connectors of a piece.

_23/09/2022_ **Work on Bezier curve piece helper: connectors**  
Add option to draw multiple bezier curves for a single piece. This is to allow for pieces with more than 2 connectors and multi-level pieces.
Add a level to connectors and to rectangles (between 0 and 1, perhaps 2).
![Image of piece with multiple parts](./progression_images/piece_with_multiple_parts.png "Bezier with user inputs")  
Now need a separate input row that automatically fills in the bezier inputs for an arc with: (startPos, radius, startAngle, endAngle)

_25/09/2022_ **Work on Bezier curve piece helper: plotting arc circles**  
Add function to plot a bezier curve representation of an arc circle. Add inputs to specify the arc circle to plot on screen. Distance between bezier points 1-2 and 3-4 is given by a formula found on https://stackoverflow.com/questions/1734745/how-to-create-circle-with-b%C3%A9zier-curves.

_29/09/2022_ **Finish Bezier curve piece helper. Start on frontend with Solid.**   
Determine 13 simple tracks with bezier curve piece helper.  
Chose Solid as JS framework for frontend of the application. Other frameworks were considered (svelte, react...). Chose because of performance and package size. Don't need a massive framework with lots of support for specific things for this like react - small app. 
See https://krausest.github.io/js-framework-benchmark/current.html
Get images of pieces as assets.

_06/10/2022_ **Fronted**  
Delete backgrounds on track images. Improve track list design. Add page routing.

_26/10/2022_ **Cpp backend**  
Make class for vec2d, Obb, Piece. Add function to rotate single vec2d, and then single Obb.

_20/11/2022_ **Cpp backend and frontend**  
Make a canvas to display tracks generated in frontend.
Add function to translate Pieces by a certain amount.
Start implementation of the track generating function.
Check OBB collision detection using SAT (https://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection).

COULD IMPROVE: OBB Normals are re-calculated at each SAT. Could instead store them.

_20/11/2022_ **Cpp backend**  
Finish collision function.
Collision between two pieces is rather expensive? Discussion on the importance to keep OBBS low.
O(n*m) for collision between two pieces.

/!\ assumption made in track generation algorithm: Two consecutive pieces can never collide with each other.
/!\ 2nd assumption: parts of different levels do not collide!

What to do if the placeable piece has no connectors available left?

Only take into account euclidian distance for validation conditions for now.


_22/11/2022_ **Cpp backend**  
First successfully generated layouts!!
4 E, 2 A, 2 D, 2 A1, 1 A2, 5 E1, generates a track with 2 separate connected pieces??!
Add a bunch of "guides":
- Try to place a piece both left and right! (pretty much done)
- Only attempt piece flipping ones that have not been placed yet. (OK I think?)
- Remove connection between connectors for a simple boolean.
- Fast flipping: no flip for straight tracks, only exchange of the connector types for symmetric ones...
- Place a piece left with more importance depending on the total amount placed already...
- Split the number of turns in multiple smaller batches
- randomize the initial set of pieces before running 
- only show placed pieces
- Take away the bezier points? (unused for the generation of tracks)
- multi threading at the start? Or for testing of tracks?


- Problem with tracks containing the same connectors.
- Somehow determine in advance that a certain track will not work (or is unlikely to)?

- Mix up the order of each generation
- Add condition for the farthest away a piece can be placed.

- Modify the validation conditions for larger tracks (make them a percentage of the number of pieces perhaps?)

- Made modulus a constant value of a vector to avoid lots of calculation!

Removed testing of the same track multiple times. This saves a LOT of time. (comparison with / without ?) Talk about the FACTORIAL nature of the problem otherwise.


Similar inputs can take SIGNIFICANTLY different run times.
This means that a lot of improvement can be done.
(look at track 1, 2, 3) => 1 and 2 were made with initial shuffling, 3 without.

HUGE problems:
 - When a rather early track is place just in from of the validation track thus blocking it.
 - When the initial track goes "too far too soon" thus making the validation piece out of reach for all the incoming tests.

How to solve these?
 - Make it more difficult for the first half of pieces placed to come close the initial piece.
 - Make it impossible (or perhaps better, more difficult) for initial tracks to be placed out of reach.

_24/11/2022_ **Cpp backend**  
Added validation angle condition.
Add flip of pieces.

Found a case of impossible track (see impossible track).

Silly idea? Because the generation time is so dependant on initial conditions and has times varying from very very small to huge...
Could try for a certain amount of time, and if not successful re-try with a different initial set of pieces...
With multiple threads trying it could be fine..?

_12/12/2022_ **Helper**
Added a heuristic in the core to not allow pieces to be placed further away than half the total distance.
COULD IMPROVE THAT HEURISTIC TO RATHER NOT GO FURTHER THAN THE TOTAL DISTANCE LEFT IN HAND? (Need a nice way to know how much distance is left in hand)

Added option to load figure in the bezier helper to add a circle.
Added a circle to each figure, encompassing all the OBBS in that figure to make a much more efficient collision detection system.

![Bounding circle](./progression_images/Bounding_circle.png "Bounding circle around E track")


_8/01/2023_ **BACKEND CORE**
Add a test for rails in between the validation connectors before validating the track.
Can talk about how to test if there are pieces in between the two validation connectors:
this is done by virtually drawing a line between the two connectors and testing if there is any placed piece that has an OBB that intersects that line.

There is a very serious performance issue with the algorithm to test for pieces in between two connectors. Has been disabled at the minute.
Need to verify the complexity of it. (or time it?)

Need to disable the complete randomness.

Also need to modify the "between connectors" algorithm to not take into account the first and last pieces.

_10/01/2023_ **WASM FRONTEND**

Put core to interact with frontend via WASM.

IMPORTANT IMPROVEMENT: Made it so that a piece is first randomly flipped to the right or to the left before attempting to place it the first time. This prevents a "spiral" like generation.