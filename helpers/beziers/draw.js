/**
 * Functions to draw on canvas.
 */


const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
canvas.width = window.innerWidth
canvas.height = window.innerHeight


// Place origin of canvas to center of screen
ctx.translate(canvas.width/2, canvas.height/2)


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
    ctx.fillRect(-canvas.width/2, -canvas.height/2,canvas.width, canvas.height)
    // x axis
    ctx.strokeStyle = 'rgba(30,30,30, 0.6)'
    ctx.setLineDash([5,15])
    ctx.beginPath()
    ctx.moveTo(-canvas.width/2,0)
    ctx.lineTo(canvas.width/2, 0)
    ctx.stroke()
    // y axis
    ctx.beginPath()
    ctx.moveTo(0,-canvas.height/2)
    ctx.lineTo(0, canvas.height/2)
    ctx.stroke()

    ctx.setLineDash([])
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

export function drawConnectors(connectors){
    connectors.forEach(conn => {
        drawVec(conn.direction.scale(40), conn.pos, conn.type ? 'orange' : 'purple')
    })
}

export function drawArc(x,y,radius,startAngle,endAngle,colour){
    ctx.strokeStyle=colour
    ctx.beginPath()
    ctx.arc(x,y, radius, startAngle, -endAngle,true)
    ctx.stroke()
}
