import { createEffect, onMount } from "solid-js"
import { CanvasDragZoom } from "../dependencies/scroll.js"

export default function TrackCanvas(props) {

    // number of iterations to plot curves
    const ITERATIONS = 50

    let canvas; 

    // On mount
    onMount(()=>{
        canvas = document.getElementById("canvas")

        // Set width and height of canvas
        canvas.width = canvas.offsetWidth;
        canvas.height = canvas.offsetHeight;

        new CanvasDragZoom(canvas, draw)
    })

    // Initial drawing on canvas
    function draw(options){       
        props.pieces.forEach(piece => {
            drawPiece(options.ctx, piece)
        })
    }

    // Draw a piece at position given on the canvas
    function drawPiece(ctx, piece) {
        // Draw each part
        piece.parts.forEach(part => {
            // Draw the bezier curve for this piece
            drawBezier(ctx, part.bezierPoints)

            // Draw the rectangles for this part
            part.rectangles.forEach(r=>{
                drawRect(ctx,r,"blue")
            })
        })

        // Draw connectors
        piece.connectors.forEach(con => {
            drawPoint(ctx, con.position.x, con.position.y, con.type? "red" : "green");
        })
    }

    // Draw point at given position on canvas
    function drawPoint(ctx, x, y, colour) {
        ctx.fillStyle = colour
        ctx.beginPath()
        ctx.arc(x, y, 3, 0, 2 * Math.PI)
        ctx.fill()
    }

    // Draw the given bezier curve
    function drawBezier(ctx, points) {
        // Check number of points is correct
        if (points.length < 2 || points.length > 4) return

        // // Draw the bezier points
        // points.forEach(p => {
        //     drawPoint(ctx, p.x, p.y, "red")
        // })

        // 2d bezier curve
        if (points.length === 2) {
            drawCurve(ctx, points, "black")
        }
        // 3d bezier curve
        if (points.length === 3) {
            let vertices = []
            for (let t = 0; t < 1; t += (1 / ITERATIONS)) {
                let x = (1 - t) ** 2 * points[0].x + 2 * (1 - t) * t * points[1].x + t ** 2 * points[2].x
                let y = (1 - t) ** 2 * points[0].y + 2 * (1 - t) * t * points[1].y + t ** 2 * points[2].y
                vertices.push(new Vec2d(x, y))
            }
        }
        // 4d bezier curve
        if (points.length === 4) {
            let vertices = []
            for (let t = 0; t < 1; t += (1 / ITERATIONS)) {
                let x = (1 - t) ** 3 * points[0].x + 3 * (1 - t) ** 2 * t * points[1].x + 3 * (1 - t) * t ** 2 * points[2].x + t ** 3 * points[3].x
                let y = (1 - t) ** 3 * points[0].y + 3 * (1 - t) ** 2 * t * points[1].y + 3 * (1 - t) * t ** 2 * points[2].y + t ** 3 * points[3].y

                vertices.push({ x: x, y: y })
            }
            drawCurve(ctx, vertices, "black")
        }
    }

    // Draw rectangle
    function drawRect(ctx, rect, colour){
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
    function drawCurve(ctx, vertices, colour) {
        ctx.strokeStyle = colour
        ctx.beginPath()
        ctx.moveTo(vertices[0].x, vertices[0].y)
        for (let i = 1; i < vertices.length; i++) {
            ctx.lineTo(vertices[i].x, vertices[i].y)
        }
        ctx.stroke()
    }

    return (
        <>
            <canvas id="canvas" className="track-canvas"></canvas>
        </>
    )
}