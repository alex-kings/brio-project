import { redraw } from "./draw.js";

function ref(id) {
    return document.getElementById(id);
}

let resultPieces = null;

// Show bounding circles
let boundingCircleInput = ref("showBoundingCircles")
let showBoundingCircles = boundingCircleInput.checked
boundingCircleInput.addEventListener("change", ()=>{
    if(boundingCircleInput.checked) {
        showBoundingCircles = true
    }
    else {
        showBoundingCircles = false        
    }
    if(resultPieces!=null){
        redraw(resultPieces,showBoundingCircles)
    }
})

// Load images
const response = await fetch('./assets/pieces.json')
const result = await response.json()
let pieces = Object.keys(result).map((key) => { return result[key] })


// Create a cell for each piece
pieces.forEach(piece => {
    const tr = document.createElement("tr")
    tr.innerHTML = `<td>${piece.id}</td>
                    <td>${piece.name}</td>
                    <td><img class="piece-img" src='./assets/pieces/${piece.id}_resized.png' alt='Piece image here.'/></td>
                    <td><input type="number" min="0" class="input-me" id='quantity${piece.id}'/></td>`
    ref("tableBody").appendChild(tr)
})

// Kick off generation
ref('generateBtn').addEventListener('click',()=>{
    // Start loader
    ref("loadingPane").style.display = "flex";
    let selection = {}
    pieces.forEach(piece => {
        if(ref(`quantity${piece.id}`).value != ""){
            selection[piece.id] = ref(`quantity${piece.id}`).value
        }
    })
    let result

    // Get seed
    let seed = parseInt(ref("seedInput").value)
    if(isNaN(seed) || seed < 0) {
        seed = -1;
        console.log("Not a correct seed, using -1 instead.")
    }

    // Get validation condition category
    let vCondition;
    document.getElementsByName("validation-conditions").forEach(e=>{
        if(e.checked) vCondition = e.value;
    })
    createModule().then(({generateTrack}) => {
        // Stop loading
        ref("loadingPane").style.display = "none"

        // Generate!
        const res = generateTrack(JSON.stringify(selection), seed, ref("twoLevel").checked, vCondition)

        result = JSON.parse(res)
        // console.log(result)
        if(result.error == null) {
            redraw(result.pieces, showBoundingCircles)
            console.log(result.pieces)
            resultPieces = result.pieces
        }
        else {
            ref("errorMsg").innerText="Couldn't generate track: time ran out."
            setTimeout(()=>{
                ref("errorMsg").innerText = "";
            }, 8000)
        }
    })
})


