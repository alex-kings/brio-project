export default class Vector2D{
    constructor(x,y){
        this.x = x
        this.y = y
    }

    // Add vector v to this vector
    add(v){
        this.x += v.x
        this.y += v.y
    }

    // Scale by a constant c
    scale(c){
        this.x = this.x * c
        this.y = this.y * c
    }
    
    // Rotate this vector by an angle a
    rotate(a){
        let sinA = Math.sin(a)
        let cosA = Math.cos(a)
        let tempX = cosA*this.x - sinA*this.y
        let tempY = sinA*this.x + cosA*this.y
        this.x = tempX
        this.y = tempY
    }
}