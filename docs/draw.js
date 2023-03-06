import { CanvasDragZoom } from "./scroll.js"
import { Vec2d } from "./Vec2d.js";

// Create drawing effect on canvas
// Set width and height of canvas
let canvas = document.getElementById("canvas")
canvas.width = canvas.offsetWidth;
canvas.height = canvas.offsetHeight;

let initialDraw = true
let startScreen = true;
let pieces = []
let circlesShowing = false;

// Initial drawing on canvas
function draw(options){    
    if(pieces.length != 0) startScreen = false;
    if(startScreen) {
        // Draw start screen
        options.ctx.font = `${canvas.width/35}px serif`
        options.ctx.fillText("Select pieces and click on \"Generate\" to build a circuit.",canvas.width/10,canvas.height/2)
    }
    // Filter out unused pieces
    let usedPieces = pieces.filter(p=>p.used)   

    options.ctx.translate(canvas.width/2, canvas.height/2)

    // Give colour to each part
    usedPieces.forEach(piece => {
        piece.parts.forEach(part => {
            part.colour = getPieceColour(piece.id)
        })
    })

    // Get all parts in a list
    let parts = usedPieces.map(p => (p.parts)).flat()
    
    // Add drawLevel to the parts
    parts.forEach(part => {
        part.drawLevel = part.level;
        part.isConnector = false;
    })

    // Get all male connectors in a list
    let connectors = usedPieces.map(p => (p.connectors)).flat().filter(c=>c.type)

    // Add drawLevel to the connectors
    connectors.forEach(con => {
        con.drawLevel = con.level + 0.5
        con.isConnector = true
    })

    // Put connectors and parts together
    let drawParts = parts.concat(connectors)

    // Sort the pieces by drawLevel
    drawParts.sort((a,b)=>(a.drawLevel > b.drawLevel))
    
    drawParts.forEach(p => {
        if(p.isConnector) {
            // Draw connector
            drawConnector(options.ctx,p)
        }
        else {
            // Draw the rectangles for this part
            p.rectangles.forEach(r=>{
                drawRect(options.ctx,r,p.colour, p.level)
            })
        }
    })

    if(circlesShowing) {
        // Draw all bounding circles
        options.ctx.strokeStyle = 'black'
        options.ctx.setLineDash([15,5]);
        usedPieces.forEach(p => {
            options.ctx.beginPath();
            options.ctx.arc(p.circleX, p.circleY, p.radius, 0, 2*Math.PI)
            options.ctx.stroke();
        })
        options.ctx.setLineDash([]);
    }

    options.ctx.translate(-canvas.width/2, -canvas.height/2)


    // // First sort the pieces by their level.
    // pieces.sort((a,b)=>(getLevel(a) > getLevel(b)));

    // pieces.forEach(piece => {
    //     // Only draw used pieces!
    //     if(piece.used) {
    //         drawPiece(options.ctx, piece, getPieceColour(piece.id))
    //     }            
    //     // if(piece.used) drawPiece(options.ctx, piece, "blue")
    //     // else drawPiece(options.ctx, piece, "gray")
    // })
}


function getLevel(piece) {
    let lowestLevel = piece.connectors[0].level
    for(let i = 1; i < piece.connectors.length; i++) {
        if(piece.connectors[i].level < lowestLevel) lowestLevel = piece.connectors[i].level
    }
    if(piece.id == "N") {
        // Ascending pieces have half-integer level.
        return lowestLevel + 0.5
    }
    return lowestLevel
}

// Piece-dependant colouring
function getPieceColour(pieceType) {
    switch (pieceType) {
        case "E":
            return '#fcba03'
        case "A":
            return '#f2ffa6'
        case "B":
            return '#c1ffa6'   
        case "C":
            return '#9de697'   
        case "D":
            return '#85bfd4'   
        case "A1":
            return '#7987b0'   
        case "B1":
            return '#bf95de'   
        case "C1":
            return '#dfbef7'   
        case "A2":
            return '#e07e9a'   
        case "B2":
            return '#bab29e'   
        case "C2":
            return '#8a8c88'   
        case "E1":
            return '#948cff'   
        case "M":
            return '#c495b7'   
    
        default:
            return '#ffa9a6';
    }
}


// Level outline colour (max 4 for now)
function getLevelColour(level) {
    if(level === 0) return "black"
    if(level === 1) return "blue"
    if(level === 2) return "orange"
    if(level === 3) return "red"
    else return "green"
}


// Take a line and returns a rectangle of given width around that line
function getRect(v1, v2){
    // Calculate unit perpendicular
    let up = v1.subtract(v2)
    up = new Vec2d(up.y, -up.x)
    up = up.scale((2)/up.modulus())

    return [v1.add(up),
            v1.subtract(up),
            v2.subtract(up),
            v2.add(up)]
}

function drawConnector(ctx,connector) {
    // Draw rectangle
    let length = 10
    let end = new Vec2d(connector.position.x + length*connector.direction.x,connector.position.y + length*connector.direction.y)
    let conRect = getRect(new Vec2d(connector.position.x, connector.position.y), end)
    drawRect(ctx,conRect,getLevelColour(connector.level),connector.level)

    // Draw circle at the end
    drawPoint(ctx,end.x, end.y, getLevelColour(connector.level), 5)
    
}

// Draw a piece at position given on the canvas
function drawPiece(ctx, piece, colour) {
    // Draw each part
    piece.parts.forEach(part => {
        // Draw the bezier curve for this piece
        // drawBezier(ctx, part.bezierPoints)

        // Draw the rectangles for this part
        part.rectangles.forEach(r=>{
            drawRect(ctx,r,colour, part.level)
        })
    })

    // Draw connectors
    piece.connectors.forEach(con => {
        // drawPoint(ctx, con.position.x, con.position.y, con.type? "red" : "green");

        // Only draw connectors of male (true) type.
        if(con.type) {
            drawConnector(ctx,con)
        }
    })
}


// Draw point at given position on canvas
function drawPoint(ctx, x, y, colour, size) {
    ctx.fillStyle = colour
    ctx.beginPath()
    ctx.arc(x, y, size, 0, 2 * Math.PI)
    ctx.fill()
}

// Draw rectangle
function drawRect(ctx, rect, colour, pieceLevel){
    let region = new Path2D()
    region.moveTo(rect[0].x, rect[0].y)
    region.lineTo(rect[1].x, rect[1].y)
    region.lineTo(rect[2].x, rect[2].y)
    region.lineTo(rect[3].x, rect[3].y)
    region.lineTo(rect[0].x, rect[0].y)

    region.closePath()

    ctx.fillStyle = colour
    ctx.fill(region, 'evenodd')

    // Stoke colour depends on the piece level.
    ctx.strokeStyle = getLevelColour(pieceLevel)
    ctx.stroke(region)
}


// Draw curve
function drawCurve(ctx, vertices, colour) {
    ctx.strokeStyle = colour
    ctx.beginPath()
    ctx.moveTo(vertices[0].x, vertices[0].y)
    for (let i = 1; i < vertices.length; i++) {
        ctx.lineTo(vertices[i].x, vertices[i].y)
    }
    ctx.stroke()
}


let cdz = new CanvasDragZoom(canvas, draw)

// Draw the given pieces on canvas.
export function redraw(newPieces, showBoundingCircles) {
    pieces = newPieces
    circlesShowing = showBoundingCircles
    cdz.redraw()
}
