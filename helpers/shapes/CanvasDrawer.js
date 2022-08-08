import Vector2D from './Vector2D.js'

export default class CanvasDrawer{
    constructor(canvas, trackImg){
        this.canvas = canvas
        this.ctx = canvas.getContext('2d')
        this.bgColor = '#BBBBFF'
        this.setCanvasSize()
        this.img = new Image()
        this.img.onload = ()=>{this.clear()}
        this.img.src = trackImg
    }

    //Set canvas width and height depending on window dimensions
    setCanvasSize(){
        this.canvas.width = this.canvas.clientWidth
        this.canvas.height = this.canvas.clientHeight
        // Center of the canvas
        //this.center = new Vector2D(this.canvas.width/2, this.canvas.height/2)
        this.center = new Vector2D(0,0)
    }

    clear(){
        this.ctx.fillStyle = this.bgColor
        this.ctx.fillRect(0,0,this.canvas.width,this.canvas.height)
        
        this.ctx.drawImage(this.img,this.center.x,this.center.y)
    }

    // Draws the OBB specified by dimensions, position and angle from x axis
    drawOBB(halfSizeX, halfSizeY, posX, posY, angle){
        // Create vertex for each corner
        let vertices = [
            new Vector2D(-halfSizeX, halfSizeY),
            new Vector2D(halfSizeX, halfSizeY),
            new Vector2D(halfSizeX, -halfSizeY),
            new Vector2D(-halfSizeX, -halfSizeY),
        ]
        // Rotate each vertex
        vertices.forEach(v => {
            v.rotate(angle)
        })
        // Place center  rectangle at position
        vertices.forEach(v=>{
            v.add(this.center)
            v.add(new Vector2D(posX, posY)) 
        })
        // Draw rectangle
        this.ctx.fillStyle='black'
        this.ctx.beginPath()
        this.ctx.lineTo(vertices[1].x, vertices[1].y)
        this.ctx.lineTo(vertices[2].x, vertices[2].y)
        this.ctx.lineTo(vertices[3].x, vertices[3].y)
        this.ctx.lineTo(vertices[0].x, vertices[0].y)
        this.ctx.closePath()
        this.ctx.stroke()
    }

    // Draw the specified circle on canvas
    drawCircle(radius, posX, posY){
        this.ctx.fillStyle = 'black'
        this.ctx.beginPath()
        this.ctx.arc(posX + this.center.x, posY+this.center.y, radius, 0, 2*Math.PI)
        this.ctx.stroke()
    }

    // Draws all the figures specified.
    drawFigures(figures){
        this.clear()
        figures.rectangles.forEach((r)=>{
            this.drawOBB(r.width/2, r.height/2, r.pos.x, r.pos.y, r.angle)
        })
        if(figures.circle.pos != undefined){
            this.drawCircle(figures.circle.radius, figures.circle.pos.x, figures.circle.pos.y)
        }
    }
}