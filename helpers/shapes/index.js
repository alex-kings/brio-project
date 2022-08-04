const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')

const w = canvas.width
const h = canvas.height

ctx.fillStyle = '#DDDDFF'
ctx.fillRect(0,0,w,h)

class Vector2D{
    constructor(x,y){
        this.x = x
        this.y = y
    }

    // Returns result of the add operation
    add(v){
        return new Vector2D(this.x + v.x, this.y + v.y)
    }

    // Returns result of the dot operation
    dot(v){
        
    }

}