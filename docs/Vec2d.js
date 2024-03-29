// Representation of a 2d vector
export class Vec2d{
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
    rotate(theta){
        const ct = Math.cos(theta)
        const st = Math.sin(theta)
        return new Vec2d(ct*this.x + st*this.y, -st*this.x + ct*this.y)
    }
}