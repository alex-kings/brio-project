import {drawPoint, drawCurve, clearScreen, drawRect, drawVec, drawConnectors} from './draw.js'
import { Vec2d } from './Vec2d.js'

// Constants
const trackWidth = 40 // mm

// Inputs
const sendBtn = document.getElementById('sendBtn')
const nameInput = document.getElementById('nameInput')
const clearBtn = document.getElementById('clearBtn')
const levelInput = document.getElementById('levelInput')
const debugBtn = document.getElementById('debugBtn')

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

const in1 = document.getElementById('in1')
const in2 = document.getElementById('in2')
const in3 = document.getElementById('in3')
const in4 = document.getElementById('in4')
const out1 = document.getElementById('out1')
const out2 = document.getElementById('out2')
const out3 = document.getElementById('out3')
const out4 = document.getElementById('out4')


// Number of vertices created for bezier curves
const ITERATIONS = 1000

// Initialise canvas
clearScreen()

// Current figure
let currentFigure = null


// Send figure to the backend to store.
sendBtn.addEventListener('click',()=>{
    if(currentFigure == null) {
        console.warn('No figure specified.')
        return
    }
    
    if(nameInput.value === ''){
        console.warn('Enter a figure name/id!')
        return
    }
    currentFigure.name = nameInput.value


    // Send figure
    console.log('Sending figure.')
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
    // currentFigure = {
    //     bezierPoints: points,
    //     rectangles: res.rectangles
    // }
    addPart({
        bezierPoints:points,
        rectangles:res.rectangles,
        level:parseInt(levelInput.value)
    })

}

// Clear screen and current figure
clearBtn.addEventListener('click',()=>{
    currentFigure = null
    clearScreen()
})

// Print current figure in console
debugBtn.addEventListener('click', ()=>{
    console.log(currentFigure)
})

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

// Get input from user
drawBtn.addEventListener('click',()=>{
    // Check if the level value is specified
    if(levelInput.value === ''){
        console.warn('Level should be specified.')
        return false
    }

    // Get number of OBBs to draw around figure
    let n = parseInt(obbsInput.value)
    if(isNaN(n)) return

    let points = []
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

    // Draw control points
    points.forEach((p)=>{
        drawPoint(p,'green')
    })

    // Plot Bezier curve
    plotBezier(points,n)

    // Get connectors
    let connectors = []

    // Assume that there are at least 2 points
    if(in1.checked) connectors.push(getConnector(points[0], points[1], true))
    if(out1.checked) connectors.push(getConnector(points[0], points[1], false))
    if(in2.checked) connectors.push(getConnector(points[1], points[0], true))
    if(out2.checked) connectors.push(getConnector(points[1], points[0], false))
    if(in3.checked) connectors.push(getConnector(points[2], points[1], true))
    if(out3.checked) connectors.push(getConnector(points[2], points[1], false))
    if(in4.checked) connectors.push(getConnector(points[3], points[2], true))
    if(out4.checked) connectors.push(getConnector(points[3], points[2], false))

    // Add connectors to the figure
    currentFigure.connectors = connectors

    drawConnectors(connectors)
})

// Add a part to the current figure
function addPart(part){
    // Check if the current figure is defined
    if(currentFigure == null){
        currentFigure = {
            parts:[part]
        }
        return
    }
    // Check if the current figure already has parts
    if(currentFigure.parts == null){
        currentFigure.parts = [part]
        return
    }
    // The current figure already has parts
    currentFigure.parts.push(part)
}

// Get connector vector with a position, a direction and a type
// Type is either false for 'in' or true for 'out'
function getConnector(vOut, vFrom, type){
    // Get connector vector
    let direction = vOut.subtract(vFrom)
    direction = direction.scale(1/direction.modulus()) // Scale to unit vector

    // Return a connector object
    return{
        pos:vOut,
        direction:direction,
        type:type,
        level:parseInt(levelInput.value)
    }
}
