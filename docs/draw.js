import { CanvasDragZoom } from "./scroll.js"

let pieces = []

// Initial drawing on canvas
function draw(options){       
    // First sort the pieces by their level.
    pieces.sort((a,b)=>(getLevel(a) > getLevel(b)));

    pieces.forEach(piece => {
        // Only draw used pieces!
        if(piece.used) {
            drawPiece(options.ctx, piece, getPieceColour(piece.id))
        }            
        // if(piece.used) drawPiece(options.ctx, piece, "blue")
        // else drawPiece(options.ctx, piece, "gray")
    })
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
        drawPoint(ctx, con.position.x, con.position.y, con.type? "red" : "green");
    })
}

// Draw point at given position on canvas
function drawPoint(ctx, x, y, colour) {
    ctx.fillStyle = colour
    ctx.beginPath()
    ctx.arc(x, y, 3, 0, 2 * Math.PI)
    ctx.fill()
}

// Draw rectangle
function drawRect(ctx, rect, colour, pieceLevel){
    // ctx.strokeStyle = colour
    // ctx.beginPath()
    // ctx.moveTo(rect[0].x, rect[0].y)
    // ctx.lineTo(rect[1].x, rect[1].y)
    // ctx.lineTo(rect[2].x, rect[2].y)
    // ctx.lineTo(rect[3].x, rect[3].y)
    // ctx.lineTo(rect[0].x, rect[0].y)
    // ctx.stroke()

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
    ctx.strokeStyle = (pieceLevel === 0 ? "black" : "red")
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


// Create drawing effect on canvas
// Set width and height of canvas
let canvas = document.getElementById("canvas")
canvas.width = canvas.offsetWidth;
canvas.height = canvas.offsetHeight;

let cdz = new CanvasDragZoom(canvas, draw)


export function redraw(newPieces) {
    pieces = newPieces
    cdz.redraw()
}