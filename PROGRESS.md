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
