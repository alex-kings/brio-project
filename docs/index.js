import { redraw } from "./draw.js"
import {makeMeasurements} from "./measurements.js"

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
    let selection = {}
    pieces.forEach(piece => {
        if(ref(`quantity${piece.id}`).value != ""){
            selection[piece.id] = ref(`quantity${piece.id}`).value
        }
    })
    // Can't generate if no piees are mentioned.
    let n = 0;
    Object.keys(selection).forEach((key)=>{
        n += parseInt(selection[key])
    })
    if(n == 0)return;

    // Start loader
    ref("loadingPane").style.display = "flex";

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

        // Keep in local storage
        locallyStore(selection);

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


// Measurements
ref("measureBtn").addEventListener("click",()=>{
    // Start loader
    ref("loadingPane").style.display = "flex";
    // Get selection
    let selection = {}
    pieces.forEach(piece => {
        if(ref(`quantity${piece.id}`).value != ""){
            selection[piece.id] = ref(`quantity${piece.id}`).value
        }
    })
    // Get measurements
    makeMeasurements(JSON.stringify(selection))
})


// Store the input values in local storage.
function locallyStore() {

}
