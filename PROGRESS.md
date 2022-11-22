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
- Try to place a piece both left and right!
- Only attempt piece flipping ones that have not been placed yet.
- Remove connection between connectors for a simple boolean.
- Fast flipping: no flip for straight tracks, only exchange of the connector types for symmetric ones...
- AVOID TESTING OF THE SAME TYPE OF TRACK TWICE!! This is done many times, and can potentially save a LOT of time.
- Place a piece left with more importance depending on the total amount placed already...
- Split the number of turns in multiple smaller batches
- randomize the initial set of pieces before running 
- only show placed pieces
- Take away the bezier points? (unused for the generation of tracks)
- multi threading at the start? Or for testing of tracks?


- Problem with tracks containing the same connectors.
- Somehow determine in advance that a certain track will not work (or is unlikely to)?

 MAJOR ISSUES :
 - { E: '8', E1: '0', A2: '0', A: '6', D: '3' }