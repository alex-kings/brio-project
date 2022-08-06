import Vector2D from "./Vector2D.js"

const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const center = new Vector2D(canvas.width/2, canvas.height/2)

// Clears the canvas and sets background
function clearCanvas(){
    ctx.fillStyle = '#BBBBFF'
    ctx.fillRect(0,0,canvas.width,canvas.height)
}

// Draws the OBB specified by dimensions, position and angle from x axis
function drawOBB(size, pos, angle){
    // Create vertices
    let vertices = [  // A,B,C,D
        new Vector2D(-size.x, size.y),
        new Vector2D(size.x, size.y),
        new Vector2D(size.x, -size.y),
        new Vector2D(-size.x, -size.y),
    ]
    // Rotate each vertex
    vertices.forEach(v => {
        v.rotate(angle)
    })
    // Place at position
    vertices.forEach(v=>{
        v.add(center)
        v.add(pos) 
    })
    // Draw rectangle
    ctx.fillStyle='black'
    //ctx.moveTo(vertices[0].x, vertices[0].y)
    ctx.beginPath()
    ctx.lineTo(vertices[1].x, vertices[1].y)
    ctx.lineTo(vertices[2].x, vertices[2].y)
    ctx.lineTo(vertices[3].x, vertices[3].y)
    ctx.lineTo(vertices[0].x, vertices[0].y)
    ctx.closePath()
    ctx.stroke()


    console.log(vertices)
}


function main(){
    // Demo
    clearCanvas()
    drawOBB(new Vector2D(30,60), new Vector2D(0,0), 2)
}


main()
