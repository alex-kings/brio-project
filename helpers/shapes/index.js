import CanvasDrawer from "./CanvasDrawer.js"

// CURRENT IMAGE
const IMAGE = '../../docs/assets/pieces/A1_resized.png'

// DOM elements
const addRectBtn = document.getElementById('plot-btn')
const rectDisplay = document.getElementById('rectangles')
const radiusInput = document.getElementById('radius-input')
const circlePxInput = document.getElementById('circle-pos-x')
const circlePyInput = document.getElementById('circle-pos-y')
const setCircleBtn = document.getElementById('circle-set')
const coeffInput = document.getElementById('coeff')
const coeffBtn = document.getElementById('set-coeff')
const debug = document.getElementById('debug')
const fileInput = document.getElementById('file-input')

// Canvas drawer
// Create canvas drawer on input of an image

let cd = new CanvasDrawer(document.getElementById('canvas'), IMAGE)

// Get file input
fileInput.addEventListener('change',()=>{
    const img = fileInput.files[0]
    const src = URL.createObjectURL(img)
    cd = new CanvasDrawer(document.getElementById('canvas'), src)
    document.getElementById('track-name').value = img.name.split('.')[0]
})


// Holds the figures to plot on canvas
const figures = {
    rectangles:[],

    // Default circle
    circle:{
        radius:parseFloat(radiusInput.value),
        pos:{
            x:parseFloat(circlePxInput.value),
            y:parseFloat(circlePyInput.value)
        }
    }
}

// Set coeff
coeffBtn.addEventListener('click',()=>{
    cd.setCoeff(parseFloat(coeffInput.value))
    updateUi()
})

// Debug button
debug.addEventListener('click',()=>{
    console.log(figures)
})

// Change circle
setCircleBtn.addEventListener('click',()=>{
    figures.circle.pos.x = parseFloat(circlePxInput.value)
    figures.circle.pos.y = parseFloat(circlePyInput.value)
    figures.circle.radius =parseFloat( radiusInput.value)

    updateUi()
})


// Add a new rectangle
addRectBtn.addEventListener('click',()=>{
    // Add rectangle with default values
    addRectangle(100,50,100,50,0)
})


// Reset canvas sizes on window resize
window.addEventListener('resize',()=>{
    cd.setCanvasSize()
    cd.drawFigures(figures)
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

// Updates the UI with the given figures
function updateUi(){
    // Draws the figures
    cd.drawFigures(figures)

    // Delete current list of rectangles
    rectDisplay.innerHTML = ""

    // Add rectangles
    figures.rectangles.forEach((r,index)=>{
        const div = document.createElement('div')
        rectDisplay.appendChild(div)
        div.classList.add('row')

        // Add inputs to this rectangle
        const widthInput = document.createElement('input')
        widthInput.placeholder = 'width'
        widthInput.classList.add('input')
        widthInput.defaultValue = r.width
        div.appendChild(widthInput)

        const heightInput = document.createElement('input')
        heightInput.placeholder = 'height'
        heightInput.classList.add('input')
        heightInput.defaultValue = r.height
        div.appendChild(heightInput)

        const xPosInput = document.createElement('input')
        xPosInput.placeholder = 'pos x'
        xPosInput.classList.add('input')
        xPosInput.defaultValue = r.pos.x
        div.appendChild(xPosInput)

        const yPosInput = document.createElement('input')
        yPosInput.placeholder = 'pos y'
        yPosInput.classList.add('input')
        yPosInput.defaultValue = r.pos.y
        div.appendChild(yPosInput)

        const angleInput = document.createElement('input')
        angleInput.placeholder = 'angle'
        angleInput.classList.add('input')
        angleInput.defaultValue = r.angle
        div.appendChild(angleInput)

        // Update values
        const setBtn = document.createElement('button')
        setBtn.classList.add('btn', 'set-btn')
        setBtn.innerHTML = 'set'

        div.appendChild(setBtn)
        setBtn.addEventListener('click',()=>{
            r.width = parseFloat(widthInput.value)
            r.height =parseFloat(heightInput.value)
            r.pos.x = parseFloat(xPosInput.value)
            r.pos.y = parseFloat(yPosInput.value)
            r.angle = parseFloat(angleInput.value)
            updateUi()
        })

        // Operate on rectangle
        addBtn(div, '^', ()=>{r.pos.y -= 1; updateUi()}) // Move up
        addBtn(div, 'v', ()=>{r.pos.y += 1; updateUi()}) // Move down
        addBtn(div, '>', ()=>{r.pos.x += 1; updateUi()}) // Move right
        addBtn(div, '<', ()=>{r.pos.x -= 1; updateUi()}) // Move left
        addBtn(div, 'sx', ()=>{r.width -= 1; updateUi()}) // Shrink x
        addBtn(div, 'ex', ()=>{r.width += 1; updateUi()}) // Expand x
        addBtn(div, 'sy', ()=>{r.height -= 1; updateUi()}) // Shrink y
        addBtn(div, 'ey', ()=>{r.height += 1; updateUi()}) // Expand y
        addBtn(div, 'rr', ()=>{r.angle += .1; updateUi()}) // Rotate clockwise
        addBtn(div, 'rl', ()=>{r.angle -= .1; updateUi()}) // Rotate counterclockwise



        // Add button to remove this rectangle
        const btn = document.createElement('button')
        div.appendChild(btn)
        btn.innerHTML="X"
        btn.classList.add('btn', 'btn-danger', 'btn-control')

        // Remove this rectangle from list on click
        btn.addEventListener('click',()=>{
            figures.rectangles.splice(index,1)
            updateUi()
        })
    })
    
}

function addBtn(parent, text, handler){
    const btn = document.createElement('button')
    btn.classList.add('btn', 'btn-control')
    btn.innerHTML = text
    parent.appendChild(btn)

    btn.addEventListener('click',handler)
}
