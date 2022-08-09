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
const circleDisplay = document.getElementById('circle-display')
const removeAll = document.getElementById('remove-all')
const rectDisplay = document.getElementById('rectangles')
const cd = new CanvasDrawer(document.getElementById('canvas'), IMAGE)



// Holds the figures to plot on canvas
const figures = {
    rectangles:[],
    circle:null
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

// Remove all button
removeAll.addEventListener('click',()=>{
    figures.rectangles = []
    figures.circle = null
    updateUi()
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
    updateUi() // update ui
}

// Add a circle to canvas and to list of figures displayed
function addCircle(r, px, py){
    // Replace circle in figures
    figures.circle = {
        radius:r,
        pos:{
            x:px,
            y:py
        }
    }

    // Update UI should be sufficient here!
    updateUi()
}

// Updates the UI with the given figures
function updateUi(){
    // Draws the figures
    cd.drawFigures(figures)

    // Delete current list of items
    circleDisplay.innerHTML=""
    rectDisplay.innerHTML = ""

    // Add circle to list of figures displayed
    if(figures.circle != null){
        let div = document.createElement('div')
        div.id = 'circle'
        circleDisplay.prepend(div)
        let label = document.createElement('label')
        div.appendChild(label)
        label.innerHTML = `Circle (${figures.circle.radius},${figures.circle.pos.x},${figures.circle.pos.y})`
    }

    // Add rectangles
    figures.rectangles.forEach((r,index)=>{
        let div = document.createElement('div')
        rectDisplay.appendChild(div)
        let label = document.createElement('label')
        div.appendChild(label)
        label.innerHTML = `${index} (${r.width},${r.height},${r.pos.x},${r.pos.y},${r.angle})`
        let btn = document.createElement('button')
        div.appendChild(btn)
        btn.innerHTML="x"
        btn.addEventListener('click',()=>{
            removeRectangle(index)
        })
    })

    function removeRectangle(index){
        figures.rectangles.splice(index,1)
        updateUi()
    }
    
}
