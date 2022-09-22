/**
 * Functions to draw on canvas.
 */

const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
canvas.width = window.innerWidth
canvas.height = window.innerHeight


// draw point at position
export function drawPoint(v, colour){
    ctx.beginPath()
    ctx.arc(v.x, v.y, 5, 0, 2*Math.PI, false)
    ctx.fillStyle = colour
    ctx.fill()
}

// Draw curve
export function drawCurve(vertices, colour){
    ctx.strokeStyle = colour
    ctx.beginPath()
    ctx.moveTo(vertices[0].x, vertices[0].y)
    for(let i = 1; i < vertices.length; i++){
        ctx.lineTo(vertices[i].x, vertices[i].y)
    }
    ctx.stroke()
}


// Clears the canvas
export function clearScreen(){
    ctx.fillStyle='white'
    ctx.fillRect(0,0,canvas.width, canvas.height)
}

// Draw rectangle given on canvas
export function drawRect(rect, colour){
    ctx.strokeStyle = colour
    ctx.beginPath()
    ctx.moveTo(rect[0].x, rect[0].y)
    ctx.lineTo(rect[1].x, rect[1].y)
    ctx.lineTo(rect[2].x, rect[2].y)
    ctx.lineTo(rect[3].x, rect[3].y)
    ctx.lineTo(rect[0].x, rect[0].y)
    ctx.stroke()
}
