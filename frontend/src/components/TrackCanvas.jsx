import { createEffect } from "solid-js"

export default function TrackCanvas(props) {

    // number of iterations to plot curves
    const ITERATIONS = 50

    createEffect(() => {

        drawTrack(props.pieces)
    })

    function drawTrack(pieces) {
        // Get DOM references to canvas
        const canvas = document.getElementById("canvas");
        const ctx = canvas.getContext("2d")

        // Set width and height of canvas
        canvas.width = canvas.offsetWidth*2;
        canvas.height = canvas.offsetHeight*2;

        // Set origin of canvas at the center
        ctx.translate(canvas.width / 2, canvas.height / 2)

        console.log(pieces)

        // Draw large curve
        drawPiece(ctx, pieces[0])
    }

    // Draw a piece at position given on the canvas
    function drawPiece(ctx, piece) {
        // Draw each part
        piece.parts.forEach(part => {
            // Draw the bezier curve for this piece
            drawBezier(ctx, part.bezierPoints)
        })
    }

    // Draw point at given position on canvas
    function drawPoint(ctx, x, y) {
        ctx.fillStyle = "red"
        ctx.beginPath()
        ctx.arc(x, y, 3, 0, 2 * Math.PI)
        ctx.fill()
    }

    // Draw the given bezier curve
    function drawBezier(ctx, points) {
        // Check number of points is correct
        if (points.length < 2 || points.length > 4) return

        // Draw the bezier points
        points.forEach(p => {
            drawPoint(ctx, p.x, p.y)
        })

        // 2d bezier curve
        if (points.length === 2) {
            drawCurve(ctx, points, "black")
        }
        // 3d bezier curve

        // 4d bezier curve
        // Collect vertices
        let vertices = []
        for (let t = 0; t < 1; t += (1 / ITERATIONS)) {
            let x = (1 - t) ** 3 * points[0].x + 3 * (1 - t) ** 2 * t * points[1].x + 3 * (1 - t) * t ** 2 * points[2].x + t ** 3 * points[3].x
            let y = (1 - t) ** 3 * points[0].y + 3 * (1 - t) ** 2 * t * points[1].y + 3 * (1 - t) * t ** 2 * points[2].y + t ** 3 * points[3].y

            vertices.push({ x: x, y: y })
        }
        drawCurve(ctx, vertices, "black")


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