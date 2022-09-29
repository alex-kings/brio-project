/**
 * The image of a piece given Bezier points.
 */

import { onMount } from "solid-js";


export default function PieceCanvas(props){

    let canvas;

    onMount(()=>{
        const ctx = canvas.getContext('2d')
        init(ctx)
    })

    function init(ctx){
        ctx.fillStyle='white'
        ctx.fillRect(0,0,canvas.width, canvas.height)
    }

    function drawBezierCurve(points, ctx){
        if(points.length === 2){
            // Draw 2d bezier curve

        }

        else if(points.length === 3){
            // Draw 3d bezier curve
        }

        else if(points.length === 4){
            // Draw 4d bezier curve
        }
    }

    return(
        <canvas ref={canvas} className="canvas"></canvas>
    )
}