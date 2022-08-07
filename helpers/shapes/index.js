import CanvasDrawer from "./CanvasDrawer.js"

// DOM elements
const wInput = document.getElementById('width-input')
const hInput = document.getElementById('height-input')
const aInput = document.getElementById('angle-input')
const plotBtn = document.getElementById('plot-btn')
const delBtn = document.getElementById('delete-btn')
const cd = new CanvasDrawer(document.getElementById('canvas'))


// Event listeners
plotBtn.addEventListener('click',()=>{
    let w = wInput.value
    let h = hInput.value
    let a = aInput.value
    // At least one input missing
    if(w==="" || h==="" || a===""){
        alert('Enter 3 valid inputs to plot rectangle!')
        return
    }
    // Inputs have to be numeric
    if(isNaN(w) || isNaN(h) || isNaN(a)){
        alert('Inputs have to be numeric!')
        return
    }
    cd.drawOBB(parseFloat(w)/2, parseFloat(h)/2,0,0, parseFloat(a))
    console.log('plotted')
})

delBtn.addEventListener('click',()=>{
    cd.clear()
})


