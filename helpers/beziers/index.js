const trackWidth = 40

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


const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
canvas.width = window.innerWidth
canvas.height = window.innerHeight

const example = [new Vec2d(120,40), new Vec2d(170,240)]
const example2 = [new Vec2d(300,40), new Vec2d(100,120), new Vec2d(320,230)]
const example3 = [new Vec2d(30,30), new Vec2d(200,200), 
                    new Vec2d(-100,450), new Vec2d(320,450)]


// Print on screen a linear bezier curve with width
function print2dBezier(points){
    // Check that we have 2 points
    if(points.length != 2) return

    ctx.strokeStyle = 'red' // Plot red line
    ctx.beginPath()
    ctx.moveTo(points[0].x, points[0].y)
    ctx.lineTo(points[1].x, points[1].y)
    ctx.stroke()

    // Get rect
    let rect = getRect(points[0], points[1])

    // draw rect
    drawRect(rect, 'black')
}

// Print quadratic bezier curve on screen
function print3dBezier(points){
    // Check that we have 3 points
    if(points.length != 3) return

    // Get Bezier Curve vertices
    let vertices = []
    for(let t = 0; t < 1; t+=0.01){
        let x = (1-t)**2*points[0].x + 2*(1-t)*t*points[1].x + t**2*points[2].x
        let y = (1-t)**2*points[0].y + 2*(1-t)*t*points[1].y + t**2*points[2].y
        vertices.push(new Vec2d(x,y))
    }
    
    // Get OBBS for the shape
    // 3 for now
    let rectangles = generateObbs(vertices, 3)

    console.log('rectangles', rectangles)

    // Draw path
    drawCurve(vertices, 'red')

    // Draw rectangles
    rectangles.forEach(rect=>{
        drawRect(rect, 'black')
    })
}

// Print cubic bezier curve on screen
function print4dBezier(points){
    // Check that we have 4 points
    if(points.length != 4)return
    
    // Collect vertices
    const vertices = []
    for(let t = 0; t < 1; t += 0.01){
        let x = (1-t)**3*points[0].x + 3*(1-t)**2*t*points[1].x + 3*(1-t)*t**2*points[2].x + t**3*points[3].x
        let y = (1-t)**3*points[0].y + 3*(1-t)**2*t*points[1].y + 3*(1-t)*t**2*points[2].y + t**3*points[3].y

        vertices.push(new Vec2d(x,y))
    }

    // Collect OBBS
    let rectangles = generateObbs(vertices, 5)

    // Draw vertices
    drawCurve(vertices, 'red')
    
    // Draw rectangles
    rectangles.forEach(rectangle =>{
        drawRect(rectangle, 'black')
    })
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
    ctx.strokeStyle = colour
    ctx.beginPath()
    ctx.arc(v.x, v.y, 3, 0, 2*Math.PI, false)
    ctx.fillStyle = 'red'
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


print2dBezier(example)
print3dBezier(example2)
print4dBezier(example3)
