// Representation of a 2d vector
class vec2d{
    constructor(x,y){
        this.x = x
        this.y = y
    }
    add(v){
        this.x += v.x
        this.y += v.y
    }
    dot(v){
        this.x *= v.x
        this.y *= v.y
    }
    scale(a){
        this.x *= a
        this.y *= a
    }
}


const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
canvas.width = window.innerWidth
canvas.height = window.innerHeight

const example = [new vec2d(10,40), new vec2d(600,120)]
const example2 = [new vec2d(10,40), new vec2d(600,120), new vec2d(200,450)]
const example3 = [new vec2d(10,40), new vec2d(600,120), new vec2d(200,450), new vec2d(600,200)]


// Print on screen a linear bezier curve
function print2dBezier(points){
    // Check that we have 2 points
    if(points.length != 2) return
    ctx.beginPath()
    ctx.moveTo(points[0].x, points[0].y)
    ctx.lineTo(points[1].x, points[1].y)
    ctx.stroke()
}

// Print quadratic bezier curve on screen
function print3dBezier(points){
    // Check that we have 3 points
    if(points.length != 3) return
    ctx.beginPath()
    ctx.moveTo(points[0].x, points[0].y)
    for(let t = 0; t < 1; t+=0.01){
        let x = (1-t)**2*points[0].x + 2*(1-t)*t*points[1].x + t**2*points[2].x
        let y = (1-t)**2*points[0].y + 2*(1-t)*t*points[1].y + t**2*points[2].y

        ctx.lineTo(x,y)
    }
    ctx.stroke()
}

// Print cubic bezier curve on screen
function print4dBezier(points){
    // Check that we have 4 points
    if(points.length != 4)return
    ctx.beginPath()
    ctx.moveTo(points[0].x, points[0].y)
    for(let t = 0; t < 1; t += 0.01){
        let x = (1-t)**3*points[0].x + 3*(1-t)**2*t*points[1].x + 3*(1-t)*t**2*points[2].x + t**3*points[3].x
        let y = (1-t)**3*points[0].y + 3*(1-t)**2*t*points[1].y + 3*(1-t)*t**2*points[2].y + t**3*points[3].y

        ctx.lineTo(x,y)
    }
    ctx.stroke()
}



print2dBezier(example)
print3dBezier(example2)
print4dBezier(example3)