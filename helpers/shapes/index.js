import CanvasDrawer from "./CanvasDrawer.js"

// DOM elements
const wInput = document.getElementById('width-input')
const hInput = document.getElementById('height-input')
const aInput = document.getElementById('angle-input')
const pxInput = document.getElementById('posx-input')
const pyInput = document.getElementById('posy-input')
const plotBtn = document.getElementById('plot-btn')
const rInput = document.getElementById('radius-input')
const circlePxInput = document.getElementById('posx-circle-input')
const circlePyInput = document.getElementById('posy-circle-input')
const circlePlotBtn = document.getElementById('add-circle-btn')
const cd = new CanvasDrawer(document.getElementById('canvas'))

// Holds the figures to plot on canvas
const figures = {
    rectangles:[],
    circle:{}
}


// Plot a new rectangle
plotBtn.addEventListener('click',()=>{
    // Get inputs
    let w = parseFloat(wInput.value)
    let h = parseFloat(hInput.value)
    let a = parseFloat(aInput.value)
    let px = parseFloat(pxInput.value)
    let py = parseFloat(pyInput.value)
    // Checks if inputs are floats
    if(isNaN(w)||isNaN(h)||isNaN(a)||isNaN(px)||isNaN(py)){
        alert('Enter numbers!')
        return
    }
    // Add a rectangle
    figures.rectangles.push({
        width:w,
        height:h,
        pos:{
            x:px,
            y:py,
        },
        angle:a
    })
    // Plots figures
    cd.drawFigures(figures)
})

// Plot a new circle
circlePlotBtn.addEventListener('click',()=>{
    // Get inputs
    let r = parseFloat(rInput.value)
    let posX = parseFloat(circlePxInput.value)
    let posY = parseFloat(circlePyInput.value)
    // Checks if inputs are floats
    if(isNaN(r)||isNaN(posX)||isNaN(posY)){
        alert('Enter numbers!')
        return
    }
    // Replace circle in figures
    figures.circle.radius = r
    figures.circle.pos = {
        x:posX,
        y:posY
    }
    // Plot figures
    cd.drawFigures(figures)
})

// Reset canvas sizes on window resize
window.addEventListener('resize',()=>{
    cd.setCanvasSize()
    cd.drawFigures(figures)
})
