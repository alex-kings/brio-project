import CanvasDrawer from "./CanvasDrawer.js"

// CURRENT IMAGE
const IMAGE = './track-images/example-tracks.jpg'

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
const figureDisplay = document.getElementById('figure-display')
const removeRectBtn = document.getElementById('remove-last-rect')
const cd = new CanvasDrawer(document.getElementById('canvas'), IMAGE)



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
    // Add rectangle
    addRectangle(w,h,px,py,a)
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
    // Add circle
    addCircle(r,posX,posY)
})

// Reset canvas sizes on window resize
window.addEventListener('resize',()=>{
    cd.setCanvasSize()
    cd.drawFigures(figures)
})

// Remove last rectangle
removeRectBtn.addEventListener('click',()=>{
    removeLastRect()
})

// Adds a rectangle to canvas and to list of figures.
function addRectangle(w, h, px, py, a){
    // Add rectangle to list of figures
    figures.rectangles.push({
        width:w,
        height:h,
        pos:{
            x:px,
            y:py,
        },
        angle:a
    })

    // Draw new rectangle
    cd.drawFigures(figures)

    // Add a rectangle to list of figures displayed
    let div = document.createElement('div')
    let rectangleNumber = figures.rectangles.length
    div.id = `rectangle${rectangleNumber}` 
    figureDisplay.appendChild(div)
    let label = document.createElement('label')
    div.appendChild(label)
    label.innerHTML = `Rectangle ${rectangleNumber} (${w},${h},${px},${py},${a})`
}

// Delete rectangle from canvas and from list of figures
function removeLastRect(){
    if(figures.rectangles.length < 1) return
    // Remove from DOM
    document.getElementById(`rectangle${figures.rectangles.length}`).remove()
    // Remove from list
    figures.rectangles.pop()
    // Re draw canvas
    cd.drawFigures(figures)
}

// Add a circle to canvas and to list of figures displayed
function addCircle(r, px, py){
    // Replace circle in figures
    figures.circle.radius = r
    figures.circle.pos = {
        x:px,
        y:py
    }
    // Plot figures
    cd.drawFigures(figures)

    // Delete previous circle element
    let previousCircle = document.getElementById('circle')
    if(previousCircle != null){
        previousCircle.remove()
    }

    // Add circle to list of figures displayed
    let div = document.createElement('div')
    div.id = 'circle'

    figureDisplay.prepend(div)
    let label = document.createElement('label')
    div.appendChild(label)
    label.innerHTML = `Circle (${r},${px},${py})`}
