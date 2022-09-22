// Representation of a 2d vector
class Vec2d{
    constructor(x,y){
        this.x = x
        this.y = y
    }
    add(v){
        return new Vec2d(this.x + v.x, this.y + v.y)
    }
    subtract(v){
        return new Vec2d(this.x - v.x, this.y - v.y)
    }
    dot(v){
        return new Vec2d(this.x * v.x, this.y * v.y)
    }
    scale(a){
        return new Vec2d(this.x * a, this.y * a)
    }
    modulus(){
        return Math.sqrt(this.x**2 + this.y**2)
    }   
}

// Constants
const trackWidth = 40
const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
canvas.width = window.innerWidth
canvas.height = window.innerHeight

// Inputs
const sendBtn = document.getElementById('sendBtn')
const nameInput = document.getElementById('nameInput')

const vInput11 = document.getElementById('vInput11')
const vInput12 = document.getElementById('vInput12')
const vInput21 = document.getElementById('vInput21')
const vInput22 = document.getElementById('vInput22')
const vInput31 = document.getElementById('vInput31')
const vInput32 = document.getElementById('vInput32')
const vInput41 = document.getElementById('vInput41')
const vInput42 = document.getElementById('vInput42')
const drawBtn = document.getElementById('drawButton')

const obbsInput = document.getElementById('obbsInput')

// Number of vertices created for bezier curves
ITERATIONS = 1000


// Current figure
let currentFigure = null


// Event listeners
sendBtn.addEventListener('click',()=>{
    if(currentFigure == null) {
        console.log('Enter a figure!')
        return
    }

    // Send figure
    console.log('Sending figure.')
    currentFigure.name = nameInput.value
    savePiece(currentFigure)
})

// Plots the Bezier curve for the given points, with n rectangles
function plotBezier(points, n){
    let res = null
    // 2d Bezier
    if(points.length === 2){
        res = print2dBezier(points)
    }

    // 3d Bezier
    else if(points.length === 3){
        res = print3dBezier(points, n)
    }

    // 4d Bezier
    else if(points.length === 4){
        res = print4dBezier(points, n)
    }

    if(res == null) return

    // Draw path
    drawCurve(res.vertices, 'red')

    // Draw rectangles
    res.rectangles.forEach(rect=>{
        drawRect(rect, 'black')
    })

    // Store in current figure
    currentFigure = {
        bezierPoints: points,
        rectangles: res.rectangles
    }

}

// Return a set of vertices to plot the bezier curve and a set of OBBS around that curve
function print2dBezier(points){
    // Check that we have 2 points
    if(points.length != 2) return

    return {
        vertices: points,
        rectangles: [getRect(points[0], points[1])]
    }
}

// Print quadratic bezier curve on screen
function print3dBezier(points, n){
    // Check that we have 3 points
    if(points.length != 3) return

    // Get Bezier Curve vertices
    let vertices = []
    for(let t = 0; t < 1; t+= (1/ITERATIONS)){
        let x = (1-t)**2*points[0].x + 2*(1-t)*t*points[1].x + t**2*points[2].x
        let y = (1-t)**2*points[0].y + 2*(1-t)*t*points[1].y + t**2*points[2].y
        vertices.push(new Vec2d(x,y))
    }
    
    // Collect OBBS
    let rectangles = generateObbs(vertices, n)

    return {
        vertices: vertices,
        rectangles: rectangles
    }
}

// Print cubic bezier curve on screen
function print4dBezier(points, n){
    // Check that we have 4 points
    if(points.length != 4)return
    
    // Collect vertices
    let vertices = []
    for(let t = 0; t < 1; t += (1/ITERATIONS)){
        let x = (1-t)**3*points[0].x + 3*(1-t)**2*t*points[1].x + 3*(1-t)*t**2*points[2].x + t**3*points[3].x
        let y = (1-t)**3*points[0].y + 3*(1-t)**2*t*points[1].y + 3*(1-t)*t**2*points[2].y + t**3*points[3].y

        vertices.push(new Vec2d(x,y))
    }

    // Collect OBBS
    let rectangles = generateObbs(vertices, n)

    return {
        vertices: vertices,
        rectangles: rectangles
    }
}

// Take a curve as a list of vertices and a number of sections n, and generates n OBBs around the curve
function generateObbs(vertices, n){
    const rectangles = []
    let sep = vertices.length / n
    let v1 = vertices[0]
    for(let i = 1; i <= n; i++){
        let v2 = (vertices[Math.floor(sep*i) - 1])
        rectangles.push(getRect(v1, v2))
        v1 = v2
    }
    return rectangles
}

// Take a line and returns a rectangle of given width around that line
function getRect(v1, v2){
    // Calculate unit perpendicular
    let up = v1.subtract(v2)
    up = new Vec2d(up.y, -up.x)
    up = up.scale((trackWidth/2)/up.modulus())

    return [v1.add(up),
            v1.subtract(up),
            v2.subtract(up),
            v2.add(up)]
}


// draw point at position
function drawPoint(v, colour){
    ctx.beginPath()
    ctx.arc(v.x, v.y, 5, 0, 2*Math.PI, false)
    ctx.fillStyle = colour
    ctx.fill()
}

// Draw rectangle given on canvas
function drawRect(rect, colour){
    ctx.strokeStyle = colour
    ctx.beginPath()
    ctx.moveTo(rect[0].x, rect[0].y)
    ctx.lineTo(rect[1].x, rect[1].y)
    ctx.lineTo(rect[2].x, rect[2].y)
    ctx.lineTo(rect[3].x, rect[3].y)
    ctx.lineTo(rect[0].x, rect[0].y)
    ctx.stroke()
}

// Draw curve
function drawCurve(vertices, colour){
    ctx.strokeStyle = colour
    ctx.beginPath()
    ctx.moveTo(vertices[0].x, vertices[0].y)
    for(let i = 1; i < vertices.length; i++){
        ctx.lineTo(vertices[i].x, vertices[i].y)
    }
    ctx.stroke()
}


// Sends piece to backend to keep
async function savePiece(piece){
    const resp = await fetch('http://localhost:3000/add_piece',{
        method:'POST',
        headers:{'Content-Type':'application/json'},
        body:JSON.stringify(piece)
    })
    const result = await resp.json()
    console.log(result)
}

// const example = [new Vec2d(120,40), new Vec2d(170,240)]
// const example2 = [new Vec2d(300,40), new Vec2d(100,120), new Vec2d(320,230)]
// const example3 = [new Vec2d(30,30), new Vec2d(200,200), 
//                     new Vec2d(-100,450), new Vec2d(320,450)]

// plotBezier(example)
// plotBezier(example2, 3)
// plotBezier(example3, 4)


// Get input from user
drawBtn.addEventListener('click',()=>{
    // Get number of OBBs to draw around figure
    let n = parseInt(obbsInput.value)
    if(isNaN(n)) return

    points = []
    let in11 = vInput11.value
    let in12 = vInput12.value
    let in21 = vInput21.value
    let in22 = vInput22.value
    let in31 = vInput31.value
    let in32 = vInput32.value
    let in41 = vInput41.value
    let in42 = vInput42.value
    
    if(in11 !== "" && in12 !== "") {
        points.push(new Vec2d(parseInt(in11), parseInt(in12)))

        if(in21 !== "" && in22 !== "") {
            points.push(new Vec2d(parseInt(in21), parseInt(in22)))
    
            if(in31 !== "" && in32 !== "") {
                points.push(new Vec2d(parseInt(in31), parseInt(in32)))
        
                if(in41 !== "" && in42 !== "") {
                    points.push(new Vec2d(parseInt(in41), parseInt(in42)))
            
                    
                }
            }
        }
    }

    // Clear canvas
    clearScreen()

    // Draw control points
    points.forEach((p)=>{
        drawPoint(p,'green')
    })

    // Plot Bezier curve
    plotBezier(points,n)
})

// Clears the canvas
function clearScreen(){
    ctx.fillStyle='white'
    ctx.fillRect(0,0,canvas.width, canvas.height)
}
