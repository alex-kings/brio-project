function ref(id) {
    return document.getElementById(id);
}

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
    let result
    createModule().then(({generateTrack}) => {
        result = JSON.parse(generateTrack(JSON.stringify(selection)))
        setTrack(result.pieces)
    })
})





