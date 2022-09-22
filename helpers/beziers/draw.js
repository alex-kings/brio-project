import { Vec2d } from "./Vec2d.js"

/**
 * Functions to draw on canvas.
 */

const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
canvas.width = window.innerWidth
canvas.height = window.innerHeight


// draw point at position
export function drawPoint(v, colour, radius){
    if(radius == null) radius = 5
    ctx.beginPath()
    ctx.arc(v.x, v.y, radius, 0, 2*Math.PI, false)
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

// Draw vector on canvas, starting at given pos
export function drawVec(vec, pos, colour){
    ctx.strokeStyle = colour
    // Draw starting point
    drawPoint(pos, colour, 2)
    drawLine(pos, vec.add(pos), colour)

    // Test
    let p = vec.add(pos)
    drawLine(p,p.add(vec.rotate(Math.PI*4/5).scale(0.2)), colour)
    drawLine(p,p.add(vec.rotate(Math.PI*-4/5).scale(0.2)), colour)
}

function drawLine(v1, v2, colour){
    ctx.strokeStyle = colour
    ctx.beginPath()
    ctx.moveTo(v1.x, v1.y)
    ctx.lineTo(v2.x, v2.y)
    ctx.stroke()
}
